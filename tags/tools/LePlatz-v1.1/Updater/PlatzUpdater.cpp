
/*
 *  LePlatz - A level editor for the Platz toolset (Uzebox - supports VIDEO_MODE 3)
 *  Copyright (C) 2009 Paul McPhee
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtNetwork>
#include <PlatzEnums.h>
#include "PlatzUpdater.h"

// This file adapted from original Qt Http Client example
PlatzUpdater::PlatzUpdater(QWidget *parent)
    : baseWin(parent), update(0), state(Idle), updateSucceeded(false)
{
    http = new QHttp(this);
    connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader&)), this, SLOT(readResponseHeader(const QHttpResponseHeader&)));
}

bool PlatzUpdater::downloadUpdates(const QString &versionUrl)
{
    update = new UpdateFile();
    update->file.setFileName("updates/LePlatz-Updates.xml");
    update->url.setUrl(versionUrl);
    state = VersionCheck;

    bool retval = beginDownload();

    if (!retval)
        state = Idle;
    return retval;
}

void PlatzUpdater::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    switch (responseHeader.statusCode()) {
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
        // these are not error conditions
        break;
    default:
        httpRequestAborted = true;

        if (http)
            http->abort();
        break;
    }
}

void PlatzUpdater::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (httpRequestAborted || state != Downloading || !update)
        return;
    emit progressChanged(update->file.fileName(), bytesRead, totalBytes, updatedList.count(), updatesQ.count() + updatedList.count() + 1);
}

// Ensure a rogue script can't mess with files outside of the LePlatz directory tree.
bool PlatzUpdater::isChildPath(const QString &path, const QDir &dir)
{
    return !dir.relativeFilePath(path).startsWith("..");
}

bool PlatzUpdater::beginDownload()
{
    if (!update || state == Idle)
        return false;

    QDir dir(QCoreApplication::applicationDirPath() + "/updates");

    if (!dir.exists()) {
        dir.setPath(QCoreApplication::applicationDirPath());

        if (!dir.mkpath("updates"))
            return false;
    }
    QFileInfo fileInfo(update->url.toString());

    if (fileInfo.fileName().isEmpty())
        return false;
    QString fileName = dir.absolutePath() + "/" + update->file.fileName();
    QDir dirSave(fileName.left(fileName.lastIndexOf("/")));

    update->file.setFileName(fileName);

    if (!dirSave.exists()) {
        if (!dirSave.mkpath(dirSave.absolutePath()))
            return false;
    }

    if (!update->file.open(QIODevice::WriteOnly))
        return false;
    QHttp::ConnectionMode mode = update->url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    http->setHost(update->url.host(), mode, update->url.port() == -1 ? 0 : update->url.port());
    httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(update->url.path(), "!$&'()*+,;=:@/");

    if (path.isEmpty())
        path = "/";
    httpGetId = http->get(path, &update->file);
    return true;
}

bool PlatzUpdater::downloadUpdateFile()
{
    if (!update)
        return false;
    return beginDownload();
}

bool PlatzUpdater::parseUpdateFile(QIODevice *device)
{
    if (!device || !device->open(QFile::ReadOnly | QFile::Text))
        return false;
    QString s;
    QXmlStreamReader reader(device);

    while(!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            s = reader.name().toString();

            if (s == "LePlatzUpdates") {
                ; // Do nothing
            } else if (s == "Update") {
                if (reader.attributes().count() == 0 || Platz::LEPLATZ_VERSIONS.contains(reader.attributes().value("version").toString()))
                    reader.skipCurrentElement();
            } else if (s == "Files") {
                parseFiles(reader);
            } else {
                reader.skipCurrentElement();
            }
        }
    }
    device->close();
    return !reader.error();
}

void PlatzUpdater::parseFiles(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "Files");

    QString s;

    while(!reader.atEnd()) {
        reader.readNext();

        if (reader.isEndElement() && reader.name() == "Files") {
            break;
        } else if (reader.isStartElement()) {
            s = reader.name().toString();

            if (s == "File") {
                UpdateFile *uf = new UpdateFile();
                updatesQ.enqueue(uf);
            } else if (s == "Name") {
                if (updatesQ.isEmpty())
                    break;
                updatesQ.last()->file.setFileName(reader.readElementText());

                if (!isChildPath(updatesQ.last()->file.fileName()))
                    reader.raiseError();
            } else if (s == "Url") {
                if (updatesQ.isEmpty())
                    break;
                updatesQ.last()->url.setUrl(reader.readElementText());
            } else {
                reader.skipCurrentElement();
            }
        }
    }
}

void PlatzUpdater::httpRequestFinished(int requestId, bool error)
{
    Q_ASSERT(state != Idle && update);

    if (requestId != httpGetId)
        return;
    if (state == VersionCheck) {
        update->file.close();

        if (error || httpRequestAborted) {
            emit downloadComplete(error, http->errorString());
            deleteLater();
            return;
        }

        if (!parseUpdateFile(&update->file)) {
            emit downloadComplete(true, "Invalid update file format.");
            deleteLater();
        } else if (updatesQ.isEmpty()) {
            emit downloadComplete(true, "Current version is up-to-date.");
            deleteLater();
        } else {
            state = Downloading;
            update = updatesQ.dequeue();

            if (!downloadUpdateFile()) {
                emit downloadComplete(true, "Incomplete update file.");
                deleteLater();
            }
        }
    } else if (state == Downloading) {
        if (error || httpRequestAborted) {
            update->file.remove();

            foreach (UpdateFile *uf, updatedList)
                uf->file.remove();
            emit downloadComplete(error, http->errorString());
            deleteLater();
            return;
        }
        update->file.close();
        updatedList.append(update);

        if (!updatesQ.isEmpty()) {
            update = updatesQ.dequeue();

            if (!downloadUpdateFile()) {
                emit downloadComplete(true, "Incomplete update file.");
                deleteLater();
            }
        } else {
            updateSucceeded = true;
            emit downloadComplete(error, http->errorString());
            deleteLater();
        }
    }
}

bool PlatzUpdater::removeFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.exists())
        return true;
    if (!isChildPath(fileName))
        return false;
    if (file.isOpen())
        file.close();
    return file.remove();
}

bool PlatzUpdater::removeDir(const QString &dirName)
{
    if (!QDir(dirName).exists())
        return true;
    bool status = true;
    QDir dir(dirName);
    QFileInfoList fil = dir.entryInfoList();

    foreach(QFileInfo fi, fil) {
        if (fi.isDir() && (QDir(fi.absoluteFilePath()) != dir) && (isChildPath(fi.absoluteFilePath(), dir)))
            status = removeDir(fi.absoluteFilePath());
        else if (fi.isFile())
            status = removeFile(fi.absoluteFilePath());
        if (!status)
            break;
    }
    if (status)
        status = QDir().rmdir(dirName);
    return status;
}

PlatzUpdater::~PlatzUpdater()
{
    if (state != Idle && http && http->state() != QHttp::Unconnected)
        http->abort();
    delete http;
    http = 0;
    UpdateFile *it = 0;

    while (!updatesQ.isEmpty()) {
        it = updatesQ.dequeue();

        if (it != update)
            delete it;
    }
    while (!updatedList.isEmpty()) {
        it = updatedList.takeFirst();

        if (it != update)
            delete it;
    }
    delete update;
    update = 0;

    if (!updateSucceeded)
        removeDir(QCoreApplication::applicationDirPath() + "/updates");
}
