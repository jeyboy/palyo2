/********************************************************************************
** Form generated from reading UI file 'captchadialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAPTCHADIALOG_H
#define UI_CAPTCHADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_CaptchaDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *captchaImg;
    QLineEdit *captchaText;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CaptchaDialog)
    {
        if (CaptchaDialog->objectName().isEmpty())
            CaptchaDialog->setObjectName(QStringLiteral("CaptchaDialog"));
        CaptchaDialog->resize(174, 114);
        gridLayout = new QGridLayout(CaptchaDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        captchaImg = new QLabel(CaptchaDialog);
        captchaImg->setObjectName(QStringLiteral("captchaImg"));
        captchaImg->setStyleSheet(QLatin1String("QLabel {\n"
"	border: 1px solid #000;\n"
"	background-color: #fff;\n"
"}"));

        gridLayout->addWidget(captchaImg, 0, 0, 1, 1);

        captchaText = new QLineEdit(CaptchaDialog);
        captchaText->setObjectName(QStringLiteral("captchaText"));
        captchaText->setMinimumSize(QSize(0, 26));

        gridLayout->addWidget(captchaText, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(CaptchaDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(CaptchaDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CaptchaDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CaptchaDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CaptchaDialog);
    } // setupUi

    void retranslateUi(QDialog *CaptchaDialog)
    {
        CaptchaDialog->setWindowTitle(QApplication::translate("CaptchaDialog", "Captcha :(", 0));
        captchaImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CaptchaDialog: public Ui_CaptchaDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAPTCHADIALOG_H
