/****************************************************************************
** Meta object code from reading C++ file 'player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../media/player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Player_t {
    QByteArrayData data[25];
    char stringdata[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Player_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Player_t qt_meta_stringdata_Player = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 15),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 4),
QT_MOC_LITERAL(5, 38, 2),
QT_MOC_LITERAL(6, 41, 11),
QT_MOC_LITERAL(7, 53, 10),
QT_MOC_LITERAL(8, 64, 9),
QT_MOC_LITERAL(9, 74, 5),
QT_MOC_LITERAL(10, 80, 4),
QT_MOC_LITERAL(11, 85, 4),
QT_MOC_LITERAL(12, 90, 11),
QT_MOC_LITERAL(13, 102, 19),
QT_MOC_LITERAL(14, 122, 3),
QT_MOC_LITERAL(15, 126, 19),
QT_MOC_LITERAL(16, 146, 16),
QT_MOC_LITERAL(17, 163, 14),
QT_MOC_LITERAL(18, 178, 19),
QT_MOC_LITERAL(19, 198, 14),
QT_MOC_LITERAL(20, 213, 10),
QT_MOC_LITERAL(21, 224, 8),
QT_MOC_LITERAL(22, 233, 20),
QT_MOC_LITERAL(23, 254, 11),
QT_MOC_LITERAL(24, 266, 6)
    },
    "Player\0playlistChanged\0\0QWidget*\0from\0"
    "to\0itemChanged\0ModelItem*\0playPause\0"
    "start\0like\0mute\0unmuteCheck\0"
    "setVolTrackbarValue\0pos\0changeTrackbarValue\0"
    "setTrackbarValue\0setTrackbarMax\0"
    "invertTimeCountdown\0onStateChanged\0"
    "MediaState\0newState\0onMediaStatusChanged\0"
    "MediaStatus\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Player[] = {

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
       1,    2,   84,    2, 0x06 /* Public */,
       6,    2,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   94,    2, 0x0a /* Public */,
       9,    0,   95,    2, 0x08 /* Private */,
      10,    0,   96,    2, 0x08 /* Private */,
      11,    0,   97,    2, 0x08 /* Private */,
      12,    1,   98,    2, 0x08 /* Private */,
      13,    1,  101,    2, 0x08 /* Private */,
      15,    1,  104,    2, 0x08 /* Private */,
      16,    1,  107,    2, 0x08 /* Private */,
      17,    1,  110,    2, 0x08 /* Private */,
      18,    0,  113,    2, 0x08 /* Private */,
      19,    1,  114,    2, 0x08 /* Private */,
      22,    1,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Player *_t = static_cast<Player *>(_o);
        switch (_id) {
        case 0: _t->playlistChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 1: _t->itemChanged((*reinterpret_cast< ModelItem*(*)>(_a[1])),(*reinterpret_cast< ModelItem*(*)>(_a[2]))); break;
        case 2: _t->playPause(); break;
        case 3: _t->start(); break;
        case 4: _t->like(); break;
        case 5: _t->mute(); break;
        case 6: _t->unmuteCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setVolTrackbarValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->changeTrackbarValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setTrackbarValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setTrackbarMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->invertTimeCountdown(); break;
        case 12: _t->onStateChanged((*reinterpret_cast< MediaState(*)>(_a[1]))); break;
        case 13: _t->onMediaStatusChanged((*reinterpret_cast< MediaStatus(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Player::*_t)(QWidget * , QWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Player::playlistChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (Player::*_t)(ModelItem * , ModelItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Player::itemChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Player::staticMetaObject = {
    { &AudioPlayer::staticMetaObject, qt_meta_stringdata_Player.data,
      qt_meta_data_Player,  qt_static_metacall, 0, 0}
};


const QMetaObject *Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Player::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Player.stringdata))
        return static_cast<void*>(const_cast< Player*>(this));
    return AudioPlayer::qt_metacast(_clname);
}

int Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AudioPlayer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Player::playlistChanged(QWidget * _t1, QWidget * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Player::itemChanged(ModelItem * _t1, ModelItem * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
