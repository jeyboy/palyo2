#include "level_tree_breadcrumb_view.h"
#include <QDebug>

LevelTreeBreadcrumbView::LevelTreeBreadcrumbView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new LevelTreeBreadcrumbModel(hash)), parent, settingsSet) {
}

LevelTreeBreadcrumbView::~LevelTreeBreadcrumbView() {

}
