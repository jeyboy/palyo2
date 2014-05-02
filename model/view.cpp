#include "view.h"
#include <QDebug>

View::View(QWidget *parent, CBHash settingsSet, QJsonObject * attrs) : QTreeView(parent) {
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
//    connect(model, SIGNAL(selectionChangeNeeded(const QModelIndex &index)), this, SLOT(changeSelection(const QModelIndex &index)));
//    connect(model, SIGNAL(selectionUpdateNeeded()), this, SLOT(updateSelection()));

    header() -> setSectionResizeMode(0, QHeaderView::Interactive);
//    header()->setStretchLastSection(false);
}

View::~View() {
    delete model;
}

Model * View::getModel() const {
    return model;
}

void View::keyPressEvent(QKeyEvent *event) {
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

ModelItem * View::activeItem(bool next) {
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

void View::proceedPrev() {
    ModelItem * item = activeItem(false);

    if (item == 0) return;

    item = prevItem(item);
    execItem(item);
}

void View::proceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);
    execItem(item);
}

void View::deleteCurrentProceedNext() {
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

void View::dragEnterEvent(QDragEnterEvent *event) {
    if (event -> source() != this && event -> mimeData() -> hasFormat("text/uri-list"))
        event -> accept();
    else event -> ignore();
}

void View::dragMoveEvent(QDragMoveEvent * event) {
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


void View::filesRoutine(ModelItem * index, QFileInfoList list){
    foreach(QFileInfo file, list) {
        if (file.isDir()) {
            ModelItem * new_index = model -> addFolder(file.fileName(), index);
            filesRoutine(new_index, QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden));
        } else {
            model -> appendRow(file.filePath(), index);
        }
    }
}

void View::filesRoutine(ModelItem * index, QList<QUrl> list){
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

void View::dropEvent(QDropEvent *event) {
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

void View::updateSelection(QModelIndex candidate) {
    if (candidate.isValid()) {
        ModelItem * item = getModel() -> getItem(candidate);

        if (item -> getState() -> isUnprocessed()) {
            if ((item = nextItem(item)))
              setCurrentIndex(getModel() -> index(item));
        }
    }
}

//void View::changeSelection(const QModelIndex & index) {
//    emit selectionChanged(index, currentIndex());
//}

void View::on_doubleClick(const QModelIndex &index) {
    ModelItem * item = model -> getItem(index);

    if (!item -> getState() -> isUnprocessed()) {
        execItem(item);
    }
}

void View::showContextMenu(const QPoint& pnt) {
    QList<QAction *> actions;

    if (indexAt(pnt).isValid()) {
        QAction * openAct = new QAction(QIcon(":/open"), "Open location", this);
        connect(openAct, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
        actions.append(openAct);
    }

    if (actions.count() > 0)
        QMenu::exec(actions, mapToGlobal(pnt));
}

void View::openLocation() {
    ModelItem * item = model -> getItem(this -> currentIndex());
    if (item -> getState() -> isUnprocessed())
        QDesktopServices::openUrl(QUrl::fromLocalFile(item -> fullpath()));
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(item -> parent() -> fullpath()));
}

QJsonObject View::toJSON() {
    QJsonObject res = model -> getItem(rootIndex()) -> toJSON();
    QJsonObject set = QJsonObject();


    foreach(QString c, settings.keys()) {
        set[c] = settings.value(c);
    }

    res["p"] = QString();
    res["set"] = set;
    res["l"] = model -> itemsCount();
    return res;
}

bool View::isRemoveFileWithItem() {
    return settings["d"];
}

bool View::isPlaylist() {
    return settings["p"];
}


CBHash View::getSettings() const {
    return settings;
}
void View::setSettings(CBHash newSettings) {
    settings = newSettings;
}

////////////////////////////////////////////////////////////
//// Dnd
////////////////////////////////////////////////////////////

void View::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPoint = event -> pos();
    }

//    if (getModel() -> root() -> childCount() > 0) //patch // in some cases after folder deletion errors thrown
    QTreeView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent * event) {
    if ((event -> buttons() == Qt::LeftButton) && (dragStartPoint - event -> pos()).manhattanLength() >= 5){
        if (selectedIndexes().length() > 0) {
            QDrag * drag = new QDrag(this);
            QMimeData * mimeData = model -> mimeData(selectedIndexes());
    //        drag -> setPixmap(toolIcon);
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }

    QTreeView::mouseMoveEvent(event);
}

void View::markSelectedAsLiked() {
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

bool View::execItem(ModelItem * item) {
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

void View::removeItem(ModelItem * item) {
    QModelIndex modelIndex = model -> index(item);
    QString delPath = item -> fullpath();
    bool isFolder = item -> getState() -> isUnprocessed();

    QModelIndex parentIndex = modelIndex.parent();
    if (!parentIndex.isValid())
        parentIndex = rootIndex();
    int row = modelIndex.row();
    ModelItem * parent = getModel() -> getItem(parentIndex);
    QModelIndex selCandidate = parentIndex.child(row + 1, 0);

    while(parent -> childCount() == 1 && parent -> parent() != 0) {
        parentIndex = getModel() -> index(parent -> parent());
        row = parent -> row();
        delPath = parent -> fullpath();
        isFolder = parent -> getState() -> isUnprocessed();
        selCandidate = parentIndex.child(row + 1, 0);

        parent = parent -> parent();
    }

    if (isFolder && item -> childTreeCount() > 1) {
        if (QMessageBox::warning(
                    parentWidget(),
                    "Folder deletion",
                    "Are you shure what you want remove the not empty folder ?",
                    QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    clearSelection();

    // fix for model rows remove emit
    QAbstractItemView::rowsAboutToBeRemoved(parentIndex, row, row);

    if (Player::instance() -> playedItem()) {
        if (Player::instance() -> playedItem() -> fullpath().startsWith(delPath))
            Player::instance() -> removePlaylist();
    }

    if (model -> removeRow(row, parentIndex)) {
        if (isRemoveFileWithItem()) {
            if (isFolder) {
                QDir delDir(delPath);
                if (delPath.split('/').length() >= 2) {
                    delDir.removeRecursively();
                }
            } else {
                QFile::remove(delPath);
            }
        }

        if (isFolder) {
            updateSelection(selCandidate);
        } else {
            QModelIndex next = parentIndex.child(row, 0);
            if (!next.isValid() || (next.isValid() && !next.data(FOLDERID).toBool())) {
                updateSelection(selCandidate);
            }
        }
    }
}

////////////////////////////////////////////////////////////


//// test needed / update need
//ModelItem * View::nextItem(QModelIndex currIndex) {
//    QModelIndex newIndex;
//    ModelItem * item;

//    while(true) {
//        newIndex = currIndex.parent().child(currIndex.row()+1, 0); //indexBelow(currIndex);

//        if (newIndex.isValid()) {
//            item = model -> getItem(newIndex);
//            qDebug() << item -> data(0);
//            if (!item->getState() -> isUnprocessed()) {
//                return item;
//            } else {
//                currIndex = newIndex.child(0, 0);
//            }
//        } else {
//            currIndex = currIndex.parent();
//            item = model -> getItem(newIndex);

//            if (!currIndex.isValid())
//                return model -> root();
//        }
//    }
//}

ModelItem * View::nextItem(ModelItem * curr) {
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
ModelItem * View::prevItem(ModelItem * curr) {
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
