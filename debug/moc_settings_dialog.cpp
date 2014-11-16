/****************************************************************************
** Meta object code from reading C++ file 'settings_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialogs/settings_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SettingsDialog_t {
    QByteArrayData data[16];
    char stringdata[448];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsDialog_t qt_meta_stringdata_SettingsDialog = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 23),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 23),
QT_MOC_LITERAL(4, 64, 23),
QT_MOC_LITERAL(5, 88, 29),
QT_MOC_LITERAL(6, 118, 30),
QT_MOC_LITERAL(7, 149, 27),
QT_MOC_LITERAL(8, 177, 28),
QT_MOC_LITERAL(9, 206, 28),
QT_MOC_LITERAL(10, 235, 37),
QT_MOC_LITERAL(11, 273, 38),
QT_MOC_LITERAL(12, 312, 41),
QT_MOC_LITERAL(13, 354, 42),
QT_MOC_LITERAL(14, 397, 24),
QT_MOC_LITERAL(15, 422, 25)
    },
    "SettingsDialog\0on_cancelButton_clicked\0"
    "\0on_acceptButton_clicked\0"
    "on_browseButton_clicked\0"
    "on_defaultColorButton_clicked\0"
    "on_listenedColorButton_clicked\0"
    "on_likedColorButton_clicked\0"
    "on_playedColorButton_clicked\0"
    "on_folderColorButton_clicked\0"
    "on_defaultItemTextColorButton_clicked\0"
    "on_selectedItemTextColorButton_clicked\0"
    "on_defaultItemInfoTextColorButton_clicked\0"
    "on_selectedItemInfoTextColorButton_clicked\0"
    "on_spectrumColor_clicked\0"
    "on_spectrumColor2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SettingsDialog *_t = static_cast<SettingsDialog *>(_o);
        switch (_id) {
        case 0: _t->on_cancelButton_clicked(); break;
        case 1: _t->on_acceptButton_clicked(); break;
        case 2: _t->on_browseButton_clicked(); break;
        case 3: _t->on_defaultColorButton_clicked(); break;
        case 4: _t->on_listenedColorButton_clicked(); break;
        case 5: _t->on_likedColorButton_clicked(); break;
        case 6: _t->on_playedColorButton_clicked(); break;
        case 7: _t->on_folderColorButton_clicked(); break;
        case 8: _t->on_defaultItemTextColorButton_clicked(); break;
        case 9: _t->on_selectedItemTextColorButton_clicked(); break;
        case 10: _t->on_defaultItemInfoTextColorButton_clicked(); break;
        case 11: _t->on_selectedItemInfoTextColorButton_clicked(); break;
        case 12: _t->on_spectrumColor_clicked(); break;
        case 13: _t->on_spectrumColor2_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SettingsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SettingsDialog.data,
      qt_meta_data_SettingsDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *SettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsDialog.stringdata))
        return static_cast<void*>(const_cast< SettingsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
