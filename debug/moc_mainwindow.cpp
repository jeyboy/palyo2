/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata[313];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 10),
QT_MOC_LITERAL(4, 40, 14),
QT_MOC_LITERAL(5, 55, 20),
QT_MOC_LITERAL(6, 76, 9),
QT_MOC_LITERAL(7, 86, 14),
QT_MOC_LITERAL(8, 101, 18),
QT_MOC_LITERAL(9, 120, 18),
QT_MOC_LITERAL(10, 139, 26),
QT_MOC_LITERAL(11, 166, 15),
QT_MOC_LITERAL(12, 182, 23),
QT_MOC_LITERAL(13, 206, 16),
QT_MOC_LITERAL(14, 223, 4),
QT_MOC_LITERAL(15, 228, 3),
QT_MOC_LITERAL(16, 232, 19),
QT_MOC_LITERAL(17, 252, 17),
QT_MOC_LITERAL(18, 270, 24),
QT_MOC_LITERAL(19, 295, 17)
    },
    "MainWindow\0outputActiveItem\0\0ModelItem*\0"
    "receiveMessage\0showAttCurrTabDialog\0"
    "showError\0showActiveElem\0showSettingsDialog\0"
    "showVKRelTabDialog\0showSoundcloudRelTabDialog\0"
    "showVKTabDialog\0showSoundcloudTabDialog\0"
    "showAttTabDialog\0Tab*\0tab\0openFolderTriggered\0"
    "nextItemTriggered\0nextItemWithDelTriggered\0"
    "prevItemTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x0a /* Public */,
       4,    1,   99,    2, 0x0a /* Public */,
       5,    0,  102,    2, 0x0a /* Public */,
       6,    1,  103,    2, 0x08 /* Private */,
       7,    0,  106,    2, 0x08 /* Private */,
       8,    0,  107,    2, 0x08 /* Private */,
       9,    0,  108,    2, 0x08 /* Private */,
      10,    0,  109,    2, 0x08 /* Private */,
      11,    0,  110,    2, 0x08 /* Private */,
      12,    0,  111,    2, 0x08 /* Private */,
      13,    1,  112,    2, 0x08 /* Private */,
      13,    0,  115,    2, 0x28 /* Private | MethodCloned */,
      16,    0,  116,    2, 0x08 /* Private */,
      17,    0,  117,    2, 0x08 /* Private */,
      18,    0,  118,    2, 0x08 /* Private */,
      19,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->outputActiveItem((*reinterpret_cast< ModelItem*(*)>(_a[1])),(*reinterpret_cast< ModelItem*(*)>(_a[2]))); break;
        case 1: _t->receiveMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->showAttCurrTabDialog(); break;
        case 3: _t->showError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->showActiveElem(); break;
        case 5: _t->showSettingsDialog(); break;
        case 6: _t->showVKRelTabDialog(); break;
        case 7: _t->showSoundcloudRelTabDialog(); break;
        case 8: _t->showVKTabDialog(); break;
        case 9: _t->showSoundcloudTabDialog(); break;
        case 10: _t->showAttTabDialog((*reinterpret_cast< Tab*(*)>(_a[1]))); break;
        case 11: _t->showAttTabDialog(); break;
        case 12: _t->openFolderTriggered(); break;
        case 13: _t->nextItemTriggered(); break;
        case 14: _t->nextItemWithDelTriggered(); break;
        case 15: _t->prevItemTriggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Tab* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
