#include "tab.h"
#include <QDebug>

void Tab::init(QJsonObject * attrs) {
    list = new ItemList(this, attrs);
//    list -> setResizeMode();

    this -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this -> layout() -> addWidget(list);
    this -> layout() -> setContentsMargins(0, 0, 0, 0);
}

Tab::Tab(CBHash params, QWidget * parent) : QWidget(parent) {
    settings = params;
    init();
}

Tab::Tab(QJsonObject json_attrs, QWidget * parent) : QWidget(parent) {
    QJsonObject set = json_attrs["s"].toObject();
    CBHash params = CBHash();

    foreach(QString key, set.keys()) {
        params.insert(key, set[key].toBool());
    }

    settings = params;
    init(&json_attrs);
}

Tab::~Tab() {
    delete list;
}

bool Tab::isRemoveFileWithItem() {
    return settings["d"];
}

bool Tab::isPlaylist() {
    return settings["p"];
}

void Tab::updateHeader(QTabWidget * parent) {
    list -> updateTabCounter(this, parent);
}

QJsonObject Tab::toJSON(QString name) {
    QJsonObject res = list-> toJSON();
    QJsonObject set = QJsonObject();


    foreach(QString c, settings.keys()) {
        set[c] = settings.value(c);
    }

    res["s"] = set;
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
