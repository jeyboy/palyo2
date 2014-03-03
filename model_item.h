#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <QDir>
#include <QVariant>
#include <QAbstractItemModel>
#include <QJsonObject>
#include <QJsonArray>

#include "model.h"
#include "itemlist.h"
#include "player.h"

#define NAMEUID 0
#define EXTENSIONUID 1
#define PATHUID 2
#define STATEID 3

#define STATE_UNPROCESSED -1
#define STATE_DEFAULT 0
#define STATE_LISTENED 1
#define STATE_LIKED 2
#define STATE_PLAYED 3

class TreeModel;

class ModelItem {
public:
    ModelItem();
    ModelItem(TreeModel * model, QJsonObject * attrs, ModelItem *parent = 0);
    ModelItem(TreeModel * model, const QString filepath, ModelItem *parent = 0, int init_state = STATE_DEFAULT);
    ~ModelItem();

    ModelItem *parent();

    ModelItem *child(int row);
    int childCount() const;
    void appendChild(ModelItem *child);
    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    int column() const;
    int columnCount() const;
    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QString fullpath() const;

    void setState(int new_state);
    int getState();

    bool play(ItemList * palylist);

    QJsonObject toJSON();

    QHash<QString, ModelItem *> * folders;
private:
    QList<ModelItem*> childItems;
    ModelItem *parentItem;

    QString path;
    QString name;
    QString extension;
    int state;
};


#endif // MODEL_ITEM_H
