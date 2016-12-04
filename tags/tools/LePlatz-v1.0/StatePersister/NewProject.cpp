
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

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <PlatzEnums.h>
#include "NewProject.h"
#include "ui_NewProject.h"

using namespace Platz;

NewProject::NewProject(Settings *settings, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::NewProject), settings(settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    // Initial settings
    ui->lblLogoImg->setPixmap(QPixmap(":/misc/LePlatzLogo.png"));
    ui->lblContextualHelp->setText("");

    if (!settings->sliceSize().isEmpty() && settings->sliceSize().isValid()) {
        ui->leSliceWidth->setText(QString::number(settings->sliceSize().width()));
        ui->leSliceHeight->setText(QString::number(settings->sliceSize().height()));
    } else {
        ui->leSliceWidth->setText("256");
        ui->leSliceHeight->setText("224");
    }

    if (!settings->spriteSize().isEmpty() && settings->spriteSize().isValid()) {
        ui->leSpriteWidth->setText(QString::number(settings->spriteSize().width()));
        ui->leSpriteHeight->setText(QString::number(settings->spriteSize().height()));
    } else {
        ui->leSpriteWidth->setText("24");
        ui->leSpriteHeight->setText("24");
    }

    ui->leSliceWidth->setValidator(new QIntValidator(0, 256, ui->leSliceWidth));
    ui->leSliceHeight->setValidator(new QIntValidator(0, 256, ui->leSliceHeight));
    ui->leSpriteWidth->setValidator(new QIntValidator(0, 256, ui->leSpriteWidth));
    ui->leSpriteHeight->setValidator(new QIntValidator(0, 256, ui->leSpriteHeight));
    ui->cboImageFormat->addItems(Platz::SUPPORTED_IMAGE_FORMATS_EXTS);
    ui->cboImageFormat->setCurrentIndex(ui->cboImageFormat->count()-1);
    displayDefaultHelp();

    // Connections
    connect(ui->pbArtPath, SIGNAL(pressed()), this, SLOT(artFolderFileDialog()));
    connect(ui->pbSrcPath, SIGNAL(pressed()), this, SLOT(srcFolderFileDialog()));
    connect(ui->pbProjectPath, SIGNAL(pressed()), this, SLOT(projectFolderFileDialog()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(createNewProject()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->leProjectName, SIGNAL(receivedFocus()), this, SLOT(displayProjectNameHelp()));
    connect(ui->leProjectName, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leProjectFolder, SIGNAL(receivedFocus()), this, SLOT(displayProjectFolderHelp()));
    connect(ui->leProjectFolder, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leArtFolder, SIGNAL(receivedFocus()), this, SLOT(displayArtFolderHelp()));
    connect(ui->leArtFolder, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leSrcFolder, SIGNAL(receivedFocus()), this, SLOT(displaySrcFolderHelp()));
    connect(ui->leSrcFolder, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leSliceWidth, SIGNAL(receivedFocus()), this, SLOT(displaySliceWidthHelp()));
    connect(ui->leSliceWidth, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leSliceHeight, SIGNAL(receivedFocus()), this, SLOT(displaySliceHeightHelp()));
    connect(ui->leSliceHeight, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leSpriteWidth, SIGNAL(receivedFocus()), this, SLOT(displaySpriteWidthHelp()));
    connect(ui->leSpriteWidth, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->leSpriteHeight, SIGNAL(receivedFocus()), this, SLOT(displaySpriteHeightHelp()));
    connect(ui->leSpriteHeight, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));

    ui->leProjectName->setFocus();
}

void NewProject::displayDefaultHelp()
{
    ui->lblContextualHelp->setText("All settings may be changed and refined after the project has been created. "
                                   "LePlatz assumes a default project path to simplify project creation.");
}

void NewProject::displayProjectNameHelp()
{
    ui->lblContextualHelp->setText("The project will be saved as an xml file under this name. By default, "
                                   "the hex file will be considered to exist under this same name.");
}

void NewProject::displayProjectFolderHelp()
{
    ui->lblContextualHelp->setText("This folder that will house your LePlatz project file. This should have write "
                                   "permissions enabled for the intended users.");
}

void NewProject::displayArtFolderHelp()
{
    ui->lblContextualHelp->setText("To enable default settings, this folder should contain:\n"
                                   "./Slices/\t./tiles" + ui->cboImageFormat->currentText() +
                                   "\n./maps" + ui->cboImageFormat->currentText() +
                                   "\t./animations" + ui->cboImageFormat->currentText());
}

void NewProject::displaySrcFolderHelp()
{
    QString projectName = ui->leProjectName->text(), xmlStr = ".xml";

    if (projectName.endsWith(xmlStr))
        projectName.chop(xmlStr.length());
    ui->lblContextualHelp->setText("To enable default settings, this folder should contain:\n"
                                   "./<Required .c & .h files>\t./data/\n"
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
                                   "./default/Makefile\t./default/" + projectName + ".hex");
#else
                                    "./default/Makefile\t\t./default/" + projectName + ".hex");
#endif
}

void NewProject::displaySliceWidthHelp()
{
    ui->lblContextualHelp->setText("The width (in pixels) of each slice. Slices provided to LePlatz for display "
                                   "as a background canvas should have widths a multiple of this value");
}

void NewProject::displaySliceHeightHelp()
{
    ui->lblContextualHelp->setText("The height (in pixels) of each slice. Slices provided to LePlatz for display "
                                   "as a background canvas should have heights of this value.");
}

void NewProject::displaySpriteWidthHelp()
{
    ui->lblContextualHelp->setText("The width (in pixels) of the largest sprite for which collision detection is required. This "
                                   "value is used to minimize collision detection overhead when crossing slice boundaries.");
}

void NewProject::displaySpriteHeightHelp()
{
    ui->lblContextualHelp->setText("The height (in pixels) of the largest sprite for which collision detection is required. This "
                                   "value is used to minimize collision detection overhead when crossing slice boundaries.");
}

QString NewProject::folderFileDialog(const QString &initPath, const QString &title)
{
    QDir dir(initPath);

    if (!dir.exists())
        dir.setPath(QDir::currentPath());
    return QFileDialog::getExistingDirectory(this, title, dir.absolutePath());
}

void NewProject::projectFolderFileDialog()
{
    QString path = folderFileDialog(ui->leProjectFolder->text(), "LePlatz Project Path");

    if (!path.isEmpty())
        ui->leProjectFolder->setText(path);
}

void NewProject::artFolderFileDialog()
{
    QString path;

    if (!ui->leArtFolder->text().isEmpty())
        path = folderFileDialog(ui->leArtFolder->text(), "LePlatz Art Path");
    else
        path = folderFileDialog(ui->leProjectFolder->text(), "LePlatz Art Path");

    if (!path.isEmpty())
        ui->leArtFolder->setText(path);
}

void NewProject::srcFolderFileDialog()
{
    QString path;

    if (!ui->leSrcFolder->text().isEmpty())
        path = folderFileDialog(ui->leSrcFolder->text(), "LePlatz Source Path");
    else
        path = folderFileDialog(ui->leProjectFolder->text(), "LePlatz Source Path");

    if (!path.isEmpty())
        ui->leSrcFolder->setText(path);
}

bool NewProject::validateInput()
{
    QDir dir(ui->leProjectFolder->text());

    if (ui->leProjectFolder->text().isEmpty() || !dir.exists()) {
        ui->lblContextualHelp->setText("Valid Project Folder required.");
        return false;
    }
    QString projectName = ui->leProjectName->text().trimmed(), xml = ".xml";

    if (projectName.endsWith(xml))
        projectName.chop(xml.length());
    // Some of the Windows OS reserved words probably don't matter due to adding .xml suffix
    // but in case that is removed for whatever reason, better to avoid them
    QRegExp regexp("(^(?=(PRN|AUX|CLOCK\\$|NUL|CON|COM\\d{1,2}|LPT\\d{1,2})?$)|[\\/:*?\"<>|])", Qt::CaseInsensitive);

    if (projectName.contains(regexp)) {
        ui->lblContextualHelp->setText("Project Name must be at least one character in length and cannot contain any of the following characters: "
                                       " \\ / : * ? \" < > | or common OS reserved words.");
        return false;
    }
    return true;
}

void NewProject::createNewProject()
{
    if (!validateInput())
        return;
    QString projectName = ui->leProjectName->text().trimmed();

    if (!projectName.endsWith(".xml"))
        projectName += ".xml";
    QString projectPath = ui->leProjectFolder->text() + "/" + projectName;

    if (QDir::isRelativePath(projectPath)) {
        ui->lblContextualHelp->setText("Project Path must be absolute.");
        return;
    }

    if (QFile::exists(projectPath)) {
        QMessageBox msgbox("Overwrite existing file?", "A file with the name " + projectName + " already exists in the specified directory. "
                           "Overwrite this file?", QMessageBox::Question, QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
        int ret = msgbox.exec();

        if (ret == QMessageBox::Cancel)
            return;
    }

    settings->setProjectPath(ui->leProjectFolder->text() + "/" + projectName);
    settings->setImageFormat(ui->cboImageFormat->currentText());
    settings->setArtFolder(ui->leArtFolder->text());
    settings->setRelativeSrcFolder(ui->leSrcFolder->text());
    settings->setSliceSize(ui->leSliceWidth->text().toInt(), ui->leSliceHeight->text().toInt());
    settings->setSpriteSize(ui->leSpriteWidth->text().toInt(), ui->leSpriteHeight->text().toInt());
    accept();
}

NewProject::~NewProject()
{
    delete ui;
}

