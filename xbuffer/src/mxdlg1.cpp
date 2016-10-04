//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** XDialog1 meta object code from reading C++ file 'xdlg1.hpp'
**
** Created: Sat Mar 28 18:27:02 1998
**      by: The Qt Meta Object Compiler ($Revision: 1.1.1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 2
#elif Q_MOC_OUTPUT_REVISION != 2
#error Moc format conflict - please regenerate all moc files
#endif

#include "/disco4/prellezo/projects/xbuffer/inc/xdlg1.hpp"
#include <qmetaobj.h>


const char *XDialog1::className() const
{
    return "XDialog1";
}

QMetaObject *XDialog1::metaObj = 0;

void XDialog1::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("XDialog1","QDialog");
    if ( !QDialog::metaObject() )
	QDialog::initMetaObject();
    typedef void(XDialog1::*m1_t0)();
    typedef void(XDialog1::*m1_t1)();
    typedef void(XDialog1::*m1_t2)();
    typedef void(XDialog1::*m1_t3)();
    typedef void(XDialog1::*m1_t4)();
    m1_t0 v1_0 = &XDialog1::onAcceptClick;
    m1_t1 v1_1 = &XDialog1::onAdd1Click;
    m1_t2 v1_2 = &XDialog1::onAddAllClick;
    m1_t3 v1_3 = &XDialog1::onRemove1Click;
    m1_t4 v1_4 = &XDialog1::onRemoveAllClick;
    QMetaData *slot_tbl = new QMetaData[5];
    slot_tbl[0].name = "onAcceptClick()";
    slot_tbl[1].name = "onAdd1Click()";
    slot_tbl[2].name = "onAddAllClick()";
    slot_tbl[3].name = "onRemove1Click()";
    slot_tbl[4].name = "onRemoveAllClick()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    metaObj = new QMetaObject( "XDialog1", "QDialog",
	slot_tbl, 5,
	0, 0 );
}
