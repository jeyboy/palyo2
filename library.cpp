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
    instance() -> items.insert(0, item);
    if (!instance() -> itemsInitResult.isRunning())
        instance() -> itemsInitResult = QtConcurrent::run(instance(), &Library::itemsInit);
}

bool Library::addItem(ModelItem * item, int state) {
    if (item -> names == 0) {
        item -> names = getNamesForItem(item);
    }

    return proceedItemNames(item -> names, state);
}

//int Library::getItemState(const QString filename) {
//    QHash<QString, int> cat = getCatalog(filename);
//    if (cat.contains(filename)) {
//        return cat.value(filename);
//    } else {
//        return -1;
//    }
//}

//void Library::setItemState(const QString filename, int state) {
//    QHash<QString, int> cat = getCatalog(filename);
//    cat.insert(filename, state);
//    instance() -> catalogs_state.append(getCatalogName(filename.at(0)));
//}


//////////////////////////////////////////////////////////////////////
/// privates
//////////////////////////////////////////////////////////////////////

void Library::itemsInit() {
    ModelItem * temp;

    while(!items.isEmpty()) {
        temp = items.takeFirst();

        temp -> names = getNamesForItem(temp);
        qDebug() << temp -> names;
    }
}

QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(com|net))")); }
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
    QHash<QString, int> cat;
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
        catState = catState || !cat.contains(name);
        cat.insert(name, state);
        if (!instance() -> catalogs_state.contains(letter))
            instance() -> catalogs_state.append(letter);
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

QHash<QString, int> Library::getCatalog(QChar letter) {
    if (instance() -> catalogs -> contains(letter)) {
        return instance() -> catalogs -> value(letter);
    } else {
        QHash<QString, int> res = load(letter);
        instance() -> catalogs -> insert(letter, res);
        return res;
    }
}

QHash<QString, int> Library::getCatalog(QString name) {
    if (name.length() == 0) return QHash<QString, int>();

    QChar c = getCatalogChar(name);
    return getCatalog(c);
}

QList<QString> * Library::getNamesForObject(QString path, QString name) {
    QList<QString> * res = new QList<QString>();

    res -> append(name);
    res -> append(prepareName(name, true));

    MediaInfo m(path);

    res -> append((m.getArtist() + m.getTitle()).toLower());

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

QHash<QString, int> Library::load(const QChar letter) {
    QHash<QString, int> res = QHash<QString, int>();

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
            res.insert(name, state);
            qDebug() << name << "\n" << state;
        }

        f.close();
    }

    return res;
}

void Library::save(const QChar c) {
    QHash<QString, int> res = instance() -> catalogs -> value(c);

    QString path = QCoreApplication::applicationDirPath() + "/library/cat_" + c;
//    qDebug() << letter << " :: " << path;

    QFile f(path);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);

        foreach(QString key, res.keys()) {
            out << QString::number(res.value(key)) + key + "\n";
        }

        f.close();
    }
}
