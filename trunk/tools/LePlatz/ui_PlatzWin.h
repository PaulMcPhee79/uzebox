/********************************************************************************
** Form generated from reading UI file 'PlatzWin.ui'
**
** Created: Tue 2. Feb 00:35:57 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLATZWIN_H
#define UI_PLATZWIN_H

#include <LePlatzComboBox.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "LePlatzSpinBox.h"
#include "PlatzGraphicsList.h"
#include "PlatzGraphicsView.h"
#include "PlatzTreeView.h"

QT_BEGIN_NAMESPACE

class Ui_PlatzWin
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionClose;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionDelete;
    QAction *actionSelectAll;
    QAction *actionAboutLePlatz;
    QAction *actionAboutPlatz;
    QAction *actionAboutUzebox;
    QAction *actionSave;
    QAction *actionReload;
    QAction *actionCompile;
    QAction *actionEmulator;
    QAction *actionPrevious;
    QAction *actionNext;
    QAction *actionAdd;
    QAction *actionRemove;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionSettings;
    QAction *actionMake;
    QAction *actionSelectCursor;
    QAction *actionBgOuterCursor;
    QAction *actionBgInnerCursor;
    QAction *actionBgObjectCursor;
    QAction *actionPlatformPathCursor;
    QAction *actionPlatformCursor;
    QAction *actionBgMutableCursor;
    QAction *actionReplicate;
    QAction *actionLock;
    QAction *actionFindReplace;
    QAction *actionBgOrdering;
    QAction *actionCheckForUpdates;
    QAction *actionInstallUpdates;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_11;
    PlatzGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lblSelectedDetailsIcon;
    QLabel *lblSelectedDetails;
    QLabel *lblSelectedDetailsImg;
    QFrame *frame;
    QVBoxLayout *verticalLayout_10;
    QLabel *lblCoords;
    QLabel *lblSliceIndex;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuSnapToX;
    QMenu *menuSnapToY;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidgetUtilities;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidgetUtilities;
    QWidget *tabTiles;
    QVBoxLayout *verticalLayout_12;
    PlatzGraphicsList *tileListWidget;
    QWidget *tabMaps;
    QVBoxLayout *verticalLayout_14;
    PlatzGraphicsList *mapListWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_5;
    PlatzGraphicsList *animListWidget;
    QWidget *tabMake;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *txtMakeOutput;
    QWidget *tabEmu;
    QHBoxLayout *horizontalLayout_2;
    QPlainTextEdit *txtEmuOutput;
    QDockWidget *dockWidgetWorldDetails;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    PlatzTreeView *treeView;
    QDockWidget *dockWidgetToolbox;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_16;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QToolButton *tbtnPointer;
    QToolButton *tbtnBgo;
    QToolButton *tbtnBgi;
    QToolButton *tbtnObj;
    QToolButton *tbtnMpp;
    QToolButton *tbtnMp;
    QToolButton *tbtnBgm;
    QStackedWidget *stackedWidget;
    QWidget *pageBlank;
    QWidget *pageOuter;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_21;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_19;
    QCheckBox *chkBgc;
    QCheckBox *chkBgmo;
    QCheckBox *chkBgq;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_18;
    QCheckBox *chkBgi;
    QPushButton *pbBgt;
    QPushButton *pbClear;
    QSpacerItem *horizontalSpacer_6;
    QWidget *pageInner;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_18;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *rbtnTile;
    QRadioButton *rbtnBgp;
    QRadioButton *rbtnBga;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *chkBgmi;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *chkBgmc;
    QSpacerItem *horizontalSpacer_9;
    QVBoxLayout *verticalLayout_17;
    QLabel *lblBgmc;
    LePlatzComboBox *cboBgmc;
    QWidget *pageMutable;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_9;
    QLabel *lblBgm;
    LePlatzComboBox *cboBgm;
    QCheckBox *chkPayload;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_15;
    QHBoxLayout *horizontalLayout_12;
    QLabel *lblMutLeft;
    QLineEdit *leMutLeft;
    QHBoxLayout *horizontalLayout_13;
    QLabel *lblMutRight;
    QLineEdit *leMutRight;
    QHBoxLayout *horizontalLayout_19;
    QHBoxLayout *horizontalLayout_20;
    QLabel *lblMutTop;
    QLineEdit *leMutTop;
    QHBoxLayout *horizontalLayout_21;
    QLabel *lblMutBtm;
    QLineEdit *leMutBottom;
    QWidget *pagePlatformPath;
    QVBoxLayout *verticalLayout_20;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_6;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_10;
    QRadioButton *rbtnXAxis;
    QRadioButton *rbtnYAxis;
    QSpacerItem *horizontalSpacer_11;
    QVBoxLayout *verticalLayout_8;
    QLabel *lblPlatformClearTile;
    LePlatzComboBox *cboPlatformClearTile;
    QSpacerItem *verticalSpacer_3;
    QWidget *pagePlatform;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_12;
    QRadioButton *rbtnSmooth;
    QRadioButton *rbtnStepped;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_10;
    QFrame *line;
    QSpacerItem *verticalSpacer_11;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lblPlatformSpeed;
    LePlatzSpinBox *spbPlatformSpeed;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *verticalSpacer_6;
    QWidget *pageTrigger;
    QVBoxLayout *verticalLayout_13;
    QSpacerItem *verticalSpacer_9;
    QGroupBox *groupBox_7;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_14;
    QRadioButton *rbtnLrud;
    QRadioButton *rbtnRldu;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout_10;
    QLabel *lblBgt;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbBgtNot;
    LePlatzComboBox *cboBgt;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *PlatzWin)
    {
        if (PlatzWin->objectName().isEmpty())
            PlatzWin->setObjectName(QString::fromUtf8("PlatzWin"));
        PlatzWin->resize(933, 850);
        actionNew = new QAction(PlatzWin);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/128x128/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon);
        actionOpen = new QAction(PlatzWin);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/128x128/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionSaveAs = new QAction(PlatzWin);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/128x128/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveAs->setIcon(icon2);
        actionClose = new QAction(PlatzWin);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/128x128/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon3);
        actionExit = new QAction(PlatzWin);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/128x128/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionUndo = new QAction(PlatzWin);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/128x128/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon5);
        actionRedo = new QAction(PlatzWin);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionRedo->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/128x128/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon6);
        actionCut = new QAction(PlatzWin);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCut->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/128x128/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon7);
        actionCopy = new QAction(PlatzWin);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionCopy->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/128x128/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon8);
        actionPaste = new QAction(PlatzWin);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionPaste->setEnabled(false);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/128x128/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon9);
        actionDelete = new QAction(PlatzWin);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/128x128/remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon10);
        actionSelectAll = new QAction(PlatzWin);
        actionSelectAll->setObjectName(QString::fromUtf8("actionSelectAll"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/128x128/selectAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectAll->setIcon(icon11);
        actionAboutLePlatz = new QAction(PlatzWin);
        actionAboutLePlatz->setObjectName(QString::fromUtf8("actionAboutLePlatz"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/128x128/mp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAboutLePlatz->setIcon(icon12);
        actionAboutPlatz = new QAction(PlatzWin);
        actionAboutPlatz->setObjectName(QString::fromUtf8("actionAboutPlatz"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/128x128/aboutPlatz.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAboutPlatz->setIcon(icon13);
        actionAboutUzebox = new QAction(PlatzWin);
        actionAboutUzebox->setObjectName(QString::fromUtf8("actionAboutUzebox"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/uzebox.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAboutUzebox->setIcon(icon14);
        actionSave = new QAction(PlatzWin);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setIcon(icon2);
        actionReload = new QAction(PlatzWin);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/128x128/reloadWorld.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReload->setIcon(icon15);
        actionCompile = new QAction(PlatzWin);
        actionCompile->setObjectName(QString::fromUtf8("actionCompile"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/128x128/compile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompile->setIcon(icon16);
        actionEmulator = new QAction(PlatzWin);
        actionEmulator->setObjectName(QString::fromUtf8("actionEmulator"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/128x128/emulator.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEmulator->setIcon(icon17);
        actionPrevious = new QAction(PlatzWin);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/128x128/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon18);
        actionNext = new QAction(PlatzWin);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/128x128/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon19);
        actionAdd = new QAction(PlatzWin);
        actionAdd->setObjectName(QString::fromUtf8("actionAdd"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/128x128/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd->setIcon(icon20);
        actionRemove = new QAction(PlatzWin);
        actionRemove->setObjectName(QString::fromUtf8("actionRemove"));
        actionRemove->setIcon(icon10);
        actionZoomIn = new QAction(PlatzWin);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/128x128/zoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon21);
        actionZoomOut = new QAction(PlatzWin);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/128x128/zoomOut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon22);
        actionSettings = new QAction(PlatzWin);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/128x128/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon23);
        actionMake = new QAction(PlatzWin);
        actionMake->setObjectName(QString::fromUtf8("actionMake"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/128x128/build.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMake->setIcon(icon24);
        actionSelectCursor = new QAction(PlatzWin);
        actionSelectCursor->setObjectName(QString::fromUtf8("actionSelectCursor"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/icons/icons/Pointer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectCursor->setIcon(icon25);
        actionBgOuterCursor = new QAction(PlatzWin);
        actionBgOuterCursor->setObjectName(QString::fromUtf8("actionBgOuterCursor"));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/icons/icons/bgo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBgOuterCursor->setIcon(icon26);
        actionBgInnerCursor = new QAction(PlatzWin);
        actionBgInnerCursor->setObjectName(QString::fromUtf8("actionBgInnerCursor"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/icons/icons/bgi.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBgInnerCursor->setIcon(icon27);
        actionBgObjectCursor = new QAction(PlatzWin);
        actionBgObjectCursor->setObjectName(QString::fromUtf8("actionBgObjectCursor"));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/icons/icons/Object.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBgObjectCursor->setIcon(icon28);
        actionPlatformPathCursor = new QAction(PlatzWin);
        actionPlatformPathCursor->setObjectName(QString::fromUtf8("actionPlatformPathCursor"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/icons/icons/mpPath.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlatformPathCursor->setIcon(icon29);
        actionPlatformCursor = new QAction(PlatzWin);
        actionPlatformCursor->setObjectName(QString::fromUtf8("actionPlatformCursor"));
        QIcon icon30;
        icon30.addFile(QString::fromUtf8(":/icons/icons/mp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlatformCursor->setIcon(icon30);
        actionBgMutableCursor = new QAction(PlatzWin);
        actionBgMutableCursor->setObjectName(QString::fromUtf8("actionBgMutableCursor"));
        QIcon icon31;
        icon31.addFile(QString::fromUtf8(":/icons/icons/bgm.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBgMutableCursor->setIcon(icon31);
        actionReplicate = new QAction(PlatzWin);
        actionReplicate->setObjectName(QString::fromUtf8("actionReplicate"));
        actionLock = new QAction(PlatzWin);
        actionLock->setObjectName(QString::fromUtf8("actionLock"));
        actionFindReplace = new QAction(PlatzWin);
        actionFindReplace->setObjectName(QString::fromUtf8("actionFindReplace"));
        actionBgOrdering = new QAction(PlatzWin);
        actionBgOrdering->setObjectName(QString::fromUtf8("actionBgOrdering"));
        actionCheckForUpdates = new QAction(PlatzWin);
        actionCheckForUpdates->setObjectName(QString::fromUtf8("actionCheckForUpdates"));
        actionInstallUpdates = new QAction(PlatzWin);
        actionInstallUpdates->setObjectName(QString::fromUtf8("actionInstallUpdates"));
        centralWidget = new QWidget(PlatzWin);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_7 = new QHBoxLayout(centralWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        graphicsView = new PlatzGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout_11->addWidget(graphicsView);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 4, -1, -1);
        lblSelectedDetailsIcon = new QLabel(centralWidget);
        lblSelectedDetailsIcon->setObjectName(QString::fromUtf8("lblSelectedDetailsIcon"));
        lblSelectedDetailsIcon->setMinimumSize(QSize(32, 32));
        lblSelectedDetailsIcon->setMaximumSize(QSize(32, 32));
        lblSelectedDetailsIcon->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(lblSelectedDetailsIcon);

        lblSelectedDetails = new QLabel(centralWidget);
        lblSelectedDetails->setObjectName(QString::fromUtf8("lblSelectedDetails"));
        lblSelectedDetails->setMaximumSize(QSize(16777215, 32));

        horizontalLayout_5->addWidget(lblSelectedDetails);

        lblSelectedDetailsImg = new QLabel(centralWidget);
        lblSelectedDetailsImg->setObjectName(QString::fromUtf8("lblSelectedDetailsImg"));
        lblSelectedDetailsImg->setMinimumSize(QSize(32, 32));
        lblSelectedDetailsImg->setMaximumSize(QSize(32, 32));
        lblSelectedDetailsImg->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(lblSelectedDetailsImg);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(64, 32));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        verticalLayout_10 = new QVBoxLayout(frame);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        lblCoords = new QLabel(frame);
        lblCoords->setObjectName(QString::fromUtf8("lblCoords"));
        lblCoords->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lblCoords);

        lblSliceIndex = new QLabel(frame);
        lblSliceIndex->setObjectName(QString::fromUtf8("lblSliceIndex"));
        lblSliceIndex->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lblSliceIndex);


        horizontalLayout_5->addWidget(frame);


        verticalLayout_11->addLayout(horizontalLayout_5);


        horizontalLayout_7->addLayout(verticalLayout_11);

        PlatzWin->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PlatzWin);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 933, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuSnapToX = new QMenu(menuEdit);
        menuSnapToX->setObjectName(QString::fromUtf8("menuSnapToX"));
        menuSnapToY = new QMenu(menuEdit);
        menuSnapToY->setObjectName(QString::fromUtf8("menuSnapToY"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        PlatzWin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PlatzWin);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setLayoutDirection(Qt::LeftToRight);
        mainToolBar->setIconSize(QSize(48, 48));
        PlatzWin->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PlatzWin);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PlatzWin->setStatusBar(statusBar);
        dockWidgetUtilities = new QDockWidget(PlatzWin);
        dockWidgetUtilities->setObjectName(QString::fromUtf8("dockWidgetUtilities"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidgetUtilities->sizePolicy().hasHeightForWidth());
        dockWidgetUtilities->setSizePolicy(sizePolicy);
        dockWidgetUtilities->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 2, -1, 2);
        tabWidgetUtilities = new QTabWidget(dockWidgetContents);
        tabWidgetUtilities->setObjectName(QString::fromUtf8("tabWidgetUtilities"));
        tabTiles = new QWidget();
        tabTiles->setObjectName(QString::fromUtf8("tabTiles"));
        verticalLayout_12 = new QVBoxLayout(tabTiles);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        tileListWidget = new PlatzGraphicsList(tabTiles);
        tileListWidget->setObjectName(QString::fromUtf8("tileListWidget"));
        tileListWidget->setViewMode(QListView::IconMode);

        verticalLayout_12->addWidget(tileListWidget);

        tabWidgetUtilities->addTab(tabTiles, QString());
        tabMaps = new QWidget();
        tabMaps->setObjectName(QString::fromUtf8("tabMaps"));
        verticalLayout_14 = new QVBoxLayout(tabMaps);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        mapListWidget = new PlatzGraphicsList(tabMaps);
        mapListWidget->setObjectName(QString::fromUtf8("mapListWidget"));
        mapListWidget->setViewMode(QListView::IconMode);

        verticalLayout_14->addWidget(mapListWidget);

        tabWidgetUtilities->addTab(tabMaps, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_5 = new QVBoxLayout(tab);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        animListWidget = new PlatzGraphicsList(tab);
        animListWidget->setObjectName(QString::fromUtf8("animListWidget"));
        animListWidget->setViewMode(QListView::IconMode);

        verticalLayout_5->addWidget(animListWidget);

        tabWidgetUtilities->addTab(tab, QString());
        tabMake = new QWidget();
        tabMake->setObjectName(QString::fromUtf8("tabMake"));
        horizontalLayout = new QHBoxLayout(tabMake);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        txtMakeOutput = new QPlainTextEdit(tabMake);
        txtMakeOutput->setObjectName(QString::fromUtf8("txtMakeOutput"));

        horizontalLayout->addWidget(txtMakeOutput);

        tabWidgetUtilities->addTab(tabMake, QString());
        tabEmu = new QWidget();
        tabEmu->setObjectName(QString::fromUtf8("tabEmu"));
        horizontalLayout_2 = new QHBoxLayout(tabEmu);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        txtEmuOutput = new QPlainTextEdit(tabEmu);
        txtEmuOutput->setObjectName(QString::fromUtf8("txtEmuOutput"));

        horizontalLayout_2->addWidget(txtEmuOutput);

        tabWidgetUtilities->addTab(tabEmu, QString());

        verticalLayout->addWidget(tabWidgetUtilities);

        dockWidgetUtilities->setWidget(dockWidgetContents);
        PlatzWin->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetUtilities);
        dockWidgetWorldDetails = new QDockWidget(PlatzWin);
        dockWidgetWorldDetails->setObjectName(QString::fromUtf8("dockWidgetWorldDetails"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(9, -1, -1, 9);
        treeView = new PlatzTreeView(dockWidgetContents_2);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        verticalLayout_3->addWidget(treeView);

        dockWidgetWorldDetails->setWidget(dockWidgetContents_2);
        PlatzWin->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetWorldDetails);
        dockWidgetToolbox = new QDockWidget(PlatzWin);
        dockWidgetToolbox->setObjectName(QString::fromUtf8("dockWidgetToolbox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidgetToolbox->sizePolicy().hasHeightForWidth());
        dockWidgetToolbox->setSizePolicy(sizePolicy1);
        dockWidgetToolbox->setMinimumSize(QSize(220, 378));
        dockWidgetToolbox->setMaximumSize(QSize(220, 380));
        dockWidgetToolbox->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        verticalLayout_16 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        groupBox = new QGroupBox(dockWidgetContents_7);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 172));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tbtnPointer = new QToolButton(groupBox);
        tbtnPointer->setObjectName(QString::fromUtf8("tbtnPointer"));
        tbtnPointer->setIconSize(QSize(32, 32));
        tbtnPointer->setAutoRaise(true);

        gridLayout->addWidget(tbtnPointer, 0, 0, 1, 1);

        tbtnBgo = new QToolButton(groupBox);
        tbtnBgo->setObjectName(QString::fromUtf8("tbtnBgo"));
        tbtnBgo->setAutoFillBackground(false);
        tbtnBgo->setIconSize(QSize(32, 32));
        tbtnBgo->setAutoRaise(true);

        gridLayout->addWidget(tbtnBgo, 0, 1, 1, 1);

        tbtnBgi = new QToolButton(groupBox);
        tbtnBgi->setObjectName(QString::fromUtf8("tbtnBgi"));
        tbtnBgi->setIconSize(QSize(32, 32));
        tbtnBgi->setAutoRaise(true);

        gridLayout->addWidget(tbtnBgi, 0, 2, 1, 1);

        tbtnObj = new QToolButton(groupBox);
        tbtnObj->setObjectName(QString::fromUtf8("tbtnObj"));
        tbtnObj->setIconSize(QSize(32, 32));
        tbtnObj->setAutoRaise(true);

        gridLayout->addWidget(tbtnObj, 1, 0, 1, 1);

        tbtnMpp = new QToolButton(groupBox);
        tbtnMpp->setObjectName(QString::fromUtf8("tbtnMpp"));
        tbtnMpp->setIconSize(QSize(32, 32));
        tbtnMpp->setAutoRaise(true);

        gridLayout->addWidget(tbtnMpp, 1, 1, 1, 1);

        tbtnMp = new QToolButton(groupBox);
        tbtnMp->setObjectName(QString::fromUtf8("tbtnMp"));
        tbtnMp->setIconSize(QSize(32, 32));
        tbtnMp->setAutoRaise(true);

        gridLayout->addWidget(tbtnMp, 1, 2, 1, 1);

        tbtnBgm = new QToolButton(groupBox);
        tbtnBgm->setObjectName(QString::fromUtf8("tbtnBgm"));
        tbtnBgm->setIconSize(QSize(32, 32));
        tbtnBgm->setAutoRaise(true);

        gridLayout->addWidget(tbtnBgm, 2, 0, 1, 1);


        verticalLayout_16->addWidget(groupBox);

        stackedWidget = new QStackedWidget(dockWidgetContents_7);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(0, 155));
        stackedWidget->setMaximumSize(QSize(200, 160));
        stackedWidget->setFrameShape(QFrame::Box);
        stackedWidget->setFrameShadow(QFrame::Sunken);
        pageBlank = new QWidget();
        pageBlank->setObjectName(QString::fromUtf8("pageBlank"));
        stackedWidget->addWidget(pageBlank);
        pageOuter = new QWidget();
        pageOuter->setObjectName(QString::fromUtf8("pageOuter"));
        verticalLayout_7 = new QVBoxLayout(pageOuter);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox_4 = new QGroupBox(pageOuter);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_21 = new QVBoxLayout(groupBox_4);
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setContentsMargins(11, 11, 11, 11);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalSpacer_4 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_4);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        chkBgc = new QCheckBox(groupBox_4);
        chkBgc->setObjectName(QString::fromUtf8("chkBgc"));

        verticalLayout_19->addWidget(chkBgc);

        chkBgmo = new QCheckBox(groupBox_4);
        chkBgmo->setObjectName(QString::fromUtf8("chkBgmo"));

        verticalLayout_19->addWidget(chkBgmo);

        chkBgq = new QCheckBox(groupBox_4);
        chkBgq->setObjectName(QString::fromUtf8("chkBgq"));

        verticalLayout_19->addWidget(chkBgq);


        horizontalLayout_14->addLayout(verticalLayout_19);

        horizontalSpacer_5 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_5);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        chkBgi = new QCheckBox(groupBox_4);
        chkBgi->setObjectName(QString::fromUtf8("chkBgi"));

        verticalLayout_18->addWidget(chkBgi);

        pbBgt = new QPushButton(groupBox_4);
        pbBgt->setObjectName(QString::fromUtf8("pbBgt"));
        pbBgt->setMinimumSize(QSize(41, 21));
        pbBgt->setMaximumSize(QSize(41, 21));

        verticalLayout_18->addWidget(pbBgt);

        pbClear = new QPushButton(groupBox_4);
        pbClear->setObjectName(QString::fromUtf8("pbClear"));
        pbClear->setMinimumSize(QSize(41, 21));
        pbClear->setMaximumSize(QSize(41, 21));

        verticalLayout_18->addWidget(pbClear);


        horizontalLayout_14->addLayout(verticalLayout_18);

        horizontalSpacer_6 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_6);


        verticalLayout_21->addLayout(horizontalLayout_14);


        verticalLayout_7->addWidget(groupBox_4);

        stackedWidget->addWidget(pageOuter);
        pageInner = new QWidget();
        pageInner->setObjectName(QString::fromUtf8("pageInner"));
        gridLayout_2 = new QGridLayout(pageInner);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_18 = new QSpacerItem(12, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_18, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        rbtnTile = new QRadioButton(pageInner);
        rbtnTile->setObjectName(QString::fromUtf8("rbtnTile"));
        rbtnTile->setChecked(true);

        verticalLayout_4->addWidget(rbtnTile);

        rbtnBgp = new QRadioButton(pageInner);
        rbtnBgp->setObjectName(QString::fromUtf8("rbtnBgp"));

        verticalLayout_4->addWidget(rbtnBgp);

        rbtnBga = new QRadioButton(pageInner);
        rbtnBga->setObjectName(QString::fromUtf8("rbtnBga"));

        verticalLayout_4->addWidget(rbtnBga);


        gridLayout_2->addLayout(verticalLayout_4, 0, 1, 2, 1);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        chkBgmi = new QCheckBox(pageInner);
        chkBgmi->setObjectName(QString::fromUtf8("chkBgmi"));

        horizontalLayout_4->addWidget(chkBgmi);

        horizontalSpacer_2 = new QSpacerItem(14, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout_4, 0, 3, 1, 1);

        horizontalSpacer_17 = new QSpacerItem(12, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_17, 1, 0, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 1, 2, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        chkBgmc = new QCheckBox(pageInner);
        chkBgmc->setObjectName(QString::fromUtf8("chkBgmc"));

        horizontalLayout_11->addWidget(chkBgmc);

        horizontalSpacer_9 = new QSpacerItem(14, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_9);


        gridLayout_2->addLayout(horizontalLayout_11, 1, 3, 1, 1);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(2);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        lblBgmc = new QLabel(pageInner);
        lblBgmc->setObjectName(QString::fromUtf8("lblBgmc"));

        verticalLayout_17->addWidget(lblBgmc);

        cboBgmc = new LePlatzComboBox(pageInner);
        cboBgmc->setObjectName(QString::fromUtf8("cboBgmc"));
        cboBgmc->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_17->addWidget(cboBgmc);


        gridLayout_2->addLayout(verticalLayout_17, 2, 0, 1, 4);

        stackedWidget->addWidget(pageInner);
        pageMutable = new QWidget();
        pageMutable->setObjectName(QString::fromUtf8("pageMutable"));
        verticalLayout_2 = new QVBoxLayout(pageMutable);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(2);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        lblBgm = new QLabel(pageMutable);
        lblBgm->setObjectName(QString::fromUtf8("lblBgm"));

        verticalLayout_9->addWidget(lblBgm);

        cboBgm = new LePlatzComboBox(pageMutable);
        cboBgm->setObjectName(QString::fromUtf8("cboBgm"));
        cboBgm->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_9->addWidget(cboBgm);


        verticalLayout_2->addLayout(verticalLayout_9);

        chkPayload = new QCheckBox(pageMutable);
        chkPayload->setObjectName(QString::fromUtf8("chkPayload"));

        verticalLayout_2->addWidget(chkPayload);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(1);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        lblMutLeft = new QLabel(pageMutable);
        lblMutLeft->setObjectName(QString::fromUtf8("lblMutLeft"));

        horizontalLayout_12->addWidget(lblMutLeft);

        leMutLeft = new QLineEdit(pageMutable);
        leMutLeft->setObjectName(QString::fromUtf8("leMutLeft"));
        sizePolicy1.setHeightForWidth(leMutLeft->sizePolicy().hasHeightForWidth());
        leMutLeft->setSizePolicy(sizePolicy1);
        leMutLeft->setMaximumSize(QSize(40, 20));
        leMutLeft->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(leMutLeft);


        horizontalLayout_15->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        lblMutRight = new QLabel(pageMutable);
        lblMutRight->setObjectName(QString::fromUtf8("lblMutRight"));

        horizontalLayout_13->addWidget(lblMutRight);

        leMutRight = new QLineEdit(pageMutable);
        leMutRight->setObjectName(QString::fromUtf8("leMutRight"));
        sizePolicy1.setHeightForWidth(leMutRight->sizePolicy().hasHeightForWidth());
        leMutRight->setSizePolicy(sizePolicy1);
        leMutRight->setMaximumSize(QSize(40, 20));
        leMutRight->setAlignment(Qt::AlignCenter);

        horizontalLayout_13->addWidget(leMutRight);


        horizontalLayout_15->addLayout(horizontalLayout_13);


        verticalLayout_15->addLayout(horizontalLayout_15);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        lblMutTop = new QLabel(pageMutable);
        lblMutTop->setObjectName(QString::fromUtf8("lblMutTop"));

        horizontalLayout_20->addWidget(lblMutTop);

        leMutTop = new QLineEdit(pageMutable);
        leMutTop->setObjectName(QString::fromUtf8("leMutTop"));
        sizePolicy1.setHeightForWidth(leMutTop->sizePolicy().hasHeightForWidth());
        leMutTop->setSizePolicy(sizePolicy1);
        leMutTop->setMaximumSize(QSize(40, 20));
        leMutTop->setAlignment(Qt::AlignCenter);

        horizontalLayout_20->addWidget(leMutTop);


        horizontalLayout_19->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        lblMutBtm = new QLabel(pageMutable);
        lblMutBtm->setObjectName(QString::fromUtf8("lblMutBtm"));

        horizontalLayout_21->addWidget(lblMutBtm);

        leMutBottom = new QLineEdit(pageMutable);
        leMutBottom->setObjectName(QString::fromUtf8("leMutBottom"));
        sizePolicy1.setHeightForWidth(leMutBottom->sizePolicy().hasHeightForWidth());
        leMutBottom->setSizePolicy(sizePolicy1);
        leMutBottom->setMaximumSize(QSize(40, 20));
        leMutBottom->setAlignment(Qt::AlignCenter);

        horizontalLayout_21->addWidget(leMutBottom);


        horizontalLayout_19->addLayout(horizontalLayout_21);


        verticalLayout_15->addLayout(horizontalLayout_19);


        verticalLayout_2->addLayout(verticalLayout_15);

        stackedWidget->addWidget(pageMutable);
        pagePlatformPath = new QWidget();
        pagePlatformPath->setObjectName(QString::fromUtf8("pagePlatformPath"));
        verticalLayout_20 = new QVBoxLayout(pagePlatformPath);
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setContentsMargins(11, 11, 11, 11);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        verticalSpacer_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_2);

        groupBox_6 = new QGroupBox(pagePlatformPath);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_8 = new QHBoxLayout(groupBox_6);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(9, -1, 0, -1);
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_10);

        rbtnXAxis = new QRadioButton(groupBox_6);
        rbtnXAxis->setObjectName(QString::fromUtf8("rbtnXAxis"));
        rbtnXAxis->setChecked(true);

        horizontalLayout_8->addWidget(rbtnXAxis);

        rbtnYAxis = new QRadioButton(groupBox_6);
        rbtnYAxis->setObjectName(QString::fromUtf8("rbtnYAxis"));

        horizontalLayout_8->addWidget(rbtnYAxis);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_11);


        verticalLayout_20->addWidget(groupBox_6);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        lblPlatformClearTile = new QLabel(pagePlatformPath);
        lblPlatformClearTile->setObjectName(QString::fromUtf8("lblPlatformClearTile"));

        verticalLayout_8->addWidget(lblPlatformClearTile);

        cboPlatformClearTile = new LePlatzComboBox(pagePlatformPath);
        cboPlatformClearTile->setObjectName(QString::fromUtf8("cboPlatformClearTile"));

        verticalLayout_8->addWidget(cboPlatformClearTile);


        verticalLayout_20->addLayout(verticalLayout_8);

        verticalSpacer_3 = new QSpacerItem(17, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_3);

        stackedWidget->addWidget(pagePlatformPath);
        pagePlatform = new QWidget();
        pagePlatform->setObjectName(QString::fromUtf8("pagePlatform"));
        verticalLayout_6 = new QVBoxLayout(pagePlatform);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_7 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_7);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_12);

        rbtnSmooth = new QRadioButton(pagePlatform);
        rbtnSmooth->setObjectName(QString::fromUtf8("rbtnSmooth"));
        rbtnSmooth->setChecked(true);

        horizontalLayout_3->addWidget(rbtnSmooth);

        rbtnStepped = new QRadioButton(pagePlatform);
        rbtnStepped->setObjectName(QString::fromUtf8("rbtnStepped"));

        horizontalLayout_3->addWidget(rbtnStepped);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_13);


        verticalLayout_6->addLayout(horizontalLayout_3);

        verticalSpacer_10 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_10);

        line = new QFrame(pagePlatform);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line);

        verticalSpacer_11 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_11);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        lblPlatformSpeed = new QLabel(pagePlatform);
        lblPlatformSpeed->setObjectName(QString::fromUtf8("lblPlatformSpeed"));

        horizontalLayout_6->addWidget(lblPlatformSpeed);

        spbPlatformSpeed = new LePlatzSpinBox(pagePlatform);
        spbPlatformSpeed->setObjectName(QString::fromUtf8("spbPlatformSpeed"));
        sizePolicy1.setHeightForWidth(spbPlatformSpeed->sizePolicy().hasHeightForWidth());
        spbPlatformSpeed->setSizePolicy(sizePolicy1);
        spbPlatformSpeed->setMinimum(-32);
        spbPlatformSpeed->setMaximum(32);
        spbPlatformSpeed->setValue(1);

        horizontalLayout_6->addWidget(spbPlatformSpeed);


        horizontalLayout_16->addLayout(horizontalLayout_6);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_16);


        verticalLayout_6->addLayout(horizontalLayout_16);

        verticalSpacer_6 = new QSpacerItem(20, 9, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_6);

        stackedWidget->addWidget(pagePlatform);
        pageTrigger = new QWidget();
        pageTrigger->setObjectName(QString::fromUtf8("pageTrigger"));
        verticalLayout_13 = new QVBoxLayout(pageTrigger);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalSpacer_9 = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_9);

        groupBox_7 = new QGroupBox(pageTrigger);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_9 = new QHBoxLayout(groupBox_7);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_14);

        rbtnLrud = new QRadioButton(groupBox_7);
        rbtnLrud->setObjectName(QString::fromUtf8("rbtnLrud"));
        rbtnLrud->setChecked(true);

        horizontalLayout_9->addWidget(rbtnLrud);

        rbtnRldu = new QRadioButton(groupBox_7);
        rbtnRldu->setObjectName(QString::fromUtf8("rbtnRldu"));

        horizontalLayout_9->addWidget(rbtnRldu);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_15);


        verticalLayout_13->addWidget(groupBox_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        lblBgt = new QLabel(pageTrigger);
        lblBgt->setObjectName(QString::fromUtf8("lblBgt"));

        horizontalLayout_10->addWidget(lblBgt);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);

        pbBgtNot = new QPushButton(pageTrigger);
        pbBgtNot->setObjectName(QString::fromUtf8("pbBgtNot"));
        pbBgtNot->setMinimumSize(QSize(41, 21));
        pbBgtNot->setMaximumSize(QSize(41, 21));

        horizontalLayout_10->addWidget(pbBgtNot);


        verticalLayout_13->addLayout(horizontalLayout_10);

        cboBgt = new LePlatzComboBox(pageTrigger);
        cboBgt->setObjectName(QString::fromUtf8("cboBgt"));
        cboBgt->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_13->addWidget(cboBgt);

        verticalSpacer_8 = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_8);

        stackedWidget->addWidget(pageTrigger);

        verticalLayout_16->addWidget(stackedWidget);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_16->addItem(verticalSpacer);

        dockWidgetToolbox->setWidget(dockWidgetContents_7);
        PlatzWin->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetToolbox);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionSelectAll);
        menuEdit->addSeparator();
        menuEdit->addAction(menuSnapToX->menuAction());
        menuEdit->addAction(menuSnapToY->menuAction());
        menuEdit->addSeparator();
        menuEdit->addAction(actionFindReplace);
        menuAbout->addAction(actionAboutLePlatz);
        menuAbout->addAction(actionAboutPlatz);
        menuAbout->addAction(actionAboutUzebox);
        menuAbout->addSeparator();
        menuAbout->addAction(actionCheckForUpdates);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionReload);
        mainToolBar->addAction(actionSettings);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoomIn);
        mainToolBar->addAction(actionZoomOut);
        mainToolBar->addAction(actionAdd);
        mainToolBar->addAction(actionRemove);
        mainToolBar->addAction(actionPrevious);
        mainToolBar->addAction(actionNext);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionLock);
        mainToolBar->addAction(actionReplicate);
        mainToolBar->addAction(actionBgOrdering);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCompile);
        mainToolBar->addAction(actionMake);
        mainToolBar->addAction(actionEmulator);
        mainToolBar->addAction(actionInstallUpdates);

        retranslateUi(PlatzWin);

        tabWidgetUtilities->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PlatzWin);
    } // setupUi

    void retranslateUi(QMainWindow *PlatzWin)
    {
        PlatzWin->setWindowTitle(QApplication::translate("PlatzWin", "LePlatz", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("PlatzWin", "New", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("PlatzWin", "New Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionOpen->setText(QApplication::translate("PlatzWin", "Open...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("PlatzWin", "Open Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSaveAs->setText(QApplication::translate("PlatzWin", "Save As...", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("PlatzWin", "Close", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionClose->setToolTip(QApplication::translate("PlatzWin", "Close Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("PlatzWin", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("PlatzWin", "Exit LePlatz", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUndo->setText(QApplication::translate("PlatzWin", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionUndo->setToolTip(QApplication::translate("PlatzWin", "Undo Last Action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRedo->setText(QApplication::translate("PlatzWin", "Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRedo->setToolTip(QApplication::translate("PlatzWin", "Redo Last Action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCut->setText(QApplication::translate("PlatzWin", "Cut", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("PlatzWin", "Copy", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("PlatzWin", "Paste", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("PlatzWin", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDelete->setToolTip(QApplication::translate("PlatzWin", "Delete Selected", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSelectAll->setText(QApplication::translate("PlatzWin", "Select All", 0, QApplication::UnicodeUTF8));
        actionAboutLePlatz->setText(QApplication::translate("PlatzWin", "LePlatz...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAboutLePlatz->setToolTip(QApplication::translate("PlatzWin", "About LePlatz", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAboutPlatz->setText(QApplication::translate("PlatzWin", "Platz...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAboutPlatz->setToolTip(QApplication::translate("PlatzWin", "About Platz", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAboutUzebox->setText(QApplication::translate("PlatzWin", "Uzebox...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAboutUzebox->setToolTip(QApplication::translate("PlatzWin", "About Uzebox", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("PlatzWin", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("PlatzWin", "Save Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReload->setText(QApplication::translate("PlatzWin", "Reload", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReload->setToolTip(QApplication::translate("PlatzWin", "Reload World Resources", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCompile->setText(QApplication::translate("PlatzWin", "Compile", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCompile->setToolTip(QApplication::translate("PlatzWin", "Compile World", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEmulator->setText(QApplication::translate("PlatzWin", "Emulator", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEmulator->setToolTip(QApplication::translate("PlatzWin", "Play in Emulator", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrevious->setText(QApplication::translate("PlatzWin", "Previous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("PlatzWin", "Go to previous slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrevious->setShortcut(QApplication::translate("PlatzWin", "Ctrl+,", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("PlatzWin", "Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("PlatzWin", "Go to next slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionNext->setShortcut(QApplication::translate("PlatzWin", "Ctrl+.", 0, QApplication::UnicodeUTF8));
        actionAdd->setText(QApplication::translate("PlatzWin", "Add", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAdd->setToolTip(QApplication::translate("PlatzWin", "Insert Slice (before: left button; after: right button)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRemove->setText(QApplication::translate("PlatzWin", "Remove", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRemove->setToolTip(QApplication::translate("PlatzWin", "Remove Slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomIn->setText(QApplication::translate("PlatzWin", "Zoom In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomIn->setToolTip(QApplication::translate("PlatzWin", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomOut->setText(QApplication::translate("PlatzWin", "Zoom Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomOut->setToolTip(QApplication::translate("PlatzWin", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSettings->setText(QApplication::translate("PlatzWin", "Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSettings->setToolTip(QApplication::translate("PlatzWin", "Edit project settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionMake->setText(QApplication::translate("PlatzWin", "Make All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMake->setToolTip(QApplication::translate("PlatzWin", "Build Hex", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSelectCursor->setText(QApplication::translate("PlatzWin", "selectCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSelectCursor->setToolTip(QApplication::translate("PlatzWin", "Select Bg Elements", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBgOuterCursor->setText(QApplication::translate("PlatzWin", "bgOuterCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBgOuterCursor->setToolTip(QApplication::translate("PlatzWin", "New Outer BG", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBgInnerCursor->setText(QApplication::translate("PlatzWin", "bgInnerCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBgInnerCursor->setToolTip(QApplication::translate("PlatzWin", "New Inner BG", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBgObjectCursor->setText(QApplication::translate("PlatzWin", "bgObjectCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBgObjectCursor->setToolTip(QApplication::translate("PlatzWin", "New BG Object", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPlatformPathCursor->setText(QApplication::translate("PlatzWin", "platformPathCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPlatformPathCursor->setToolTip(QApplication::translate("PlatzWin", "New Moving Platform Path", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPlatformCursor->setText(QApplication::translate("PlatzWin", "platformCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPlatformCursor->setToolTip(QApplication::translate("PlatzWin", "New Moving Platform", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBgMutableCursor->setText(QApplication::translate("PlatzWin", "bgMutableCursor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBgMutableCursor->setToolTip(QApplication::translate("PlatzWin", "New Mutable BG", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReplicate->setText(QApplication::translate("PlatzWin", "Replicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReplicate->setToolTip(QApplication::translate("PlatzWin", "Replicate Slice to selected (Copy: left button; Assign replica: right button). Assign to self to undo replication link.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLock->setText(QApplication::translate("PlatzWin", "Un/Lock", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLock->setToolTip(QApplication::translate("PlatzWin", "Lock/Unlock BG Ordering for selected slice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFindReplace->setText(QApplication::translate("PlatzWin", "Find/Replace...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFindReplace->setToolTip(QApplication::translate("PlatzWin", "Find/Replace Src Defines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBgOrdering->setText(QApplication::translate("PlatzWin", "Bg Order", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBgOrdering->setToolTip(QApplication::translate("PlatzWin", "Sort unlocked slice (left-to-right)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCheckForUpdates->setText(QApplication::translate("PlatzWin", "Check for updates...", 0, QApplication::UnicodeUTF8));
        actionInstallUpdates->setText(QApplication::translate("PlatzWin", "Install Updates", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionInstallUpdates->setToolTip(QApplication::translate("PlatzWin", "Install Updates", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblSelectedDetailsIcon->setText(QString());
        lblSelectedDetails->setText(QString());
        lblSelectedDetailsImg->setText(QString());
        lblCoords->setText(QString());
        lblSliceIndex->setText(QString());
        menuFile->setTitle(QApplication::translate("PlatzWin", "&File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("PlatzWin", "Edit", 0, QApplication::UnicodeUTF8));
        menuSnapToX->setTitle(QApplication::translate("PlatzWin", "Snap to X", 0, QApplication::UnicodeUTF8));
        menuSnapToY->setTitle(QApplication::translate("PlatzWin", "Snap to Y", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("PlatzWin", "About", 0, QApplication::UnicodeUTF8));
        dockWidgetUtilities->setWindowTitle(QApplication::translate("PlatzWin", "Utilities", 0, QApplication::UnicodeUTF8));
        tabWidgetUtilities->setTabText(tabWidgetUtilities->indexOf(tabTiles), QApplication::translate("PlatzWin", "Tiles", 0, QApplication::UnicodeUTF8));
        tabWidgetUtilities->setTabText(tabWidgetUtilities->indexOf(tabMaps), QApplication::translate("PlatzWin", "Maps", 0, QApplication::UnicodeUTF8));
        tabWidgetUtilities->setTabText(tabWidgetUtilities->indexOf(tab), QApplication::translate("PlatzWin", "Animations", 0, QApplication::UnicodeUTF8));
        tabWidgetUtilities->setTabText(tabWidgetUtilities->indexOf(tabMake), QApplication::translate("PlatzWin", "Make Output", 0, QApplication::UnicodeUTF8));
        tabWidgetUtilities->setTabText(tabWidgetUtilities->indexOf(tabEmu), QApplication::translate("PlatzWin", "Emulator Output", 0, QApplication::UnicodeUTF8));
        dockWidgetWorldDetails->setWindowTitle(QApplication::translate("PlatzWin", "World Details", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PlatzWin", "World Interaction Tools", 0, QApplication::UnicodeUTF8));
        tbtnPointer->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnBgo->setStyleSheet(QString());
        tbtnBgo->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnBgi->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnObj->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnMpp->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnMp->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        tbtnBgm->setText(QApplication::translate("PlatzWin", "...", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        chkBgc->setText(QApplication::translate("PlatzWin", "BGC", 0, QApplication::UnicodeUTF8));
        chkBgmo->setText(QApplication::translate("PlatzWin", "BGM", 0, QApplication::UnicodeUTF8));
        chkBgq->setText(QApplication::translate("PlatzWin", "BGQ", 0, QApplication::UnicodeUTF8));
        chkBgi->setText(QApplication::translate("PlatzWin", "BGI", 0, QApplication::UnicodeUTF8));
        pbBgt->setText(QApplication::translate("PlatzWin", "BGT", 0, QApplication::UnicodeUTF8));
        pbClear->setText(QApplication::translate("PlatzWin", "Clear", 0, QApplication::UnicodeUTF8));
        rbtnTile->setText(QApplication::translate("PlatzWin", "Tile", 0, QApplication::UnicodeUTF8));
        rbtnBgp->setText(QApplication::translate("PlatzWin", "BGP", 0, QApplication::UnicodeUTF8));
        rbtnBga->setText(QApplication::translate("PlatzWin", "BGA", 0, QApplication::UnicodeUTF8));
        chkBgmi->setText(QApplication::translate("PlatzWin", "BGM", 0, QApplication::UnicodeUTF8));
        chkBgmc->setText(QApplication::translate("PlatzWin", "BGMC", 0, QApplication::UnicodeUTF8));
        lblBgmc->setText(QApplication::translate("PlatzWin", "Mutable Class Ids:", 0, QApplication::UnicodeUTF8));
        lblBgm->setText(QApplication::translate("PlatzWin", "Mutable Bg Ids:", 0, QApplication::UnicodeUTF8));
        chkPayload->setText(QApplication::translate("PlatzWin", "Custom Payload", 0, QApplication::UnicodeUTF8));
        lblMutLeft->setText(QApplication::translate("PlatzWin", "Left:", 0, QApplication::UnicodeUTF8));
        leMutLeft->setInputMask(QString());
        leMutLeft->setText(QString());
        lblMutRight->setText(QApplication::translate("PlatzWin", "Right:", 0, QApplication::UnicodeUTF8));
        leMutRight->setInputMask(QString());
        leMutRight->setText(QString());
        lblMutTop->setText(QApplication::translate("PlatzWin", "Top:", 0, QApplication::UnicodeUTF8));
        leMutTop->setInputMask(QString());
        leMutTop->setText(QString());
        lblMutBtm->setText(QApplication::translate("PlatzWin", "Btm:", 0, QApplication::UnicodeUTF8));
        leMutBottom->setInputMask(QString());
        leMutBottom->setText(QString());
        groupBox_6->setTitle(QString());
        rbtnXAxis->setText(QApplication::translate("PlatzWin", "X Axis", 0, QApplication::UnicodeUTF8));
        rbtnYAxis->setText(QApplication::translate("PlatzWin", "Y Axis", 0, QApplication::UnicodeUTF8));
        lblPlatformClearTile->setText(QApplication::translate("PlatzWin", "Clear Tile:", 0, QApplication::UnicodeUTF8));
        rbtnSmooth->setText(QApplication::translate("PlatzWin", "Smooth", 0, QApplication::UnicodeUTF8));
        rbtnStepped->setText(QApplication::translate("PlatzWin", "Stepped", 0, QApplication::UnicodeUTF8));
        lblPlatformSpeed->setText(QApplication::translate("PlatzWin", "Speed   ", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QString());
        rbtnLrud->setText(QApplication::translate("PlatzWin", "LRUD", 0, QApplication::UnicodeUTF8));
        rbtnRldu->setText(QApplication::translate("PlatzWin", "RLDU", 0, QApplication::UnicodeUTF8));
        lblBgt->setText(QApplication::translate("PlatzWin", "Trigger Ids:", 0, QApplication::UnicodeUTF8));
        pbBgtNot->setText(QApplication::translate("PlatzWin", "~BGT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlatzWin: public Ui_PlatzWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATZWIN_H
