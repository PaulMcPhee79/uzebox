
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

    ui->cboVideoMode->addItem("Mode 2 (192x208)", QVariant(QSize(192, 208)));
    ui->cboVideoMode->addItem("Mode 3 (256x224)", QVariant(QSize(256, 224)));
    connect(ui->cboVideoMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSpinBoxRanges(int)));
    ui->spbSliceHeight->setValue(Settings::VMODE3_SCREEN_TILES_V);
    ui->cboVideoMode->setCurrentIndex(1);

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
    connect(ui->cboVideoMode, SIGNAL(receivedFocus()), this, SLOT(displayVideoModeHelp()));
    connect(ui->cboVideoMode, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->spbOffsetY, SIGNAL(valueChanged(int)), this, SLOT(syncSliceHeight(int)));
    connect(ui->spbOffsetY, SIGNAL(receivedFocus()), this, SLOT(displayOffsetYHelp()));
    connect(ui->spbOffsetY, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    connect(ui->spbSliceHeight, SIGNAL(valueChanged(int)), this, SLOT(syncOffsetY(int)));
    connect(ui->spbSliceHeight, SIGNAL(receivedFocus()), this, SLOT(displaySliceHeightHelp()));
    connect(ui->spbSliceHeight, SIGNAL(focusLost()), this, SLOT(displayDefaultHelp()));
    ui->leProjectName->setFocus();
}

void NewProject::syncSliceHeight(int offsetY)
{
    int vTiles = (ui->cboVideoMode->currentIndex() == 0) ? Settings::VMODE2_SCREEN_TILES_V : Settings::VMODE3_SCREEN_TILES_V;
    int sliceHeight = ui->spbSliceHeight->value();

    if ((offsetY + sliceHeight) > vTiles)
        ui->spbSliceHeight->setValue(vTiles-offsetY);
    updateVideoModeRes();
}

void NewProject::syncOffsetY(int sliceHeight)
{
    int vTiles = (ui->cboVideoMode->currentIndex() == 0) ? Settings::VMODE2_SCREEN_TILES_V : Settings::VMODE3_SCREEN_TILES_V;
    int offsetY = ui->spbOffsetY->value();

    if ((offsetY + sliceHeight) > vTiles)
        ui->spbOffsetY->setValue(vTiles-sliceHeight);
    updateVideoModeRes();
}

void NewProject::updateVideoModeRes()
{
    Q_ASSERT(ui->cboVideoMode->count() > 1);
    int sliceHeight = ui->spbSliceHeight->value();
    ui->cboVideoMode->setItemText(0, QString("Mode 2 (192x%1)").arg(qMin(208,8*sliceHeight)));
    ui->cboVideoMode->setItemText(1, QString("Mode 3 (256x%1)").arg(8*sliceHeight));
}

void NewProject::setSpinBoxRanges(int index)
{
    if (ui->cboVideoMode->count() > index) {
        if (index == 0) {
            ui->spbOffsetY->setValue(qMin(ui->spbOffsetY->value(), settings->VMODE2_SCREEN_TILES_V));
            ui->spbOffsetY->setMaximum(settings->VMODE2_SCREEN_TILES_V);
            ui->spbSliceHeight->setValue(qMin(ui->spbSliceHeight->value(), settings->VMODE2_SCREEN_TILES_V));
            ui->spbSliceHeight->setMaximum(settings->VMODE2_SCREEN_TILES_V);
            syncOffsetY(ui->spbSliceHeight->value());   // In case of no value change signal, ensure rules are still enforced
        } else {
            ui->spbOffsetY->setValue(qMin(ui->spbOffsetY->value(), settings->VMODE3_SCREEN_TILES_V));
            ui->spbOffsetY->setMaximum(settings->VMODE3_SCREEN_TILES_V);
            ui->spbSliceHeight->setValue(qMin(ui->spbSliceHeight->value(), settings->VMODE3_SCREEN_TILES_V));
            ui->spbSliceHeight->setMaximum(settings->VMODE3_SCREEN_TILES_V);
            syncOffsetY(ui->spbSliceHeight->value());   // In case of no value change signal, ensure rules are still enforced
        }
    }
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

void NewProject::displayVideoModeHelp()
{
    ui->lblContextualHelp->setText("Resolution represents vram array horizontally, and screen tiles vertically (both converted to pixels). "
                                   "This is because only horizontal scrolling is currently supported.");
}

void NewProject::displaySliceHeightHelp()
{
    ui->lblContextualHelp->setText("The primary direction of travel within your game world. Platz will optimize collision detection and drawing "
                                   "routines based on this setting. Individual slices may be altered through the project settings dialog.");
}

void NewProject::displayOffsetYHelp()
{
    ui->lblContextualHelp->setText("Y-axis offset refers to screen regions that appear above your slices (Overlay Lines in Mode 3, and Screen Sections in mode 2). "
                                   "Increasing this value will reduce the vertical region of your slices on which Platz will draw.");
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

    settings->setVideoMode(ui->cboVideoMode->currentIndex()+2);

    QSize sliceSize = ui->cboVideoMode->itemData(ui->cboVideoMode->currentIndex(), Qt::UserRole).toSize();

    sliceSize.setHeight(ui->spbSliceHeight->value()*8);
    settings->setProjectPath(ui->leProjectFolder->text() + "/" + projectName);
    settings->setImageFormat(ui->cboImageFormat->currentText());
    settings->setArtFolder(ui->leArtFolder->text());
    settings->setRelativeSrcFolder(ui->leSrcFolder->text());
    settings->setSliceSize(sliceSize);
    settings->setOffsetY(ui->spbOffsetY->value()*8);
    accept();
}

NewProject::~NewProject()
{
    delete ui;
}

