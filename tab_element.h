#ifndef TAB_ELEMENT_H
#define TAB_ELEMENT_H

#include <QStandardItem>
#include <QIcon>

class TabElement : QStandardItem {
public:
    explicit TabElement(const QString &text, QString &file_path, QString &file_extension, bool item_state) : QStandardItem(text) {
        state = item_state;
        path = file_path;
        extension = file_extension;
    }
    TabElement(const QIcon &icon, const QString &text, QString &file_path, QString &file_extension, bool item_state) : QStandardItem(icon, text) {
        state = item_state;
        path = file_path;
        extension = file_extension;
    }
private:
    bool state;
    QString path;
    QString extension;
};

#endif // TAB_ELEMENT_H
