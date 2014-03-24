#include "itemlist.h"
#include <QDebug>

ItemList::ItemList(QWidget *parent, CBHash settingsSet, QJsonObject * attrs) : QTreeView(parent) {
    tab = (Tab *)parent;
    settings = settingsSet;
    setIndentation(10);

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

    setExpandsOnDoubleClick(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//  setDragDropOverwriteMode(true);
//  setMovement(QTreeView::Snap);

//  setFlow(QListView::TopToBottom);

    model = new TreeModel(attrs);
    setModel(model);
//    setTreePosition(2);
//    setRootIndex();
//    setRowHidden(0, rootIndex(), true);

    expandAll();

    setItemDelegate(new ModelItemDelegate(this));

    connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(on_click(const QModelIndex&)));   
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_doubleClick(const QModelIndex&)));
}

ItemList::~ItemList() {
    delete model;
}

TreeModel * ItemList::getModel() const {
    return model;
}

void ItemList::keyPressEvent(QKeyEvent *event) {
    if (event ->key() == Qt::Key_Enter || event ->key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        qDebug() << list;

        if (list.count() > 0) {
            ModelItem * item = model -> getItem(list.first());
            item -> play(this);
        }
    } else if (event ->key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        QModelIndex modelIndex;
        bool delFile = isRemoveFileWithItem();

        QString delPath;
        for(int i = list.count() - 1; i >= 0; i--) {
            modelIndex = list.at(i);

            if (delFile) {
                delPath = model -> getItem(modelIndex) -> fullpath();
                QFile::remove(delPath);
            }

            model -> removeRow(modelIndex.row(), modelIndex.parent());
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
    if (item)
        item -> play(this);
}

void ItemList::proceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);
    if (item)
        item -> play(this);
}

void ItemList::deleteCurrentProceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);

    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem()) {
            bool delFile = isRemoveFileWithItem();

            QModelIndex modelIndex = model -> index(Player::instance() -> playedItem());
            if (delFile) {
                QString delPath = Player::instance() -> playedItem() -> fullpath();
                QFile::remove(delPath);
            }
            model -> removeRow(modelIndex.row(), modelIndex.parent());
        }
    }

    if (item)
        item -> play(this);
}

void ItemList::dragEnterEvent(QDragEnterEvent *event) {
   if (event->mimeData()->hasFormat(DnD::instance() -> listItems)
       || event->mimeData()->hasFormat(DnD::instance() -> files)){
       event -> accept();
   } else {
        event -> ignore();
   }
}

void ItemList::dragMoveEvent(QDragMoveEvent * event) {
    event ->accept();

//  if (e->source() != this) {
//      e->accept();
//  } else {
//      e->ignore();
//  }
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
      if (event->mimeData()->hasFormat(DnD::instance() -> listItems)) {

          event->accept();
      } else if (event->mimeData()->hasFormat(DnD::instance() -> files)) {
          if(event -> mimeData() -> hasUrls()) {          
              QList<QUrl> list = event -> mimeData() -> urls();
              ModelItem * index = model -> buildPath(QFileInfo(list.first().toLocalFile()).path()); // -> parent();
//              model -> startRowInsertion(model->index(index));
              filesRoutine(index, list);
//              model -> endRowInsertion();
              model -> repaint();

              QModelIndex modelIndex = model -> index(index);
              expand(modelIndex);
              scrollTo(modelIndex);
          }

          event->accept();
      } else {  event->ignore(); }



//    if (event->mimeData()->hasFormat("application/x-QListView-DragAndDrop")) {
//        QByteArray itemData = event->mimeData()->data("application/x-QListView-DragAndDrop");
//        QDataStream dataStream(&itemData,QIODevice::ReadOnly);

//        QPoint itemPoint;
//        dataStream >> itemPoint;

//        ListWidgetItem *item = dynamic_cast<ListWidgetItem *> (childAt(itemPoint));
//        if(!item)
//            return;

//        QModelIndex index = indexAt(event->pos());
//        int row = index.row();
//        model()->insertRows(row,1);

//        QModelIndex idx =model()->index(row,0);
//        setIndexWidget(idx,item);

//        if (event->source() == this) {
//            event->setDropAction(Qt::MoveAction);
//            foreach (ListWidgetItem *widget,itemList)
//            {
//                widget->setStyleSheet("#ListWidget { border-top: 0px solid black; }");
//            }
//            itemList.clear();
////            model()->removeRow(n+1);

//            event->accept();
//        }
//    } else{
//        event->ignore();
//    }
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


void ItemList::on_click(const QModelIndex &index) {
    ModelItem * item = model->getItem(index);

    if (item -> getState() -> isUnprocessed()) {
        if (isExpanded(index)) {
          collapse(index);
        } else { expand(index); }
    }
}

void ItemList::on_doubleClick(const QModelIndex &index) {
    ModelItem * item = model->getItem(index);

    if (!item -> getState() -> isUnprocessed()) {
        item -> play(this);
    }
}

QJsonObject ItemList::toJSON() {
    QJsonObject res = model -> getItem(rootIndex()) -> toJSON();
    QJsonObject set = QJsonObject();


    foreach(QString c, settings.keys()) {
        set[c] = settings.value(c);
    }

    res["s"] = set;
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
        drag -> exec(Qt::CopyAction, Qt::CopyAction);
    }

    QTreeView::mouseMoveEvent(event);
}

void ItemList::markSelectedAsLiked() {
    ModelItem * temp;

    foreach (const QModelIndex &index, selectedIndexes()) {
        if (index.isValid()) {
            temp = model -> getItem(index);
            if (!temp -> getState() -> isUnprocessed()) {
                temp -> getState() -> setLiked(temp);
                model -> refreshItem(temp);
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
    bool first_elem = curr -> parent() == 0;

    while(true) {
        if (first_elem) {
            first_elem = false;
        } else {
            item = curr -> parent() -> child(curr -> row() + 1);
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
            curr = curr -> parent();

            if (curr -> parent() == 0)
                return 0;
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
            item = curr -> parent() -> child(curr -> row() - 1);
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
            curr = curr -> parent();

            if (curr -> parent() == 0)
                return 0;
        }
    }
}
