#include "library.h"
#include "qdebug.h"

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

void Library::initItem(LibraryItem * libItem) {
    libItem -> item() -> getState() -> setProceed();
    QtConcurrent::run(this, &Library::itemsInit, libItem);
}

bool Library::addItem(ModelItem * item, int state) {
    if (item -> names == 0) {
        item -> names = getNamesForItem(item);
    }

    if (state == STATE_LIKED)
        state = 1;
    else state = 0;

    return proceedItemNames(item -> names, state);
}

void Library::restoreItemState(LibraryItem * libItem) {
    QHash<QString, int> * cat;
    bool isListened = false;
    int temp;
    QString name;
    QList<QString>::iterator i;

    for (i = libItem -> item() -> names -> begin(); i != libItem -> item() -> names -> end(); ++i) {
        name = (*i);
        cat = getCatalog(name);

        if (cat -> contains(name)) {
            temp = cat -> value(name);

            if (temp == 1) {
                libItem -> refresh(STATE_LIKED);
                return;
            }

            isListened = isListened || (temp == 0);
        }
    }

    if (isListened)
        libItem -> refresh(STATE_LISTENED);
}

//void Library::setItemState(const QString filename, int state) {
//    QHash<QString, int> cat = getCatalog(filename);
//    cat.insert(filename, state);
//    instance() -> catalogs_state.append(getCatalogName(filename.at(0)));
//}


//////////////////////////////////////////////////////////////////////
/// privates
//////////////////////////////////////////////////////////////////////

void Library::saveCatalogs() {
    if (!catsSaveResult.isRunning()) {
        catsSaveResult = QtConcurrent::run(this, &Library::save);
    }
}

void Library::itemsInit(LibraryItem * libItem) {
    if (libItem -> item() -> isExist()) {
        if (saveBlock.tryLock(-1)) {
            libItem -> item() -> names = getNamesForItem(libItem -> item());
            restoreItemState(libItem);
            saveBlock.unlock();
        }
    } else {
        libItem -> refresh(STATE_NOT_EXIST);
    }
}

QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(\\w+)(\\.(\\w+))?)")); }
QString Library::forwardNumberPreFilter(QString title)	{ return title.remove(QRegExp("\\A\\d{1,}.|\\(\\w*\\d{1,}\\w*\\)")); }
QString Library::spacesFilter(QString title) 			{ return title.remove(QRegExp("(\\W|[_])")); }
QString Library::forwardNumberFilter(QString title)		{ return title.remove(QRegExp("\\A\\d{1,}")); }

QString Library::libraryPath() {
    return QCoreApplication::applicationDirPath() + "/library/";
}

QString Library::prepareName(QString gipoTitle, bool additional) {
    if (additional)
        return forwardNumberFilter(gipoTitle);
    else {
        QString temp = sitesFilter(gipoTitle.toLower());
        return spacesFilter(forwardNumberPreFilter(temp));
    }
}

bool Library::proceedItemNames(QList<QString> * names, int state) {
    QHash<QString, int> * cat;
    QChar letter;
    bool catState = false, catalog_has_item, catalog_state_has_item;
    QString name;
    QList<QString> * saveList;
    QList<QString>::iterator i;

    for (i = names -> begin(); i != names -> end(); ++i) {
        saveList = 0;
        name = (*i);
        letter = getCatalogChar(name);
        cat = getCatalog(letter);

        catalog_has_item = cat -> contains(name);

        if (!catalog_has_item || (catalog_has_item && cat -> value(name) < state)) {
            qDebug() << "********************************** insert Name " << name;
            cat -> insert(name, state);

            catalog_state_has_item = catalogs_state.contains(letter);
            if (catalog_state_has_item) {
                saveList = catalogs_state.value(letter);
                qDebug() << "LLL: " << letter << " Libb: " << (*saveList);
            }

            if (catalog_has_item) {
                if (saveList) {
                    delete saveList;
                }
                catalogs_state.insert(letter, 0);
            } else {
                if (!catalog_state_has_item) {
                    if (saveList == 0) {
                        saveList = new QList<QString>();
                    }
                }

                saveList -> append(name);
                catalogs_state.insert(letter, saveList);
            }
        }
    }

    return catState;
}

QChar Library::getCatalogChar(QString name) {
    if (name.length() == 0) return '_';
    return name.at(0);
}

//QChar Library::getCatalogChar(QChar l) {
//    if (l >= (int)'a' && l <= (int)'z') return l;
//    if (l == (int)'ъ' || l == (int)'ё' || l == (int)'ь' || l == (int)'ы') return '_';
//    if (l >= (int)'а' && l <= (int)'я') return l;
//    return '_';
//}

QHash<QString, int> * Library::getCatalog(QChar letter) {
    if (catalogs -> contains(letter)) {
        return catalogs -> value(letter);
    } else {
        QHash<QString, int> * res = load(letter);
        catalogs -> insert(letter, res);
        return res;
    }
}

QHash<QString, int> * Library::getCatalog(QString name) {
    if (name.length() == 0) return new QHash<QString, int>();

    QChar c = getCatalogChar(name);
    return getCatalog(c);
}

QList<QString> * Library::getNamesForObject(QString path, QString name) {
    QList<QString> * res = new QList<QString>();

    res -> append(name);
    QString temp = prepareName(name, true);
    if (temp != name)
        res -> append(temp);

    MediaInfo m(path);

    QString temp2 = prepareName(m.getArtist() + m.getTitle());
    if (!temp2.isEmpty() && temp2 != name && temp2 != temp)
        res -> append(temp2);

    return res;
}

QList<QString> * Library::getNamesForItem(ModelItem * item) {
    QString name = prepareName(item -> data(NAMEUID).toString());

    return getNamesForObject(item -> fullpath(), name);
}

QList<QString> * Library::getNamesForItem(QString path) {
    QString name = path.section('/', -1, -1);
    name = prepareName(name.section('.', 0, -2));

    return getNamesForObject(path, name);
}

QHash<QString, int> * Library::load(const QChar letter) {
    QHash<QString, int> * res = new QHash<QString, int>();

    QString path = libraryPath() + "cat_" + letter;

    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar;
        QString name;
        int state;

        while(!f.atEnd()) {
            ar = f.readLine();
            state = ar.mid(0, 1).toInt();
            name = QString::fromLocal8Bit(ar.mid(1, ar.length() - 3));
            res -> insert(name, state);
        }

        f.close();
    }

    return res;
}

void Library::save() {
    if (saveBlock.tryLock(-1)) {
        QHash<QString, int> * res;
        QHash<QChar, QList<QString> *>::iterator i = catalogs_state.begin();

        bool result;

        while(i != catalogs_state.end()) {
            qDebug() << i.key();
            res = catalogs -> value(i.key());

            if (i.value()) {
                result = fileDump(i.key(), *i.value(), QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

                if(result) {
                    delete catalogs_state.value(i.key());
                    catalogs_state.insert(i.key(), 0);
                }
            } else {
                QList<QString> keys = res -> keys();
                result = fileDump(i.key(), keys, QIODevice::WriteOnly | QIODevice::Text);
            }

            if (result) {
                i = catalogs_state.erase(i);
            } else {
                i++;
            }
        }

        saveBlock.unlock();
    }
}

bool Library::fileDump(QChar key, QList<QString> &keysList, QFlags<QIODevice::OpenModeFlag> openFlags) {
    QString path, val;
    QList<QString>::const_iterator cat_i = keysList.cbegin();
    QHash<QString, int> * res = catalogs -> value(key);

    path = libraryPath() + "cat_" + key;
    qDebug() << "SSave list "<< keysList;

    QFile f(path);
    if (f.open(openFlags)) {
        QTextStream out(&f);

        while(cat_i != keysList.cend()) {
            val = *cat_i;
            qDebug() << "Curr val " << val;
            out << QString::number(res -> value(val)) + val + "\n";
            cat_i++;
        }

        f.close();

        return true;
    }

    return false;
}
