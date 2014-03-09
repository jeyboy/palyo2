#include "tab.h"
#include <QDebug>

void Tab::init(CBHash params, QJsonObject * attrs) {
    list = new ItemList(this, params, attrs);
//    list -> setResizeMode();

    this -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this -> layout() -> addWidget(list);
    this -> layout() -> setContentsMargins(0, 0, 0, 0);
}

Tab::Tab(CBHash params, QWidget * parent) : QWidget(parent) {
    init(params);
}

Tab::Tab(QJsonObject json_attrs, QWidget * parent) : QWidget(parent) {
    QJsonObject set = json_attrs["s"].toObject();
    CBHash params = CBHash();

    foreach(QString key, set.keys()) {
        params.insert(key, set[key].toBool());
    }

    init(params, &json_attrs);
}

Tab::~Tab() {
    delete list;
}

void Tab::updateHeader(QTabWidget * parent) {
    list -> updateTabCounter(this, parent);
}

QJsonObject Tab::toJSON(QString name) {
    QJsonObject res = list-> toJSON();
    res["n"] = name;
    return res;
}

void Tab::proceedPrev() {
    list -> proceedPrev();
}

void Tab::proceedNext() {
    list -> proceedNext();
}

void Tab::deleteCurrentProceedNext() {
    list -> deleteCurrentProceedNext();
}
