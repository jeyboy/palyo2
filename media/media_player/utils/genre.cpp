#include "genre.h"

Genre *Genre::self = 0;

Genre *Genre::instance() {
    if(!self)
        self = new Genre();
    return self;
}

int Genre::toInt(QString name) const {
    return genres -> key(name, 12);
}
