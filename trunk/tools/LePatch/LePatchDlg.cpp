
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

#include <QMessageBox>
#include <Patcher.h>
#include "LePatchDlg.h"
#include "ui_LePatchDlg.h"

LePatchDlg::LePatchDlg(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint), ui(new Ui::LePatchDlg),
        logFile("./patchLog.txt"), updating(false)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/128x128/LePatch-tick.png"));
    interpreter = new Interpreter(this, 10000);

    connect(ui->pbDetails, SIGNAL(clicked()), this, SLOT(toggleDetails()));
    connect(ui->pbApplyUpdate, SIGNAL(clicked()), this, SLOT(applyUpdate()));
    connect(ui->pbExit, SIGNAL(clicked()), this, SLOT(exitProgram()));
    connect(interpreter, SIGNAL(commandCountInitialised(int)), this, SLOT(setCommandCount(int)));
    connect(interpreter, SIGNAL(commandCountChanged(int)), this, SLOT(setCommandsRemaining(int)));
    connect(interpreter, SIGNAL(fileMoveStarted(QString,QString)), this, SLOT(fileMoveStarted(QString,QString)));
    connect(interpreter, SIGNAL(fileRemovalStarted(QString)), this, SLOT(fileRemovalStarted(QString)));
    connect(interpreter, SIGNAL(fileCopyStarted(QString,QString)), this, SLOT(fileCopyStarted(QString,QString)));
    connect(interpreter, SIGNAL(filePatchStarted(QString)), this, SLOT(filePatchStarted(QString)));
    connect(interpreter, SIGNAL(dirMakeStarted(QString)), this, SLOT(dirMakeStarted(QString)));
    connect(interpreter, SIGNAL(dirRemoveStarted(QString)), this, SLOT(dirRemoveStarted(QString)));
    connect(interpreter, SIGNAL(dirCopyStarted(QString,QString)), this, SLOT(dirCopyStarted(QString,QString)));
    connect(interpreter, SIGNAL(dirRestoreStarted(QString)), this, SLOT(dirRestoreStarted(QString)));
    connect(interpreter, SIGNAL(updateStarted()), this, SLOT(updateStarted()));
    connect(interpreter, SIGNAL(updateFinished(bool)), this, SLOT(updateFinished(bool)));
    connect(interpreter, SIGNAL(updateReversionStarted()), this, SLOT(updateReversionStarted()));
    connect(interpreter, SIGNAL(updateReversionFinished(bool)), this, SLOT(updateReversionFinished(bool)));
    connect(interpreter, SIGNAL(fileMoveFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(fileRemovalFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(fileCopyFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(filePatchFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(dirMakeFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(dirRemoveFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(dirCopyFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(dirRestoredFinished(bool)), this, SLOT(commandComplete(bool)));
    connect(interpreter, SIGNAL(updateScriptParsed(bool)), this, SLOT(scriptParseComplete(bool)));
    connect(interpreter, SIGNAL(fatalError(QString)), this, SLOT(fatalError(QString)));

    // Init ui
    ui->lblDetails->setText("Apply Update to begin.");
    ui->prbProgress->setValue(0);
    ui->lstDetails->setVisible(false);
    setMinimumHeight(91);
    setMaximumHeight(91);
    resize(width(), 91);
}

void LePatchDlg::setVersion(const QString &version)
{
    if (interpreter)
        interpreter->setVersion(version);
}

void LePatchDlg::setUpdateScript(const QString &script)
{
    if (interpreter)
        interpreter->setUpdateScript(script);
}

void LePatchDlg::scriptParseComplete(bool error)
{
    if (error)
        printLn("Error reading update script.");
    else
        printLn("Update script parsed successfully.");
}

void LePatchDlg::initLogFile()
{
    if (logFile.exists() && logFile.isOpen())
        logFile.close();
    logFile.open(QFile::WriteOnly | QFile::Text);
}

void LePatchDlg::applyUpdate()
{
    if (!interpreter)
        return;
    if (!updating) {
        initLogFile();
        ui->prbProgress->setValue(0);
        ui->lstDetails->clear();
        interpreter->applyUpdate();
    } else {
        interpreter->cancelUpdate();
    }
}

void LePatchDlg::setCommandCount(int count)
{
    if (count > 0) {
        ui->prbProgress->setValue(0);
        ui->prbProgress->setRange(0, count);
    }
}

void LePatchDlg::setCommandsRemaining(int count)
{
    ui->prbProgress->setValue(ui->prbProgress->maximum()-count);
}

void LePatchDlg::toggleDetails()
{
    ui->lstDetails->setVisible(!ui->lstDetails->isVisible());

    if (ui->lstDetails->isVisible()) {
        setMinimumHeight(300);
        setMaximumHeight(300);
        resize(width(), 300);
        ui->pbDetails->setText("Hide Details");
    } else {
        setMinimumHeight(91);
        setMaximumHeight(91);
        resize(width(), 91);
        ui->pbDetails->setText("Show Details");
    }
}

void LePatchDlg::print(const QString &s, bool append)
{
    if (!append) {
        ui->lblDetails->setText(s);
        ui->lstDetails->addItem(s);
    } else {
        int count = ui->lstDetails->count();

        if (count)
            ui->lstDetails->item(count-1)->setText(ui->lstDetails->item(count-1)->text() + s);
        else
            ui->lstDetails->addItem(s);
        ui->lblDetails->setText(ui->lblDetails->text() + s);
    }
    ui->lstDetails->scrollToBottom();

    if (logFile.isOpen())
        logFile.write(s.toAscii());
}

void LePatchDlg::printLn(const QString &s, bool append)
{
    print(s, append);

    if (logFile.isOpen())
        logFile.write(QString("\n").toAscii());
}

void LePatchDlg::fileMoveStarted(QString from, QString to)
{
    print("Moving file " + from + " to " + to + "...");
}

void LePatchDlg::fileRemovalStarted(QString fileName)
{
    print("Removing file " + fileName + "...");
}

void LePatchDlg::fileCopyStarted(QString from, QString to)
{
    print("Copying file " + from + " to " + to + "...");
}

void LePatchDlg::filePatchStarted(QString fileName)
{
    print("Patching file " + fileName + "...");
}

void LePatchDlg::dirMakeStarted(QString dirName)
{
    print("Making dir " + dirName + "...");
}

void LePatchDlg::dirRemoveStarted(QString dirName)
{
    print("Removing dir " + dirName + "...");
}

void LePatchDlg::dirCopyStarted(QString from, QString to)
{
    print("Copying dir " + from + " to " + to + "...");
}

void LePatchDlg::dirRestoreStarted(QString dirName)
{
    print("Restoring dir " + dirName + "...");
}

void LePatchDlg::updateStarted()
{
    ui->pbExit->setDisabled(true);
    ui->pbApplyUpdate->setText("Cancel Update");
    updating = true;
    printLn("Update started...");
}

void LePatchDlg::updateFinished(bool error)
{
    if (error) {
        printLn("Update failed.");
    } else {
        ui->pbApplyUpdate->setDisabled(true);
        printLn("Update applied successfully. Exit to close and launch LePlatz.");
    }
    updating = false;

    if (logFile.isOpen())
        logFile.close();
    ui->pbExit->setEnabled(true);
}

void LePatchDlg::updateReversionStarted()
{
    ui->pbApplyUpdate->setDisabled(true);
    printLn("Restoring previous version...");
}

void LePatchDlg::updateReversionFinished(bool error)
{
    if (error) {
        printLn("Restoration could not complete. Download latest program version if problems persist.");
    } else {
        printLn("Restored to previous version successfully. Update aborted.");
        ui->pbApplyUpdate->setText("Apply Update");
        ui->pbApplyUpdate->setEnabled(true);
        updating = false;
    }
}

void LePatchDlg::commandComplete(bool error)
{
    printLn((error)?"failed":"done", true);
}

void LePatchDlg::fatalError(QString msg)
{
    QMessageBox msgbox("Error", msg, QMessageBox::Critical,
                       QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton, this);
    msgbox.exec();
    exit(1);
}

void LePatchDlg::exitProgram()
{
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    QProcess::startDetached(QCoreApplication::applicationDirPath() + "/LePlatz.exe");
#else
    QProcess::startDetached(QCoreApplication::applicationDirPath() + "/LePlatz");
#endif
    close();
}

LePatchDlg::~LePatchDlg()
{
    if (logFile.isOpen())
        logFile.close();
    delete interpreter;
    delete ui;
}

