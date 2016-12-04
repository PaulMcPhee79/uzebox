/********************************************************************************
** Form generated from reading UI file 'About.ui'
**
** Created: Mon 1. Feb 22:43:56 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *lblAboutLogo;
    QLabel *lblAboutText;
    QLabel *lblAboutTitle;
    QLabel *lblUrl;
    QPushButton *pbOk;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(540, 430);
        About->setMinimumSize(QSize(540, 430));
        About->setMaximumSize(QSize(540, 430));
        lblAboutLogo = new QLabel(About);
        lblAboutLogo->setObjectName(QString::fromUtf8("lblAboutLogo"));
        lblAboutLogo->setGeometry(QRect(10, 20, 123, 335));
        lblAboutText = new QLabel(About);
        lblAboutText->setObjectName(QString::fromUtf8("lblAboutText"));
        lblAboutText->setGeometry(QRect(150, 54, 371, 301));
        lblAboutText->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        lblAboutText->setWordWrap(true);
        lblAboutText->setMargin(5);
        lblAboutTitle = new QLabel(About);
        lblAboutTitle->setObjectName(QString::fromUtf8("lblAboutTitle"));
        lblAboutTitle->setGeometry(QRect(150, 22, 371, 31));
        QFont font;
        font.setPointSize(20);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setKerning(true);
        lblAboutTitle->setFont(font);
        lblAboutTitle->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lblAboutTitle->setMargin(5);
        lblAboutTitle->setOpenExternalLinks(false);
        lblUrl = new QLabel(About);
        lblUrl->setObjectName(QString::fromUtf8("lblUrl"));
        lblUrl->setGeometry(QRect(150, 360, 371, 31));
        lblUrl->setMargin(5);
        lblUrl->setOpenExternalLinks(true);
        pbOk = new QPushButton(About);
        pbOk->setObjectName(QString::fromUtf8("pbOk"));
        pbOk->setGeometry(QRect(440, 398, 75, 23));

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "About", 0, QApplication::UnicodeUTF8));
        lblAboutLogo->setText(QApplication::translate("About", "*Pixmap*", 0, QApplication::UnicodeUTF8));
        lblAboutText->setText(QApplication::translate("About", "*Text*", 0, QApplication::UnicodeUTF8));
        lblAboutTitle->setText(QApplication::translate("About", "*Title*", 0, QApplication::UnicodeUTF8));
        lblUrl->setText(QApplication::translate("About", "*URL*", 0, QApplication::UnicodeUTF8));
        pbOk->setText(QApplication::translate("About", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
