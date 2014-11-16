/********************************************************************************
** Form generated from reading UI file 'toolbardialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBARDIALOG_H
#define UI_TOOLBARDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ToolbarDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *toolbarName;

    void setupUi(QDialog *ToolbarDialog)
    {
        if (ToolbarDialog->objectName().isEmpty())
            ToolbarDialog->setObjectName(QStringLiteral("ToolbarDialog"));
        ToolbarDialog->resize(313, 76);
        buttonBox = new QDialogButtonBox(ToolbarDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(20, 40, 281, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(ToolbarDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 46, 21));
        toolbarName = new QLineEdit(ToolbarDialog);
        toolbarName->setObjectName(QStringLiteral("toolbarName"));
        toolbarName->setGeometry(QRect(60, 10, 231, 20));

        retranslateUi(ToolbarDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ToolbarDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ToolbarDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ToolbarDialog);
    } // setupUi

    void retranslateUi(QDialog *ToolbarDialog)
    {
        ToolbarDialog->setWindowTitle(QApplication::translate("ToolbarDialog", "Dialog", 0));
        label->setText(QApplication::translate("ToolbarDialog", "Name", 0));
    } // retranslateUi

};

namespace Ui {
    class ToolbarDialog: public Ui_ToolbarDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBARDIALOG_H
