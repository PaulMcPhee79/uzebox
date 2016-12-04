/********************************************************************************
** Form generated from reading UI file 'PlatzSettings.ui'
**
** Created: Mon 1. Feb 22:43:56 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLATZSETTINGS_H
#define UI_PLATZSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_7;
    QSplitter *splitter;
    QTreeWidget *treeWidget;
    QFrame *frameSettings;
    QVBoxLayout *verticalLayout_8;
    QStackedWidget *stackedWidget;
    QWidget *pageProjectSettings;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QTreeWidget *twPs;
    QStackedWidget *swPs;
    QWidget *page_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblOffsetY;
    QSpinBox *spbOffsetY;
    QSpacerItem *horizontalSpacer;
    QWidget *page;
    QWidget *page_11;
    QHBoxLayout *horizontalLayout_21;
    QLabel *lblSlicePath;
    QLineEdit *leSlicePath;
    QPushButton *pbSlicePath;
    QWidget *page_12;
    QHBoxLayout *horizontalLayout_22;
    QLabel *lblTilePath;
    QLineEdit *leTilePath;
    QPushButton *pbTilePath;
    QWidget *page_13;
    QHBoxLayout *horizontalLayout_23;
    QLabel *lblMapPath;
    QLineEdit *leMapPath;
    QPushButton *pbMapPath;
    QWidget *page_14;
    QHBoxLayout *horizontalLayout_24;
    QLabel *lblAnimPath;
    QLineEdit *leAnimPath;
    QPushButton *pbAnimPath;
    QWidget *page_15;
    QHBoxLayout *horizontalLayout_25;
    QLabel *lblPlatzPath;
    QLineEdit *lePlatzPath;
    QPushButton *pbPlatzPath;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblProjectSrcFolder;
    QLineEdit *leSrcFolder;
    QPushButton *pbSrcFolder;
    QWidget *page_7;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lblProjectMakefilePath;
    QLineEdit *leMakefilePath;
    QPushButton *pbMakefilePath;
    QWidget *page_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lblProjectHexPath;
    QLineEdit *leHexfilePath;
    QPushButton *pbHexfilePath;
    QWidget *pageLePlatzSettings;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *twLps;
    QStackedWidget *swLps;
    QWidget *page_9;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lblLePlatzMakePath;
    QLineEdit *leMakeExePath;
    QPushButton *pbMakeExePath;
    QWidget *page_10;
    QHBoxLayout *horizontalLayout_7;
    QLabel *lblLePlatzEmuPath;
    QLineEdit *leEmuExePath;
    QPushButton *pbEmuExePath;
    QWidget *page_4;
    QVBoxLayout *verticalLayout;
    QLabel *lblUpdatesUrl;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *leUpdatesUrl;
    QPushButton *pbUpdatesUrlReset;
    QWidget *page_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *lblCanvasColor;
    QToolButton *tbtnCanvasColor;
    QSpacerItem *horizontalSpacer_3;
    QFrame *line;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_20;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pbOk;
    QPushButton *pbCancel;
    QPushButton *pbApply;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(630, 414);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SettingsDialog->sizePolicy().hasHeightForWidth());
        SettingsDialog->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(SettingsDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        splitter = new QSplitter(SettingsDialog);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Horizontal);
        treeWidget = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->setAutoExpandDelay(-1);
        splitter->addWidget(treeWidget);
        treeWidget->header()->setVisible(false);
        frameSettings = new QFrame(splitter);
        frameSettings->setObjectName(QString::fromUtf8("frameSettings"));
        sizePolicy.setHeightForWidth(frameSettings->sizePolicy().hasHeightForWidth());
        frameSettings->setSizePolicy(sizePolicy);
        frameSettings->setFrameShape(QFrame::NoFrame);
        frameSettings->setFrameShadow(QFrame::Plain);
        frameSettings->setLineWidth(0);
        verticalLayout_8 = new QVBoxLayout(frameSettings);
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        stackedWidget = new QStackedWidget(frameSettings);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        pageProjectSettings = new QWidget();
        pageProjectSettings->setObjectName(QString::fromUtf8("pageProjectSettings"));
        sizePolicy.setHeightForWidth(pageProjectSettings->sizePolicy().hasHeightForWidth());
        pageProjectSettings->setSizePolicy(sizePolicy);
        verticalLayout_10 = new QVBoxLayout(pageProjectSettings);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        groupBox = new QGroupBox(pageProjectSettings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        twPs = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        twPs->setHeaderItem(__qtreewidgetitem1);
        twPs->setObjectName(QString::fromUtf8("twPs"));
        twPs->setItemsExpandable(false);
        twPs->setColumnCount(1);
        twPs->header()->setVisible(false);

        verticalLayout_5->addWidget(twPs);

        swPs = new QStackedWidget(groupBox);
        swPs->setObjectName(QString::fromUtf8("swPs"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(swPs->sizePolicy().hasHeightForWidth());
        swPs->setSizePolicy(sizePolicy2);
        swPs->setMinimumSize(QSize(322, 70));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        horizontalLayout_2 = new QHBoxLayout(page_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblOffsetY = new QLabel(page_3);
        lblOffsetY->setObjectName(QString::fromUtf8("lblOffsetY"));

        horizontalLayout_2->addWidget(lblOffsetY);

        spbOffsetY = new QSpinBox(page_3);
        spbOffsetY->setObjectName(QString::fromUtf8("spbOffsetY"));
        spbOffsetY->setMaximum(28);

        horizontalLayout_2->addWidget(spbOffsetY);

        horizontalSpacer = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        swPs->addWidget(page_3);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        swPs->addWidget(page);
        page_11 = new QWidget();
        page_11->setObjectName(QString::fromUtf8("page_11"));
        horizontalLayout_21 = new QHBoxLayout(page_11);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        lblSlicePath = new QLabel(page_11);
        lblSlicePath->setObjectName(QString::fromUtf8("lblSlicePath"));

        horizontalLayout_21->addWidget(lblSlicePath);

        leSlicePath = new QLineEdit(page_11);
        leSlicePath->setObjectName(QString::fromUtf8("leSlicePath"));

        horizontalLayout_21->addWidget(leSlicePath);

        pbSlicePath = new QPushButton(page_11);
        pbSlicePath->setObjectName(QString::fromUtf8("pbSlicePath"));
        pbSlicePath->setMaximumSize(QSize(31, 25));

        horizontalLayout_21->addWidget(pbSlicePath);

        swPs->addWidget(page_11);
        page_12 = new QWidget();
        page_12->setObjectName(QString::fromUtf8("page_12"));
        horizontalLayout_22 = new QHBoxLayout(page_12);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        lblTilePath = new QLabel(page_12);
        lblTilePath->setObjectName(QString::fromUtf8("lblTilePath"));

        horizontalLayout_22->addWidget(lblTilePath);

        leTilePath = new QLineEdit(page_12);
        leTilePath->setObjectName(QString::fromUtf8("leTilePath"));

        horizontalLayout_22->addWidget(leTilePath);

        pbTilePath = new QPushButton(page_12);
        pbTilePath->setObjectName(QString::fromUtf8("pbTilePath"));
        pbTilePath->setMaximumSize(QSize(31, 25));

        horizontalLayout_22->addWidget(pbTilePath);

        swPs->addWidget(page_12);
        page_13 = new QWidget();
        page_13->setObjectName(QString::fromUtf8("page_13"));
        horizontalLayout_23 = new QHBoxLayout(page_13);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        lblMapPath = new QLabel(page_13);
        lblMapPath->setObjectName(QString::fromUtf8("lblMapPath"));

        horizontalLayout_23->addWidget(lblMapPath);

        leMapPath = new QLineEdit(page_13);
        leMapPath->setObjectName(QString::fromUtf8("leMapPath"));

        horizontalLayout_23->addWidget(leMapPath);

        pbMapPath = new QPushButton(page_13);
        pbMapPath->setObjectName(QString::fromUtf8("pbMapPath"));
        pbMapPath->setMaximumSize(QSize(31, 25));

        horizontalLayout_23->addWidget(pbMapPath);

        swPs->addWidget(page_13);
        page_14 = new QWidget();
        page_14->setObjectName(QString::fromUtf8("page_14"));
        horizontalLayout_24 = new QHBoxLayout(page_14);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        lblAnimPath = new QLabel(page_14);
        lblAnimPath->setObjectName(QString::fromUtf8("lblAnimPath"));

        horizontalLayout_24->addWidget(lblAnimPath);

        leAnimPath = new QLineEdit(page_14);
        leAnimPath->setObjectName(QString::fromUtf8("leAnimPath"));

        horizontalLayout_24->addWidget(leAnimPath);

        pbAnimPath = new QPushButton(page_14);
        pbAnimPath->setObjectName(QString::fromUtf8("pbAnimPath"));
        pbAnimPath->setMaximumSize(QSize(31, 25));

        horizontalLayout_24->addWidget(pbAnimPath);

        swPs->addWidget(page_14);
        page_15 = new QWidget();
        page_15->setObjectName(QString::fromUtf8("page_15"));
        horizontalLayout_25 = new QHBoxLayout(page_15);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        lblPlatzPath = new QLabel(page_15);
        lblPlatzPath->setObjectName(QString::fromUtf8("lblPlatzPath"));

        horizontalLayout_25->addWidget(lblPlatzPath);

        lePlatzPath = new QLineEdit(page_15);
        lePlatzPath->setObjectName(QString::fromUtf8("lePlatzPath"));

        horizontalLayout_25->addWidget(lePlatzPath);

        pbPlatzPath = new QPushButton(page_15);
        pbPlatzPath->setObjectName(QString::fromUtf8("pbPlatzPath"));
        pbPlatzPath->setMaximumSize(QSize(31, 25));

        horizontalLayout_25->addWidget(pbPlatzPath);

        swPs->addWidget(page_15);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        horizontalLayout_3 = new QHBoxLayout(page_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lblProjectSrcFolder = new QLabel(page_2);
        lblProjectSrcFolder->setObjectName(QString::fromUtf8("lblProjectSrcFolder"));
        lblProjectSrcFolder->setFrameShape(QFrame::NoFrame);

        horizontalLayout_3->addWidget(lblProjectSrcFolder);

        leSrcFolder = new QLineEdit(page_2);
        leSrcFolder->setObjectName(QString::fromUtf8("leSrcFolder"));

        horizontalLayout_3->addWidget(leSrcFolder);

        pbSrcFolder = new QPushButton(page_2);
        pbSrcFolder->setObjectName(QString::fromUtf8("pbSrcFolder"));
        pbSrcFolder->setMaximumSize(QSize(31, 23));

        horizontalLayout_3->addWidget(pbSrcFolder);

        swPs->addWidget(page_2);
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        horizontalLayout_4 = new QHBoxLayout(page_7);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lblProjectMakefilePath = new QLabel(page_7);
        lblProjectMakefilePath->setObjectName(QString::fromUtf8("lblProjectMakefilePath"));

        horizontalLayout_4->addWidget(lblProjectMakefilePath);

        leMakefilePath = new QLineEdit(page_7);
        leMakefilePath->setObjectName(QString::fromUtf8("leMakefilePath"));

        horizontalLayout_4->addWidget(leMakefilePath);

        pbMakefilePath = new QPushButton(page_7);
        pbMakefilePath->setObjectName(QString::fromUtf8("pbMakefilePath"));
        pbMakefilePath->setMaximumSize(QSize(31, 23));

        horizontalLayout_4->addWidget(pbMakefilePath);

        swPs->addWidget(page_7);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        horizontalLayout_5 = new QHBoxLayout(page_8);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lblProjectHexPath = new QLabel(page_8);
        lblProjectHexPath->setObjectName(QString::fromUtf8("lblProjectHexPath"));

        horizontalLayout_5->addWidget(lblProjectHexPath);

        leHexfilePath = new QLineEdit(page_8);
        leHexfilePath->setObjectName(QString::fromUtf8("leHexfilePath"));

        horizontalLayout_5->addWidget(leHexfilePath);

        pbHexfilePath = new QPushButton(page_8);
        pbHexfilePath->setObjectName(QString::fromUtf8("pbHexfilePath"));
        pbHexfilePath->setMaximumSize(QSize(31, 23));

        horizontalLayout_5->addWidget(pbHexfilePath);

        swPs->addWidget(page_8);

        verticalLayout_5->addWidget(swPs);


        verticalLayout_10->addWidget(groupBox);

        stackedWidget->addWidget(pageProjectSettings);
        pageLePlatzSettings = new QWidget();
        pageLePlatzSettings->setObjectName(QString::fromUtf8("pageLePlatzSettings"));
        sizePolicy.setHeightForWidth(pageLePlatzSettings->sizePolicy().hasHeightForWidth());
        pageLePlatzSettings->setSizePolicy(sizePolicy);
        verticalLayout_9 = new QVBoxLayout(pageLePlatzSettings);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox_2 = new QGroupBox(pageLePlatzSettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        twLps = new QTreeWidget(groupBox_2);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setText(0, QString::fromUtf8("1"));
        twLps->setHeaderItem(__qtreewidgetitem2);
        twLps->setObjectName(QString::fromUtf8("twLps"));
        twLps->setItemsExpandable(false);
        twLps->setColumnCount(1);

        verticalLayout_2->addWidget(twLps);

        swLps = new QStackedWidget(groupBox_2);
        swLps->setObjectName(QString::fromUtf8("swLps"));
        sizePolicy2.setHeightForWidth(swLps->sizePolicy().hasHeightForWidth());
        swLps->setSizePolicy(sizePolicy2);
        swLps->setMinimumSize(QSize(322, 70));
        page_9 = new QWidget();
        page_9->setObjectName(QString::fromUtf8("page_9"));
        horizontalLayout_6 = new QHBoxLayout(page_9);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        lblLePlatzMakePath = new QLabel(page_9);
        lblLePlatzMakePath->setObjectName(QString::fromUtf8("lblLePlatzMakePath"));

        horizontalLayout_6->addWidget(lblLePlatzMakePath);

        leMakeExePath = new QLineEdit(page_9);
        leMakeExePath->setObjectName(QString::fromUtf8("leMakeExePath"));

        horizontalLayout_6->addWidget(leMakeExePath);

        pbMakeExePath = new QPushButton(page_9);
        pbMakeExePath->setObjectName(QString::fromUtf8("pbMakeExePath"));
        pbMakeExePath->setMaximumSize(QSize(31, 23));

        horizontalLayout_6->addWidget(pbMakeExePath);

        swLps->addWidget(page_9);
        page_10 = new QWidget();
        page_10->setObjectName(QString::fromUtf8("page_10"));
        horizontalLayout_7 = new QHBoxLayout(page_10);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        lblLePlatzEmuPath = new QLabel(page_10);
        lblLePlatzEmuPath->setObjectName(QString::fromUtf8("lblLePlatzEmuPath"));

        horizontalLayout_7->addWidget(lblLePlatzEmuPath);

        leEmuExePath = new QLineEdit(page_10);
        leEmuExePath->setObjectName(QString::fromUtf8("leEmuExePath"));

        horizontalLayout_7->addWidget(leEmuExePath);

        pbEmuExePath = new QPushButton(page_10);
        pbEmuExePath->setObjectName(QString::fromUtf8("pbEmuExePath"));
        pbEmuExePath->setMaximumSize(QSize(31, 23));

        horizontalLayout_7->addWidget(pbEmuExePath);

        swLps->addWidget(page_10);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        verticalLayout = new QVBoxLayout(page_4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblUpdatesUrl = new QLabel(page_4);
        lblUpdatesUrl->setObjectName(QString::fromUtf8("lblUpdatesUrl"));

        verticalLayout->addWidget(lblUpdatesUrl);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        leUpdatesUrl = new QLineEdit(page_4);
        leUpdatesUrl->setObjectName(QString::fromUtf8("leUpdatesUrl"));

        horizontalLayout_8->addWidget(leUpdatesUrl);

        pbUpdatesUrlReset = new QPushButton(page_4);
        pbUpdatesUrlReset->setObjectName(QString::fromUtf8("pbUpdatesUrlReset"));

        horizontalLayout_8->addWidget(pbUpdatesUrlReset);


        verticalLayout->addLayout(horizontalLayout_8);

        swLps->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        horizontalLayout_9 = new QHBoxLayout(page_5);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        lblCanvasColor = new QLabel(page_5);
        lblCanvasColor->setObjectName(QString::fromUtf8("lblCanvasColor"));

        horizontalLayout_9->addWidget(lblCanvasColor);

        tbtnCanvasColor = new QToolButton(page_5);
        tbtnCanvasColor->setObjectName(QString::fromUtf8("tbtnCanvasColor"));
        tbtnCanvasColor->setMinimumSize(QSize(32, 32));
        tbtnCanvasColor->setMaximumSize(QSize(32, 32));

        horizontalLayout_9->addWidget(tbtnCanvasColor);

        horizontalSpacer_3 = new QSpacerItem(143, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_3);

        swLps->addWidget(page_5);

        verticalLayout_2->addWidget(swLps);


        verticalLayout_9->addWidget(groupBox_2);

        stackedWidget->addWidget(pageLePlatzSettings);

        verticalLayout_8->addWidget(stackedWidget);

        line = new QFrame(frameSettings);
        line->setObjectName(QString::fromUtf8("line"));
        sizePolicy2.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy2);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_8->addWidget(line);

        splitter->addWidget(frameSettings);

        verticalLayout_7->addWidget(splitter);

        frame = new QFrame(SettingsDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        horizontalLayout_20 = new QHBoxLayout(frame);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        horizontalLayout_20->setContentsMargins(0, 6, 0, 0);
        horizontalSpacer_5 = new QSpacerItem(337, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_5);

        pbOk = new QPushButton(frame);
        pbOk->setObjectName(QString::fromUtf8("pbOk"));

        horizontalLayout_20->addWidget(pbOk);

        pbCancel = new QPushButton(frame);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

        horizontalLayout_20->addWidget(pbCancel);

        pbApply = new QPushButton(frame);
        pbApply->setObjectName(QString::fromUtf8("pbApply"));

        horizontalLayout_20->addWidget(pbApply);


        verticalLayout_7->addWidget(frame);


        horizontalLayout->addLayout(verticalLayout_7);


        retranslateUi(SettingsDialog);

        stackedWidget->setCurrentIndex(0);
        swPs->setCurrentIndex(0);
        swLps->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        SettingsDialog->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Project Settings", 0, QApplication::UnicodeUTF8));
        lblOffsetY->setText(QApplication::translate("SettingsDialog", "Y-axis offset (in tiles):", 0, QApplication::UnicodeUTF8));
        lblSlicePath->setText(QApplication::translate("SettingsDialog", "Slice Path:", 0, QApplication::UnicodeUTF8));
        pbSlicePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblTilePath->setText(QApplication::translate("SettingsDialog", "Tile path:", 0, QApplication::UnicodeUTF8));
        pbTilePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblMapPath->setText(QApplication::translate("SettingsDialog", "Map Path:", 0, QApplication::UnicodeUTF8));
        pbMapPath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblAnimPath->setText(QApplication::translate("SettingsDialog", "Anim Path:", 0, QApplication::UnicodeUTF8));
        pbAnimPath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblPlatzPath->setText(QApplication::translate("SettingsDialog", "Platz Path:", 0, QApplication::UnicodeUTF8));
        pbPlatzPath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblProjectSrcFolder->setText(QApplication::translate("SettingsDialog", "Source Folder:", 0, QApplication::UnicodeUTF8));
        pbSrcFolder->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblProjectMakefilePath->setText(QApplication::translate("SettingsDialog", "Makefile Path:", 0, QApplication::UnicodeUTF8));
        pbMakefilePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblProjectHexPath->setText(QApplication::translate("SettingsDialog", "Hex Path:", 0, QApplication::UnicodeUTF8));
        pbHexfilePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "LePlatz Settings", 0, QApplication::UnicodeUTF8));
        lblLePlatzMakePath->setText(QApplication::translate("SettingsDialog", "Make Path:", 0, QApplication::UnicodeUTF8));
        pbMakeExePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblLePlatzEmuPath->setText(QApplication::translate("SettingsDialog", "Emulator Path:", 0, QApplication::UnicodeUTF8));
        pbEmuExePath->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        lblUpdatesUrl->setText(QApplication::translate("SettingsDialog", "LePlatz Updates URL:", 0, QApplication::UnicodeUTF8));
        pbUpdatesUrlReset->setText(QApplication::translate("SettingsDialog", "Default", 0, QApplication::UnicodeUTF8));
        lblCanvasColor->setText(QApplication::translate("SettingsDialog", "Canvas Background Color:", 0, QApplication::UnicodeUTF8));
        tbtnCanvasColor->setText(QApplication::translate("SettingsDialog", "...", 0, QApplication::UnicodeUTF8));
        pbOk->setText(QApplication::translate("SettingsDialog", "Ok", 0, QApplication::UnicodeUTF8));
        pbCancel->setText(QApplication::translate("SettingsDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        pbApply->setText(QApplication::translate("SettingsDialog", "Apply", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATZSETTINGS_H
