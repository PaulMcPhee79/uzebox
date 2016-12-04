/********************************************************************************
** Form generated from reading UI file 'LePatchDlg.ui'
**
** Created: Mon 1. Feb 22:50:35 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEPATCHDLG_H
#define UI_LEPATCHDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LePatchDlg
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *prbProgress;
    QLabel *lblDetails;
    QListWidget *lstDetails;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbDetails;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbApplyUpdate;
    QPushButton *pbExit;

    void setupUi(QDialog *LePatchDlg)
    {
        if (LePatchDlg->objectName().isEmpty())
            LePatchDlg->setObjectName(QString::fromUtf8("LePatchDlg"));
        LePatchDlg->resize(500, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LePatchDlg->sizePolicy().hasHeightForWidth());
        LePatchDlg->setSizePolicy(sizePolicy);
        LePatchDlg->setMinimumSize(QSize(500, 300));
        LePatchDlg->setMaximumSize(QSize(1650, 300));
        verticalLayout = new QVBoxLayout(LePatchDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        prbProgress = new QProgressBar(LePatchDlg);
        prbProgress->setObjectName(QString::fromUtf8("prbProgress"));
        prbProgress->setValue(24);

        verticalLayout->addWidget(prbProgress);

        lblDetails = new QLabel(LePatchDlg);
        lblDetails->setObjectName(QString::fromUtf8("lblDetails"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblDetails->sizePolicy().hasHeightForWidth());
        lblDetails->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(lblDetails);

        lstDetails = new QListWidget(LePatchDlg);
        lstDetails->setObjectName(QString::fromUtf8("lstDetails"));

        verticalLayout->addWidget(lstDetails);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbDetails = new QPushButton(LePatchDlg);
        pbDetails->setObjectName(QString::fromUtf8("pbDetails"));

        horizontalLayout->addWidget(pbDetails);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbApplyUpdate = new QPushButton(LePatchDlg);
        pbApplyUpdate->setObjectName(QString::fromUtf8("pbApplyUpdate"));

        horizontalLayout->addWidget(pbApplyUpdate);

        pbExit = new QPushButton(LePatchDlg);
        pbExit->setObjectName(QString::fromUtf8("pbExit"));

        horizontalLayout->addWidget(pbExit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LePatchDlg);

        QMetaObject::connectSlotsByName(LePatchDlg);
    } // setupUi

    void retranslateUi(QDialog *LePatchDlg)
    {
        LePatchDlg->setWindowTitle(QApplication::translate("LePatchDlg", "LePatch - LePlatz Patch Tool", 0, QApplication::UnicodeUTF8));
        lblDetails->setText(QApplication::translate("LePatchDlg", "*", 0, QApplication::UnicodeUTF8));
        pbDetails->setText(QApplication::translate("LePatchDlg", "View Details", 0, QApplication::UnicodeUTF8));
        pbApplyUpdate->setText(QApplication::translate("LePatchDlg", "Apply Update", 0, QApplication::UnicodeUTF8));
        pbExit->setText(QApplication::translate("LePatchDlg", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LePatchDlg: public Ui_LePatchDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEPATCHDLG_H
