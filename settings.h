#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : QSettings {
public:
  void setSize(int width, int height) {
    this.setValue("size/width", width);
    this.setValue("size/height", height);
  }

  int getWidth() { return this.value("size/width").toInt(); }
  int getHeight() { return this.value("size/height").toInt(); }

  QString getLibraryPath() { return QDir(QCoreApplication::applicationDirPath()).filePath("library");  }
};

#endif // SETTINGS_H
