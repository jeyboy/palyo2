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

private slots:
    void on_addPreset_clicked();

    void on_presets_currentIndexChanged(const QString &arg1);

    void on_addExtension_clicked();

private:
    void proceedFilter(QString & filter, QStringList & preset);

    Ui::ExtensionDialog *ui;
};

#endif // EXTENSION_DIALOG_H
