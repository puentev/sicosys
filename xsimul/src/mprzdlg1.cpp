//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** PRZDialog1 meta object code from reading C++ file 'przdlg1.hpp'
**
** Created: Sun May 13 18:52:18 2001
**      by: The Qt MOC ($Id: mprzdlg1.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_PRZDialog1
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "../inc/przdlg1.hpp"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *PRZDialog1::className() const
{
    return "PRZDialog1";
}

QMetaObject *PRZDialog1::metaObj = 0;

void PRZDialog1::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("PRZDialog1","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString PRZDialog1::tr(const char* s)
{
    return qApp->translate( "PRZDialog1", s, 0 );
}

QString PRZDialog1::tr(const char* s, const char * c)
{
    return qApp->translate( "PRZDialog1", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* PRZDialog1::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(PRZDialog1::*m1_t0)();
    typedef void(PRZDialog1::*m1_t1)(const char*);
    typedef void(PRZDialog1::*m1_t2)(bool);
    typedef void(PRZDialog1::*m1_t3)(bool);
    m1_t0 v1_0 = Q_AMPERSAND PRZDialog1::onAcceptClick;
    m1_t1 v1_1 = Q_AMPERSAND PRZDialog1::onTrafficSelect;
    m1_t2 v1_2 = Q_AMPERSAND PRZDialog1::onBimodalCheck;
    m1_t3 v1_3 = Q_AMPERSAND PRZDialog1::onBufferCheck;
    QMetaData *slot_tbl = QMetaObject::new_metadata(4);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(4);
    slot_tbl[0].name = "onAcceptClick()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "onTrafficSelect(const char*)";
    slot_tbl[1].ptr = (QMember)v1_1;
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "onBimodalCheck(bool)";
    slot_tbl[2].ptr = (QMember)v1_2;
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "onBufferCheck(bool)";
    slot_tbl[3].ptr = (QMember)v1_3;
    slot_tbl_access[3] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"PRZDialog1", "QDialog",
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
