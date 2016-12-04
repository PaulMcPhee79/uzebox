
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

#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPointer>
#ifdef LEPLATZ_DEBUG
    #include <modeltest.h>
#endif
#include <WorldCompiler.h>
#include <PlatzTreeView.h>
#include <PlatzGraphicsView.h>
#include <PlatzGraphicsScene.h>
#include <PlatzGraphicsList.h>
#include <WorldItem.h>
#include <PlatzWriter.h>
#include <PlatzReader.h>
#include <NewProject.h>
#include <SettingsDialog.h>
#include <SourceParser.h>
#include <About.h>
#include "PlatzWin.h"
#include "ui_PlatzWin.h"

static const int STATUS_DELAY = 10000;

PlatzWin::PlatzWin(const QString &cmdLineProject, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PlatzWin), procMake(new QProcess(this)), procEmu(new QProcess(this)),
        settings(new Settings(MAX_RECENT_PROJECTS)), projectPath(QDir::currentPath()), activeProject(false),
        recProjMenu(0), bgoTrigger(false), unsavedChanges(false)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icons/128x128/mp.png"));
    WorldItem::loadWorldItemIcons();
    WorldItem::loadWorldDetailPixmaps();

    // Misc ui settings
    ui->actionNew->setIcon(QIcon(":/icons/128x128/new.png"));
    ui->actionOpen->setIcon(QIcon(":/icons/128x128/open.png"));
    ui->actionSave->setIcon(QIcon(":/icons/128x128/save.png"));
    ui->actionSaveAs->setIcon(QIcon(":/icons/128x128/save.png"));
    ui->actionReload->setIcon(QIcon(":/icons/128x128/reloadWorld.png"));
    ui->actionSettings->setIcon(QIcon(":/icons/128x128/settings.png"));
    ui->actionZoomIn->setIcon(QIcon(":/icons/128x128/zoomIn.png"));
    ui->actionZoomOut->setIcon(QIcon(":/icons/128x128/zoomOut.png"));
    ui->actionAdd->setIcon(QIcon(":/icons/128x128/add.png"));
    ui->actionRemove->setIcon(QIcon(":/icons/128x128/remove.png"));
    ui->actionDelete->setIcon(QIcon(":/icons/128x128/remove.png"));
    ui->actionPrevious->setIcon(QIcon(":/icons/128x128/previous.png"));
    ui->actionNext->setIcon(QIcon(":/icons/128x128/next.png"));
    ui->actionLock->setIcon(QIcon(":/icons/128x128/lock.png"));
    ui->actionReplicate->setIcon(QIcon(":/icons/128x128/slice.png"));
    ui->actionCompile->setIcon(QIcon(":/icons/128x128/compile.png"));
    ui->actionMake->setIcon(QIcon(":/icons/128x128/build.png"));
    ui->actionEmulator->setIcon(QIcon(":/icons/128x128/emulator.png"));
    ui->actionClose->setIcon(QIcon(":/icons/128x128/close.png"));
    ui->actionExit->setIcon(QIcon(":/icons/128x128/exit.png"));
    ui->actionUndo->setIcon(QIcon(":/icons/128x128/undo.png"));
    ui->actionRedo->setIcon(QIcon(":/icons/128x128/redo.png"));
    ui->actionCut->setIcon(QIcon(":/icons/128x128/cut.png"));
    ui->actionCopy->setIcon(QIcon(":/icons/128x128/copy.png"));
    ui->actionPaste->setIcon(QIcon(":/icons/128x128/paste.png"));
    ui->actionSelectAll->setIcon(QIcon(":/icons/128x128/selectAll.png"));
    ui->actionAboutLePlatz->setIcon(QIcon(":/icons/128x128/mp.png"));
    ui->actionAboutPlatz->setIcon(QIcon(":/icons/128x128/aboutPlatz.png"));
    ui->actionAboutUzebox->setIcon(QIcon(":/misc/uzebox.png"));
    ui->actionSelectCursor->setIcon(QIcon(":/icons/128x128/pointer.png"));
    ui->actionBgOuterCursor->setIcon(QIcon(":/icons/128x128/bgo.png"));
    ui->actionBgInnerCursor->setIcon(QIcon(":/icons/128x128/bgi.png"));
    ui->actionBgObjectCursor->setIcon(QIcon(":/icons/128x128/object.png"));
    ui->actionPlatformPathCursor->setIcon(QIcon(":/icons/128x128/mpPath.png"));
    ui->actionPlatformCursor->setIcon(QIcon(":/icons/128x128/mp.png"));
    ui->actionBgMutableCursor->setIcon(QIcon(":/icons/128x128/bgm.png"));
    ui->chkBgmi->setEnabled(false);
    ui->leMutLeft->setValidator(new QIntValidator(0, 255, ui->leMutLeft));
    ui->leMutRight->setValidator(new QIntValidator(0, 255, ui->leMutRight));
    ui->leMutTop->setValidator(new QIntValidator(0, 255, ui->leMutTop));
    ui->leMutBottom->setValidator(new QIntValidator(0, 255, ui->leMutBottom));
    ui->leMutLeft->setEnabled(false);
    ui->leMutRight->setEnabled(false);
    ui->leMutTop->setEnabled(false);
    ui->leMutBottom->setEnabled(false);
    ui->dockWidgetToolbox->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    ui->dockWidgetUtilities->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    ui->dockWidgetWorldDetails->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    ui->actionSelectCursor->setToolTip("Select background elements (H to toggle selection hiding)");
    ui->actionBgOuterCursor->setToolTip("Define new Outer BG");
    ui->actionBgInnerCursor->setToolTip("Define new Inner BG");
    ui->actionBgObjectCursor->setToolTip("Define new BG Object");
    ui->actionPlatformPathCursor->setToolTip("Define new Moving Platform Path");
    ui->actionPlatformCursor->setToolTip("Define new Moving Platform");
    ui->actionBgMutableCursor->setToolTip("Define new Mutable BG (must be tied to an existing Inner BG)");

    // Menus
    recProjMenu = new QMenu("Recent Projects", ui->menuFile);
    // Note: takeAt(2) because don't know how else to reference the separator
    ui->menuFile->insertMenu(ui->menuFile->actions().takeAt(2), recProjMenu);
    snapToGrp = new QActionGroup(this);
    snapToGrp->addAction(ui->actionSnapTo1);
    snapToGrp->addAction(ui->actionSnapTo2);
    snapToGrp->addAction(ui->actionSnapTo4);
    snapToGrp->addAction(ui->actionSnapTo8);
    ui->actionSnapTo8->setChecked(true);
    snapToGrp->setExclusive(true);
    connect(snapToGrp, SIGNAL(triggered(QAction*)), this, SLOT(setSnapToResolution(QAction*)));

    recProjActionGrp = new QActionGroup(this);

    for (int i = 0; i < settings->maxRecentProjects(); i++)
        recProjActionGrp->addAction("");
    connect(settings, SIGNAL(recentProjectsChanged(QStringList)), this, SLOT(updateRecentProjects(QStringList)));
    connect(recProjActionGrp, SIGNAL(triggered(QAction*)), this, SLOT(loadProject(QAction*)));

    // Init external process comms slots
    connect(procMake, SIGNAL(readyReadStandardOutput()), this, SLOT(makeOutputReady()));
    connect(procMake, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(makeFinished(int,QProcess::ExitStatus)));
    connect(procMake, SIGNAL(error(QProcess::ProcessError)), this, SLOT(makeError(QProcess::ProcessError)));
    connect(procEmu, SIGNAL(readyReadStandardOutput()), this, SLOT(emuOutputReady()));
    connect(procEmu, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(emuFinished(int,QProcess::ExitStatus)));
    connect(procEmu, SIGNAL(error(QProcess::ProcessError)), this, SLOT(emuError(QProcess::ProcessError)));

    // Init treeview
    model = new PlatzDataModel(ui->treeView);
#ifdef LEPLATZ_DEBUG
    new ModelTest(model, this);
#endif
    ui->treeView->setModel(model);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setWindowTitle(QObject::tr("Platz World Data"));
    ui->treeView->setUniformRowHeights(true);
    connect(model, SIGNAL(rowsRemoved(const QModelIndex,int,int)), this, SLOT(flagUnsavedChanges()));
    connect(model, SIGNAL(rowsInserted(const QModelIndex,int,int)), this, SLOT(flagUnsavedChanges()));

    // Init model/views
    ui->graphicsView->setModel(model);
    connect(ui->treeView, SIGNAL(selectedIndexChanged(QModelIndex)), model, SLOT(setSelectedIndex(QModelIndex)));
    connect(ui->graphicsView->scene(), SIGNAL(selectionChanged(const QModelIndex&)), model, SLOT(setSelectedIndex(QModelIndex)));
    connect(model, SIGNAL(selectedIndexChanged(QModelIndex)), ui->graphicsView, SLOT(frameItem(QModelIndex)));
    connect(model, SIGNAL(selectedIndexChanged(QModelIndex)), ui->graphicsView->scene(), SLOT(setSelection(const QModelIndex&)));
    connect(model, SIGNAL(selectedIndexChanged(QModelIndex)), ui->treeView, SLOT(setCurrentIndex(QModelIndex)));
    connect(model, SIGNAL(selectedIndexParentChanged(QModelIndex)), ui->treeView, SLOT(expand(QModelIndex)));
    //connect(model, SIGNAL(selectedIndexChanged(QModelIndex)), this, SLOT(updateDetailDataDisplay(QModelIndex)));
    connect(model, SIGNAL(currentIndexChanged(QModelIndex)), this, SLOT(updateDetailDataDisplay(QModelIndex)));

    connect(ui->graphicsView, SIGNAL(receivedDrop(BgInner::BgiFlags,int)), this, SLOT(flagUnsavedChanges()));
    connect(ui->graphicsView, SIGNAL(mouseCoordsChanged(QString)), ui->lblCoords, SLOT(setText(QString)));
    connect(ui->graphicsView, SIGNAL(sliceIndexChanged(QString)), ui->lblSliceIndex, SLOT(setText(QString)));
    connect(model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this, SLOT(updateDetailImageDisplay()));
    {
        PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());
        scene->initSceneDrawTools();
        scene->setSnapToResolution(8);
        connect(scene, SIGNAL(currentTileIndexChanged(int)), ui->tileListWidget, SLOT(setCurrentImage(int)));
        connect(scene, SIGNAL(currentMapIndexChanged(int)), ui->mapListWidget, SLOT(setCurrentImage(int)));
        connect(scene, SIGNAL(currentAnimIndexChanged(int)), ui->animListWidget, SLOT(setCurrentImage(int)));
        connect(scene, SIGNAL(currentItemDataChanged(const WorldItem*)), this, SLOT(updateDetailDataDisplay(const WorldItem*)));
        connect(ui->actionSelectAll, SIGNAL(triggered()), scene, SLOT(selectAll()));
    }

    // NOTE: Need to have a data model class managing the data for these two listWidgets (which will become QListViews). That way we
    // can emit the one signal and handle image comparisons internally.

    // ListWidgets
    ui->tileListWidget->setType(BgInner::BG0);
    ui->mapListWidget->setType(BgInner::BGP);
    ui->animListWidget->setType(BgInner::BGA);
    ui->mapListWidget->setDelimiterTile(QImage(":/misc/pixDelim.png"));
    ui->animListWidget->setDelimiterTile(QImage(":/misc/pixDelim.png"));
    connect(ui->graphicsView, SIGNAL(tileDrawn(WorldItem*, const QImage&)), ui->tileListWidget, SLOT(imageToMapIndex(WorldItem*, const QImage&)));
    connect(ui->graphicsView, SIGNAL(mapDrawn(WorldItem*, const QImage&)), ui->mapListWidget, SLOT(imageToMapIndex(WorldItem*, const QImage&)));
    connect(ui->graphicsView, SIGNAL(animDrawn(WorldItem*, const QImage&)), ui->animListWidget, SLOT(imageToMapIndex(WorldItem*, const QImage&)));
    connect(ui->tileListWidget, SIGNAL(imageMapped(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));
    connect(ui->mapListWidget, SIGNAL(imageMapped(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));
    connect(ui->animListWidget, SIGNAL(imageMapped(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));
    connect(ui->tileListWidget, SIGNAL(currentImageChanged(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));
    connect(ui->mapListWidget, SIGNAL(currentImageChanged(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));
    connect(ui->animListWidget, SIGNAL(currentImageChanged(QImage)), this, SLOT(updateDetailImageDisplay(QImage)));

    // Toolbar
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newProject()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(initSaveProject()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(initSaveAsProject()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(initLoadProject()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeProject()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(prepareToClose()));
    connect(ui->actionDelete, SIGNAL(triggered()), ui->graphicsView, SLOT(deleteSelected()));
    connect(ui->actionReload, SIGNAL(triggered()), this, SLOT(reloadResources()));
    connect(ui->actionCompile, SIGNAL(triggered()), this, SLOT(compileWorld()));
    connect(ui->actionMake, SIGNAL(triggered()), this, SLOT(buildSource()));
    connect(ui->actionEmulator, SIGNAL(triggered()), this, SLOT(playInEmulator()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), this, SIGNAL(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), this, SIGNAL(zoomOut()));
    connect(this, SIGNAL(zoomIn(int)), ui->graphicsView, SLOT(zoomView(int)));
    connect(this, SIGNAL(zoomOut(int)), ui->graphicsView, SLOT(zoomView(int)));
    connect(ui->actionPrevious, SIGNAL(triggered()), ui->graphicsView, SLOT(previousSlice()));
    connect(ui->actionNext, SIGNAL(triggered()), ui->graphicsView, SLOT(nextSlice()));
    connect(ui->actionLock, SIGNAL(triggered()), this, SLOT(toggleSelectedSliceLock()));
    connect(ui->actionReplicate, SIGNAL(triggered()), this, SLOT(replicateSlice()));
    connect(ui->actionCompile, SIGNAL(triggered()), this, SLOT(compileWorld()));
    connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(removeSlice()));
    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(insertSlice()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionAboutLePlatz, SIGNAL(triggered()), this, SLOT(aboutLePlatz()));
    connect(ui->actionAboutPlatz, SIGNAL(triggered()), this, SLOT(aboutPlatz()));
    connect(ui->actionAboutUzebox, SIGNAL(triggered()), this, SLOT(aboutUzebox()));

    // Toolbox
    ui->tbtnPointer->setDefaultAction(ui->actionSelectCursor);
    ui->tbtnBgo->setDefaultAction(ui->actionBgOuterCursor);
    ui->tbtnBgi->setDefaultAction(ui->actionBgInnerCursor);
    ui->tbtnObj->setDefaultAction(ui->actionBgObjectCursor);
    ui->tbtnMpp->setDefaultAction(ui->actionPlatformPathCursor);
    ui->tbtnMp->setDefaultAction(ui->actionPlatformCursor);
    ui->tbtnBgm->setDefaultAction(ui->actionBgMutableCursor);

    QActionGroup *toolboxActionGroup = new QActionGroup(this);
    toolboxActionGroup->addAction(ui->actionSelectCursor);
    toolboxActionGroup->addAction(ui->actionBgOuterCursor);
    toolboxActionGroup->addAction(ui->actionBgInnerCursor);
    toolboxActionGroup->addAction(ui->actionBgObjectCursor);
    toolboxActionGroup->addAction(ui->actionPlatformPathCursor);
    toolboxActionGroup->addAction(ui->actionPlatformCursor);
    toolboxActionGroup->addAction(ui->actionBgMutableCursor);
    connect(this, SIGNAL(imModeTriggered(QAction*)), toolboxActionGroup, SIGNAL(triggered(QAction*)));
    connect(toolboxActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(setInteractionMode(QAction*)));
    connect(toolboxActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(setToolboxOptions(QAction*)));

    connect(ui->pbClear, SIGNAL(pressed()), this, SLOT(clearBgoCheckBoxes()));
    PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());

    // Bgo attributes
    connect(ui->chkBgc, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->chkBgi, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->chkBgmo, SIGNAL(toggled(bool)), ui->chkBgmi, SLOT(setEnabled(bool)));
    connect(ui->chkBgmo, SIGNAL(toggled(bool)), this, SLOT(updateBgiToolboxAttributes()));
    connect(ui->chkBgmo, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->chkBgq, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->rbtnLrud, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->rbtnRldu, SIGNAL(toggled(bool)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->cboBgt, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBgoToolboxAttributes()));
    connect(ui->pbBgt, SIGNAL(clicked()), this, SLOT(toggleBgoTriggerMode()));
    connect(ui->pbBgtNot, SIGNAL(clicked()), this, SLOT(toggleBgoTriggerMode()));
    connect(this, SIGNAL(bgoAttributesChanged(int)), scene, SLOT(setBgoFlags(int)));
    connect(this, SIGNAL(bgoTriggerChanged(int,int)), scene, SLOT(setBgoTriggers(int,int)));

    // Bgi attributes
    connect(ui->rbtnBga, SIGNAL(toggled(bool)), this, SLOT(updateBgiToolboxAttributes()));
    connect(ui->rbtnBgp, SIGNAL(toggled(bool)), this, SLOT(updateBgiToolboxAttributes()));
    connect(ui->rbtnTile, SIGNAL(toggled(bool)), this, SLOT(updateBgiToolboxAttributes()));
    connect(ui->chkBgmi, SIGNAL(toggled(bool)), this, SLOT(updateBgiToolboxAttributes()));
    connect(ui->cboBgm, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBgiToolboxAttributes()));
    connect(this, SIGNAL(bgiTileChanged(int)), scene, SLOT(setBgiTile(int)));
    connect(this, SIGNAL(bgiFlagsChanged(int)), scene, SLOT(setBgiFlags(int)));

    // Bgm attributes
    connect(ui->chkPayload, SIGNAL(toggled(bool)), ui->leMutLeft, SLOT(setEnabled(bool)));
    connect(ui->chkPayload, SIGNAL(toggled(bool)), ui->leMutRight, SLOT(setEnabled(bool)));
    connect(ui->chkPayload, SIGNAL(toggled(bool)), ui->leMutTop, SLOT(setEnabled(bool)));
    connect(ui->chkPayload, SIGNAL(toggled(bool)), ui->leMutBottom, SLOT(setEnabled(bool)));
    connect(ui->leMutLeft, SIGNAL(editingFinished()), this, SLOT(updateBgmToolboxAttributes()));
    connect(ui->leMutRight, SIGNAL(editingFinished()), this, SLOT(updateBgmToolboxAttributes()));
    connect(ui->leMutTop, SIGNAL(editingFinished()), this, SLOT(updateBgmToolboxAttributes()));
    connect(ui->leMutBottom, SIGNAL(editingFinished()), this, SLOT(updateBgmToolboxAttributes()));
    connect(ui->chkPayload, SIGNAL(toggled(bool)), scene, SLOT(setPayloadToCustom(bool)));
    connect(this, SIGNAL(bgmCustomPayloadChanged(Platz::MutablePayload)), scene, SLOT(setCustomPayload(Platz::MutablePayload)));

    // Platform attributes
    connect(ui->rbtnSmooth, SIGNAL(toggled(bool)), this, SLOT(updatePlatformToolboxAttributes()));
    connect(ui->rbtnStepped, SIGNAL(toggled(bool)), this, SLOT(updatePlatformToolboxAttributes()));
    connect(ui->rbtnXAxis, SIGNAL(toggled(bool)), this, SLOT(updatePlatformToolboxAttributes()));
    connect(ui->rbtnYAxis, SIGNAL(toggled(bool)), this, SLOT(updatePlatformToolboxAttributes()));
    connect(ui->spbPlatformSpeed, SIGNAL(valueChanged(int)), this, SLOT(updatePlatformToolboxAttributes()));
    connect(this, SIGNAL(platformAxisChanged(BgPlatformPath::PlatformAxis)), scene, SLOT(setPlatformAxis(BgPlatformPath::PlatformAxis)));
    connect(this, SIGNAL(platformStyleChanged(BgPlatform::PlatformStyle)), scene, SLOT(setPlatformStyle(BgPlatform::PlatformStyle)));
    connect(this, SIGNAL(platformClearTileChanged(QString)), scene, SLOT(setPlatformClearTile(QString)));
    connect(this, SIGNAL(platformVelocityChanged(int)), scene, SLOT(setPlatformVelocity(int)));

    // Settings
    connect(settings, SIGNAL(projectNameChanged(QString)), this, SLOT(setProjectName(QString)));
    connect(settings, SIGNAL(projectPathChanged(QString)), this, SLOT(setProjectPath(QString)));
    connect(settings, SIGNAL(slicePathChanged(QString)), this, SLOT(setSlicePath(QString)));
    connect(settings, SIGNAL(tilePathChanged(QString)), this, SLOT(setTilePath(QString)));
    connect(settings, SIGNAL(mapPathChanged(QString)), this, SLOT(setMapPath(QString)));
    connect(settings, SIGNAL(animPathChanged(QString)), this, SLOT(setAnimPath(QString)));
    connect(settings, SIGNAL(srcFolderChanged(QString)), this, SLOT(setSrcFolder(QString)));
    connect(settings, SIGNAL(makefilePathChanged(QString)), this, SLOT(setMakefilePath(QString)));
    connect(settings, SIGNAL(hexfilePathChanged(QString)), this, SLOT(setHexfilePath(QString)));
    connect(settings, SIGNAL(platzfilePathChanged(QString)), this, SLOT(setPlatzfilePath(QString)));
    connect(settings, SIGNAL(makeExePathChanged(QString)), this, SLOT(setMakeExePath(QString)));
    connect(settings, SIGNAL(emuExePathChanged(QString)), this, SLOT(setEmuExePath(QString)));
    connect(settings, SIGNAL(sliceSizeChanged(QSize)), ui->graphicsView, SLOT(setSliceSize(QSize)));
    connect(settings, SIGNAL(sliceSizeChanged(QSize)), this, SLOT(setSliceSize(QSize)));
    connect(settings, SIGNAL(spriteSizeChanged(QSize)), this, SLOT(setSpriteSize(QSize)));

    // Restore window state
    QByteArray winGeometry, winLayout;

    if (settings->loadLePlatzSettings(winGeometry, winLayout)) {
        restoreGeometry(winGeometry);
        restoreState(winLayout);
    } else {
        this->setWindowState(Qt::WindowMaximized);
    }
    clearBgoCheckBoxes();
    updatePlatformToolboxAttributes();
    updateBgiToolboxAttributes();
    updateBgoToolboxAttributes();

    if (!cmdLineProject.isEmpty())
        loadProject(cmdLineProject);
}

void PlatzWin::setProjectName(const QString &name)
{
    projectName = name;

    if (!name.isEmpty())
        setWindowTitle(projectName + " - LePlatz");
    else
        setWindowTitle("LePlatz");
}

void PlatzWin::aboutLePlatz()
{
    QPointer<About> about = new About(this);
    about->setWindowTitle("About LePlatz");
    about->setTitle("LePlatz v1.0");
    about->setText("LePlatz is a Level Editor for the Platz Toolset. It allows developers to define, through a graphical interface, "
                   "the display of their Platz levels and how their characters interact within them. \n\nThis process was previously "
                   "done by hand in a text editor. LePlatz affords much a more efficient and iterative development cycle by significantly "
                   "reducing the overhead associated with Platz level design.");
    about->setUrl("URL: <a href=\"http://uzebox.org/wiki/index.php?title=LePlatz_Level_Editor\">LePlatz: Platz Level Editor</a>");
    about->setPixmap(QPixmap(":/misc/LePlatzLogo_sml.png"));
    about->exec();
    delete about;
}

void PlatzWin::aboutPlatz()
{
    QPointer<About> about = new About(this);
    about->setWindowTitle("About Platz");
    about->setTitle("Platz Toolset");
    about->setText("Platz is a platformer toolset for video mode 3. It handles most of the tedious tasks common to side-scrollers and "
                   "allows you to focus on your game logic. More specifically, Platz will manage: "
                    "\n\n - Loading and displaying level data"
                    "\n - Player/level collision detection"
                    "\n - Moving platforms"
                    "\n - Animated backgrounds"
                    "\n - Patterned backgrounds"
                    "\n - Event notification and more"
                    "\n\nPlatz levels are stored as Fill instructions and have on average roughly one tenth of the flash footprint of "
                    "traditional maps. This affords huge levels with the potential for up to 255 unique screens (world slices in Platz "
                    "terminology) of level data, any of which may be repeated multiple times for no extra flash outlay.");
    about->setUrl("URL: <a href=\"http://uzebox.org/wiki/index.php?title=Platz_Toolset\">Platz Toolset</a>");
    about->setPixmap(QPixmap(":/misc/PlatzLogo_sml.png"));
    about->exec();
    delete about;
}

void PlatzWin::aboutUzebox()
{
    QPointer<About> about = new About(this);
    about->setWindowTitle("About Uzebox");
    about->setTitle("Uzebox");
    about->setText("The Uzebox is a retro-minimalist open source game console. It is based on an AVR 8-bit "
                  "general purpose microcontroller made by Atmel. The particularity of the system is that it uses an "
                  "interrupt driven kernel and has no frame buffer. Functions such as video sync generation, tile "
                  "rendering and music mixing is done realtime by a background task so games can easily be developed in C. "
                  "\n\nThe design goal was to be as simple as possible yet have good enough sound and graphics while leaving "
                  "enough resources to implement interesting games. Emphasis was put on making it easy and fun to assemble and "
                  "program for any hobbyists. The final design contains only two chips: an ATmega644 and an AD725 RGB-to-NTSC converter.");
    about->setUrl("URL: <a href=\"http://belogic.com/uzebox/\">Uzebox Homepage</a>");
    about->setPixmap(QPixmap(":/misc/UzeboxLogo.png"));
    about->exec();
    delete about;
}

void PlatzWin::setSnapToResolution(QAction* action)
{
    // Would like to store values within the actions, but for now...
    bool ok;
    int resolution = action->text().toInt(&ok, 10);

    if (!ok)
        return;
    PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());

    if (scene)
        scene->setSnapToResolution(resolution);
}

void PlatzWin::toggleSelectedSliceLock()
{
    if (!model)
        return;
    WorldItem *w = model->selectedItem();

    if (w && w->type() == WorldItem::Slice) {
        Slice *slice = static_cast<Slice*>(w);
        slice->setLockedOrdering(!slice->lockedOrdering());
        updateDetailDataDisplay(slice);
        unsavedChanges = true;
    }
}

void PlatzWin::replicateSlice()
{
    ui->graphicsView->setMode(Platz::IM_REPLICATE);
}

void PlatzWin::updateRecentProjects(const QStringList &recentProjects)
{
    if (!(recProjMenu && recProjActionGrp))
        return;
    // Clear but don't delete
    foreach (QAction *a, recProjMenu->actions())
        recProjMenu->removeAction(a);

    int i = 0;
    QAction *action = 0;

    foreach (QString s, recentProjects) {
        action = recProjActionGrp->actions().takeAt(i++);
        action->setText(s);
        recProjMenu->addAction(action);
    }
}

void PlatzWin::updateDetailDataDisplay(const QModelIndex &index)
{
    updateDetailDataDisplay(static_cast<WorldItem*>(index.internalPointer()));
}

void PlatzWin::updateDetailDataDisplay(const WorldItem *item)
{
    QString detailData;
    QPixmap detailPix;

    if (!item) {
        detailData = "";
        detailPix = WorldItem::worldDetailPixmap(WorldItem::Level);
    } else {
        detailData = item->detailData();
        detailPix = WorldItem::worldDetailPixmap(item->type());
    }
    ui->lblSelectedDetails->setText(detailData);
    ui->lblSelectedDetailsIcon->setPixmap(detailPix);
}

void PlatzWin::updateDetailImageDisplay(const QImage &img)
{
    ui->lblSelectedDetailsImg->setPixmap(QPixmap());

    if (!img.isNull())
        ui->lblSelectedDetailsImg->setPixmap(QPixmap::fromImage(img).scaled(QSize(32,32), Qt::IgnoreAspectRatio));    
}

void PlatzWin::populateCombo(const QStringList &ids, QComboBox *cbo)
{
    if (!cbo)
        return;
    int i = 0;

    cbo->clear();

    foreach (QString s, ids)
        cbo->addItem(s, i++);
}

void PlatzWin::updatePlatformToolboxAttributes()
{
    BgPlatform::PlatformStyle style;
    BgPlatformPath::PlatformAxis axis;

    if (ui->rbtnSmooth->isChecked())
        style = BgPlatform::Smooth;
    else
        style = BgPlatform::Stepped;
    if (ui->rbtnXAxis->isChecked())
        axis = BgPlatformPath::AxisX;
    else
        axis = BgPlatformPath::AxisY;
    emit platformStyleChanged(style);
    emit platformAxisChanged(axis);
    emit platformClearTileChanged(ui->cboPlatformClearTile->currentText());
    emit platformVelocityChanged(ui->spbPlatformSpeed->value());
}

void PlatzWin::updateBgiToolboxAttributes()
{
    int tile = -1, flags = 0;

    if (ui->rbtnBga->isChecked())
        flags = BgInner::BGA;
    else if (ui->rbtnBgp->isChecked())
        flags = BgInner::BGP;
    // else already 0 for Tile
    if (ui->chkBgmi->isEnabled() && ui->chkBgmi->isChecked())
        flags |= BgInner::BGM;
    if (ui->graphicsView->mode() == Platz::IM_MUTABLE_BG)
        tile = ui->cboBgm->itemData(ui->cboBgm->currentIndex()).toInt();
    emit bgiTileChanged(tile);
    emit bgiFlagsChanged(flags);
}

void PlatzWin::updateBgmToolboxAttributes()
{
    bool ok;
    int left, right, top, bottom;

    left = ui->leMutLeft->text().toInt(&ok);
    left = qMax(left, 0);
    right = ui->leMutRight->text().toInt(&ok);
    right = qMax(right, 0);
    top = ui->leMutTop->text().toInt(&ok);
    top = qMax(top, 0);
    bottom = ui->leMutBottom->text().toInt(&ok);
    bottom = qMax(bottom, 0);

    // Not looking for a valid QRect here - this is like a
    // void container of custom data that must fit in four ints
    MutablePayload payload;
    payload.left = left;
    payload.right = right;
    payload.top = top;
    payload.btm = bottom;
    emit bgmCustomPayloadChanged(payload);
}

void PlatzWin::updateBgoToolboxAttributes()
{
    int flags = 0, trigOrientation = 0, trigId = 0;

    if (bgoTrigger) {
        flags |= BgOuter::BGT;
        trigId = ui->cboBgt->itemData(ui->cboBgt->currentIndex()).toInt();
        trigOrientation = (ui->rbtnLrud->isChecked()) ? 1 : -1;
    } else {
        if (ui->chkBgc->isChecked())
            flags |= BgOuter::BGC;
        if (ui->chkBgi->isChecked())
            flags |= BgOuter::BGI;
        if (ui->chkBgmo->isChecked())
            flags |= BgOuter::BGM;
        if (ui->chkBgq->isChecked())
            flags |= BgOuter::BGQ;
    }
    emit bgoAttributesChanged(flags);
    emit bgoTriggerChanged(trigId, trigOrientation);
}

void PlatzWin::toggleBgoTriggerMode()
{
    if (!bgoTrigger)
        ui->stackedWidget->setCurrentIndex(6);
    else
        ui->stackedWidget->setCurrentIndex(1);
    bgoTrigger = !bgoTrigger;
    updateBgoToolboxAttributes();
}

void PlatzWin::clearBgoCheckBoxes()
{
    ui->chkBgc->setChecked(false);
    ui->chkBgi->setChecked(false);
    ui->chkBgmo->setChecked(false);
    ui->chkBgq->setChecked(false);
}

InteractionMode PlatzWin::fkeyToIntMode(int fkey)
{
    switch (fkey) {
        case Qt::Key_F1: return IM_SELECT;
        case Qt::Key_F2: return IM_OUTER_BG;
        case Qt::Key_F3: return IM_INNER_BG;
        case Qt::Key_F4: return IM_OBJECT_BG;
        case Qt::Key_F5: return IM_PLATFORM_PATH;
        case Qt::Key_F6: return IM_PLATFORM;
        case Qt::Key_F7: return IM_MUTABLE_BG;
        default: return IM_SELECT;
    }
}

void PlatzWin::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers()&Qt::ControlModifier) {
        switch (event->key()) {
            case Qt::Key_F1:
            case Qt::Key_F2:
            case Qt::Key_F3:
            case Qt::Key_F4:
            case Qt::Key_F5:
            case Qt::Key_F6:
            case Qt::Key_F7:
            {
                PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());
                scene->hideOuterJoin(fkeyToIntMode(event->key()));
                break;
            }
            case Qt::Key_B:
                buildSource();
                break;
            case Qt::Key_N:
                newProject();
                break;
            case Qt::Key_O:
                initLoadProject();
                break;
            case Qt::Key_P:
                playInEmulator();
                break;
            case Qt::Key_R:
                reloadResources();
                break;
            case Qt::Key_S:
                saveProject(projectPath);
                break;
            default:
                break;
        }
    } else {
        switch (event->key()) {
            case Qt::Key_Plus:
                emit zoomIn();
                break;
            case Qt::Key_Minus:
                emit zoomOut();
                break;
            case Qt::Key_Escape:
            {
                PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());
                scene->cancelSelection();
                break;
            }
            case Qt::Key_H:
            {
                PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());
                if (scene->isSelectionVisible())
                    scene->hideSelection();
                else
                    scene->showSelection();
                break;
            }
            case Qt::Key_F1:
                emit imModeTriggered(ui->actionSelectCursor);
                break;
            case Qt::Key_F2:
                emit imModeTriggered(ui->actionBgOuterCursor);
                break;
            case Qt::Key_F3:
                emit imModeTriggered(ui->actionBgInnerCursor);
                break;
            case Qt::Key_F4:
                emit imModeTriggered(ui->actionBgObjectCursor);
                break;
            case Qt::Key_F5:
                emit imModeTriggered(ui->actionPlatformPathCursor);
                break;
            case Qt::Key_F6:
                emit imModeTriggered(ui->actionPlatformCursor);
                break;
            case Qt::Key_F7:
                emit imModeTriggered(ui->actionBgMutableCursor);
                break;
            default:
                QMainWindow::keyPressEvent(event);
                break;
        }
    }
}

void PlatzWin::newProject()
{
    if (!closeProject())
        return;

    // Create new project
    Q_ASSERT(settings);

    if (settings->showNewProjectDialog(this) == QDialog::Accepted) {
        QString msg;

        QApplication::setOverrideCursor(Qt::WaitCursor);
        loadResources(msg);
        ui->treeView->setExpanded(model->indexOf(0, 0, model->root()), true);
        setWindowTitle(projectName + " - LePlatz");
        activeProject = true;
        saveProject(projectPath);
        settings->addRecentProject(projectPath);
        QApplication::restoreOverrideCursor();

        if (!msg.isEmpty())
            ui->statusBar->showMessage("New Project created. Problems loading: " + msg, STATUS_DELAY);
    }
}

bool PlatzWin::closeProject()
{
    if (activeProject && unsavedChanges) {
        QMessageBox msgbox("Save current project", "Save changes to currently open project before creating a new project?", QMessageBox::Question,
                           QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel, this);
        int ret = msgbox.exec();

        if (ret == QMessageBox::Save) {
            if (!saveProject(projectPath)) {
                QMessageBox retry("Save failed!", "'OK' to continue current operation or 'Cancel' to abort?", QMessageBox::Critical,
                                  QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton, this);
                ret = retry.exec();

                if (ret == QMessageBox::Cancel)
                    return false;
            }
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    activeProject = false;
    unsavedChanges = false;
    settings->resetProjectSettings();
    updateDetailImageDisplay(QImage());

    // Close open project
    PlatzGraphicsScene *scene = static_cast<PlatzGraphicsScene*>(ui->graphicsView->scene());

    if (scene)
        scene->setMutableParent(0);
    model->clear();
    ui->graphicsView->clearBackground();
    ui->tileListWidget->clear();
    ui->mapListWidget->clear();
    ui->animListWidget->clear();
    return true;
}

void PlatzWin::showSettings()
{
    if (settings->showEditSettingsDialog(this) == QDialog::Accepted) {
        if (activeProject)
            unsavedChanges = true;
    }
}

void PlatzWin::setInteractionMode(QAction *action)
{
    ui->graphicsView->setMode(static_cast<Platz::InteractionMode>(action->actionGroup()->actions().indexOf(action)));
}

void PlatzWin::setToolboxOptions(QAction *action)
{
    int index;
    Platz::InteractionMode mode = static_cast<Platz::InteractionMode>(action->actionGroup()->actions().indexOf(action));

    switch (mode) {
        case Platz::IM_OUTER_BG:
            index = (bgoTrigger) ? 6 : 1;
            break;
        case Platz::IM_INNER_BG:
            index = 2;
            updateBgiToolboxAttributes();   // Revert tile from mutable id (FIXME: this updateBgi function does too much - split it up)
            break;
        case Platz::IM_MUTABLE_BG:
            index = 3;
            updateBgiToolboxAttributes();   // Init tile to mutable id
            break;
        case Platz::IM_PLATFORM_PATH:
            index = 4;
            break;
        case Platz::IM_PLATFORM:
            index = 5;
            break;
        case Platz::IM_REPLICATE:
        case Platz::IM_SELECT:
            index = ui->stackedWidget->currentIndex();
            break;
        default:
            index = 0;
            break;
    }
    ui->stackedWidget->setCurrentIndex(index);
}

void PlatzWin::reloadResources()
{
    QString msg;

    loadResources(msg);

    if (!msg.isEmpty())
        ui->statusBar->showMessage("Failed to load: " + msg, STATUS_DELAY);
    else
        ui->statusBar->showMessage("Project resources reloaded successfully." + msg, STATUS_DELAY);
}

void PlatzWin::loadResources(QString &msg)
{
    ui->graphicsView->clearBackground();
    ui->tileListWidget->clearImages();
    ui->mapListWidget->clearImages();
    ui->animListWidget->clearImages();
    ui->cboBgm->clear();
    ui->cboBgt->clear();
    ui->cboPlatformClearTile->clear();
    WorldItem::mutableIds.clear();
    WorldItem::triggerIds.clear();
    WorldItem::platClrTileIds.clear();

    if (!QFile::exists(tilePath)) {
        msg += ((msg.isEmpty())?"":", ");
        msg += "Tiles";
    } else {
        ui->tileListWidget->populate(8,8,32,32,120,QPixmap(tilePath));
    }

    if (!QFile::exists(mapPath)) {
        msg += ((msg.isEmpty())?"":", ");
        msg += "Maps";
    } else {
        ui->mapListWidget->populate(64,64,120,QPixmap(mapPath));
    }

    if (!QFile::exists(animPath)) {
        msg += ((msg.isEmpty())?"":", ");
        msg += "Animations";
    } else {
        ui->animListWidget->populate(64,64,120,QPixmap(animPath));
    }

    if (!loadBackground()) {
        msg += ((msg.isEmpty())?"":", ");
        msg += "Slices";
    }

    QDir dir(srcFolder);

    if (!dir.exists()) {
        msg += ((msg.isEmpty())?"":", ");
        msg += "Src defines";

    } else {
        SourceParser parser;
        WorldItem::mutableIds = parser.parseForStrings(srcFolder, MUTABLE_IDS_MARKER);
        WorldItem::triggerIds = parser.parseForStrings(srcFolder, TRIGGER_IDS_MARKER);
        WorldItem::platClrTileIds = parser.parseForStrings(srcFolder, PLATFORM_CLR_TILE_IDS_MARKER);
        populateCombo(WorldItem::mutableIds, ui->cboBgm);
        populateCombo(WorldItem::triggerIds, ui->cboBgt);
        populateCombo(WorldItem::platClrTileIds, ui->cboPlatformClearTile);
    }

    updatePlatformToolboxAttributes();
    updateBgiToolboxAttributes();
    updateBgoToolboxAttributes();
    ui->graphicsView->synchronizeModelView();
}

bool PlatzWin::loadBackground()
{
    bool retval = ui->graphicsView->loadBackground(slicePath);

    if (!retval)
        ui->statusBar->showMessage("Failed to load slice images. Incompatible images at: " + QDir(slicePath).absolutePath(), STATUS_DELAY);
    return retval;
}

void PlatzWin::insertSlice()
{
    ui->graphicsView->setMode(Platz::IM_INSERT);
}

void PlatzWin::removeSlice()
{
    ui->graphicsView->setMode(Platz::IM_REMOVE);
}

void PlatzWin::compileWorld()
{
    QModelIndex index = model->validateModel();

    if (index.isValid()) {
        ui->statusBar->showMessage("Compilation aborted - Incomplete world model. Address selected item.", STATUS_DELAY);
        model->setSelectedIndex(index);
        return;
    }

    QFile file(platzfilePath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ui->statusBar->showMessage("Cannot open " + QFileInfo(platzfilePath).absoluteFilePath() + ". " + file.errorString(), STATUS_DELAY);
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    WorldCompiler wc(model->root());
    wc.setSliceSize(sliceSize);
    wc.setSpriteSize(spriteSize);

    if (wc.compileWorld(&file, projectName))
        ui->statusBar->showMessage("World compiled successfully. Output to " + QFileInfo(platzfilePath).absoluteFilePath(), STATUS_DELAY);
    else
        ui->statusBar->showMessage("World compilation failed. Check path and write permissions", STATUS_DELAY);
    file.close();
    QApplication::restoreOverrideCursor();
}

bool PlatzWin::initLoadProject()
{
    // Ensure we have a valid default path
    if (!QFile::exists(projectPath))
        projectPath = QDir::currentPath();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
            projectPath, tr("LePlatz Project Files (*.xml)"));
    if (fileName.isEmpty() || !QFile::exists(fileName))
        return false;
    return loadProject(fileName);
}

bool PlatzWin::loadProject(QAction *action)
{
    if (!action)
        return false;
    return loadProject(action->text());
}

bool PlatzWin::loadProject(const QString &path)
{
    if (!closeProject())
        return false;
    bool retval = true;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    retval = settings->loadProject(path, model);

    if (!retval) {
        ui->statusBar->showMessage("Failed to load project. Check path and permissions.", STATUS_DELAY);
    } else {
        QString msg;

        loadResources(msg);
        setWindowTitle(projectName + " - LePlatz");
        settings->addRecentProject(projectPath);
        ui->treeView->setExpanded(model->indexOf(0, 0, model->root()), true);
        activeProject = true;
        unsavedChanges = false;
        retval = true;

        if (msg.isEmpty())
            ui->statusBar->showMessage("Project loaded successfully", STATUS_DELAY);
        else
            ui->statusBar->showMessage("Project loaded. Problems loading: " + msg, STATUS_DELAY);
    }
    QApplication::restoreOverrideCursor();
    return retval;
}

void PlatzWin::initSaveProject()
{
    saveProject(projectPath);
}

bool PlatzWin::initSaveAsProject()
{
    QDir dir;
    QString path;

    if (QFile::exists(projectPath))
        dir.setPath(projectPath);
    else
        dir.setPath(QDir::currentPath());
    path = QFileDialog::getSaveFileName(this, "Save As...", dir.absolutePath(), "*.xml");

    if (!path.endsWith(".xml")) {
        ui->statusBar->showMessage("Invalid LePlatz project file. Save aborted.", STATUS_DELAY);
        return false;
    }
    return saveProject(path);
}

bool PlatzWin::prepareToClose()
{
    if (activeProject)
        if (!closeProject())
            return false;
    return close();
}

bool PlatzWin::saveProject(const QString &path)
{
    if (!activeProject) {
        ui->statusBar->showMessage("No project to save. Open or create a new LePlatz project.", STATUS_DELAY);
        return true;
    }
    Q_ASSERT(settings);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool retval = settings->saveProject(path, model->root());
    QApplication::restoreOverrideCursor();

    if (retval) {
        ui->statusBar->showMessage("Project saved successfully to " + QFileInfo(path).absoluteFilePath(), STATUS_DELAY);
        settings->addRecentProject(path);
        unsavedChanges = false;
    } else {
        ui->statusBar->showMessage("Project failed. Check write permissions at: " + QFileInfo(path).absoluteFilePath(), STATUS_DELAY);
    }
    return retval;
}

// Consider paths via environment variables
void PlatzWin::buildSource()
{
    QStringList arguments;

    ui->txtMakeOutput->clear();
    arguments << "--directory=" + QFileInfo(makefilePath).dir().absolutePath()
                << "--makefile=" + QFileInfo(makefilePath).fileName();
    procMake->setProcessChannelMode(QProcess::MergedChannels);
    procMake->setReadChannel(QProcess::StandardOutput);
    procMake->start(makeExePath, arguments);
}

void PlatzWin::playInEmulator()
{
    QDir dir;
    QString path = dir.absoluteFilePath(emuExePath);
    QStringList arguments;
    arguments << hexfilePath;
    procEmu->setProcessChannelMode(QProcess::MergedChannels);
    procEmu->setReadChannel(QProcess::StandardOutput);
    procEmu->setWorkingDirectory(QFileInfo(emuExePath).dir().absolutePath());
    ui->txtEmuOutput->clear();
    procEmu->start(path, arguments);
}

void PlatzWin::makeOutputReady()
{
    ui->txtMakeOutput->appendPlainText(QString(procMake->readAllStandardOutput()));
}

void PlatzWin::emuOutputReady()
{
    ui->txtEmuOutput->appendPlainText(QString(procEmu->readAllStandardOutput()));
}

void PlatzWin::makeFinished(int exitCode, QProcess::ExitStatus)
{
    ui->txtMakeOutput->appendPlainText("Exit code: " + QString::number(exitCode));
}

void PlatzWin::emuFinished(int exitCode, QProcess::ExitStatus)
{
    ui->txtEmuOutput->appendPlainText("Exit code: " + QString::number(exitCode));
}

void PlatzWin::makeError(QProcess::ProcessError error)
{
    PrintProcError(error, ui->txtMakeOutput);
}

void PlatzWin::emuError(QProcess::ProcessError error)
{
    PrintProcError(error, ui->txtEmuOutput);
}

void PlatzWin::PrintProcError(QProcess::ProcessError error, QPlainTextEdit *txtEdit)
{
    QString errmsg("Error: ");

    switch(error) {
        case QProcess::FailedToStart:
            errmsg += "The process failed to start.\n";
            break;
        case QProcess::Crashed:
            errmsg += "The process crashed some time after starting successfully.\n";
            break;
        case QProcess::ReadError:
            errmsg += "An error occurred when attempting to read from the process.\n";
            break;
        case QProcess::UnknownError:
        default:
            errmsg += "An unknown error occurred.\n";
            break;
    }
    txtEdit->appendPlainText(errmsg);
}

void PlatzWin::closeEvent(QCloseEvent *event)
{
    if (!prepareToClose())
        event->ignore();
    else
        QMainWindow::closeEvent(event);
}

bool PlatzWin::close()
{
    settings->saveLePlatzSettings(saveGeometry(), saveState());
    return QMainWindow::close();
}

void PlatzWin::changeEvent (QEvent *event)
{
    // This forces a background draw and prevents a weird "whitewash" effect
    // when the window is restored
    if (event->type() == QEvent::WindowStateChange)
        ui->graphicsView->setBackgroundBrush(ui->graphicsView->backgroundBrush());
    QMainWindow::changeEvent(event);
}

PlatzWin::~PlatzWin()
{
    if (procMake) {
        if (procMake->isOpen())
            procMake->close();
        delete procMake;
    }

    if (procEmu) {
        if (procEmu->isOpen())
            procEmu->close();
        delete procEmu;
    }
    foreach (QAction *a, recProjActionGrp->actions()) {
        recProjActionGrp->removeAction(a);
        delete a;
    }
    delete recProjActionGrp;
    delete recProjMenu;
    recProjMenu = 0;    // Don't want ui->menuFile possibly deleting strays
    delete ui;
}
