/****************************************************************************
** Meta object code from reading C++ file 'dzinfopane.h'
**
** Created: Wed Aug 29 17:54:45 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dzinfopane.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dzinfopane.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DzSceneInfoPaneExAction[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_DzSceneInfoPaneExAction[] = {
    "DzSceneInfoPaneExAction\0"
};

void DzSceneInfoPaneExAction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DzSceneInfoPaneExAction::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DzSceneInfoPaneExAction::staticMetaObject = {
    { &DzPaneAction::staticMetaObject, qt_meta_stringdata_DzSceneInfoPaneExAction,
      qt_meta_data_DzSceneInfoPaneExAction, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DzSceneInfoPaneExAction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DzSceneInfoPaneExAction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DzSceneInfoPaneExAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DzSceneInfoPaneExAction))
        return static_cast<void*>(const_cast< DzSceneInfoPaneExAction*>(this));
    return DzPaneAction::qt_metacast(_clname);
}

int DzSceneInfoPaneExAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DzPaneAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DzSceneInfoPaneEx[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      34,   29,   18,   18, 0x08,
      55,   18,   18,   18, 0x08,
      70,   18,   18,   18, 0x08,
      87,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DzSceneInfoPaneEx[] = {
    "DzSceneInfoPaneEx\0\0refresh()\0node\0"
    "refreshInfo(DzNode*)\0blockRefresh()\0"
    "unblockRefresh()\0showContextMenu(QPoint)\0"
};

void DzSceneInfoPaneEx::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DzSceneInfoPaneEx *_t = static_cast<DzSceneInfoPaneEx *>(_o);
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: _t->refreshInfo((*reinterpret_cast< DzNode*(*)>(_a[1]))); break;
        case 2: _t->blockRefresh(); break;
        case 3: _t->unblockRefresh(); break;
        case 4: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DzSceneInfoPaneEx::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DzSceneInfoPaneEx::staticMetaObject = {
    { &DzPane::staticMetaObject, qt_meta_stringdata_DzSceneInfoPaneEx,
      qt_meta_data_DzSceneInfoPaneEx, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DzSceneInfoPaneEx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DzSceneInfoPaneEx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DzSceneInfoPaneEx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DzSceneInfoPaneEx))
        return static_cast<void*>(const_cast< DzSceneInfoPaneEx*>(this));
    return DzPane::qt_metacast(_clname);
}

int DzSceneInfoPaneEx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DzPane::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
