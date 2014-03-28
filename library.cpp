#include "library.h"
#include "qdebug.h"

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

void Library::initItem(ModelItem * item) {
    item -> getState() -> setProceed();
    items.insert(0, item);
    if (!itemsInitResult.isRunning())
        itemsInitResult = QtConcurrent::run(this, &Library::itemsInit);
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

void Library::restoreItemState(ModelItem * item) {
    QHash<QString, int> * cat;
    bool isListened = false;
    int temp;
    QString name;
    QList<QString>::iterator i;

    for (i = item -> names -> begin(); i != item -> names -> end(); ++i) {
        name = (*i);
        cat = getCatalog(name);

        if (cat -> contains(name)) {
            temp = cat -> value(name);

            if (temp == 1) {
                item -> getState() -> setLiked();
                return;
            }

            isListened = isListened || (temp == 0);
        }
    }

    if (isListened)
        item -> getState() -> setListened();
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
    if (!catsSaveResult.isRunning())
        catsSaveResult = QtConcurrent::run(this, &Library::save);
}

void Library::itemsInit() {
    ModelItem * temp;

    while(!items.isEmpty()) {
        temp = items.takeFirst();

        temp -> names = getNamesForItem(temp);
        restoreItemState(temp);
    }
}

QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(\\w+)(\\.(\\w+))?)")); }
QString Library::forwardNumberPreFilter(QString title)	{ return title.remove(QRegExp("\\A\\d{1,}.|\\(\\w*\\d{1,}\\w*\\)")); }
QString Library::spacesFilter(QString title) 			{ return title.remove(QRegExp("(\\W|[_])")); }
QString Library::forwardNumberFilter(QString title)		{ return title.remove(QRegExp("\\A\\d{1,} ")); }

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
    bool catState = false;
    QString name;
    QList<QString>::iterator i;

    // test needed
    for (i = names -> begin(); i != names -> end(); ++i) {
        name = (*i);
        qDebug() << "Name: " << name;
        letter = getCatalogChar(name);
        cat = getCatalog(letter);

        if (cat -> contains(name)) {
            catState = catState || !cat -> contains(name);
            if (cat -> value(name) > state)
                cat -> insert(name, state);
        } else {
            cat -> insert(name, state);
        }

        if (!catalogs_state.contains(letter))
            catalogs_state.append(letter);
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

    QString path = QCoreApplication::applicationDirPath() + "/library/cat_" + letter;

    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar;
        QString name;
        int state;

        while(!f.atEnd()) {
            ar = f.readLine();
            state = ar.mid(0, 1).toInt();
            name = QString(ar.mid(1, ar.length() - 3));
            res -> insert(name, state);
            qDebug() << name << "\n" << state;
        }

        f.close();
    }

    return res;
}

// split cats on overwritten and appended
void Library::save() {
    QHash<QString, int> * res;
    QString path;
    QChar letter;
    QList<QChar>::iterator i;

    for (i = catalogs_state.begin(); i != catalogs_state.end(); ) {
        letter = (*i);

        res = catalogs -> value(letter);

        path = QCoreApplication::applicationDirPath() + "/library/cat_" + letter;
        qDebug() << " :: " << path;

        QFile f(path);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << " :: in " << path << " | " << res -> keys();
            QTextStream out(&f);

            foreach(QString key, res -> keys()) {
                qDebug() << QString::number(res -> value(key)) + key + "\n";
                out << QString::number(res -> value(key)) + key + "\n";
            }

            f.close();
            i = catalogs_state.erase(i);
        } else {
            i++;
        }
    }
}
