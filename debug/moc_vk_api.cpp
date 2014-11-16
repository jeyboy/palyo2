/****************************************************************************
** Meta object code from reading C++ file 'vk_api.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../web/socials/vk_api.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vk_api.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VkApi_t {
    QByteArrayData data[8];
    char stringdata[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VkApi_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VkApi_t qt_meta_stringdata_VkApi = {
    {
QT_MOC_LITERAL(0, 0, 5),
QT_MOC_LITERAL(1, 6, 17),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 12),
QT_MOC_LITERAL(4, 38, 15),
QT_MOC_LITERAL(5, 54, 26),
QT_MOC_LITERAL(6, 81, 13),
QT_MOC_LITERAL(7, 95, 11)
    },
    "VkApi\0audioListReceived\0\0QJsonObject&\0"
    "audioListUpdate\0QHash<ModelItem*,QString>&\0"
    "errorReceived\0showCaptcha"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VkApi[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    2,   37,    2, 0x06 /* Public */,
       6,    2,   42,    2, 0x06 /* Public */,
       7,    0,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    2,    2,
    QMetaType::Void,

       0        // eod
};

void VkApi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VkApi *_t = static_cast<VkApi *>(_o);
        switch (_id) {
        case 0: _t->audioListReceived((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 1: _t->audioListUpdate((*reinterpret_cast< QJsonObject(*)>(_a[1])),(*reinterpret_cast< QHash<ModelItem*,QString>(*)>(_a[2]))); break;
        case 2: _t->errorReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->showCaptcha(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VkApi::*_t)(QJsonObject & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VkApi::audioListReceived)) {
                *result = 0;
            }
        }
        {
            typedef void (VkApi::*_t)(QJsonObject & , QHash<ModelItem*,QString> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VkApi::audioListUpdate)) {
                *result = 1;
            }
        }
        {
            typedef void (VkApi::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VkApi::errorReceived)) {
                *result = 2;
            }
        }
        {
            typedef void (VkApi::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VkApi::showCaptcha)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject VkApi::staticMetaObject = {
    { &WebApi::staticMetaObject, qt_meta_stringdata_VkApi.data,
      qt_meta_data_VkApi,  qt_static_metacall, 0, 0}
};


const QMetaObject *VkApi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VkApi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VkApi.stringdata))
        return static_cast<void*>(const_cast< VkApi*>(this));
    if (!strcmp(_clname, "TeuAuth"))
        return static_cast< TeuAuth*>(const_cast< VkApi*>(this));
    return WebApi::qt_metacast(_clname);
}

int VkApi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WebApi::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void VkApi::audioListReceived(QJsonObject & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VkApi::audioListUpdate(QJsonObject & _t1, QHash<ModelItem*,QString> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VkApi::errorReceived(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VkApi::showCaptcha()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
