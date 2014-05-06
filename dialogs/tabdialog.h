#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

#ifndef CBHASH
#define CBHASH

  #include <QHash>
  typedef QHash <QString, bool> CBHash;
  Q_DECLARE_METATYPE(CBHash);
#endif // CBHASH


namespace Ui {
  class TabDialog;
}

class TabDialog : public QDialog {
  Q_OBJECT

public:
  explicit TabDialog(QWidget *parent = 0);
  ~TabDialog();

  QString getName();
  void setName(QString name);

  CBHash getSettings();
  void setSettings(CBHash hash);

private:
  Ui::TabDialog *ui;
};

#endif // TABDIALOG_H