/********************************************************************************
** Form generated from reading UI file 'tabdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABDIALOG_H
#define UI_TABDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_TabDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *tabName;
    QGroupBox *groupBox;
    QRadioButton *isListRadio;
    QRadioButton *isTreeRadio;
    QRadioButton *isOneLevelTreeRadio;
    QRadioButton *isVkRadio;
    QRadioButton *isOneLevelTreeBreadcrumbRadio;
    QGroupBox *groupBox_2;
    QCheckBox *playlist;
    QCheckBox *interactive;
    QCheckBox *deleteFile;

    void setupUi(QDialog *TabDialog)
    {
        if (TabDialog->objectName().isEmpty())
            TabDialog->setObjectName(QStringLiteral("TabDialog"));
        TabDialog->resize(302, 181);
        buttonBox = new QDialogButtonBox(TabDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 150, 261, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        label = new QLabel(TabDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 31, 21));
        label->setScaledContents(true);
        tabName = new QLineEdit(TabDialog);
        tabName->setObjectName(QStringLiteral("tabName"));
        tabName->setGeometry(QRect(50, 10, 241, 20));
        groupBox = new QGroupBox(TabDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(140, 40, 151, 101));
        isListRadio = new QRadioButton(groupBox);
        isListRadio->setObjectName(QStringLiteral("isListRadio"));
        isListRadio->setGeometry(QRect(60, 20, 41, 17));
        isTreeRadio = new QRadioButton(groupBox);
        isTreeRadio->setObjectName(QStringLiteral("isTreeRadio"));
        isTreeRadio->setGeometry(QRect(10, 20, 41, 17));
        isTreeRadio->setChecked(true);
        isOneLevelTreeRadio = new QRadioButton(groupBox);
        isOneLevelTreeRadio->setObjectName(QStringLiteral("isOneLevelTreeRadio"));
        isOneLevelTreeRadio->setGeometry(QRect(10, 44, 121, 17));
        isVkRadio = new QRadioButton(groupBox);
        isVkRadio->setObjectName(QStringLiteral("isVkRadio"));
        isVkRadio->setGeometry(QRect(110, 20, 41, 17));
        isOneLevelTreeBreadcrumbRadio = new QRadioButton(groupBox);
        isOneLevelTreeBreadcrumbRadio->setObjectName(QStringLiteral("isOneLevelTreeBreadcrumbRadio"));
        isOneLevelTreeBreadcrumbRadio->setGeometry(QRect(10, 65, 121, 31));
        groupBox_2 = new QGroupBox(TabDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 40, 131, 101));
        playlist = new QCheckBox(groupBox_2);
        playlist->setObjectName(QStringLiteral("playlist"));
        playlist->setGeometry(QRect(10, 80, 121, 17));
        interactive = new QCheckBox(groupBox_2);
        interactive->setObjectName(QStringLiteral("interactive"));
        interactive->setGeometry(QRect(10, 50, 121, 17));
        deleteFile = new QCheckBox(groupBox_2);
        deleteFile->setObjectName(QStringLiteral("deleteFile"));
        deleteFile->setGeometry(QRect(10, 20, 121, 17));
#ifndef QT_NO_SHORTCUT
        label->setBuddy(tabName);
#endif // QT_NO_SHORTCUT

        retranslateUi(TabDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TabDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TabDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TabDialog);
    } // setupUi

    void retranslateUi(QDialog *TabDialog)
    {
        TabDialog->setWindowTitle(QApplication::translate("TabDialog", "Dialog", 0));
        label->setText(QApplication::translate("TabDialog", "Name", 0));
#ifndef QT_NO_TOOLTIP
        tabName->setToolTip(QApplication::translate("TabDialog", "This name will be shown in tab header", 0));
#endif // QT_NO_TOOLTIP
        tabName->setText(QString());
#ifndef QT_NO_TOOLTIP
        groupBox->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>List type let you set type of elements placement relative to its logical path.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("TabDialog", "List type", 0));
#ifndef QT_NO_TOOLTIP
        isListRadio->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>In this case you will see items, without any relation to the folder</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        isListRadio->setText(QApplication::translate("TabDialog", "List", 0));
#ifndef QT_NO_TOOLTIP
        isTreeRadio->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p> In this case you will see full path, which will be splited on folders.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        isTreeRadio->setText(QApplication::translate("TabDialog", "Tree", 0));
#ifndef QT_NO_TOOLTIP
        isOneLevelTreeRadio->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>In this case you will see only parent folder of item. If you include two folders with the same name - it will be shown as one folder and will contain items from both folders.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        isOneLevelTreeRadio->setText(QApplication::translate("TabDialog", "One Level Tree", 0));
#ifndef QT_NO_TOOLTIP
        isVkRadio->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>In this case you will see items, without any relation to the folder</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        isVkRadio->setText(QApplication::translate("TabDialog", "Vk", 0));
#ifndef QT_NO_TOOLTIP
        isOneLevelTreeBreadcrumbRadio->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>In this case you will see only parent folder of item. If you include two folders with the same name - it will be shown as one folder and will contain items from both folders.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        isOneLevelTreeBreadcrumbRadio->setText(QApplication::translate("TabDialog", "One Level Tree With \n"
" Breadcrumb", 0));
        groupBox_2->setTitle(QApplication::translate("TabDialog", "List options", 0));
#ifndef QT_NO_TOOLTIP
        playlist->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>After ending playing of current element automatically initiated next item</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        playlist->setText(QApplication::translate("TabDialog", "Play as playlist", 0));
        interactive->setText(QApplication::translate("TabDialog", "Interactive", 0));
#ifndef QT_NO_TOOLTIP
        deleteFile->setToolTip(QApplication::translate("TabDialog", "<html><head/><body><p>With this option when you remove item, you are initiate deletion  of object from your hard disk ( or account data storage)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        deleteFile->setText(QApplication::translate("TabDialog", "Delete file with item", 0));
    } // retranslateUi

};

namespace Ui {
    class TabDialog: public Ui_TabDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABDIALOG_H
