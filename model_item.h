#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <QDir>
#include <QVariant>
#include <QAbstractItemModel>
#include <QJsonObject>
#include <QJsonArray>

#include "model.h"
#include "item_state.h"

#define NAMEUID 0
#define EXTENSIONUID 1
#define PATHUID 2
#define STATEID 3

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

    ItemState * getState() const;
    void setState(int new_state);

    QJsonObject toJSON();

    QHash<QString, ModelItem *> * folders;
    QList<QString> * names;
private:
    void init(bool isFolder);
    void rootItemInit();

    QList<ModelItem*> childItems;
    ModelItem *parentItem;  

    QString path;
    QString name;
    QString extension;
    ItemState * state;
};


#endif // MODEL_ITEM_H
