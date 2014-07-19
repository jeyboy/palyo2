#ifndef EXTENSION_DIALOG_H
#define EXTENSION_DIALOG_H

#include <QDialog>

namespace Ui {
class ExtensionDialog;
}

class ExtensionDialog : public QDialog {
    Q_OBJECT

public:
    explicit ExtensionDialog(QWidget *parent = 0);
    ~ExtensionDialog();

private:
    Ui::ExtensionDialog *ui;
};

#endif // EXTENSION_DIALOG_H
