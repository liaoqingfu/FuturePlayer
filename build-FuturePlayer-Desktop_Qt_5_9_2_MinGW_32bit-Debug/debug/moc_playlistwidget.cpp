/****************************************************************************
** Meta object code from reading C++ file 'playlistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../playlistwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playlistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlayListWidget_t {
    QByteArrayData data[16];
    char stringdata0[243];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlayListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlayListWidget_t qt_meta_stringdata_PlayListWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PlayListWidget"
QT_MOC_LITERAL(1, 15, 15), // "ItemDoubleClick"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 4), // "name"
QT_MOC_LITERAL(4, 37, 11), // "ItemDeleted"
QT_MOC_LITERAL(5, 49, 1), // "n"
QT_MOC_LITERAL(6, 51, 14), // "signalClearAll"
QT_MOC_LITERAL(7, 66, 16), // "signalActionOpen"
QT_MOC_LITERAL(8, 83, 20), // "signalClearValidItem"
QT_MOC_LITERAL(9, 104, 19), // "signalOpenDirectory"
QT_MOC_LITERAL(10, 124, 14), // "slotActionOpen"
QT_MOC_LITERAL(11, 139, 20), // "slotActionDeleteItem"
QT_MOC_LITERAL(12, 160, 24), // "slotActionClearValidItem"
QT_MOC_LITERAL(13, 185, 18), // "slotActionClearAll"
QT_MOC_LITERAL(14, 204, 14), // "slotActionPlay"
QT_MOC_LITERAL(15, 219, 23) // "slotActionOpenDirectory"

    },
    "PlayListWidget\0ItemDoubleClick\0\0name\0"
    "ItemDeleted\0n\0signalClearAll\0"
    "signalActionOpen\0signalClearValidItem\0"
    "signalOpenDirectory\0slotActionOpen\0"
    "slotActionDeleteItem\0slotActionClearValidItem\0"
    "slotActionClearAll\0slotActionPlay\0"
    "slotActionOpenDirectory"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       6,    0,   80,    2, 0x06 /* Public */,
       7,    0,   81,    2, 0x06 /* Public */,
       8,    0,   82,    2, 0x06 /* Public */,
       9,    1,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   86,    2, 0x0a /* Public */,
      11,    0,   87,    2, 0x0a /* Public */,
      12,    0,   88,    2, 0x0a /* Public */,
      13,    0,   89,    2, 0x0a /* Public */,
      14,    0,   90,    2, 0x0a /* Public */,
      15,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PlayListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlayListWidget *_t = static_cast<PlayListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ItemDoubleClick((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ItemDeleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->signalClearAll(); break;
        case 3: _t->signalActionOpen(); break;
        case 4: _t->signalClearValidItem(); break;
        case 5: _t->signalOpenDirectory((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slotActionOpen(); break;
        case 7: _t->slotActionDeleteItem(); break;
        case 8: _t->slotActionClearValidItem(); break;
        case 9: _t->slotActionClearAll(); break;
        case 10: _t->slotActionPlay(); break;
        case 11: _t->slotActionOpenDirectory(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PlayListWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::ItemDoubleClick)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (PlayListWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::ItemDeleted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (PlayListWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::signalClearAll)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (PlayListWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::signalActionOpen)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (PlayListWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::signalClearValidItem)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (PlayListWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlayListWidget::signalOpenDirectory)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject PlayListWidget::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_PlayListWidget.data,
      qt_meta_data_PlayListWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlayListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlayListWidget.stringdata0))
        return static_cast<void*>(this);
    return QListWidget::qt_metacast(_clname);
}

int PlayListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void PlayListWidget::ItemDoubleClick(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PlayListWidget::ItemDeleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PlayListWidget::signalClearAll()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PlayListWidget::signalActionOpen()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PlayListWidget::signalClearValidItem()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void PlayListWidget::signalOpenDirectory(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
