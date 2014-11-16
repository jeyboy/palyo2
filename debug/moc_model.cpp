/****************************************************************************
** Meta object code from reading C++ file 'model.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../model/model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Model_t {
    QByteArrayData data[15];
    char stringdata[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Model_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Model_t qt_meta_stringdata_Model = {
    {
QT_MOC_LITERAL(0, 0, 5),
QT_MOC_LITERAL(1, 6, 17),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 8),
QT_MOC_LITERAL(4, 34, 12),
QT_MOC_LITERAL(5, 47, 5),
QT_MOC_LITERAL(6, 53, 11),
QT_MOC_LITERAL(7, 65, 11),
QT_MOC_LITERAL(8, 77, 11),
QT_MOC_LITERAL(9, 89, 11),
QT_MOC_LITERAL(10, 101, 7),
QT_MOC_LITERAL(11, 109, 15),
QT_MOC_LITERAL(12, 125, 7),
QT_MOC_LITERAL(13, 133, 8),
QT_MOC_LITERAL(14, 142, 9)
    },
    "Model\0itemsCountChanged\0\0newCount\0"
    "expandNeeded\0index\0spoilNeeded\0"
    "showMessage\0showSpinner\0hideSpinner\0"
    "updated\0libraryResponse\0refresh\0"
    "expanded\0collapsed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Model[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       7,    1,   78,    2, 0x06 /* Public */,
       8,    0,   81,    2, 0x06 /* Public */,
       9,    0,   82,    2, 0x06 /* Public */,
      10,    0,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      13,    1,   86,    2, 0x0a /* Public */,
      14,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, QMetaType::QModelIndex,    5,

       0        // eod
};

void Model::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Model *_t = static_cast<Model *>(_o);
        switch (_id) {
        case 0: _t->itemsCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->expandNeeded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->spoilNeeded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->showMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->showSpinner(); break;
        case 5: _t->hideSpinner(); break;
        case 6: _t->updated(); break;
        case 7: _t->libraryResponse(); break;
        case 8: _t->refresh(); break;
        case 9: _t->expanded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: _t->collapsed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Model::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::itemsCountChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (Model::*_t)(const QModelIndex & ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::expandNeeded)) {
                *result = 1;
            }
        }
        {
            typedef void (Model::*_t)(const QModelIndex & ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::spoilNeeded)) {
                *result = 2;
            }
        }
        {
            typedef void (Model::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::showMessage)) {
                *result = 3;
            }
        }
        {
            typedef void (Model::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::showSpinner)) {
                *result = 4;
            }
        }
        {
            typedef void (Model::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::hideSpinner)) {
                *result = 5;
            }
        }
        {
            typedef void (Model::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Model::updated)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject Model::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_Model.data,
      qt_meta_data_Model,  qt_static_metacall, 0, 0}
};


const QMetaObject *Model::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Model::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Model.stringdata))
        return static_cast<void*>(const_cast< Model*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int Model::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Model::itemsCountChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Model::expandNeeded(const QModelIndex & _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< Model *>(this), &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Model::spoilNeeded(const QModelIndex & _t1)const
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< Model *>(this), &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Model::showMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Model::showSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void Model::hideSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Model::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
