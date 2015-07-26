/****************************************************************************
** Meta object code from reading C++ file 'visionsettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/VSAGUI/visionsettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visionsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VisionSettings_t {
    QByteArrayData data[18];
    char stringdata0[447];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VisionSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VisionSettings_t qt_meta_stringdata_VisionSettings = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VisionSettings"
QT_MOC_LITERAL(1, 15, 36), // "on_cb_use_adaptContrast_state..."
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 4), // "arg1"
QT_MOC_LITERAL(4, 58, 38), // "on_sb_adaptContrKernel_editin..."
QT_MOC_LITERAL(5, 97, 41), // "on_sb_adaptContrastFactor_edi..."
QT_MOC_LITERAL(6, 139, 26), // "on_cb_useBlur_stateChanged"
QT_MOC_LITERAL(7, 166, 30), // "on_sb_blurMask_editingFinished"
QT_MOC_LITERAL(8, 197, 22), // "on_rb_useLight_toggled"
QT_MOC_LITERAL(9, 220, 7), // "checked"
QT_MOC_LITERAL(10, 228, 31), // "on_cb_ignoreBorder_stateChanged"
QT_MOC_LITERAL(11, 260, 28), // "on_cb_fillHoles_stateChanged"
QT_MOC_LITERAL(12, 289, 33), // "on_sb_sigmaFactor_editingFini..."
QT_MOC_LITERAL(13, 323, 21), // "on_rb_useOpen_toggled"
QT_MOC_LITERAL(14, 345, 22), // "on_rb_useClose_toggled"
QT_MOC_LITERAL(15, 368, 22), // "on_rb_useErode_toggled"
QT_MOC_LITERAL(16, 391, 23), // "on_rb_useDilate_toggled"
QT_MOC_LITERAL(17, 415, 31) // "on_sb_morphMask_editingFinished"

    },
    "VisionSettings\0on_cb_use_adaptContrast_stateChanged\0"
    "\0arg1\0on_sb_adaptContrKernel_editingFinished\0"
    "on_sb_adaptContrastFactor_editingFinished\0"
    "on_cb_useBlur_stateChanged\0"
    "on_sb_blurMask_editingFinished\0"
    "on_rb_useLight_toggled\0checked\0"
    "on_cb_ignoreBorder_stateChanged\0"
    "on_cb_fillHoles_stateChanged\0"
    "on_sb_sigmaFactor_editingFinished\0"
    "on_rb_useOpen_toggled\0on_rb_useClose_toggled\0"
    "on_rb_useErode_toggled\0on_rb_useDilate_toggled\0"
    "on_sb_morphMask_editingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VisionSettings[] = {

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
       1,    1,   84,    2, 0x08 /* Private */,
       4,    0,   87,    2, 0x08 /* Private */,
       5,    0,   88,    2, 0x08 /* Private */,
       6,    1,   89,    2, 0x08 /* Private */,
       7,    0,   92,    2, 0x08 /* Private */,
       8,    1,   93,    2, 0x08 /* Private */,
      10,    1,   96,    2, 0x08 /* Private */,
      11,    1,   99,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      13,    1,  103,    2, 0x08 /* Private */,
      14,    1,  106,    2, 0x08 /* Private */,
      15,    1,  109,    2, 0x08 /* Private */,
      16,    1,  112,    2, 0x08 /* Private */,
      17,    0,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,

       0        // eod
};

void VisionSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VisionSettings *_t = static_cast<VisionSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_cb_use_adaptContrast_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_sb_adaptContrKernel_editingFinished(); break;
        case 2: _t->on_sb_adaptContrastFactor_editingFinished(); break;
        case 3: _t->on_cb_useBlur_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_sb_blurMask_editingFinished(); break;
        case 5: _t->on_rb_useLight_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_cb_ignoreBorder_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_cb_fillHoles_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_sb_sigmaFactor_editingFinished(); break;
        case 9: _t->on_rb_useOpen_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_rb_useClose_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_rb_useErode_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_rb_useDilate_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->on_sb_morphMask_editingFinished(); break;
        default: ;
        }
    }
}

const QMetaObject VisionSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_VisionSettings.data,
      qt_meta_data_VisionSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VisionSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisionSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VisionSettings.stringdata0))
        return static_cast<void*>(const_cast< VisionSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int VisionSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
