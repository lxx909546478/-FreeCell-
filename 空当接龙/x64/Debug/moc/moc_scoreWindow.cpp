/****************************************************************************
** Meta object code from reading C++ file 'ScoreWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../ScoreWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScoreWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ScoreWindow_t {
    QByteArrayData data[11];
    char stringdata0[126];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScoreWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScoreWindow_t qt_meta_stringdata_ScoreWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ScoreWindow"
QT_MOC_LITERAL(1, 12, 15), // "signalDataReady"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "signalTestConn"
QT_MOC_LITERAL(4, 44, 12), // "isConnecting"
QT_MOC_LITERAL(5, 57, 14), // "signalNextGame"
QT_MOC_LITERAL(6, 72, 8), // "showData"
QT_MOC_LITERAL(7, 81, 10), // "lookUpHost"
QT_MOC_LITERAL(8, 92, 9), // "QHostInfo"
QT_MOC_LITERAL(9, 102, 4), // "info"
QT_MOC_LITERAL(10, 107, 18) // "slotNextGameButton"

    },
    "ScoreWindow\0signalDataReady\0\0"
    "signalTestConn\0isConnecting\0signalNextGame\0"
    "showData\0lookUpHost\0QHostInfo\0info\0"
    "slotNextGameButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScoreWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       5,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   49,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
      10,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,

       0        // eod
};

void ScoreWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScoreWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalDataReady(); break;
        case 1: _t->signalTestConn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->signalNextGame(); break;
        case 3: _t->showData(); break;
        case 4: _t->lookUpHost((*reinterpret_cast< QHostInfo(*)>(_a[1]))); break;
        case 5: _t->slotNextGameButton(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QHostInfo >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ScoreWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScoreWindow::signalDataReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ScoreWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScoreWindow::signalTestConn)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ScoreWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScoreWindow::signalNextGame)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ScoreWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ScoreWindow.data,
    qt_meta_data_ScoreWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ScoreWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScoreWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScoreWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScoreWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ScoreWindow::signalDataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScoreWindow::signalTestConn(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ScoreWindow::signalNextGame()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
