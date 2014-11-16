/********************************************************************************
** Form generated from reading UI file 'settings_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_DIALOG_H
#define UI_SETTINGS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout;
    QPushButton *acceptButton;
    QPushButton *cancelButton;
    QTabWidget *settingsTabs;
    QWidget *common_tab;
    QGroupBox *groupBox;
    QLineEdit *downloadPath;
    QPushButton *browseButton;
    QCheckBox *showCheckboxes;
    QCheckBox *drawMetrics;
    QCheckBox *spoilOnActivate;
    QGroupBox *groupBox_2;
    QCheckBox *showInfo;
    QLabel *label;
    QWidget *tabber_tab;
    QComboBox *tabPositionSelect;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *itemFontSelect;
    QSpinBox *itemFontSize;
    QSpinBox *itemInfoFontSize;
    QLabel *label_9;
    QComboBox *itemInfoFontSelect;
    QLabel *label_10;
    QPushButton *defaultItemTextColorButton;
    QLabel *label_11;
    QPushButton *selectedItemTextColorButton;
    QLabel *label_12;
    QPushButton *selectedItemInfoTextColorButton;
    QLabel *label_13;
    QPushButton *defaultItemInfoTextColorButton;
    QSpinBox *itemHeightSize;
    QLabel *label_14;
    QCheckBox *scrollButtonsCheck;
    QCheckBox *useSystemIconsCheck;
    QWidget *hot_keys_tab;
    QTreeView *treeView;
    QWidget *colors_tab;
    QPushButton *defaultColorButton;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *listenedColorButton;
    QLabel *label_4;
    QPushButton *likedColorButton;
    QLabel *label_5;
    QPushButton *playedColorButton;
    QLabel *label_6;
    QPushButton *folderColorButton;
    QCheckBox *useGradientCheck;
    QWidget *tab;
    QLabel *label_15;
    QSpinBox *spectrumUpdateFrequecy;
    QLabel *label_16;
    QSpinBox *spectrumBarsCount;
    QCheckBox *spectrumMonocolorUse;
    QLabel *label_17;
    QPushButton *spectrumColor;
    QLabel *label_18;
    QSpinBox *spectrumHeight;
    QLabel *label_19;
    QPushButton *spectrumColor2;
    QLabel *label_20;
    QCheckBox *spectrumComboUse;
    QSpinBox *spectrumMultiplier;
    QLabel *label_21;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QStringLiteral("SettingsDialog"));
        SettingsDialog->resize(356, 313);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SettingsDialog->sizePolicy().hasHeightForWidth());
        SettingsDialog->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        acceptButton = new QPushButton(SettingsDialog);
        acceptButton->setObjectName(QStringLiteral("acceptButton"));

        gridLayout->addWidget(acceptButton, 1, 1, 1, 1);

        cancelButton = new QPushButton(SettingsDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 0, 1, 1);

        settingsTabs = new QTabWidget(SettingsDialog);
        settingsTabs->setObjectName(QStringLiteral("settingsTabs"));
        settingsTabs->setTabPosition(QTabWidget::North);
        settingsTabs->setUsesScrollButtons(false);
        common_tab = new QWidget();
        common_tab->setObjectName(QStringLiteral("common_tab"));
        groupBox = new QGroupBox(common_tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(4, 10, 321, 51));
        downloadPath = new QLineEdit(groupBox);
        downloadPath->setObjectName(QStringLiteral("downloadPath"));
        downloadPath->setEnabled(false);
        downloadPath->setGeometry(QRect(10, 22, 231, 20));
        browseButton = new QPushButton(groupBox);
        browseButton->setObjectName(QStringLiteral("browseButton"));
        browseButton->setGeometry(QRect(250, 20, 61, 23));
        showCheckboxes = new QCheckBox(common_tab);
        showCheckboxes->setObjectName(QStringLiteral("showCheckboxes"));
        showCheckboxes->setGeometry(QRect(10, 70, 311, 17));
        drawMetrics = new QCheckBox(common_tab);
        drawMetrics->setObjectName(QStringLiteral("drawMetrics"));
        drawMetrics->setGeometry(QRect(10, 170, 311, 17));
        spoilOnActivate = new QCheckBox(common_tab);
        spoilOnActivate->setObjectName(QStringLiteral("spoilOnActivate"));
        spoilOnActivate->setGeometry(QRect(10, 190, 311, 17));
        groupBox_2 = new QGroupBox(common_tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(4, 100, 321, 61));
        showInfo = new QCheckBox(groupBox_2);
        showInfo->setObjectName(QStringLiteral("showInfo"));
        showInfo->setGeometry(QRect(10, 20, 311, 17));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 40, 321, 16));
        settingsTabs->addTab(common_tab, QString());
        tabber_tab = new QWidget();
        tabber_tab->setObjectName(QStringLiteral("tabber_tab"));
        tabPositionSelect = new QComboBox(tabber_tab);
        tabPositionSelect->setObjectName(QStringLiteral("tabPositionSelect"));
        tabPositionSelect->setGeometry(QRect(80, 40, 241, 22));
        label_7 = new QLabel(tabber_tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 40, 61, 21));
        label_8 = new QLabel(tabber_tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 70, 71, 21));
        itemFontSelect = new QComboBox(tabber_tab);
        itemFontSelect->setObjectName(QStringLiteral("itemFontSelect"));
        itemFontSelect->setGeometry(QRect(80, 70, 201, 22));
        itemFontSize = new QSpinBox(tabber_tab);
        itemFontSize->setObjectName(QStringLiteral("itemFontSize"));
        itemFontSize->setGeometry(QRect(290, 70, 34, 21));
        itemFontSize->setMinimum(2);
        itemFontSize->setValue(10);
        itemInfoFontSize = new QSpinBox(tabber_tab);
        itemInfoFontSize->setObjectName(QStringLiteral("itemInfoFontSize"));
        itemInfoFontSize->setGeometry(QRect(290, 140, 34, 21));
        itemInfoFontSize->setMinimum(2);
        itemInfoFontSize->setValue(10);
        label_9 = new QLabel(tabber_tab);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 140, 71, 21));
        itemInfoFontSelect = new QComboBox(tabber_tab);
        itemInfoFontSelect->setObjectName(QStringLiteral("itemInfoFontSelect"));
        itemInfoFontSelect->setGeometry(QRect(80, 140, 201, 22));
        label_10 = new QLabel(tabber_tab);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 100, 81, 31));
        defaultItemTextColorButton = new QPushButton(tabber_tab);
        defaultItemTextColorButton->setObjectName(QStringLiteral("defaultItemTextColorButton"));
        defaultItemTextColorButton->setGeometry(QRect(100, 100, 31, 23));
        label_11 = new QLabel(tabber_tab);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(160, 100, 91, 31));
        selectedItemTextColorButton = new QPushButton(tabber_tab);
        selectedItemTextColorButton->setObjectName(QStringLiteral("selectedItemTextColorButton"));
        selectedItemTextColorButton->setGeometry(QRect(260, 100, 31, 23));
        label_12 = new QLabel(tabber_tab);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 170, 81, 31));
        selectedItemInfoTextColorButton = new QPushButton(tabber_tab);
        selectedItemInfoTextColorButton->setObjectName(QStringLiteral("selectedItemInfoTextColorButton"));
        selectedItemInfoTextColorButton->setGeometry(QRect(260, 170, 31, 23));
        label_13 = new QLabel(tabber_tab);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(160, 170, 91, 31));
        defaultItemInfoTextColorButton = new QPushButton(tabber_tab);
        defaultItemInfoTextColorButton->setObjectName(QStringLiteral("defaultItemInfoTextColorButton"));
        defaultItemInfoTextColorButton->setGeometry(QRect(100, 170, 31, 23));
        itemHeightSize = new QSpinBox(tabber_tab);
        itemHeightSize->setObjectName(QStringLiteral("itemHeightSize"));
        itemHeightSize->setGeometry(QRect(80, 10, 91, 21));
        itemHeightSize->setMinimum(18);
        itemHeightSize->setValue(18);
        label_14 = new QLabel(tabber_tab);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 10, 61, 21));
        scrollButtonsCheck = new QCheckBox(tabber_tab);
        scrollButtonsCheck->setObjectName(QStringLiteral("scrollButtonsCheck"));
        scrollButtonsCheck->setGeometry(QRect(180, 10, 151, 17));
        useSystemIconsCheck = new QCheckBox(tabber_tab);
        useSystemIconsCheck->setObjectName(QStringLiteral("useSystemIconsCheck"));
        useSystemIconsCheck->setGeometry(QRect(10, 210, 151, 17));
        settingsTabs->addTab(tabber_tab, QString());
        hot_keys_tab = new QWidget();
        hot_keys_tab->setObjectName(QStringLiteral("hot_keys_tab"));
        treeView = new QTreeView(hot_keys_tab);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(0, 0, 374, 296));
        treeView->setRootIsDecorated(false);
        treeView->setUniformRowHeights(true);
        treeView->setItemsExpandable(false);
        treeView->setExpandsOnDoubleClick(false);
        settingsTabs->addTab(hot_keys_tab, QString());
        colors_tab = new QWidget();
        colors_tab->setObjectName(QStringLiteral("colors_tab"));
        defaultColorButton = new QPushButton(colors_tab);
        defaultColorButton->setObjectName(QStringLiteral("defaultColorButton"));
        defaultColorButton->setGeometry(QRect(120, 50, 31, 23));
        label_2 = new QLabel(colors_tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 91, 21));
        label_3 = new QLabel(colors_tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 80, 91, 21));
        listenedColorButton = new QPushButton(colors_tab);
        listenedColorButton->setObjectName(QStringLiteral("listenedColorButton"));
        listenedColorButton->setGeometry(QRect(120, 80, 31, 23));
        label_4 = new QLabel(colors_tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 110, 91, 21));
        likedColorButton = new QPushButton(colors_tab);
        likedColorButton->setObjectName(QStringLiteral("likedColorButton"));
        likedColorButton->setGeometry(QRect(120, 110, 31, 23));
        label_5 = new QLabel(colors_tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 140, 91, 21));
        playedColorButton = new QPushButton(colors_tab);
        playedColorButton->setObjectName(QStringLiteral("playedColorButton"));
        playedColorButton->setGeometry(QRect(120, 140, 31, 23));
        label_6 = new QLabel(colors_tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 170, 91, 21));
        folderColorButton = new QPushButton(colors_tab);
        folderColorButton->setObjectName(QStringLiteral("folderColorButton"));
        folderColorButton->setGeometry(QRect(120, 170, 31, 23));
        useGradientCheck = new QCheckBox(colors_tab);
        useGradientCheck->setObjectName(QStringLiteral("useGradientCheck"));
        useGradientCheck->setGeometry(QRect(20, 20, 131, 17));
        settingsTabs->addTab(colors_tab, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        label_15 = new QLabel(tab);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 10, 141, 21));
        spectrumUpdateFrequecy = new QSpinBox(tab);
        spectrumUpdateFrequecy->setObjectName(QStringLiteral("spectrumUpdateFrequecy"));
        spectrumUpdateFrequecy->setGeometry(QRect(150, 10, 91, 21));
        spectrumUpdateFrequecy->setMinimum(5);
        spectrumUpdateFrequecy->setMaximum(100);
        spectrumUpdateFrequecy->setValue(18);
        label_16 = new QLabel(tab);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(10, 40, 141, 21));
        spectrumBarsCount = new QSpinBox(tab);
        spectrumBarsCount->setObjectName(QStringLiteral("spectrumBarsCount"));
        spectrumBarsCount->setGeometry(QRect(150, 40, 91, 21));
        spectrumBarsCount->setMinimum(5);
        spectrumBarsCount->setMaximum(80);
        spectrumBarsCount->setValue(18);
        spectrumMonocolorUse = new QCheckBox(tab);
        spectrumMonocolorUse->setObjectName(QStringLiteral("spectrumMonocolorUse"));
        spectrumMonocolorUse->setGeometry(QRect(10, 150, 231, 17));
        label_17 = new QLabel(tab);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 180, 81, 21));
        spectrumColor = new QPushButton(tab);
        spectrumColor->setObjectName(QStringLiteral("spectrumColor"));
        spectrumColor->setGeometry(QRect(90, 180, 31, 23));
        label_18 = new QLabel(tab);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(250, 10, 41, 21));
        spectrumHeight = new QSpinBox(tab);
        spectrumHeight->setObjectName(QStringLiteral("spectrumHeight"));
        spectrumHeight->setGeometry(QRect(150, 70, 91, 21));
        spectrumHeight->setMinimum(40);
        spectrumHeight->setMaximum(150);
        spectrumHeight->setValue(40);
        label_19 = new QLabel(tab);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(10, 70, 141, 21));
        spectrumColor2 = new QPushButton(tab);
        spectrumColor2->setObjectName(QStringLiteral("spectrumColor2"));
        spectrumColor2->setGeometry(QRect(220, 180, 31, 23));
        label_20 = new QLabel(tab);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(140, 180, 81, 21));
        spectrumComboUse = new QCheckBox(tab);
        spectrumComboUse->setObjectName(QStringLiteral("spectrumComboUse"));
        spectrumComboUse->setGeometry(QRect(10, 100, 181, 17));
        spectrumMultiplier = new QSpinBox(tab);
        spectrumMultiplier->setObjectName(QStringLiteral("spectrumMultiplier"));
        spectrumMultiplier->setGeometry(QRect(150, 120, 91, 21));
        spectrumMultiplier->setMinimum(1);
        spectrumMultiplier->setMaximum(9);
        spectrumMultiplier->setValue(9);
        label_21 = new QLabel(tab);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(10, 120, 141, 21));
        settingsTabs->addTab(tab, QString());

        gridLayout->addWidget(settingsTabs, 0, 0, 1, 2);


        retranslateUi(SettingsDialog);

        settingsTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Dialog", 0));
        acceptButton->setText(QApplication::translate("SettingsDialog", "Ok", 0));
        cancelButton->setText(QApplication::translate("SettingsDialog", "Cancel", 0));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Default save path for downloads", 0));
        browseButton->setText(QApplication::translate("SettingsDialog", "Browse...", 0));
        showCheckboxes->setText(QApplication::translate("SettingsDialog", "Show checkboxes for batch operations", 0));
        drawMetrics->setText(QApplication::translate("SettingsDialog", "Draw metric grid on sliders", 0));
        spoilOnActivate->setText(QApplication::translate("SettingsDialog", "Spoil item on activation", 0));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "Item info (bitrate, duration, etc...)", 0));
        showInfo->setText(QApplication::translate("SettingsDialog", "Show", 0));
        label->setText(QApplication::translate("SettingsDialog", "<html><head/><body><p><span style=\" font-weight:600; color:#ff0000;\">Very slow for remote files. Require restart of program</span></p></body></html>", 0));
        settingsTabs->setTabText(settingsTabs->indexOf(common_tab), QApplication::translate("SettingsDialog", "Common", 0));
        label_7->setText(QApplication::translate("SettingsDialog", "Tab position", 0));
        label_8->setText(QApplication::translate("SettingsDialog", "Item font", 0));
        label_9->setText(QApplication::translate("SettingsDialog", "Item info font", 0));
        label_10->setText(QApplication::translate("SettingsDialog", "Default item \n"
"text color", 0));
        defaultItemTextColorButton->setText(QString());
        label_11->setText(QApplication::translate("SettingsDialog", "Selected item \n"
"text color", 0));
        selectedItemTextColorButton->setText(QString());
        label_12->setText(QApplication::translate("SettingsDialog", "Default item info \n"
" text color", 0));
        selectedItemInfoTextColorButton->setText(QString());
        label_13->setText(QApplication::translate("SettingsDialog", "Selected item info \n"
" text color", 0));
        defaultItemInfoTextColorButton->setText(QString());
        label_14->setText(QApplication::translate("SettingsDialog", "Item height", 0));
        scrollButtonsCheck->setText(QApplication::translate("SettingsDialog", "Use scroll buttons on tabs", 0));
        useSystemIconsCheck->setText(QApplication::translate("SettingsDialog", "Use system icons for elems", 0));
        settingsTabs->setTabText(settingsTabs->indexOf(tabber_tab), QApplication::translate("SettingsDialog", "Playlist", 0));
        settingsTabs->setTabText(settingsTabs->indexOf(hot_keys_tab), QApplication::translate("SettingsDialog", "Hot keys", 0));
        defaultColorButton->setText(QString());
        label_2->setText(QApplication::translate("SettingsDialog", "Default color", 0));
        label_3->setText(QApplication::translate("SettingsDialog", "Listened color", 0));
        listenedColorButton->setText(QString());
        label_4->setText(QApplication::translate("SettingsDialog", "Liked color", 0));
        likedColorButton->setText(QString());
        label_5->setText(QApplication::translate("SettingsDialog", "Played color", 0));
        playedColorButton->setText(QString());
        label_6->setText(QApplication::translate("SettingsDialog", "Folder color", 0));
        folderColorButton->setText(QString());
        useGradientCheck->setText(QApplication::translate("SettingsDialog", "Use gradient", 0));
        settingsTabs->setTabText(settingsTabs->indexOf(colors_tab), QApplication::translate("SettingsDialog", "Item Colors", 0));
        label_15->setText(QApplication::translate("SettingsDialog", "Spectrum update frequency", 0));
        label_16->setText(QApplication::translate("SettingsDialog", "Spectrum bars count", 0));
        spectrumMonocolorUse->setText(QApplication::translate("SettingsDialog", "Use custom colors", 0));
        label_17->setText(QApplication::translate("SettingsDialog", "Custom color 1", 0));
        spectrumColor->setText(QString());
        label_18->setText(QApplication::translate("SettingsDialog", "millis", 0));
        label_19->setText(QApplication::translate("SettingsDialog", "Spectrum height", 0));
        spectrumColor2->setText(QString());
        label_20->setText(QApplication::translate("SettingsDialog", "Custom color 2", 0));
        spectrumComboUse->setText(QApplication::translate("SettingsDialog", "Combine channels to one layer", 0));
        label_21->setText(QApplication::translate("SettingsDialog", "Signal multiplier", 0));
        settingsTabs->setTabText(settingsTabs->indexOf(tab), QApplication::translate("SettingsDialog", "Spectrum", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_DIALOG_H
