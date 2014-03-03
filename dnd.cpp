#include "dnd.h"

DnD *DnD::self = 0;

DnD *DnD::instance() {
    if(!self)
        self = new DnD();
    return self;
}
