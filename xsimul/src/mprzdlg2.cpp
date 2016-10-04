//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** PRZDialog2 meta object code from reading C++ file 'przdlg2.hpp'
**
** Created: Sun May 13 18:52:18 2001
**      by: The Qt MOC ($Id: mprzdlg2.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_PRZDialog2
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "../inc/przdlg2.hpp"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *PRZDialog2::className() const
{
    return "PRZDialog2";
}

QMetaObject *PRZDialog2::metaObj = 0;

void PRZDialog2::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("PRZDialog2","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString PRZDialog2::tr(const char* s)
{
    return qApp->translate( "PRZDialog2", s, 0 );
}

QString PRZDialog2::tr(const char* s, const char * c)
{
    return qApp->translate( "PRZDialog2", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* PRZDialog2::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(PRZDialog2::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND PRZDialog2::onAcceptClick;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "onAcceptClick()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"PRZDialog2", "QDialog",
	slot_tbl, 1,
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
