#ifndef MODEL_ITEM_H
#define MODEL_ITEM_H

#include <QFile>
#include <QDir>
#include <QVariant>
#include <QAbstractItemModel>
#include <QJsonObject>
#include <QJsonArray>

#include "model_item_state.h"
#include "library.h"

#define NAMEUID 0
#define EXTENSIONUID 1
#define PATHUID 2
#define STATEID 3

class ModelItem {
public:
    ModelItem();
    ModelItem(QJsonObject * attrs, ModelItem *parent = 0);
    ModelItem(const QString filepath, ModelItem *parent = 0, int init_state = STATE_DEFAULT);
    ~ModelItem();

    ModelItem *parent();

    ModelItem *child(int row);
    int childTreeCount() const;
    int childCount() const;
    void insertChild(int pos, ModelItem *item);
    void appendChild(ModelItem *child);
    bool removeChildren(int position, int count);

    int column() const;
    int columnCount() const;

    int row() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QString fullpath() const;

    ModelItemState * getState() const;
    void setState(int new_state, bool append_to_library = true);

    void dropExpandProceedFlags();

    QJsonObject toJSON();

    bool isExist();

    QHash<QString, ModelItem *> * foldersList() const;
    int removeFolder(QString name);

    QList<QString> * names;
private:
    void init(bool isFolder);
    void rootItemInit();

    QHash<QString, ModelItem *> * folders;

    QList<ModelItem*> childItems;
    ModelItem *parentItem;  

    QString path;
    QString name;
    QString extension;
    ModelItemState * state;
};


#endif // MODEL_ITEM_H
