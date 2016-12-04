
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

#ifndef PLATZUPDATER_H
#define PLATZUPDATER_H

#include <QObject>
#include <QHttp>
#include <QQueue>
#include <QDir>
#include <QUrl>
#include <QXmlStreamReader>

class PlatzUpdater : public QObject
{
    Q_OBJECT
public:
    struct UpdateFile {
        QFile file;
        QUrl url;

        UpdateFile()
            : file(""), url("")
        {}
    };

    PlatzUpdater(QWidget *parent = 0);
    ~PlatzUpdater();

    bool downloadUpdates(const QString &versionUrl);

private slots:
    bool beginDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

signals:
    void progressChanged(const QString &fileName, int bytesRead, int totalBytes, int fileIndex, int totalFiles);
    void downloadComplete(bool error, const QString &errMsg);
private:
    enum UpdateState {
        Idle,
        VersionCheck,
        Downloading
    };

    bool isChildPath(const QString &path, const QDir &dir = QDir::current());
    bool downloadUpdateFile();
    bool parseUpdateFile(QIODevice *device);
    void parseFiles(QXmlStreamReader &reader);
    bool removeFile(const QString &fileName);
    bool removeDir(const QString &dirName);

    QWidget *baseWin;
    QHttp *http;
    UpdateFile *update;
    QQueue<UpdateFile*> updatesQ;
    QList<UpdateFile*> updatedList;
    int httpGetId;
    bool httpRequestAborted;
    UpdateState state;
    bool updateSucceeded;
};

#endif // PLATZUPDATER_H
