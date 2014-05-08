#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include "model/model.h"

class ListModel : public Model {
    Q_OBJECT

public:
    ListModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~ListModel();
};

#endif // LIST_MODEL_H
