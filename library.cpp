#include "library.h"
#include "qdebug.h"

//TODO: all funs require tests

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

//default state - listened
bool Library::addItem(const QString filename, int state = 1) {
    QHash<QString, int> cat = getCatalog(filename);
    bool catState = !cat.contains(filename);
    cat.insert(filename, state);

    return catState;
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

    QString path = QCoreApplication::applicationDirPath() + "library/cat_" + letter;
    qDebug() << letter << " :: " << path;

    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar;
        QString name;
        int state;

        while(!f.atEnd()) {
            ar = f.readLine();
            state = ar.remove(0, 1).toInt();
            name = QString(ar);
            res.insert(name, state);
            qDebug() << name << "::" << state;
        }

        f.close();
    }

    return res;
}

void Library::save(const QChar letter) {
    QChar c = getCatalogName(letter);
    QHash<QString, int> res = instance() -> catalogs -> value(c);

    QString path = QCoreApplication::applicationDirPath() + "library/cat_" + c;
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
