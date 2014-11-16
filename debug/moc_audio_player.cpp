/****************************************************************************
** Meta object code from reading C++ file 'audio_player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../media/audio_player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audio_player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioPlayer_t {
    QByteArrayData data[45];
    char stringdata[570];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioPlayer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioPlayer_t qt_meta_stringdata_AudioPlayer = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 17),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 13),
QT_MOC_LITERAL(4, 45, 13),
QT_MOC_LITERAL(5, 59, 13),
QT_MOC_LITERAL(6, 73, 12),
QT_MOC_LITERAL(7, 86, 10),
QT_MOC_LITERAL(8, 97, 18),
QT_MOC_LITERAL(9, 116, 11),
QT_MOC_LITERAL(10, 128, 15),
QT_MOC_LITERAL(11, 144, 20),
QT_MOC_LITERAL(12, 165, 15),
QT_MOC_LITERAL(13, 181, 15),
QT_MOC_LITERAL(14, 197, 7),
QT_MOC_LITERAL(15, 205, 6),
QT_MOC_LITERAL(16, 212, 12),
QT_MOC_LITERAL(17, 225, 12),
QT_MOC_LITERAL(18, 238, 4),
QT_MOC_LITERAL(19, 243, 5),
QT_MOC_LITERAL(20, 249, 6),
QT_MOC_LITERAL(21, 256, 4),
QT_MOC_LITERAL(22, 261, 13),
QT_MOC_LITERAL(23, 275, 16),
QT_MOC_LITERAL(24, 292, 15),
QT_MOC_LITERAL(25, 308, 16),
QT_MOC_LITERAL(26, 325, 11),
QT_MOC_LITERAL(27, 337, 8),
QT_MOC_LITERAL(28, 346, 15),
QT_MOC_LITERAL(29, 362, 16),
QT_MOC_LITERAL(30, 379, 16),
QT_MOC_LITERAL(31, 396, 3),
QT_MOC_LITERAL(32, 400, 9),
QT_MOC_LITERAL(33, 410, 15),
QT_MOC_LITERAL(34, 426, 12),
QT_MOC_LITERAL(35, 439, 12),
QT_MOC_LITERAL(36, 452, 11),
QT_MOC_LITERAL(37, 464, 16),
QT_MOC_LITERAL(38, 481, 18),
QT_MOC_LITERAL(39, 500, 7),
QT_MOC_LITERAL(40, 508, 12),
QT_MOC_LITERAL(41, 521, 11),
QT_MOC_LITERAL(42, 533, 12),
QT_MOC_LITERAL(43, 546, 10),
QT_MOC_LITERAL(44, 557, 12)
    },
    "AudioPlayer\0remoteUnprocessed\0\0"
    "volumeChanged\0playbackEnded\0downloadEnded\0"
    "stateChanged\0MediaState\0mediaStatusChanged\0"
    "MediaStatus\0spectrumChanged\0"
    "QList<QVector<int> >\0positionChanged\0"
    "durationChanged\0started\0stoped\0"
    "signalUpdate\0calcSpectrum\0play\0pause\0"
    "resume\0stop\0endOfPlayback\0endOfDownloading\0"
    "slidePosForward\0slidePosBackward\0"
    "setPosition\0position\0slideVolForward\0"
    "slideVolBackward\0setChannelVolume\0val\0"
    "setVolume\0MediaStateFlags\0StoppedState\0"
    "PlayingState\0PausedState\0MediaStatusFlags\0"
    "UnknownMediaStatus\0NoMedia\0LoadingMedia\0"
    "LoadedMedia\0StalledMedia\0EndOfMedia\0"
    "InvalidMedia"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       2,  188, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  144,    2, 0x06 /* Public */,
       3,    1,  145,    2, 0x06 /* Public */,
       4,    0,  148,    2, 0x06 /* Public */,
       5,    0,  149,    2, 0x06 /* Public */,
       6,    1,  150,    2, 0x06 /* Public */,
       8,    1,  153,    2, 0x06 /* Public */,
      10,    1,  156,    2, 0x06 /* Public */,
      12,    1,  159,    2, 0x06 /* Public */,
      13,    1,  162,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  165,    2, 0x08 /* Private */,
      15,    0,  166,    2, 0x08 /* Private */,
      16,    0,  167,    2, 0x08 /* Private */,
      17,    0,  168,    2, 0x08 /* Private */,
      18,    0,  169,    2, 0x0a /* Public */,
      19,    0,  170,    2, 0x0a /* Public */,
      20,    0,  171,    2, 0x0a /* Public */,
      21,    0,  172,    2, 0x0a /* Public */,
      22,    0,  173,    2, 0x0a /* Public */,
      23,    0,  174,    2, 0x0a /* Public */,
      24,    0,  175,    2, 0x0a /* Public */,
      25,    0,  176,    2, 0x0a /* Public */,
      26,    1,  177,    2, 0x0a /* Public */,
      28,    0,  180,    2, 0x0a /* Public */,
      29,    0,  181,    2, 0x0a /* Public */,
      30,    1,  182,    2, 0x0a /* Public */,
      32,    1,  185,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

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
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Int,   31,

 // enums: name, flags, count, data
      33, 0x0,    3,  196,
      37, 0x0,    7,  202,

 // enum data: key, value
      34, uint(AudioPlayer::StoppedState),
      35, uint(AudioPlayer::PlayingState),
      36, uint(AudioPlayer::PausedState),
      38, uint(AudioPlayer::UnknownMediaStatus),
      39, uint(AudioPlayer::NoMedia),
      40, uint(AudioPlayer::LoadingMedia),
      41, uint(AudioPlayer::LoadedMedia),
      42, uint(AudioPlayer::StalledMedia),
      43, uint(AudioPlayer::EndOfMedia),
      44, uint(AudioPlayer::InvalidMedia),

       0        // eod
};

void AudioPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioPlayer *_t = static_cast<AudioPlayer *>(_o);
        switch (_id) {
        case 0: _t->remoteUnprocessed(); break;
        case 1: _t->volumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->playbackEnded(); break;
        case 3: _t->downloadEnded(); break;
        case 4: _t->stateChanged((*reinterpret_cast< MediaState(*)>(_a[1]))); break;
        case 5: _t->mediaStatusChanged((*reinterpret_cast< MediaStatus(*)>(_a[1]))); break;
        case 6: _t->spectrumChanged((*reinterpret_cast< QList<QVector<int> >(*)>(_a[1]))); break;
        case 7: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->durationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->started(); break;
        case 10: _t->stoped(); break;
        case 11: _t->signalUpdate(); break;
        case 12: _t->calcSpectrum(); break;
        case 13: _t->play(); break;
        case 14: _t->pause(); break;
        case 15: _t->resume(); break;
        case 16: _t->stop(); break;
        case 17: _t->endOfPlayback(); break;
        case 18: _t->endOfDownloading(); break;
        case 19: _t->slidePosForward(); break;
        case 20: _t->slidePosBackward(); break;
        case 21: _t->setPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->slideVolForward(); break;
        case 23: _t->slideVolBackward(); break;
        case 24: _t->setChannelVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QVector<int> > >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::remoteUnprocessed)) {
                *result = 0;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::volumeChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (AudioPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::playbackEnded)) {
                *result = 2;
            }
        }
        {
            typedef void (AudioPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::downloadEnded)) {
                *result = 3;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(MediaState );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::stateChanged)) {
                *result = 4;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(MediaStatus );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::mediaStatusChanged)) {
                *result = 5;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(QList<QVector<int> > );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::spectrumChanged)) {
                *result = 6;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::positionChanged)) {
                *result = 7;
            }
        }
        {
            typedef void (AudioPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioPlayer::durationChanged)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject AudioPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioPlayer.data,
      qt_meta_data_AudioPlayer,  qt_static_metacall, 0, 0}
};


const QMetaObject *AudioPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioPlayer.stringdata))
        return static_cast<void*>(const_cast< AudioPlayer*>(this));
    return QObject::qt_metacast(_clname);
}

int AudioPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void AudioPlayer::remoteUnprocessed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AudioPlayer::volumeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AudioPlayer::playbackEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void AudioPlayer::downloadEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void AudioPlayer::stateChanged(MediaState _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AudioPlayer::mediaStatusChanged(MediaStatus _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AudioPlayer::spectrumChanged(QList<QVector<int> > _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AudioPlayer::positionChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AudioPlayer::durationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
