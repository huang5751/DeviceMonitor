/****************************************************************************
** Meta object code from reading C++ file 'tcpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/core/tcpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpServer_t {
    QByteArrayData data[26];
    char stringdata0[346];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpServer_t qt_meta_stringdata_TcpServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpServer"
QT_MOC_LITERAL(1, 10, 13), // "serverStarted"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "port"
QT_MOC_LITERAL(4, 30, 13), // "serverStopped"
QT_MOC_LITERAL(5, 44, 15), // "clientConnected"
QT_MOC_LITERAL(6, 60, 10), // "clientInfo"
QT_MOC_LITERAL(7, 71, 18), // "clientDisconnected"
QT_MOC_LITERAL(8, 90, 8), // "dataSent"
QT_MOC_LITERAL(9, 99, 8), // "deviceId"
QT_MOC_LITERAL(10, 108, 5), // "value"
QT_MOC_LITERAL(11, 114, 9), // "timestamp"
QT_MOC_LITERAL(12, 124, 17), // "simulationStarted"
QT_MOC_LITERAL(13, 142, 17), // "simulationStopped"
QT_MOC_LITERAL(14, 160, 13), // "statusChanged"
QT_MOC_LITERAL(15, 174, 6), // "status"
QT_MOC_LITERAL(16, 181, 20), // "remoteStartRequested"
QT_MOC_LITERAL(17, 202, 19), // "remoteStopRequested"
QT_MOC_LITERAL(18, 222, 16), // "remoteClearChart"
QT_MOC_LITERAL(19, 239, 23), // "remoteSetUpdateInterval"
QT_MOC_LITERAL(20, 263, 2), // "ms"
QT_MOC_LITERAL(21, 266, 12), // "sendTestData"
QT_MOC_LITERAL(22, 279, 15), // "onNewConnection"
QT_MOC_LITERAL(23, 295, 20), // "onClientDisconnected"
QT_MOC_LITERAL(24, 316, 11), // "onReadyRead"
QT_MOC_LITERAL(25, 328, 17) // "sendDataToClients"

    },
    "TcpServer\0serverStarted\0\0port\0"
    "serverStopped\0clientConnected\0clientInfo\0"
    "clientDisconnected\0dataSent\0deviceId\0"
    "value\0timestamp\0simulationStarted\0"
    "simulationStopped\0statusChanged\0status\0"
    "remoteStartRequested\0remoteStopRequested\0"
    "remoteClearChart\0remoteSetUpdateInterval\0"
    "ms\0sendTestData\0onNewConnection\0"
    "onClientDisconnected\0onReadyRead\0"
    "sendDataToClients"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       4,    0,  102,    2, 0x06 /* Public */,
       5,    1,  103,    2, 0x06 /* Public */,
       7,    1,  106,    2, 0x06 /* Public */,
       8,    3,  109,    2, 0x06 /* Public */,
      12,    0,  116,    2, 0x06 /* Public */,
      13,    0,  117,    2, 0x06 /* Public */,
      14,    1,  118,    2, 0x06 /* Public */,
      16,    0,  121,    2, 0x06 /* Public */,
      17,    0,  122,    2, 0x06 /* Public */,
      18,    0,  123,    2, 0x06 /* Public */,
      19,    1,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    0,  127,    2, 0x0a /* Public */,
      22,    0,  128,    2, 0x08 /* Private */,
      23,    0,  129,    2, 0x08 /* Private */,
      24,    0,  130,    2, 0x08 /* Private */,
      25,    0,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UShort,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::QString,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serverStarted((*reinterpret_cast< quint16(*)>(_a[1]))); break;
        case 1: _t->serverStopped(); break;
        case 2: _t->clientConnected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->clientDisconnected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->dataSent((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->simulationStarted(); break;
        case 6: _t->simulationStopped(); break;
        case 7: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->remoteStartRequested(); break;
        case 9: _t->remoteStopRequested(); break;
        case 10: _t->remoteClearChart(); break;
        case 11: _t->remoteSetUpdateInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->sendTestData(); break;
        case 13: _t->onNewConnection(); break;
        case 14: _t->onClientDisconnected(); break;
        case 15: _t->onReadyRead(); break;
        case 16: _t->sendDataToClients(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpServer::*)(quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::serverStarted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::serverStopped)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::clientConnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::clientDisconnected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QString , double , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::dataSent)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::simulationStarted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::simulationStopped)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::statusChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::remoteStartRequested)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::remoteStopRequested)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::remoteClearChart)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::remoteSetUpdateInterval)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TcpServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TcpServer.data,
    qt_meta_data_TcpServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::serverStarted(quint16 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpServer::serverStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpServer::clientConnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TcpServer::clientDisconnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TcpServer::dataSent(QString _t1, double _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TcpServer::simulationStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TcpServer::simulationStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TcpServer::statusChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void TcpServer::remoteStartRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TcpServer::remoteStopRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TcpServer::remoteClearChart()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TcpServer::remoteSetUpdateInterval(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
