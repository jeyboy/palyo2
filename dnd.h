#ifndef DND_H
#define DND_H

#include <QString>

class DnD {
public:
    QString files;
    QString listItems;
    static DnD * instance();

private:
    DnD(){
        files = "text/uri-list";
        listItems = "application/x-QListView-DragAndDrop";
    }

    static DnD *self;
};

#endif // DND_H
