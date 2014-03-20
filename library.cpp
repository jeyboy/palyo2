#include "library.h"
#include "qdebug.h"

//TODO: all funs require tests

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

//QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/)?(www\\.)?[\\w-]+\\.(com|net))")); }
QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(com|net))")); }
QString Library::forwardNumberPreFilter(QString title)	{ return title.remove(QRegExp("\\A\\d{1,}.|\\(\\w*\\d{1,}\\w*\\)")); }
QString Library::spacesFilter(QString title) 			{ return title.remove(QRegExp("(\\W|[_])")); }
QString Library::forwardNumberFilter(QString title)		{ return title.remove(QRegExp("\\A\\d{1,} ")); }

QString Library::prepareName(QString gipoTitle) {
    QString temp = sitesFilter(gipoTitle.toLower());
    return spacesFilter(forwardNumberPreFilter(temp));

//    if (!titles.contains(temp)) titles.add(temp);
//    temp = forwardNumberFilter(temp);
//    if (!titles.contains(temp)) titles.add(temp);
}

bool Library::proceedItem(const QString preparedName, int state, bool last) {
    QHash<QString, int> cat = getCatalog(preparedName);
    bool catState = !cat.contains(preparedName);
    cat.insert(preparedName, state);
    instance() -> catalogs_state.append(getCatalogName(preparedName.at(0)));

    if (last)
        return catState;
    else
        return catState & proceedItem(forwardNumberFilter(preparedName), state);
}

void Library::initItem(ModelItem * item) {
    instance() -> items.insert(0, item);
    if (!instance() -> itemsInitResult.isRunning())
        instance() -> itemsInitResult = QtConcurrent::run(instance(), &Library::itemsInit);
}


//default state - listened
bool Library::addItem(const QString filename, int state) {
    return proceedItem(prepareName(filename), state, false);
}

int Library::getItemState(const QString filename) {
    QHash<QString, int> cat = getCatalog(filename);
    if (cat.contains(filename)) {
        return cat.value(filename);
    } else {
        return -1;
    }
}
void Library::setItemState(const QString filename, int state) {
    QHash<QString, int> cat = getCatalog(filename);
    cat.insert(filename, state);
    instance() -> catalogs_state.append(getCatalogName(filename.at(0)));
}

void Library::itemsInit() {
    ModelItem * temp;

    while(!items.isEmpty()) {
        temp = items.takeFirst();


    }
}

QChar Library::getCatalogName(QChar l) {
    if (l >= (int)'a' && l <= (int)'z') return l;
    if (l == (int)'ъ' || l == (int)'ё' || l == (int)'ь' || l == (int)'ы') return '_';
    if (l >= (int)'а' && l <= (int)'я') return l;
    return '_';
}

QHash<QString, int> Library::getCatalog(QString name) {
    if (name.length() == 0) return QHash<QString, int>();

    QChar c = getCatalogName(name.at(0));
    if (instance() -> catalogs -> contains(c)) {
        return instance() -> catalogs -> value(c);
    } else {
        QHash<QString, int> res = load(c);
        instance() -> catalogs -> insert(c, res);
        return res;
    }
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

void Library::save(const QChar letter) {
    QChar c = getCatalogName(letter);
    QHash<QString, int> res = instance() -> catalogs -> value(c);

    QString path = QCoreApplication::applicationDirPath() + "/library/cat_" + c;
    qDebug() << letter << " :: " << path;

    QFile f(path);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);

        foreach(QString key, res.keys()) {
            out << QString::number(res.value(key)) + key + "\n";
        }

        f.close();
    }
}
