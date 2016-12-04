
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
#include <ui_PlatzSettings.h>
#include <PlatzEnums.h>
#include "Settings.h"
#include "SettingsDialog.h"

using namespace Platz;

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::SettingsDialog), settings(settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    // Initial settings
    ui->splitter->setSizes(QList<int>() << 140 << 360);
    ui->lePsSliceWidth->setValidator(new QIntValidator(0, 256, ui->lePsSliceWidth));
    ui->lePsSliceHeight->setValidator(new QIntValidator(0, 256, ui->lePsSliceHeight));
    ui->lePsSpriteWidth->setValidator(new QIntValidator(0, 256, ui->lePsSpriteWidth));
    ui->lePsSpriteHeight->setValidator(new QIntValidator(0, 256, ui->lePsSpriteHeight));

    // Populate trees
    QTreeWidgetItem *projectSettings = new QTreeWidgetItem(ui->treeWidget);
    projectSettings->setText(0, "Project Settings");
    QTreeWidgetItem *lePlatzSettings = new QTreeWidgetItem(ui->treeWidget);
    lePlatzSettings->setText(0, "LePlatz Settings");
    ui->twPs->setHeaderLabels(QStringList() << QString("Name") << QString("Value"));
    ui->twPs->header()->resizeSection(0, 140);
    ui->twLps->setHeaderLabels(QStringList() << QString("Name") << QString("Value"));
    ui->twLps->header()->resizeSection(0, 140);
    QList<QTreeWidgetItem*> items;
    items.append(QList<QTreeWidgetItem*>() << new QTreeWidgetItem(QStringList() << QString("Slice Size") << QString("Width: %1\tHeight: %2").arg(
            settings->sliceSize().width()).arg(
            settings->sliceSize().height()))
            << new QTreeWidgetItem(QStringList() << QString("Largest Sprite") << QString("Width: %1\tHeight: %2").arg(
            settings->spriteSize().width()).arg(settings->spriteSize().height()))
            << new QTreeWidgetItem(QStringList() << QString("Slice Path") << settings->slicePath())
            << new QTreeWidgetItem(QStringList() << QString("Tile Path") << settings->tilePath())
            << new QTreeWidgetItem(QStringList() << QString("Map Path") << settings->mapPath())
            << new QTreeWidgetItem(QStringList() << QString("Anim Path") << settings->animPath())
            << new QTreeWidgetItem(QStringList() << QString("Platz Path") << settings->platzfilePath())
            << new QTreeWidgetItem(QStringList() << QString("Source Folder") << settings->srcFolder())
            << new QTreeWidgetItem(QStringList() << QString("Makefile Path") << settings->makefilePath())
            << new QTreeWidgetItem(QStringList() << QString("Hex Path") << settings->hexfilePath())
    );
    ui->twPs->insertTopLevelItems(0, items);
    items.clear();
    items.append(QList<QTreeWidgetItem*>() << new QTreeWidgetItem(QStringList() << QString("Make Path/Cmd") << settings->makeExePath())
            << new QTreeWidgetItem(QStringList() << QString("Emulator Path") << settings->emuExePath())
    );
    ui->twLps->insertTopLevelItems(0, items);

    // Populate stacked widgets
    ui->lePsSliceWidth->setText(QString::number(settings->sliceSize().width()));
    ui->lePsSliceHeight->setText(QString::number(settings->sliceSize().height()));
    ui->lePsSpriteWidth->setText(QString::number(settings->spriteSize().width()));
    ui->lePsSpriteHeight->setText(QString::number(settings->spriteSize().height()));
    ui->leSlicePath->setText(settings->slicePath());
    ui->leTilePath->setText(settings->tilePath());
    ui->leMapPath->setText(settings->mapPath());
    ui->leAnimPath->setText(settings->animPath());
    ui->lePlatzPath->setText(settings->platzfilePath());
    ui->leSrcFolder->setText(settings->srcFolder());
    ui->leMakefilePath->setText(settings->makefilePath());
    ui->leHexfilePath->setText(settings->hexfilePath());
    ui->leMakeExePath->setText(settings->makeExePath());
    ui->leEmuExePath->setText(settings->emuExePath());

    // Hook everything up
    connect(ui->pbApply, SIGNAL(pressed()), this, SLOT(applySettings()));
    connect(ui->pbOk, SIGNAL(pressed()), this, SLOT(okSettings()));
    connect(ui->pbCancel, SIGNAL(pressed()), this, SLOT(reject()));
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(settingSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->twPs, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(settingPsItemSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->twLps, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(settingLpsItemSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    // Tree updates
    connect(ui->lePsSliceWidth, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->lePsSliceHeight, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->lePsSpriteWidth, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->lePsSpriteHeight, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leSlicePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leTilePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leMapPath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leAnimPath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->lePlatzPath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leSrcFolder, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leMakefilePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leHexfilePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leMakeExePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    connect(ui->leEmuExePath, SIGNAL(textChanged(QString)), this, SLOT(updateTrees(QString)));
    // File dialogs
    connect(ui->pbSlicePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbTilePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbMapPath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbAnimPath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbPlatzPath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbSrcFolder, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbMakefilePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbHexfilePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbMakeExePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
    connect(ui->pbEmuExePath, SIGNAL(pressed()), this, SLOT(pathFileDialog()));
}

// Qt bug fix for 4.5.4 should fix QTreeWidget not updating properly when QTreeWidgetItem.setText() is called on index = 0
void SettingsDialog::updateTrees(const QString &text)
{
    int twIndex = 0, itemIndex = 0;
    QTreeWidget *tree = 0;
    
    if (ui->stackedWidget->currentIndex() == 0) {
        tree = ui->twPs;
        twIndex = 0;
        itemIndex = ui->swPs->currentIndex();
    } else {
        tree = ui->twLps;
        twIndex = 1;
        itemIndex = ui->swLps->currentIndex();
    }

    QTreeWidgetItem *item = tree->topLevelItem(itemIndex);

    if (item) {
        if (twIndex == 0 && itemIndex == 0)
            item->setText(1, QString("Width: %1\tHeight: %2").arg(ui->lePsSliceWidth->text()).arg(ui->lePsSliceHeight->text()));
        else if (twIndex == 0 && itemIndex == 1)
            item->setText(1, QString("Width: %1\tHeight: %2").arg(ui->lePsSpriteWidth->text()).arg(ui->lePsSpriteHeight->text()));
        else
            item->setText(1, text);
    }
}

void SettingsDialog::pathFileDialog()
{
    PathType type = FilePath;
    QString openPath, filter;
    QLineEdit *le = 0;

    if (ui->stackedWidget->currentIndex() == 0) {
        switch (ui->swPs->currentIndex()) {
            case 2:
                le = ui->leSlicePath;
                openPath = settings->slicePath();
                type = DirPath;
                break;
            case 3:
                le = ui->leTilePath;
                openPath = settings->tilePath();
                filter = SUPPORTED_IMAGE_FORMATS_STR;
                break;
            case 4:
                le = ui->leMapPath;
                openPath = settings->mapPath();
                filter = SUPPORTED_IMAGE_FORMATS_STR;
                break;
            case 5:
                le = ui->leAnimPath;
                openPath = settings->animPath();
                filter = SUPPORTED_IMAGE_FORMATS_STR;
                break;
            case 6:
                le = ui->lePlatzPath;
                openPath = settings->platzfilePath();
                filter = PLATZ_INCLUDE_FORMATS_STR;
                break;
            case 7:
                le = ui->leSrcFolder;
                openPath = settings->srcFolder();
                type = DirPath;
                break;
            case 8:
                le = ui->leMakefilePath;
                openPath = settings->makefilePath();
                filter = "*";
                break;
            case 9:
                le = ui->leHexfilePath;
                openPath = settings->hexfilePath();
                filter = INTEL_HEX_FILES_STR;
                break;
            default:
                break;
        }
    } else {
        switch (ui->swLps->currentIndex()) {
            case 0:
                le = ui->leMakeExePath;
                openPath = settings->makeExePath();
                filter = SUPPORTED_EXE_FORMATS_STR;
                break;
            case 1:
                le = ui->leEmuExePath;
                openPath = settings->emuExePath();
                filter = SUPPORTED_EXE_FORMATS_STR;
                break;
            default:
                break;
        }
    }

    if (le) {
        QString path = le->text();
        showSettingsDialog(path, openPath, type, filter);
        le->setText(path);
    }
}

void SettingsDialog::showSettingsDialog(QString &path, QString openPath, const PathType &type, const QString &filter)
{
    QString fileName;

    if (type == FilePath) {
        if (!QFile::exists(openPath))
            openPath = QDir::currentPath();
        fileName = QFileDialog::getOpenFileName(this, tr("LePlatz supported formats"), openPath, filter);
    } else {
        QDir dir(openPath);
        if (!dir.exists())
            dir.setPath(QDir::currentPath());
        fileName = QFileDialog::getExistingDirectory(this, tr("LePlatz settings path"), dir.absolutePath());
    }
    if (fileName.isEmpty())
        return;
    path = fileName;
}

void SettingsDialog::okSettings()
{
    applySettings();
    accept();
}

void SettingsDialog::applySettings()
{
    bool ok;
    int wid, hgt;

    wid = ui->lePsSliceWidth->text().toInt(&ok);
    if (ok)
        hgt = ui->lePsSliceHeight->text().toInt(&ok);
    if (ok)
        settings->setSliceSize(wid, hgt);
    wid = ui->lePsSpriteWidth->text().toInt(&ok);
    if (ok)
        hgt = ui->lePsSpriteHeight->text().toInt(&ok);
    if (ok)
        settings->setSpriteSize(wid, hgt);
    settings->setSlicePath(ui->leSlicePath->text());
    settings->setTilePath(ui->leTilePath->text());
    settings->setMapPath(ui->leMapPath->text());
    settings->setAnimPath(ui->leAnimPath->text());
    settings->setSrcFolder(ui->leSrcFolder->text());
    settings->setMakefilePath(ui->leMakefilePath->text());
    settings->setHexfilePath(ui->leHexfilePath->text());
    settings->setPlatzfilePath(ui->lePlatzPath->text());
    settings->setEmuExePath(ui->leEmuExePath->text());
    settings->setMakeExePath(ui->leMakeExePath->text());
}

void SettingsDialog::settingSelected(QTreeWidgetItem* current, QTreeWidgetItem*)
{
    if (!current)
        return;
    if (current->text(0) == "Project Settings")
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}

void SettingsDialog::settingPsItemSelected(QTreeWidgetItem *current, QTreeWidgetItem*)
{
    if (current)
        ui->swPs->setCurrentIndex(ui->twPs->indexOfTopLevelItem(current));
}

void SettingsDialog::settingLpsItemSelected(QTreeWidgetItem *current, QTreeWidgetItem*)
{
    if (current)
        ui->swLps->setCurrentIndex(ui->twLps->indexOfTopLevelItem(current));
}

SettingsDialog::~SettingsDialog()
{
    if (ui) {
        ui->treeWidget->clear();
        ui->twLps->clear();
        ui->twPs->clear();
        delete ui;
    }
}
