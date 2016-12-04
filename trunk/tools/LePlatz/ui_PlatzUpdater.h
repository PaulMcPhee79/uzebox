/********************************************************************************
** Form generated from reading UI file 'PlatzUpdater.ui'
**
** Created: Tue 19. Jan 12:43:11 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLATZUPDATER_H
#define UI_PLATZUPDATER_H

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

class Ui_PlatzUpdater
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblDetails;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbShowDetails;
    QPushButton *pbCancel;
    QListWidget *listWidget;

    void setupUi(QDialog *PlatzUpdater)
    {
        if (PlatzUpdater->objectName().isEmpty())
            PlatzUpdater->setObjectName(QString::fromUtf8("PlatzUpdater"));
        PlatzUpdater->resize(388, 306);
        verticalLayout = new QVBoxLayout(PlatzUpdater);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblDetails = new QLabel(PlatzUpdater);
        lblDetails->setObjectName(QString::fromUtf8("lblDetails"));

        verticalLayout->addWidget(lblDetails);

        progressBar = new QProgressBar(PlatzUpdater);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbShowDetails = new QPushButton(PlatzUpdater);
        pbShowDetails->setObjectName(QString::fromUtf8("pbShowDetails"));

        horizontalLayout->addWidget(pbShowDetails);

        pbCancel = new QPushButton(PlatzUpdater);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

        horizontalLayout->addWidget(pbCancel);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(PlatzUpdater);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(PlatzUpdater);

        QMetaObject::connectSlotsByName(PlatzUpdater);
    } // setupUi

    void retranslateUi(QDialog *PlatzUpdater)
    {
        PlatzUpdater->setWindowTitle(QApplication::translate("PlatzUpdater", "Dialog", 0, QApplication::UnicodeUTF8));
        lblDetails->setText(QApplication::translate("PlatzUpdater", "*", 0, QApplication::UnicodeUTF8));
        pbShowDetails->setText(QApplication::translate("PlatzUpdater", "Show Details", 0, QApplication::UnicodeUTF8));
        pbCancel->setText(QApplication::translate("PlatzUpdater", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlatzUpdater: public Ui_PlatzUpdater {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLATZUPDATER_H
