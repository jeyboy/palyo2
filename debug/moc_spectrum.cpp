/****************************************************************************
** Meta object code from reading C++ file 'spectrum.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../override/spectrum.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spectrum.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Spectrum_t {
    QByteArrayData data[8];
    char stringdata[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Spectrum_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Spectrum_t qt_meta_stringdata_Spectrum = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 16),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 8),
QT_MOC_LITERAL(4, 36, 13),
QT_MOC_LITERAL(5, 50, 9),
QT_MOC_LITERAL(6, 60, 11),
QT_MOC_LITERAL(7, 72, 20)
    },
    "Spectrum\0bandCountChanged\0\0newCount\0"
    "heightChanged\0newHeight\0dataUpdated\0"
    "QList<QVector<int> >"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Spectrum[] = {

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
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       6,    1,   35,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void Spectrum::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Spectrum *_t = static_cast<Spectrum *>(_o);
        switch (_id) {
        case 0: _t->bandCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->heightChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->dataUpdated((*reinterpret_cast< QList<QVector<int> >(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QVector<int> > >(); break;
            }
            break;
        }
    }
}

const QMetaObject Spectrum::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_Spectrum.data,
      qt_meta_data_Spectrum,  qt_static_metacall, 0, 0}
};


const QMetaObject *Spectrum::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Spectrum::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Spectrum.stringdata))
        return static_cast<void*>(const_cast< Spectrum*>(this));
    return QToolBar::qt_metacast(_clname);
}

int Spectrum::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
