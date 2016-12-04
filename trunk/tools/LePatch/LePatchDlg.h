
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

#ifndef LEPATCHDLG_H
#define LEPATCHDLG_H

#include <QDialog>
#include <Interpreter.h>

namespace Ui {
    class LePatchDlg;
}

class LePatchDlg : public QDialog {
    Q_OBJECT
public:
    LePatchDlg(QWidget *parent = 0);
    ~LePatchDlg();
    
    void setVersion(const QString &version);
    void setUpdateScript(const QString &script);

public slots:
    void scriptParseComplete(bool error);
    void setCommandCount(int count);
    void setCommandsRemaining(int count);
    void fileMoveStarted(QString from, QString to);
    void fileRemovalStarted(QString fileName);
    void fileCopyStarted(QString from, QString to);
    void filePatchStarted(QString fileName);
    void dirMakeStarted(QString dirName);
    void dirRemoveStarted(QString dirName);
    void dirCopyStarted(QString from, QString to);
    void dirRestoreStarted(QString dirName);
    void updateStarted();
    void updateFinished(bool error);
    void updateReversionStarted();
    void updateReversionFinished(bool error);
    void commandComplete(bool error);
    void fatalError(QString msg);

private slots:
    void toggleDetails();
    void applyUpdate();
    void exitProgram();

private:
    void print(const QString &s, bool append = false);
    void printLn(const QString &s, bool append = false);
    void initLogFile();

    Ui::LePatchDlg *ui;
    Interpreter *interpreter;
    QFile logFile;
    bool updating;
};

#endif // LEPATCHDLG_H
