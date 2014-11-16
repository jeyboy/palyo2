/****************************************************************************
** Meta object code from reading C++ file 'soundcloud_model.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../model/web/soundcloud/soundcloud_model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soundcloud_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SoundcloudModel_t {
    QByteArrayData data[8];
    char stringdata[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SoundcloudModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SoundcloudModel_t qt_meta_stringdata_SoundcloudModel = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 7),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 15),
QT_MOC_LITERAL(4, 41, 12),
QT_MOC_LITERAL(5, 54, 11),
QT_MOC_LITERAL(6, 66, 10),
QT_MOC_LITERAL(7, 77, 26)
    },
    "SoundcloudModel\0refresh\0\0proceedResponse\0"
    "QJsonObject&\0QJsonArray&\0ModelItem*\0"
    "QHash<ModelItem*,QString>&"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SoundcloudModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x09 /* Protected */,
       3,    3,   33,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 6, 0x80000000 | 7,    2,    2,    2,

       0        // eod
};

void SoundcloudModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SoundcloudModel *_t = static_cast<SoundcloudModel *>(_o);
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: _t->proceedResponse((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 2: _t->proceedResponse((*reinterpret_cast< QJsonArray(*)>(_a[1])),(*reinterpret_cast< ModelItem*(*)>(_a[2])),(*reinterpret_cast< QHash<ModelItem*,QString>(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject SoundcloudModel::staticMetaObject = {
    { &WebModel::staticMetaObject, qt_meta_stringdata_SoundcloudModel.data,
      qt_meta_data_SoundcloudModel,  qt_static_metacall, 0, 0}
};


const QMetaObject *SoundcloudModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SoundcloudModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SoundcloudModel.stringdata))
        return static_cast<void*>(const_cast< SoundcloudModel*>(this));
    return WebModel::qt_metacast(_clname);
}

int SoundcloudModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WebModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
