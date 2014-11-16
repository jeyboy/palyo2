/****************************************************************************
** Meta object code from reading C++ file 'toolbars.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../toolbars.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolbars.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolBars_t {
    QByteArrayData data[19];
    char stringdata[294];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolBars_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolBars_t qt_meta_stringdata_ToolBars = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 13),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 4),
QT_MOC_LITERAL(4, 29, 4),
QT_MOC_LITERAL(5, 34, 20),
QT_MOC_LITERAL(6, 55, 21),
QT_MOC_LITERAL(7, 77, 24),
QT_MOC_LITERAL(8, 102, 7),
QT_MOC_LITERAL(9, 110, 25),
QT_MOC_LITERAL(10, 136, 15),
QT_MOC_LITERAL(11, 152, 20),
QT_MOC_LITERAL(12, 173, 14),
QT_MOC_LITERAL(13, 188, 8),
QT_MOC_LITERAL(14, 197, 6),
QT_MOC_LITERAL(15, 204, 17),
QT_MOC_LITERAL(16, 222, 20),
QT_MOC_LITERAL(17, 243, 23),
QT_MOC_LITERAL(18, 267, 26)
    },
    "ToolBars\0folderDropped\0\0name\0path\0"
    "changeToolbarMovable\0changeToolbarsMovable\0"
    "toolbarVisibilityChanged\0visible\0"
    "toolbarOrientationChanged\0Qt::Orientation\0"
    "removePanelHighlight\0panelHighlight\0"
    "QAction*\0action\0addPanelTriggered\0"
    "removePanelTriggered\0addPanelButtonTriggered\0"
    "removePanelButtonTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolBars[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x0a /* Public */,
       5,    0,   79,    2, 0x0a /* Public */,
       6,    0,   80,    2, 0x0a /* Public */,
       7,    1,   81,    2, 0x0a /* Public */,
       9,    1,   84,    2, 0x0a /* Public */,
      11,    0,   87,    2, 0x0a /* Public */,
      12,    1,   88,    2, 0x0a /* Public */,
      12,    0,   91,    2, 0x2a /* Public | MethodCloned */,
      15,    0,   92,    2, 0x0a /* Public */,
      16,    0,   93,    2, 0x0a /* Public */,
      17,    0,   94,    2, 0x0a /* Public */,
      18,    0,   95,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ToolBars::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolBars *_t = static_cast<ToolBars *>(_o);
        switch (_id) {
        case 0: _t->folderDropped((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->changeToolbarMovable(); break;
        case 2: _t->changeToolbarsMovable(); break;
        case 3: _t->toolbarVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->toolbarOrientationChanged((*reinterpret_cast< Qt::Orientation(*)>(_a[1]))); break;
        case 5: _t->removePanelHighlight(); break;
        case 6: _t->panelHighlight((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 7: _t->panelHighlight(); break;
        case 8: _t->addPanelTriggered(); break;
        case 9: _t->removePanelTriggered(); break;
        case 10: _t->addPanelButtonTriggered(); break;
        case 11: _t->removePanelButtonTriggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    }
}

const QMetaObject ToolBars::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ToolBars.data,
      qt_meta_data_ToolBars,  qt_static_metacall, 0, 0}
};


const QMetaObject *ToolBars::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBars::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolBars.stringdata))
        return static_cast<void*>(const_cast< ToolBars*>(this));
    return QObject::qt_metacast(_clname);
}

int ToolBars::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
