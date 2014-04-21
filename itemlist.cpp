#include "itemlist.h"
#include <QDebug>

ItemList::ItemList(QWidget *parent, CBHash settingsSet, QJsonObject * attrs) : QTreeView(parent) {   
    settings = settingsSet;
    setIndentation(8);

//    setStyleSheet(QString(
//                      "QTreeView {"
//                          "background-color: black;"
//                      "}"
//                      ));

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

    setExpandsOnDoubleClick(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//  setDragDropOverwriteMode(true);
//  setMovement(QTreeView::Snap);

//  setFlow(QListView::TopToBottom);

    model = new Model(attrs);
    setModel(model);
//    setTreePosition(2);
//    setRootIndex();
//    setRowHidden(0, rootIndex(), true);

//    expandAll();

    setItemDelegate(new ModelItemDelegate(this));

    setContextMenuPolicy(Qt::CustomContextMenu);
    setIconSize(QSize(0,0));

    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_doubleClick(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), model, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), model, SLOT(collapsed(const QModelIndex &)));

    connect(model, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(model, SIGNAL(itemsCountChanged(int)), parent, SLOT(updateHeader(int)));

    header() -> setSectionResizeMode(0, QHeaderView::Interactive);
//    header()->setStretchLastSection(false);
}

ItemList::~ItemList() {
    delete model;
}

Model * ItemList::getModel() const {
    return model;
}

void ItemList::keyPressEvent(QKeyEvent *event) {
    if (event ->key() == Qt::Key_Enter || event ->key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (list.count() > 0) {
            ModelItem * item = model -> getItem(list.first());
            execItem(item);
        }
    } else if (event ->key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        QModelIndex modelIndex;

        for(int i = list.count() - 1; i >= 0; i--) {
            modelIndex = list.at(i);
            removeItem(model -> getItem(modelIndex));
        }
    } else { QTreeView::keyPressEvent(event); }
}

ModelItem * ItemList::activeItem(bool next) {
    ModelItem * item = 0;

    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem()) {
            item = Player::instance() -> playedItem();
        }
    }

    // has some bug on unprocessed select
    if (item == 0) {
        QModelIndexList list = selectedIndexes();

        if (list.count() > 0) {
            item = model -> getItem(list.first());

            if (!item -> getState() -> isUnprocessed()) {
                QModelIndex m;
                if (next) {
                    m = this -> indexAbove(list.first());
                } else { m = this -> indexBelow(list.first()); }

                if (m.isValid()) {
                   item = model -> getItem(m);
                } else {
                   item = model -> getItem(list.first().parent());
                }
            }
        } else {
            item = model -> getItem(this -> rootIndex());
        }
    }

    return item;
}

void ItemList::proceedPrev() {
    ModelItem * item = activeItem(false);

    if (item == 0) return;

    item = prevItem(item);
    execItem(item);
}

void ItemList::proceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);
    execItem(item);
}

void ItemList::deleteCurrentProceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);

    // check logic !!!
    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem()) {
            removeItem(Player::instance() -> playedItem());
        }
    }

    execItem(item);
}

void ItemList::dragEnterEvent(QDragEnterEvent *event) {
    if (event -> source() != this && event -> mimeData() -> hasFormat("text/uri-list"))
        event -> accept();
    else event -> ignore();
}

void ItemList::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> source() != this && event -> mimeData() -> hasFormat("text/uri-list"))
        event -> accept();
    else event -> ignore();
}


//static QStringList nameFiltersFromString(const QString &nameFilter);

//QStringList entryList(Filters filters = NoFilter, SortFlags sort = NoSort) const;
//QStringList entryList(const QStringList &nameFilters, Filters filters = NoFilter,
//                      SortFlags sort = NoSort) const;

//QFileInfoList entryInfoList(Filters filters = NoFilter, SortFlags sort = NoSort) const;
//QFileInfoList entryInfoList(const QStringList &nameFilters, Filters filters = NoFilter,
//                            SortFlags sort = NoSort) const;


void ItemList::filesRoutine(ModelItem * index, QFileInfoList list){
    foreach(QFileInfo file, list) {
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden));
        } else {
            model -> appendRow(file.filePath(), index);
        }
    }
}

void ItemList::filesRoutine(ModelItem * index, QList<QUrl> list){
    foreach(QUrl url, list) {
        QFileInfo file = QFileInfo(url.toLocalFile());
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden));
        } else {
            model -> appendRow(file.filePath(), index);
        }
    }
}

void ItemList::dropEvent(QDropEvent *event) {
    if (event -> source() != this && event -> mimeData() -> hasUrls()) {
        QList<QUrl> list = event -> mimeData() -> urls();
        ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path()); // -> parent();
        filesRoutine(index, list);
        model -> repaint();

        QModelIndex modelIndex = model -> index(index);
        expand(modelIndex);
        scrollTo(modelIndex);
        event -> accept();
    } else event -> ignore();
}

void ItemList::startDrag(Qt::DropActions /*supportedActions*/) {
//    QByteArray itemData;
//    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

//    ListWidgetItem *item = dynamic_cast<ListWidgetItem *>(childAt(dragPoint));

//    dataStream << dragPoint;

//    QMimeData *mimeData = new QMimeData;
//    mimeData->setData("application/x-QListView-DragAndDrop", itemData);

//    drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    drag->setHotSpot(mapToParent(QPoint(item->x(),y)));
//    drag->setPixmap(currentPixmap);

//    QModelIndex indx = indexAt(dragPoint);
//    if (drag->exec(Qt::MoveAction | Qt::CopyAction) == Qt::MoveAction)
//     {
//        model()->removeRow(indx.row());
//     }
}

void ItemList::on_doubleClick(const QModelIndex &index) {
    ModelItem * item = model -> getItem(index);

    if (!item -> getState() -> isUnprocessed()) {
        execItem(item);
    }
}

void ItemList::showContextMenu(const QPoint& pnt) {
    QList<QAction *> actions;

    if (indexAt(pnt).isValid()) {
        QAction * openAct = new QAction(QIcon(":/open"), "Open location", this);
        connect(openAct, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
        actions.append(openAct);
    }

    if (actions.count() > 0)
        QMenu::exec(actions, mapToGlobal(pnt));
}

void ItemList::openLocation() {
    ModelItem * item = model -> getItem(this -> currentIndex());
    if (item -> getState() -> isUnprocessed())
        QDesktopServices::openUrl(QUrl::fromLocalFile(item -> fullpath()));
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(item -> parent() -> fullpath()));
}

QJsonObject ItemList::toJSON() {
    QJsonObject res = model -> getItem(rootIndex()) -> toJSON();
    QJsonObject set = QJsonObject();


    foreach(QString c, settings.keys()) {
        set[c] = settings.value(c);
    }

    res["p"] = QString();
    res["s"] = set;
    res["l"] = model -> itemsCount();
    return res;
}

bool ItemList::isRemoveFileWithItem() {
    return settings["d"];
}

bool ItemList::isPlaylist() {
    return settings["p"];
}


CBHash ItemList::getSettings() const {
    return settings;
}
void ItemList::setSettings(CBHash newSettings) {
    settings = newSettings;
}

////////////////////////////////////////////////////////////
//// Dnd
////////////////////////////////////////////////////////////

void ItemList::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPoint = event -> pos();
    }

    QTreeView::mousePressEvent(event);
}

void ItemList::mouseMoveEvent(QMouseEvent * event) {
    if ((event -> buttons() == Qt::LeftButton) && (dragStartPoint - event -> pos()).manhattanLength() >= 5){
        QDrag * drag = new QDrag(this);
        QMimeData * mimeData = model -> mimeData(selectedIndexes());
        qDebug() << mimeData ->text().length();
//        drag -> setPixmap(toolIcon);
        drag -> setMimeData(mimeData);
        drag -> exec();//(Qt::CopyAction, Qt::CopyAction);
    }

    QTreeView::mouseMoveEvent(event);
}

void ItemList::markSelectedAsLiked() {
    ModelItem * temp;
    foreach (const QModelIndex &index, selectedIndexes()) {
        if (index.isValid()) {
            temp = model -> getItem(index);
            if (!temp -> getState() -> isUnprocessed()) {
                temp -> setState(STATE_LIKED);
                model -> refreshItem(temp);
            }
        }
    }
}

////////////////////////////////////////////////////////////

bool ItemList::execItem(ModelItem * item) {
    if (item) {
        scrollTo(model -> index(item));
        if (item -> isExist()) {
            Player::instance() -> playItem(this, item);
            return true;
        }
        else {
            item -> getState() -> setNotExist();
            model -> refreshItem(item);
        }
    } else return true;

    return false;
}

void ItemList::removeItem(ModelItem * item) {
    QModelIndex modelIndex = model -> index(item);
    QString delPath = item -> fullpath();

    if (Player::instance() -> playedItem()) {
        if (Player::instance() -> playedItem() -> fullpath().startsWith(delPath))
            Player::instance() -> removePlaylist();
    }

    if (model -> removeRow(modelIndex.row(), modelIndex.parent())) {
        if (isRemoveFileWithItem()) {
            if (item -> getState() -> isUnprocessed()) {
                QDir(delPath).removeRecursively();
            } else {
                QFile::remove(delPath);
            }
        }
    }
}

////////////////////////////////////////////////////////////


// test needed / update need
ModelItem * ItemList::nextItem(QModelIndex currIndex) {
    QModelIndex newIndex;
    ModelItem * item;

    while(true) {
        newIndex = currIndex.parent().child(currIndex.row()+1, 0); //indexBelow(currIndex);

        if (newIndex.isValid()) {
            item = model -> getItem(newIndex);
            qDebug() << item -> data(0);
            if (!item->getState() -> isUnprocessed()) {
                return item;
            } else {
                currIndex = newIndex.child(0, 0);
            }
        } else {
            currIndex = currIndex.parent();
            item = model -> getItem(newIndex);

            if (!currIndex.isValid())
                return model -> root();
        }
    }
}

ModelItem * ItemList::nextItem(ModelItem * curr) {
    ModelItem * item = curr;
    bool first_elem = curr -> parent() == 0 || curr -> getState() -> isUnprocessed();

    while(true) {
        if (first_elem) {
            first_elem = false;
        } else {
            item = item -> parent() -> child(item -> row() + 1);
        }

        if (item != 0) {
            if (!item -> getState() -> isUnprocessed()) {
                return item;
            } else {
                curr = item;
                item = curr -> child(0);
                first_elem = true;
            }
        } else {
            if (curr -> parent() == 0)
                return 0;

            item = curr;
            curr = curr -> parent();
        }
    }
}
ModelItem * ItemList::prevItem(ModelItem * curr) {
    ModelItem * item = curr;
    bool last_elem = false;

    while(true) {
        if (last_elem) {
            last_elem = false;
        } else {
            item = item -> parent() -> child(item -> row() - 1);
        }

        if (item != 0) {
            if (!item->getState() -> isUnprocessed()) {
                return item;
            } else {
                curr = item;
                item = curr -> child(item -> childCount() - 1);
                last_elem = true;
            }
        } else {
            if (curr -> parent() == 0)
                return 0;

            item = curr;
            curr = curr -> parent();
        }
    }
}
