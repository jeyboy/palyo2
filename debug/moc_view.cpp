/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../model/view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_View_t {
    QByteArrayData data[18];
    char stringdata[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_View_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_View_t qt_meta_stringdata_View = {
    {
QT_MOC_LITERAL(0, 0, 4),
QT_MOC_LITERAL(1, 5, 11),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 11),
QT_MOC_LITERAL(4, 30, 7),
QT_MOC_LITERAL(5, 38, 15),
QT_MOC_LITERAL(6, 54, 9),
QT_MOC_LITERAL(7, 64, 12),
QT_MOC_LITERAL(8, 77, 11),
QT_MOC_LITERAL(9, 89, 13),
QT_MOC_LITERAL(10, 103, 11),
QT_MOC_LITERAL(11, 115, 13),
QT_MOC_LITERAL(12, 129, 5),
QT_MOC_LITERAL(13, 135, 15),
QT_MOC_LITERAL(14, 151, 12),
QT_MOC_LITERAL(15, 164, 8),
QT_MOC_LITERAL(16, 173, 11),
QT_MOC_LITERAL(17, 185, 11)
    },
    "View\0showSpinner\0\0hideSpinner\0shuffle\0"
    "updateSelection\0candidate\0startRoutine\0"
    "stopRoutine\0setHeaderText\0showMessage\0"
    "onDoubleClick\0index\0showContextMenu\0"
    "openLocation\0download\0downloadAll\0"
    "modelUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_View[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   86,    2, 0x0a /* Public */,
       5,    1,   87,    2, 0x0a /* Public */,
       7,    0,   90,    2, 0x0a /* Public */,
       8,    0,   91,    2, 0x0a /* Public */,
       9,    1,   92,    2, 0x0a /* Public */,
      10,    1,   95,    2, 0x0a /* Public */,
      11,    1,   98,    2, 0x09 /* Protected */,
      13,    1,  101,    2, 0x09 /* Protected */,
      14,    0,  104,    2, 0x09 /* Protected */,
      15,    0,  105,    2, 0x09 /* Protected */,
      16,    0,  106,    2, 0x09 /* Protected */,
      17,    0,  107,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QModelIndex,   12,
    QMetaType::Void, QMetaType::QPoint,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void View::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        View *_t = static_cast<View *>(_o);
        switch (_id) {
        case 0: _t->showSpinner(); break;
        case 1: _t->hideSpinner(); break;
        case 2: _t->shuffle(); break;
        case 3: _t->updateSelection((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 4: _t->startRoutine(); break;
        case 5: _t->stopRoutine(); break;
        case 6: _t->setHeaderText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->showMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->onDoubleClick((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 10: _t->openLocation(); break;
        case 11: _t->download(); break;
        case 12: _t->downloadAll(); break;
        case 13: _t->modelUpdate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (View::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View::showSpinner)) {
                *result = 0;
            }
        }
        {
            typedef void (View::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&View::hideSpinner)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject View::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_View.data,
      qt_meta_data_View,  qt_static_metacall, 0, 0}
};


const QMetaObject *View::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *View::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View.stringdata))
        return static_cast<void*>(const_cast< View*>(this));
    return QTreeView::qt_metacast(_clname);
}

int View::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void View::showSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void View::hideSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
