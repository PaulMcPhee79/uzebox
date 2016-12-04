/********************************************************************************
** Form generated from reading UI file 'FindReplace.ui'
**
** Created: Mon 1. Feb 22:43:56 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDREPLACE_H
#define UI_FINDREPLACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindReplace
{
public:
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *lblCategory;
    QComboBox *cboCategory;
    QLabel *lblReplaceThis;
    QComboBox *cboReplaceThis;
    QLabel *lblReplaceWith;
    QComboBox *cboReplaceWith;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QRadioButton *rbtnApplyAll;
    QRadioButton *rbtnApplyRange;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spbRangeLower;
    QLabel *lblRangeTo;
    QSpinBox *spbRangeUpper;
    QSpacerItem *horizontalSpacer;
    QLabel *lblReplaceResult;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pbReplace;
    QPushButton *pbClose;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *FindReplace)
    {
        if (FindReplace->objectName().isEmpty())
            FindReplace->setObjectName(QString::fromUtf8("FindReplace"));
        FindReplace->resize(500, 240);
        FindReplace->setMinimumSize(QSize(500, 240));
        FindReplace->setMaximumSize(QSize(500, 240));
        horizontalLayout_3 = new QHBoxLayout(FindReplace);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 6, -1, 6);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame = new QFrame(FindReplace);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 6, -1, 6);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lblCategory = new QLabel(frame);
        lblCategory->setObjectName(QString::fromUtf8("lblCategory"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lblCategory);

        cboCategory = new QComboBox(frame);
        cboCategory->setObjectName(QString::fromUtf8("cboCategory"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cboCategory);

        lblReplaceThis = new QLabel(frame);
        lblReplaceThis->setObjectName(QString::fromUtf8("lblReplaceThis"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lblReplaceThis);

        cboReplaceThis = new QComboBox(frame);
        cboReplaceThis->setObjectName(QString::fromUtf8("cboReplaceThis"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cboReplaceThis);

        lblReplaceWith = new QLabel(frame);
        lblReplaceWith->setObjectName(QString::fromUtf8("lblReplaceWith"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lblReplaceWith);

        cboReplaceWith = new QComboBox(frame);
        cboReplaceWith->setObjectName(QString::fromUtf8("cboReplaceWith"));

        formLayout->setWidget(2, QFormLayout::FieldRole, cboReplaceWith);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 100));
        groupBox->setMaximumSize(QSize(16777215, 100));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setContentsMargins(-1, 6, -1, 0);
        rbtnApplyAll = new QRadioButton(groupBox);
        rbtnApplyAll->setObjectName(QString::fromUtf8("rbtnApplyAll"));
        rbtnApplyAll->setMinimumSize(QSize(71, 31));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, rbtnApplyAll);

        rbtnApplyRange = new QRadioButton(groupBox);
        rbtnApplyRange->setObjectName(QString::fromUtf8("rbtnApplyRange"));
        rbtnApplyRange->setMinimumSize(QSize(71, 31));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, rbtnApplyRange);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        spbRangeLower = new QSpinBox(groupBox);
        spbRangeLower->setObjectName(QString::fromUtf8("spbRangeLower"));
        spbRangeLower->setMinimumSize(QSize(51, 21));

        horizontalLayout->addWidget(spbRangeLower);

        lblRangeTo = new QLabel(groupBox);
        lblRangeTo->setObjectName(QString::fromUtf8("lblRangeTo"));
        lblRangeTo->setMinimumSize(QSize(25, 31));
        lblRangeTo->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lblRangeTo);

        spbRangeUpper = new QSpinBox(groupBox);
        spbRangeUpper->setObjectName(QString::fromUtf8("spbRangeUpper"));
        spbRangeUpper->setMinimumSize(QSize(51, 21));

        horizontalLayout->addWidget(spbRangeUpper);

        horizontalSpacer = new QSpacerItem(17, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout);


        verticalLayout->addWidget(groupBox);


        verticalLayout_3->addWidget(frame);

        lblReplaceResult = new QLabel(FindReplace);
        lblReplaceResult->setObjectName(QString::fromUtf8("lblReplaceResult"));
        lblReplaceResult->setMinimumSize(QSize(0, 21));
        lblReplaceResult->setMaximumSize(QSize(16777215, 21));

        verticalLayout_3->addWidget(lblReplaceResult);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pbReplace = new QPushButton(FindReplace);
        pbReplace->setObjectName(QString::fromUtf8("pbReplace"));

        verticalLayout_2->addWidget(pbReplace);

        pbClose = new QPushButton(FindReplace);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));

        verticalLayout_2->addWidget(pbClose);

        verticalSpacer_2 = new QSpacerItem(20, 88, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        retranslateUi(FindReplace);

        QMetaObject::connectSlotsByName(FindReplace);
    } // setupUi

    void retranslateUi(QDialog *FindReplace)
    {
        FindReplace->setWindowTitle(QApplication::translate("FindReplace", "Find & Replace Src Defines", 0, QApplication::UnicodeUTF8));
        lblCategory->setText(QApplication::translate("FindReplace", "Category:", 0, QApplication::UnicodeUTF8));
        lblReplaceThis->setText(QApplication::translate("FindReplace", "Replace this:", 0, QApplication::UnicodeUTF8));
        lblReplaceWith->setText(QApplication::translate("FindReplace", "With:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FindReplace", "Apply to slices", 0, QApplication::UnicodeUTF8));
        rbtnApplyAll->setText(QApplication::translate("FindReplace", "All", 0, QApplication::UnicodeUTF8));
        rbtnApplyRange->setText(QApplication::translate("FindReplace", "Range", 0, QApplication::UnicodeUTF8));
        lblRangeTo->setText(QApplication::translate("FindReplace", "to", 0, QApplication::UnicodeUTF8));
        lblReplaceResult->setText(QString());
        pbReplace->setText(QApplication::translate("FindReplace", "Replace", 0, QApplication::UnicodeUTF8));
        pbClose->setText(QApplication::translate("FindReplace", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FindReplace: public Ui_FindReplace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDREPLACE_H
