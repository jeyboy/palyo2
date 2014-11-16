/********************************************************************************
** Form generated from reading UI file 'relationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RELATIONDIALOG_H
#define UI_RELATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RelationDialog
{
public:
    QGroupBox *groupBox;
    QListView *friendsList;
    QGroupBox *groupBox_2;
    QListView *groupsList;
    QPushButton *cancelButton;

    void setupUi(QDialog *RelationDialog)
    {
        if (RelationDialog->objectName().isEmpty())
            RelationDialog->setObjectName(QStringLiteral("RelationDialog"));
        RelationDialog->resize(432, 369);
        RelationDialog->setMinimumSize(QSize(432, 369));
        RelationDialog->setMaximumSize(QSize(432, 369));
        groupBox = new QGroupBox(RelationDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 201, 331));
        friendsList = new QListView(groupBox);
        friendsList->setObjectName(QStringLiteral("friendsList"));
        friendsList->setGeometry(QRect(10, 20, 181, 301));
        friendsList->setProperty("showDropIndicator", QVariant(false));
        friendsList->setAlternatingRowColors(true);
        groupBox_2 = new QGroupBox(RelationDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(220, 0, 201, 331));
        groupsList = new QListView(groupBox_2);
        groupsList->setObjectName(QStringLiteral("groupsList"));
        groupsList->setGeometry(QRect(10, 20, 181, 301));
        groupsList->setProperty("showDropIndicator", QVariant(false));
        groupsList->setAlternatingRowColors(true);
        cancelButton = new QPushButton(RelationDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(10, 340, 411, 23));

        retranslateUi(RelationDialog);

        QMetaObject::connectSlotsByName(RelationDialog);
    } // setupUi

    void retranslateUi(QDialog *RelationDialog)
    {
        RelationDialog->setWindowTitle(QApplication::translate("RelationDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("RelationDialog", "Friends", 0));
        groupBox_2->setTitle(QApplication::translate("RelationDialog", "Groups", 0));
        cancelButton->setText(QApplication::translate("RelationDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class RelationDialog: public Ui_RelationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RELATIONDIALOG_H
