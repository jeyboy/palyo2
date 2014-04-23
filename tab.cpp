#include "tab.h"
#include <QDebug>

void Tab::init(CBHash params, QJsonObject * attrs) {
    list = new ItemList(this, params, attrs);
//    list -> setResizeMode();

    this -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this -> layout() -> addWidget(list);
    this -> layout() -> setContentsMargins(0, 0, 0, 0);
    tabber = (QTabWidget *)parent();
}

Tab::Tab(CBHash params, QWidget * parent) : QWidget(parent) {
    init(params);
}

Tab::Tab(QJsonObject json_attrs, QWidget * parent) : QWidget(parent) {
    QJsonObject set = json_attrs["set"].toObject();
    CBHash params = CBHash();

    foreach(QString key, set.keys()) {
        params.insert(key, set[key].toBool());
    }

    init(params, &json_attrs);
}

Tab::~Tab() {
    delete list;
//    delete tabber;
}

QString Tab::getName() const {
    int i = tabber -> indexOf((QWidget*)this);
    return tabber -> tabText(i).split('(').first(); //.section('(', 0, -1);
}
void Tab::setName(QString newName) {
    setNameWithCount(newName);
}

ItemList * Tab::getList() const {
    return list;
}

void Tab::updateHeader(int new_count) {
    setNameWithCount(getName());
}

void Tab::setNameWithCount(QString name) {
    int i = tabber -> indexOf((QWidget*)this);
    tabber -> setTabText(i, name + "(" + QString::number(list -> getModel() -> itemsCount()) + ")");
}

QJsonObject Tab::toJSON(QString name) {
    QJsonObject res = list -> toJSON();
    res["n"] = name;
    return res;
}
