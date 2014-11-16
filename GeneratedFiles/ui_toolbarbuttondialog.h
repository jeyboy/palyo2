/********************************************************************************
** Form generated from reading UI file 'toolbarbuttondialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBARBUTTONDIALOG_H
#define UI_TOOLBARBUTTONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ToolbarButtonDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *buttonName;
    QLabel *label_2;
    QLineEdit *buttonPath;
    QPushButton *browseButton;

    void setupUi(QDialog *ToolbarButtonDialog)
    {
        if (ToolbarButtonDialog->objectName().isEmpty())
            ToolbarButtonDialog->setObjectName(QStringLiteral("ToolbarButtonDialog"));
        ToolbarButtonDialog->resize(385, 106);
        buttonBox = new QDialogButtonBox(ToolbarButtonDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 70, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(ToolbarButtonDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 46, 21));
        buttonName = new QLineEdit(ToolbarButtonDialog);
        buttonName->setObjectName(QStringLiteral("buttonName"));
        buttonName->setGeometry(QRect(60, 10, 311, 20));
        label_2 = new QLabel(ToolbarButtonDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 40, 46, 21));
        buttonPath = new QLineEdit(ToolbarButtonDialog);
        buttonPath->setObjectName(QStringLiteral("buttonPath"));
        buttonPath->setGeometry(QRect(60, 40, 251, 20));
        browseButton = new QPushButton(ToolbarButtonDialog);
        browseButton->setObjectName(QStringLiteral("browseButton"));
        browseButton->setGeometry(QRect(320, 40, 51, 23));
#ifndef QT_NO_SHORTCUT
        label->setBuddy(buttonName);
        label_2->setBuddy(buttonName);
#endif // QT_NO_SHORTCUT

        retranslateUi(ToolbarButtonDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ToolbarButtonDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ToolbarButtonDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ToolbarButtonDialog);
    } // setupUi

    void retranslateUi(QDialog *ToolbarButtonDialog)
    {
        ToolbarButtonDialog->setWindowTitle(QApplication::translate("ToolbarButtonDialog", "Dialog", 0));
        label->setText(QApplication::translate("ToolbarButtonDialog", "Name", 0));
        label_2->setText(QApplication::translate("ToolbarButtonDialog", "Path", 0));
        browseButton->setText(QApplication::translate("ToolbarButtonDialog", "Browse", 0));
    } // retranslateUi

};

namespace Ui {
    class ToolbarButtonDialog: public Ui_ToolbarButtonDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBARBUTTONDIALOG_H
