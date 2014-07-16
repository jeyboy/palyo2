#include "widget.h"
#include <QDebug>

void Widget::init(CBHash params, QJsonObject * hash) {
    switch(params["t"]) {
        case VIEW_LIST: {
            view = (View *)new ListView(this, params, hash);
        break;}
        case VIEW_LEVEL_TREE: {
            view = (View *)new LevelTreeView(this, params, hash);
        break;}
        case VIEW_LEVEL_TREE_BREADCRUMB: {
            view = (View *)new LevelTreeBreadcrumbView(this, params, hash);
        break;}
        case VIEW_VK: {
            if (hash != 0 && hash -> contains("uid"))
                params.insert("uid", hash -> value("uid").toString().toInt());
            view = (View *)new VkView(this, params, hash);
        break;}
        case VIEW_SOUNDCLOUD: {
            if (hash != 0 && hash -> contains("uid"))
                params.insert("uid", hash -> value("uid").toString().toInt());
            view = (View *)new SoundcloudView(this, params, hash);
        break;}
        default: {
            view = (View *)new TreeView(this, params, hash);
        break;}
    }
//    view -> setResizeMode();

    this -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom, this));
//    this -> layout() -> addWidget(view);
    setWidget(view);


    spinnerContainer = new QLabel;
    spinnerContainer -> setHidden(true);
    spinnerContainer -> setPixmap(QPixmap(":/sync"));
    this -> layout() -> addWidget(spinnerContainer);
    this -> layout() -> setAlignment(spinnerContainer, Qt::AlignCenter);


    this -> layout() -> setContentsMargins(0, 0, 0, 0);

    connect(view, SIGNAL(showSpinner()), this, SLOT(startRoutine()));
    connect(view, SIGNAL(hideSpinner()), this, SLOT(stopRoutine()));

    if (hash == 0 && (params["t"] == VIEW_VK))
        view -> getModel() -> refresh();
}

Widget::Widget(CBHash params, const QString &title, QWidget *parent,
               Qt::WindowFlags flags) : QDockWidget(title, parent, flags) {
    init(params);
}
Widget::Widget(QJsonObject hash, const QString &title, QWidget *parent,
               Qt::WindowFlags flags) : QDockWidget(title, parent, flags) {
    QJsonObject set = hash["set"].toObject();
    CBHash params = CBHash();

    foreach(QString key, set.keys()) {
        params.insert(key, set[key].toInt());
    }

    init(params, &hash);
}

Widget::~Widget() {
//    delete view;
}

QString Widget::getName() const {
    return windowIconText().split('(').first(); //.section('(', 0, -1);
}
void Widget::setName(QString newName) {
    setNameWithCount(newName);
}

View * Widget::getView() const {
    return view;
}

void Widget::updateHeader(int /*new_count*/) {
    setNameWithCount(getName());
}

void Widget::setNameWithCount(QString name) {
    setWindowIconText(name + "(" + QString::number(view -> getModel() -> itemsCount()) + ")");
}

QJsonObject Widget::toJSON(QString name) {
    QJsonObject res = view -> toJSON();
    res["n"] = name;
    if (Player::instance() -> currentPlaylist() == view) {
        res["pv"] = true;
        res["pp"] = Player::instance() -> playedItem() -> toPath();

        if (!Player::instance() -> playedItem() -> isRemote())
            res["pt"] = Player::instance() -> getPosition();
    }
    return res;
}

bool Widget::isEditable() const {
    return view -> isEditable();
}

void Widget::startRoutine() {
    view -> setHidden(true);
    spinnerContainer -> setHidden(false);
}
void Widget::stopRoutine() {
    spinnerContainer -> setHidden(true);
    view -> setHidden(false);
}