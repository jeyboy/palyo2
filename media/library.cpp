#include "library.h"
#include "player.h"
#include "qdebug.h"

Library *Library::self = 0;

Library *Library::instance() {
    if(!self)
        self = new Library();
    return self;
}

//QFutureWatcher

void Library::clearRemote() {
    currRemote = 0;
    remote_items.clear();
    remote_collations.clear();
}

void Library::removeRemoteItem(ModelItem * item) {
    if (item == currRemote) {
        currRemote = 0;
    } else {
        remote_items.removeAll(item);
        remote_collations.take(item);
    }
}

void Library::initItem(ModelItem * item, const QObject * caller, const char * slot) {
    QFutureWatcher<ModelItem *> * initiator = new QFutureWatcher<ModelItem *>();
    connect(initiator, SIGNAL(finished()), caller, slot);
    initiator -> setFuture(QtConcurrent::run(this, &Library::itemsInit, item));
    if (item -> isRemote() && !item -> hasInfo()) {
        remote_items.append(item);
        remote_collations.insert(item, FuncContainer(caller, slot));

        if (remote_items.count() > remote_items_max) {
            ModelItem * i = remote_items.takeFirst();
            remote_collations.take(i);
            i -> getState() -> unsetProceed();
        }
    }
}

bool Library::addItem(ModelItem * item, int state) {
    initItemInfo(item);

    if (state == STATE_LIKED)
        state = 1;
    else state = 0;

    return proceedItemNames(item -> getTitlesCache(), state);
}

QString Library::filenameFilter(QString title) {
    return forwardNumberPreFilter(sitesFilter(title)).remove(QRegExp("[^\\w-\\. ()]*")).mid(0, 200);
}

void Library::setRemoteItemMax(int newMax) {
    remote_items_max = newMax;
}

void Library::restoreItemState(ModelItem * item) {
    QHash<QString, int> * cat;
    bool isListened = false;
    int temp;
    QString name;
    QList<QString>::iterator i;

    for (i = item -> getTitlesCache() -> begin(); i != item -> getTitlesCache() -> end(); ++i) {
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

void Library::startRemoteInfoProc() {
    if (!remote_items.isEmpty()) {
        currRemote = remote_items.takeLast();
        FuncContainer func = remote_collations.take(currRemote);
        if (!currRemote -> hasInfo()) {
            QFutureWatcher<ModelItem *> * initiator = new QFutureWatcher<ModelItem *>();
            connect(initiator, SIGNAL(finished()), func.obj, func.slot);
            initiator -> setFuture(QtConcurrent::run(this, &Library::procRemoteInfo, currRemote));
        }
    }
}

ModelItem * Library::procRemoteInfo(ModelItem * item) {
    if (currRemote == 0) return 0;
    QHash<QString, QString> info = Player::instance() -> getFileInfo(item -> toUrl());
    if (currRemote == 0) return 0;
    item -> setDuration(info.value("duration"));
    if (currRemote == 0) return 0;
    item -> setInfo(info.value("info"));

//    TODO: get genre
//    item -> setGenre();
    return item;
}

void Library::saveCatalogs() {
    if (!catsSaveResult.isRunning()) {
        catsSaveResult = QtConcurrent::run(this, &Library::save);
    }
}

ModelItem * Library::itemsInit(ModelItem * item) {
    if (item -> isExist()) {
        if (saveBlock.tryLock(-1)) {
            initItemInfo(item);

            restoreItemState(item);
            saveBlock.unlock();
        }
    } else {
        item -> getState() -> setNotExist();
    }

    return item;
}

QString Library::sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(\\w+)(\\.(\\w+))*)")); }
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
        QString temp = sitesFilter(gipoTitle.toLower().replace('_', ' '));
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
                if (saveList != 0)
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

//QList<QString> * Library::getNamesForObject(QString path, QString name) {
//    QList<QString> * res = new QList<QString>();

//    res -> append(name);
//    QString temp = prepareName(name, true);
//    if (temp != name)
//        res -> append(temp);

//    MediaInfo m(path);

//    QString temp2 = prepareName(m.getArtist() + m.getTitle());
//    if (!temp2.isEmpty() && temp2 != name && temp2 != temp)
//        res -> append(temp2);

//    return res;
//}

void Library::initItemInfo(ModelItem * item) {
    QList<QString> * res;

    if (!item -> cacheIsPrepared() || !item -> hasInfo()) {
        QString name = prepareName(item -> data(TITLEID).toString());

        if (item -> isRemote()) {
            if (!item -> cacheIsPrepared()) {
                res = new QList<QString>();

                res -> append(name);
                QString temp = prepareName(name, true);
                if (temp != name)
                    res -> append(temp);

                item -> setCache(res);
            }

//            if (!item -> hasInfo()) {
//                //TODO: decrease requests rate
////                QHash<QString, QString> info = Player::instance() -> getRemoteFileInfo(item -> fullPath());

////                item -> setDuration(info.value("duration"));
////                item -> setInfo(info.value("info"));

//                //TODO: get genre
////                item -> setGenre();
//            }

        } else {
            MediaInfo m(item -> fullPath(), item -> hasInfo());

            if (!item -> cacheIsPrepared()) {
                res = new QList<QString>();

                res -> append(name);
                QString temp = prepareName(name, true);
                if (temp != name)
                    res -> append(temp);


                QString temp2 = prepareName(m.getArtist() + m.getTitle());
                if (!temp2.isEmpty() && temp2 != name && temp2 != temp)
                    res -> append(temp2);

                item -> setCache(res);
            }

            if (!item -> hasInfo() && m.initiated()) {
                int bitrate = m.getBitrate();

                if (bitrate == 0) {
                    QHash<QString, QString> info = Player::instance() -> getFileInfo(item -> toUrl(), true);
                    bitrate = info.value("bitrate").toInt();
                }

                item -> setInfo(Format::toInfo(Format::toUnits(m.getSize()), bitrate, m.getSampleRate(), m.getChannels()));
                item -> setDuration(Duration::fromSeconds(m.getDuration()));
                item -> setGenre(Genre::instance() -> toInt(m.getGenre()));
            }
        }
    }
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
            name = QString::fromUtf8(ar.mid(1, ar.length() - 3));
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
        out.setCodec("UTF-8");

        while(cat_i != keysList.cend()) {
            val = *cat_i;
            qDebug() << "Curr val " << val;
            out << QString::number(res -> value(val)) << val.toUtf8() << "\n";
            cat_i++;
        }

        f.close();

        return true;
    }

    return false;
}
