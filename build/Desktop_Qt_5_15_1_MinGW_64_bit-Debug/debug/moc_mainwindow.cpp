/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[25];
    char stringdata0[368];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 14), // "onStartClicked"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "onStopClicked"
QT_MOC_LITERAL(4, 41, 12), // "updateStatus"
QT_MOC_LITERAL(5, 54, 16), // "onTestQmlClicked"
QT_MOC_LITERAL(6, 71, 19), // "onSimulationStarted"
QT_MOC_LITERAL(7, 91, 19), // "onSimulationStopped"
QT_MOC_LITERAL(8, 111, 19), // "onDeviceDataUpdated"
QT_MOC_LITERAL(9, 131, 8), // "deviceId"
QT_MOC_LITERAL(10, 140, 8), // "newValue"
QT_MOC_LITERAL(11, 149, 19), // "onStatisticsUpdated"
QT_MOC_LITERAL(12, 169, 6), // "status"
QT_MOC_LITERAL(13, 176, 17), // "onTcpStartClicked"
QT_MOC_LITERAL(14, 194, 16), // "onTcpStopClicked"
QT_MOC_LITERAL(15, 211, 16), // "onTcpTestClicked"
QT_MOC_LITERAL(16, 228, 17), // "onTcpDataReceived"
QT_MOC_LITERAL(17, 246, 5), // "value"
QT_MOC_LITERAL(18, 252, 9), // "timestamp"
QT_MOC_LITERAL(19, 262, 18), // "onTcpStatusChanged"
QT_MOC_LITERAL(20, 281, 22), // "onRemoteStartRequested"
QT_MOC_LITERAL(21, 304, 21), // "onRemoteStopRequested"
QT_MOC_LITERAL(22, 326, 18), // "onRemoteClearChart"
QT_MOC_LITERAL(23, 345, 19), // "onRemoteSetInterval"
QT_MOC_LITERAL(24, 365, 2) // "ms"

    },
    "MainWindow\0onStartClicked\0\0onStopClicked\0"
    "updateStatus\0onTestQmlClicked\0"
    "onSimulationStarted\0onSimulationStopped\0"
    "onDeviceDataUpdated\0deviceId\0newValue\0"
    "onStatisticsUpdated\0status\0onTcpStartClicked\0"
    "onTcpStopClicked\0onTcpTestClicked\0"
    "onTcpDataReceived\0value\0timestamp\0"
    "onTcpStatusChanged\0onRemoteStartRequested\0"
    "onRemoteStopRequested\0onRemoteClearChart\0"
    "onRemoteSetInterval\0ms"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    0,  100,    2, 0x08 /* Private */,
       4,    0,  101,    2, 0x08 /* Private */,
       5,    0,  102,    2, 0x08 /* Private */,
       6,    0,  103,    2, 0x08 /* Private */,
       7,    0,  104,    2, 0x08 /* Private */,
       8,    2,  105,    2, 0x08 /* Private */,
      11,    1,  110,    2, 0x08 /* Private */,
      13,    0,  113,    2, 0x08 /* Private */,
      14,    0,  114,    2, 0x08 /* Private */,
      15,    0,  115,    2, 0x08 /* Private */,
      16,    3,  116,    2, 0x08 /* Private */,
      19,    1,  123,    2, 0x08 /* Private */,
      20,    0,  126,    2, 0x08 /* Private */,
      21,    0,  127,    2, 0x08 /* Private */,
      22,    0,  128,    2, 0x08 /* Private */,
      23,    1,  129,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,    9,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Double, QMetaType::QString,    9,   17,   18,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onStartClicked(); break;
        case 1: _t->onStopClicked(); break;
        case 2: _t->updateStatus(); break;
        case 3: _t->onTestQmlClicked(); break;
        case 4: _t->onSimulationStarted(); break;
        case 5: _t->onSimulationStopped(); break;
        case 6: _t->onDeviceDataUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 7: _t->onStatisticsUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onTcpStartClicked(); break;
        case 9: _t->onTcpStopClicked(); break;
        case 10: _t->onTcpTestClicked(); break;
        case 11: _t->onTcpDataReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 12: _t->onTcpStatusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->onRemoteStartRequested(); break;
        case 14: _t->onRemoteStopRequested(); break;
        case 15: _t->onRemoteClearChart(); break;
        case 16: _t->onRemoteSetInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
