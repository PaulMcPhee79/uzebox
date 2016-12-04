
/*
 *  LePatch - A patch tool for LePlatz
 *  Copyright (C) 2010 Paul McPhee
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

#include <QFile>
#include <QXmlStreamReader>
#include <QStringList>
#include "Interpreter.h"

const QString Interpreter::UPDATES_DIR = "./updates/";
const QString Interpreter::BACKUP_DIR = UPDATES_DIR + "backup/";

Interpreter::Interpreter(QObject *parent, int timeout)
    : QThread(parent), timeout(timeout), version(-1), updateScript(""), cancel(false)
{
    patcher = new Patcher(this, timeout);
    patcher->setInterpreter(this);
    connect(this, SIGNAL(applyPatch(QString,QString,QString,QString)), patcher, SLOT(applyPatch(QString,QString,QString,QString)));
}

void Interpreter::setVersion(const QString &v)
{
    if (isRunning())
        return;
    version = convertVersion(v);

    if (version < 100)  // 1.0 was initial release
        emit fatalError("Invalid version arg supplied: " + v + ". Aborting update.");
}

int Interpreter::convertVersion(const QString &v)
{
    QStringList s(v.split(QRegExp("\\.")));

    if (s.count() == 2) {
        bool ok;
        int major, minor;

        major = s.at(0).toInt(&ok);

        if (ok)
            minor = s.at(1).toInt(&ok);
        if (ok)
            return 100*major + minor;
    }
    return -1;
}

void Interpreter::setUpdateScript(const QString &script) {
    if (!isRunning())
        updateScript = UPDATES_DIR + script;
}

void Interpreter::applyUpdate()
{
    rwlock.lockForWrite();
    cancel = false;
    rwlock.unlock();
    start();
}

void Interpreter::cancelUpdate()
{
    rwlock.lockForWrite();
    cancel = true;
    condition.wakeOne();
    rwlock.unlock();
}

bool Interpreter::isPatchCancelled()
{
    bool patchState;
    rwlock.lockForRead();
    patchState = cancel;
    rwlock.unlock();
    return patchState;
}

void Interpreter::run()
{
    Q_ASSERT(patcher);
    emit updateStarted();

    if (!validateUpdateScript(updateScript)) {
        emit updateScriptParsed(true);
        cancel = true;
    } else {
        emit updateScriptParsed(false);
    }
    emit commandCountInitialised(cmds.count());
    bool status, push = true;

    revertCmds.clear();
    status = !isPatchCancelled();

    while (status && cmds.count()) {
        if (cmds.head().at(0) == "mv") {
            emit fileMoveStarted(cmds.head().at(1), cmds.head().at(2));
            status = moveFile(cmds.head().at(1), cmds.head().at(2));
            emit fileMoveFinished(!status);
        } else if (cmds.head().at(0) == "rm") {
            QString rmFile, buFile;

            if (cmds.head().count() == 3) {
                rmFile = cmds.head().at(2);
                buFile = BACKUP_DIR + rmFile;
            } else {
                rmFile = cmds.head().at(1);
                buFile = "";
            }
            if (QFile(rmFile).exists()) {
                emit fileRemovalStarted(rmFile);
                status = removeFile(rmFile, buFile);
                emit fileRemovalFinished(!status);
            } else {
                push = false;
            }
        } else if (cmds.head().at(0) == "cp") {
            emit fileCopyStarted(cmds.head().at(1), cmds.head().at(2));
            status = copyFile(cmds.head().at(1), cmds.head().at(2));
            emit fileCopyFinished(!status);
        } else if (cmds.head().at(0) == "mkdir") {
            if (!QDir(cmds.head().at(1)).exists()) {
                emit dirMakeStarted(cmds.head().at(1));
                status = makeDir(cmds.head().at(1));
                emit dirMakeFinished(!status);
            } else {
                push = false;
            }
        } else if (cmds.head().at(0) == "rmdir") {
            QString rmDir, buDir;

            if (cmds.head().count() == 3) {
                rmDir = cmds.head().at(2);
                buDir = BACKUP_DIR;
            } else {
                rmDir = cmds.head().at(1);
                buDir = "";
            }
            if (QDir(rmDir).exists()) {
                emit dirRemoveStarted(rmDir);
                status = removeDir(rmDir, buDir);
                emit dirRemoveFinished(!status);
            } else {
                push = false;
            }
        } else if (cmds.head().at(0) == "cpdir") {
            emit dirCopyStarted(cmds.head().at(1), cmds.head().at(2));
            status = copyDir(cmds.head().at(1), cmds.head().at(2));
            emit dirCopyFinished(!status);
        } else if (cmds.head().at(0) == "patch") {
            emit filePatchStarted(cmds.head().at(4));
            status = patchFile(cmds.head().at(1), cmds.head().at(2), cmds.head().at(3), cmds.head().at(4));

            if (status) {
                status = (exec() == 0)?true:false;

#if !defined(Q_OS_WIN32) && !defined(Q_OS_WIN64)
                if (status) {
                    QFile patchedFile(cmds.head().at(4));

                    if (patchedFile.exists()) {
                        if (patchedFile.isOpen())
                            patchedFile.close();
                        status = patchedFile.setPermissions(QFile::ExeOwner|QFile::ExeUser|QFile::ExeGroup|QFile::ExeOther|
                                QFile::WriteOwner|QFile::WriteUser|QFile::WriteGroup|QFile::WriteOther|
                                QFile::ReadOwner|QFile::ReadUser|QFile::ReadGroup|QFile::ReadOther);
                    }
                }
#endif
            }
            emit filePatchFinished(!status);
        } else {
            status = false;
        }
        if (status && push) {
            revertCmds.push(cmds.dequeue());
            emit commandCountChanged(cmds.count());
        } else if (status) {
            push = true;
            cmds.dequeue();
            emit commandCountChanged(cmds.count());
        }
        status = status && !isPatchCancelled();
    }

    if (!status)
        revertUpdate();
    emit dirRemoveStarted(BACKUP_DIR);
    emit dirRemoveFinished(!removeDir(BACKUP_DIR));

    if (status) {
        emit dirRemoveStarted(UPDATES_DIR);
        emit dirRemoveFinished(!removeDir(UPDATES_DIR));
    }
    emit updateFinished(!status);
}

bool Interpreter::revertUpdate()
{
    bool status = true;
    QStringList s;
    int totalCmds = revertCmds.count();

    emit commandCountInitialised(revertCmds.count());
    emit commandCountChanged(0);
    emit updateReversionStarted();

    while (status && revertCmds.count()) {
        s = revertCmds.pop();
        emit commandCountChanged(totalCmds - revertCmds.count());

        if (s.at(0) == "mv") {
            emit fileMoveStarted(s.at(2), s.at(1));
            status = moveFile(s.at(2), s.at(1));
            emit fileMoveFinished(!status);
        } else if (s.at(0) == "rm") {
            if (s.count() == 3 && s.at(1) == "-b") {    // Otherwise file was not backed up
                emit fileMoveStarted(BACKUP_DIR + s.at(2), s.at(2));
                status = moveFile(BACKUP_DIR + s.at(2), s.at(2));
                emit fileMoveFinished(!status);
            }
        } else if (s.at(0) == "cp") {
            emit fileRemovalStarted(s.at(2));
            status = removeFile(s.at(2));
            emit fileRemovalFinished(!status);
        } else if (s.at(0) == "mkdir") {
            emit dirRemoveStarted(s.at(1));
            status = removeDir(s.at(1));
            emit dirRemoveFinished(!status);
        } else if (s.at(0) == "rmdir") {
            if (s.count() == 3 && s.at(1) == "-b") {    // Otherwise directory was not backed up
                emit dirRestoreStarted(s.at(2));
                status = restoreDir(s.at(2), BACKUP_DIR + s.at(2));
                emit dirRestoredFinished(!status);
            }
        } else if (s.at(0) == "cpdir") {
            emit dirRemoveStarted(s.at(2));
            status = removeDir(s.at(2));
            emit dirRemoveFinished(!status);
        } else if (s.at(0) == "patch") {
            // Old file will be restored when mv is reverted
            QFile file(s.at(4));

            if (file.exists()) {
                emit fileRemovalStarted(file.fileName());
                status = removeFile(file.fileName());
                emit fileRemovalFinished(!status);
            }
        }
    }
    emit updateReversionFinished(!status);
    return status;
}

bool Interpreter::validateUpdateScript(const QString &script)
{
    QFile file(script);

    if (!file.exists() || !file.open(QFile::ReadOnly | QFile::Text))
        return false;
    cmds.clear();

    bool validCmds = true;
    QXmlStreamReader reader(&file);

    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            if (reader.name() == "LePlatzUpdates") {
                ; // Do nothing
            } else if (reader.name() == "Update") {
                if (reader.attributes().count()) {
                    int v = convertVersion(reader.attributes().value("version").toString());

                    if (v > version)
                        continue;
                }
                reader.skipCurrentElement();
            } else if (reader.name() == "UpdateCmds") {
                ; // Do nothing
            } else if (reader.name() == "Cmd") {
                QStringList s(reader.readElementText().split(QRegExp("\\s"))); // Spaces in paths not supported

                if (validateCmd(s))
                    cmds.append(s);
                else
                    validCmds = false;
            } else {
                reader.skipCurrentElement();
            }
        }
    }
    file.close();
    return validCmds && !reader.error();
}

bool Interpreter::validateCmd(const QStringList &cmd)
{
    if (cmd.count() == 0)
        return false;
    if (cmd.at(0) == "mv") {
        return cmd.count() == 3;
    } else if (cmd.at(0) == "rm") {
        return cmd.count() == 2 || (cmd.count() == 3 && cmd.at(1) == "-b");
    } else if (cmd.at(0) == "cp") {
        return cmd.count() == 3;
    } else if (cmd.at(0) == "mkdir") {
        return cmd.count() == 2;
    } else if (cmd.at(0) == "rmdir") {
        return cmd.count() == 2 || (cmd.count() == 3 && cmd.at(1) == "-b");
    } else if (cmd.at(0) == "cpdir") {
        return cmd.count() == 3;
    } else if (cmd.at(0) == "patch") {
        return cmd.count() == 5;
    } else {
        return false;
    }
}

// Ensure a rogue script can't mess with files outside of the LePlatz directory tree. This assumes
// that the patch is applied from within the top-level LePlatz directory (the default).
bool Interpreter::isChildPath(const QString &path, const QDir &dir)
{
    return !dir.relativeFilePath(path).startsWith("..");
}

bool Interpreter::moveFile(const QString &from, const QString &to)
{
    QFile file(from);
    QDir dir(to.left(to.lastIndexOf("/")));

    if (QFile(to).exists()) {
        if (moveFile(to, BACKUP_DIR + to))
            revertCmds.push(QStringList() << "mv" << to << (BACKUP_DIR + to));
        else
            return false;
    }
    if (!dir.exists()) {
        if (!makeDir(dir.absolutePath()))
            return false;
    }
    if (!isChildPath(from) || !isChildPath(to))
        return false;
    if (!file.exists())
        return false;
    if (file.isOpen())
        file.close();
    return file.rename(to);
}

bool Interpreter::removeFile(const QString &fileName, const QString &backupName)
{
    QFile file(fileName);

    if (!file.exists()) // Don't let missing file prevent update
        return true;
    if (!isChildPath(fileName) || (!backupName.isEmpty() && !isChildPath(backupName)))
        return false;
    if (file.isOpen())
        file.close();
    if (!backupName.isEmpty() && !copyFile(fileName, backupName))
        return false;
    return file.remove();
}

bool Interpreter::copyFile(const QString &from, const QString &to)
{
    if (from == to)
        return true;
    QFile fileFrom(from), fileTo(to);
    QDir dir(to.left(to.lastIndexOf("/")));

    if (!fileFrom.exists() || !isChildPath(from) || !isChildPath(to))
        return false;
    if (fileTo.exists()) {
        if (moveFile(to, BACKUP_DIR + to))
            revertCmds.push(QStringList() << "mv" << to << (BACKUP_DIR + to));
        else
            return false;
    }
    if (!dir.exists()) {
        if (!makeDir(dir.absolutePath()))
            return false;
    }
    if (fileFrom.isOpen())
        fileFrom.close();
    return fileFrom.copy(to);
}

bool Interpreter::makeDir(const QString &dirPath)
{
    QDir dir(dirPath);

    if (dir.exists())
        return true;
    if (isChildPath(dirPath))
        return dir.mkpath(dir.absolutePath());
    return false;
}

bool Interpreter::copyDir(const QString &from, const QString &to)
{
    if (!QDir(from).exists() || !isChildPath(from) || !isChildPath(to))
        return false;
    bool status = true;
    QDir dir(from);
    QFileInfoList fil = dir.entryInfoList();

    if (isPatchCancelled())
        return false;

    if (!QDir(to).exists())
        status = QDir().mkdir(to);
    foreach(QFileInfo fi, fil) {
        if (!status)
            break;
        if (fi.isDir() && (QDir(fi.absoluteFilePath()) != dir) && (isChildPath(fi.absoluteFilePath(), dir)))
            status = copyDir(fi.absoluteFilePath(), to + "/" + dir.relativeFilePath(fi.absoluteFilePath()));
        else if (fi.isFile())
            status = copyFile(fi.absoluteFilePath(), to + "/" + dir.relativeFilePath(fi.absoluteFilePath()));
    }
    return status;
}

bool Interpreter::removeDir(const QString &dirName, const QString &backupDirName)
{
    if (!QDir(dirName).exists())    // Don't let missing dir prevent update
        return true;
    if (!isChildPath(dirName))
        return false;
    bool status = true;
    QDir dir(dirName);
    QFileInfoList fil = dir.entryInfoList(QDir::Hidden|QDir::AllDirs|QDir::Files);

    if (isPatchCancelled())
        return false;

    foreach(QFileInfo fi, fil) {
        if (fi.isDir() && (QDir(fi.absoluteFilePath()) != dir) && (isChildPath(fi.absoluteFilePath(), dir)))
            status = removeDir(fi.absoluteFilePath(), backupDirName);
        else if (fi.isFile() && !backupDirName.isEmpty())
            status = removeFile(fi.absoluteFilePath(), backupDirName + "/" + QDir::current().relativeFilePath(fi.absoluteFilePath()));
        else if (fi.isFile())
            status = removeFile(fi.absoluteFilePath(), backupDirName);
        if (!status)
            break;
    }
    if (status)
        status = QDir().rmdir(dirName);
    return status;
}

bool Interpreter::restoreDir(const QString &origDirName, const QString &currDirName)
{
    if (!isChildPath(origDirName) || !isChildPath(currDirName))
        return false;
    bool status = true;
    QDir dir(currDirName);
    QFileInfoList fil = dir.entryInfoList();

    if (isPatchCancelled())
        return false;

    foreach(QFileInfo fi, fil) {
        if (fi.isDir() && (QDir(fi.absoluteFilePath()) != dir) && (isChildPath(fi.absoluteFilePath(), dir))) {
            status = restoreDir(origDirName + "/" + QDir(currDirName).relativeFilePath(fi.absoluteFilePath()) + "/", fi.absoluteFilePath());
        } else if (fi.isFile()) {
            status = copyFile(fi.absoluteFilePath(), origDirName + "/" + QDir(currDirName).relativeFilePath(fi.absoluteFilePath()));
        }
        if (!status)
            break;
    }
    if (!QDir(origDirName).exists())
        status =  QDir().mkdir(origDirName);
    return status;
}

bool Interpreter::patchFile(const QString &tool, const QString &oldFile, const QString &deltaFile, const QString &newFile)
{
#if !defined(Q_OS_WIN32) && !defined(Q_OS_WIN64)
    QFile f(tool);

    if (!f.exists())
        return false;
    if (f.isOpen())
        f.close();
    if (!f.setPermissions(QFile::ExeOwner|QFile::ExeUser))
        return false;
#endif
    if (!isChildPath(tool) || !isChildPath(oldFile) || !isChildPath(deltaFile) || !isChildPath(newFile))
        return false;
    if (!patcher || !QFile::exists(tool) || !QFile::exists(oldFile) || !QFile::exists(deltaFile) || (QFile::exists(newFile) && !removeFile(newFile)))
        return false;
    emit applyPatch(tool, oldFile, deltaFile, newFile);
    return true;
}

Interpreter::~Interpreter()
{
    rwlock.lockForWrite();
    cancel = true;
    condition.wakeOne();
    rwlock.unlock();
    wait();
    delete patcher;
}
