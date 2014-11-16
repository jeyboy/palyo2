/****************************************************************************
** Meta object code from reading C++ file 'vk_model.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../model/web/vk/vk_model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vk_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VkModel_t {
    QByteArrayData data[11];
    char stringdata[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VkModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VkModel_t qt_meta_stringdata_VkModel = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 7),
QT_MOC_LITERAL(2, 16, 0),
QT_MOC_LITERAL(3, 17, 11),
QT_MOC_LITERAL(4, 29, 15),
QT_MOC_LITERAL(5, 45, 12),
QT_MOC_LITERAL(6, 58, 16),
QT_MOC_LITERAL(7, 75, 11),
QT_MOC_LITERAL(8, 87, 10),
QT_MOC_LITERAL(9, 98, 26),
QT_MOC_LITERAL(10, 125, 22)
    },
    "VkModel\0refresh\0\0refreshWall\0"
    "proceedWallList\0QJsonObject&\0"
    "proceedAudioList\0QJsonArray&\0ModelItem*\0"
    "QHash<ModelItem*,QString>&\0"
    "proceedAudioListUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VkModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    1,   46,    2, 0x09 /* Protected */,
       6,    1,   49,    2, 0x09 /* Protected */,
       6,    3,   52,    2, 0x09 /* Protected */,
      10,    2,   59,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 8, 0x80000000 | 9,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 9,    2,    2,

       0        // eod
};

void VkModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VkModel *_t = static_cast<VkModel *>(_o);
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: _t->refreshWall(); break;
        case 2: _t->proceedWallList((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 3: _t->proceedAudioList((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 4: _t->proceedAudioList((*reinterpret_cast< QJsonArray(*)>(_a[1])),(*reinterpret_cast< ModelItem*(*)>(_a[2])),(*reinterpret_cast< QHash<ModelItem*,QString>(*)>(_a[3]))); break;
        case 5: _t->proceedAudioListUpdate((*reinterpret_cast< QJsonObject(*)>(_a[1])),(*reinterpret_cast< QHash<ModelItem*,QString>(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject VkModel::staticMetaObject = {
    { &WebModel::staticMetaObject, qt_meta_stringdata_VkModel.data,
      qt_meta_data_VkModel,  qt_static_metacall, 0, 0}
};


const QMetaObject *VkModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VkModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VkModel.stringdata))
        return static_cast<void*>(const_cast< VkModel*>(this));
    return WebModel::qt_metacast(_clname);
}

int VkModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WebModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
