//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** XBufferView meta object code from reading C++ file 'xbview.hpp'
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

#include "/disco4/prellezo/projects/xbuffer/inc/xbview.hpp"
#include <qmetaobj.h>


const char *XBufferView::className() const
{
    return "XBufferView";
}

QMetaObject *XBufferView::metaObj = 0;

void XBufferView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("XBufferView","QDialog");
    if ( !QDialog::metaObject() )
	QDialog::initMetaObject();
    typedef void(XBufferView::*m1_t0)();
    typedef void(XBufferView::*m1_t1)();
    m1_t0 v1_0 = &XBufferView::onAverageClick;
    m1_t1 v1_1 = &XBufferView::onPrintClick;
    QMetaData *slot_tbl = new QMetaData[2];
    slot_tbl[0].name = "onAverageClick()";
    slot_tbl[1].name = "onPrintClick()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    metaObj = new QMetaObject( "XBufferView", "QDialog",
	slot_tbl, 2,
	0, 0 );
}
