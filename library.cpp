#include "library.h"

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

bool Library::hasItem(const QString filename) {}
bool Library::addItem(const QString filename) {}

void Library::load(const QChar letter) {}
void Library::save(const QChar letter) {}
