#ifndef MODEL_PROXY_H
#define MODEL_PROXY_H

#include <QSortFilterProxyModel>

class ModelProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ModelProxy(QObject *parent = 0);
};

#endif // MODEL_PROXY_H
