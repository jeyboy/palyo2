#include "level_tree_view.h"
#include <QDebug>

LevelTreeView::LevelTreeView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeModel(hash)), parent, settingsSet) {
}

LevelTreeView::~LevelTreeView() {

}
