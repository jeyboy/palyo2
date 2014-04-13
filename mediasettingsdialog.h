#ifndef MEDIASETTINGSDIALOG_H
#define MEDIASETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
  class MediaSettingsDialog;
}

class MediaSettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit MediaSettingsDialog(QWidget *parent = 0);
  ~MediaSettingsDialog();

private slots:
    void on_closeButton_clicked();

private:
  Ui::MediaSettingsDialog *ui;
};

#endif // MEDIASETTINGSDIALOG_H
