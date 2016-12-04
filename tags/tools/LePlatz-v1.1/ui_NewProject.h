/********************************************************************************
** Form generated from reading UI file 'NewProject.ui'
**
** Created: Mon 1. Feb 22:43:56 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECT_H
#define UI_NEWPROJECT_H

#include <FocusComboBox.h>
#include <FocusLineEdit.h>
#include <FocusSpinBox.h>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_NewProject
{
public:
    QFrame *line;
    QDialogButtonBox *buttonBox;
    QLabel *lblContextualHelp;
    QLabel *lblLogoImg;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QSpacerItem *verticalSpacer;
    QLabel *lblProjectName;
    QHBoxLayout *horizontalLayout_2;
    FocusLineEdit *leProjectName;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_8;
    QLabel *lblProjectPath;
    QHBoxLayout *horizontalLayout;
    FocusLineEdit *leProjectFolder;
    QPushButton *pbProjectPath;
    QSpacerItem *verticalSpacer_6;
    QLabel *lblArtPath;
    QHBoxLayout *horizontalLayout_4;
    FocusLineEdit *leArtFolder;
    QPushButton *pbArtPath;
    QSpacerItem *verticalSpacer_7;
    QLabel *lblImgFormat;
    QHBoxLayout *horizontalLayout_3;
    FocusComboBox *cboImageFormat;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_9;
    QLabel *lblSrcPath;
    QHBoxLayout *horizontalLayout_5;
    FocusLineEdit *leSrcFolder;
    QPushButton *pbSrcPath;
    QSpacerItem *verticalSpacer_10;
    QLabel *lvlVideoMode;
    QHBoxLayout *horizontalLayout_7;
    FocusComboBox *cboVideoMode;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_11;
    QLabel *lblOffsetY_2;
    QHBoxLayout *horizontalLayout_6;
    FocusSpinBox *spbSliceHeight;
    QLabel *lblSliceHeightTiles;
    QSpacerItem *horizontalSpacer;
    QLabel *lblOffsetY;
    FocusSpinBox *spbOffsetY;
    QLabel *lblOffsetYTiles;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_12;

    void setupUi(QDialog *NewProject)
    {
        if (NewProject->objectName().isEmpty())
            NewProject->setObjectName(QString::fromUtf8("NewProject"));
        NewProject->resize(600, 480);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NewProject->sizePolicy().hasHeightForWidth());
        NewProject->setSizePolicy(sizePolicy);
        NewProject->setMinimumSize(QSize(600, 480));
        NewProject->setMaximumSize(QSize(600, 480));
        line = new QFrame(NewProject);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 430, 581, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        buttonBox = new QDialogButtonBox(NewProject);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(430, 448, 156, 23));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lblContextualHelp = new QLabel(NewProject);
        lblContextualHelp->setObjectName(QString::fromUtf8("lblContextualHelp"));
        lblContextualHelp->setGeometry(QRect(150, 360, 441, 71));
        lblContextualHelp->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lblContextualHelp->setWordWrap(true);
        lblLogoImg = new QLabel(NewProject);
        lblLogoImg->setObjectName(QString::fromUtf8("lblLogoImg"));
        lblLogoImg->setGeometry(QRect(10, 13, 123, 335));
        lblLogoImg->setMinimumSize(QSize(123, 335));
        lblLogoImg->setMaximumSize(QSize(123, 335));
        groupBox = new QGroupBox(NewProject);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(141, 10, 451, 341));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMaximumSize(QSize(460, 600));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        verticalSpacer = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(0, QFormLayout::FieldRole, verticalSpacer);

        lblProjectName = new QLabel(groupBox);
        lblProjectName->setObjectName(QString::fromUtf8("lblProjectName"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lblProjectName);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        leProjectName = new FocusLineEdit(groupBox);
        leProjectName->setObjectName(QString::fromUtf8("leProjectName"));

        horizontalLayout_2->addWidget(leProjectName);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_2);

        verticalSpacer_8 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(2, QFormLayout::FieldRole, verticalSpacer_8);

        lblProjectPath = new QLabel(groupBox);
        lblProjectPath->setObjectName(QString::fromUtf8("lblProjectPath"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lblProjectPath);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        leProjectFolder = new FocusLineEdit(groupBox);
        leProjectFolder->setObjectName(QString::fromUtf8("leProjectFolder"));

        horizontalLayout->addWidget(leProjectFolder);

        pbProjectPath = new QPushButton(groupBox);
        pbProjectPath->setObjectName(QString::fromUtf8("pbProjectPath"));
        pbProjectPath->setMaximumSize(QSize(31, 25));

        horizontalLayout->addWidget(pbProjectPath);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout);

        verticalSpacer_6 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(4, QFormLayout::FieldRole, verticalSpacer_6);

        lblArtPath = new QLabel(groupBox);
        lblArtPath->setObjectName(QString::fromUtf8("lblArtPath"));

        formLayout->setWidget(5, QFormLayout::LabelRole, lblArtPath);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        leArtFolder = new FocusLineEdit(groupBox);
        leArtFolder->setObjectName(QString::fromUtf8("leArtFolder"));

        horizontalLayout_4->addWidget(leArtFolder);

        pbArtPath = new QPushButton(groupBox);
        pbArtPath->setObjectName(QString::fromUtf8("pbArtPath"));
        pbArtPath->setMaximumSize(QSize(31, 25));

        horizontalLayout_4->addWidget(pbArtPath);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_4);

        verticalSpacer_7 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(6, QFormLayout::FieldRole, verticalSpacer_7);

        lblImgFormat = new QLabel(groupBox);
        lblImgFormat->setObjectName(QString::fromUtf8("lblImgFormat"));

        formLayout->setWidget(7, QFormLayout::LabelRole, lblImgFormat);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        cboImageFormat = new FocusComboBox(groupBox);
        cboImageFormat->setObjectName(QString::fromUtf8("cboImageFormat"));

        horizontalLayout_3->addWidget(cboImageFormat);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_3);

        verticalSpacer_9 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(8, QFormLayout::FieldRole, verticalSpacer_9);

        lblSrcPath = new QLabel(groupBox);
        lblSrcPath->setObjectName(QString::fromUtf8("lblSrcPath"));

        formLayout->setWidget(9, QFormLayout::LabelRole, lblSrcPath);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        leSrcFolder = new FocusLineEdit(groupBox);
        leSrcFolder->setObjectName(QString::fromUtf8("leSrcFolder"));

        horizontalLayout_5->addWidget(leSrcFolder);

        pbSrcPath = new QPushButton(groupBox);
        pbSrcPath->setObjectName(QString::fromUtf8("pbSrcPath"));
        pbSrcPath->setMaximumSize(QSize(31, 25));

        horizontalLayout_5->addWidget(pbSrcPath);


        formLayout->setLayout(9, QFormLayout::FieldRole, horizontalLayout_5);

        verticalSpacer_10 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(10, QFormLayout::FieldRole, verticalSpacer_10);

        lvlVideoMode = new QLabel(groupBox);
        lvlVideoMode->setObjectName(QString::fromUtf8("lvlVideoMode"));

        formLayout->setWidget(11, QFormLayout::LabelRole, lvlVideoMode);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        cboVideoMode = new FocusComboBox(groupBox);
        cboVideoMode->setObjectName(QString::fromUtf8("cboVideoMode"));
        cboVideoMode->setMinimumSize(QSize(171, 0));
        cboVideoMode->setMaximumSize(QSize(171, 16777215));

        horizontalLayout_7->addWidget(cboVideoMode);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);


        formLayout->setLayout(11, QFormLayout::FieldRole, horizontalLayout_7);

        verticalSpacer_11 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(12, QFormLayout::FieldRole, verticalSpacer_11);

        lblOffsetY_2 = new QLabel(groupBox);
        lblOffsetY_2->setObjectName(QString::fromUtf8("lblOffsetY_2"));

        formLayout->setWidget(13, QFormLayout::LabelRole, lblOffsetY_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        spbSliceHeight = new FocusSpinBox(groupBox);
        spbSliceHeight->setObjectName(QString::fromUtf8("spbSliceHeight"));
        spbSliceHeight->setMaximum(32);

        horizontalLayout_6->addWidget(spbSliceHeight);

        lblSliceHeightTiles = new QLabel(groupBox);
        lblSliceHeightTiles->setObjectName(QString::fromUtf8("lblSliceHeightTiles"));

        horizontalLayout_6->addWidget(lblSliceHeightTiles);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        lblOffsetY = new QLabel(groupBox);
        lblOffsetY->setObjectName(QString::fromUtf8("lblOffsetY"));

        horizontalLayout_6->addWidget(lblOffsetY);

        spbOffsetY = new FocusSpinBox(groupBox);
        spbOffsetY->setObjectName(QString::fromUtf8("spbOffsetY"));
        spbOffsetY->setMaximum(32);

        horizontalLayout_6->addWidget(spbOffsetY);

        lblOffsetYTiles = new QLabel(groupBox);
        lblOffsetYTiles->setObjectName(QString::fromUtf8("lblOffsetYTiles"));

        horizontalLayout_6->addWidget(lblOffsetYTiles);

        horizontalSpacer_6 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        formLayout->setLayout(13, QFormLayout::FieldRole, horizontalLayout_6);

        verticalSpacer_12 = new QSpacerItem(351, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(14, QFormLayout::FieldRole, verticalSpacer_12);


        retranslateUi(NewProject);

        QMetaObject::connectSlotsByName(NewProject);
    } // setupUi

    void retranslateUi(QDialog *NewProject)
    {
        NewProject->setWindowTitle(QApplication::translate("NewProject", "New Project", 0, QApplication::UnicodeUTF8));
        lblContextualHelp->setText(QApplication::translate("NewProject", "Contextual Help*", 0, QApplication::UnicodeUTF8));
        lblLogoImg->setText(QString());
        groupBox->setTitle(QApplication::translate("NewProject", "Project Settings", 0, QApplication::UnicodeUTF8));
        lblProjectName->setText(QApplication::translate("NewProject", "Project Name:", 0, QApplication::UnicodeUTF8));
        lblProjectPath->setText(QApplication::translate("NewProject", "Project Folder:", 0, QApplication::UnicodeUTF8));
        pbProjectPath->setText(QApplication::translate("NewProject", "...", 0, QApplication::UnicodeUTF8));
        lblArtPath->setText(QApplication::translate("NewProject", "Art Folder:", 0, QApplication::UnicodeUTF8));
        pbArtPath->setText(QApplication::translate("NewProject", "...", 0, QApplication::UnicodeUTF8));
        lblImgFormat->setText(QApplication::translate("NewProject", "Image Format:", 0, QApplication::UnicodeUTF8));
        lblSrcPath->setText(QApplication::translate("NewProject", "Source Folder:", 0, QApplication::UnicodeUTF8));
        pbSrcPath->setText(QApplication::translate("NewProject", "...", 0, QApplication::UnicodeUTF8));
        lvlVideoMode->setText(QApplication::translate("NewProject", "Video Mode:", 0, QApplication::UnicodeUTF8));
        lblOffsetY_2->setText(QApplication::translate("NewProject", "Slice height:", 0, QApplication::UnicodeUTF8));
        lblSliceHeightTiles->setText(QApplication::translate("NewProject", "tiles", 0, QApplication::UnicodeUTF8));
        lblOffsetY->setText(QApplication::translate("NewProject", "Y-axis offset:", 0, QApplication::UnicodeUTF8));
        lblOffsetYTiles->setText(QApplication::translate("NewProject", "tiles", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewProject: public Ui_NewProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECT_H
