//#ifndef TABBER_H
//#define TABBER_H

//#include <QWidget>
//#include <QTabWidget>

//#include "misc/data_store.h"
//#include "dialogs/tabdialog.h"

////#include <QHash>
////typedef QHash <char, int> CBHash;
////Q_DECLARE_METATYPE(CBHash);

//#include "override/tab.h"

//class Tabber : public QTabWidget {
//    Q_OBJECT
//public:
//    explicit Tabber(QWidget *parent = 0);
//    ~Tabber();

//    int addTab(QString name, CBHash settings);
//    Tab * toActiveTab();
//    Tab * currentTab();
//    Tab * commonTab();
//    void load();
//    void save();

//    void updateIconSize();

//private:
//    Tab * commonPlaylist;
//    DataStore * store;

//private slots:
//    void updateActiveTab(QWidget *, QWidget *);
//    void handleCurrentChanged(int index);
//    void handleTabCloseRequested(int index);
//};

//#endif // TABBER_H
