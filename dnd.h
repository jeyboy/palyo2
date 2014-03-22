#ifndef DND_H
#define DND_H

#include <QString>

class DnD {
public:
    QString files;
    QString listItems;
    static DnD * instance();

    static void close() {
        delete self;
    }

private:
    DnD(){
        files = "text/uri-list";
        listItems = "application/x-QListView-DragAndDrop";
    }

    static DnD *self;
};

#endif // DND_H
