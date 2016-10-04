//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** XBufferMainWindow meta object code from reading C++ file 'xbuffer.hpp'
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

#include "/disco4/prellezo/projects/xbuffer/inc/xbuffer.hpp"
#include <qmetaobj.h>


const char *XBufferMainWindow::className() const
{
    return "XBufferMainWindow";
}

QMetaObject *XBufferMainWindow::metaObj = 0;

void XBufferMainWindow::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("XBufferMainWindow","QDialog");
    if ( !QDialog::metaObject() )
	QDialog::initMetaObject();
    typedef void(XBufferMainWindow::*m1_t0)();
    typedef void(XBufferMainWindow::*m1_t1)();
    typedef void(XBufferMainWindow::*m1_t2)();
    typedef void(XBufferMainWindow::*m1_t3)();
    typedef void(XBufferMainWindow::*m1_t4)();
    typedef void(XBufferMainWindow::*m1_t5)();
    typedef void(XBufferMainWindow::*m1_t6)(const QString&);
    m1_t0 v1_0 = &XBufferMainWindow::stop;
    m1_t1 v1_1 = &XBufferMainWindow::run;
    m1_t2 v1_2 = &XBufferMainWindow::onOpenMenu;
    m1_t3 v1_3 = &XBufferMainWindow::onViewMenu;
    m1_t4 v1_4 = &XBufferMainWindow::onDelayMenu;
    m1_t5 v1_5 = &XBufferMainWindow::onAboutMenu;
    m1_t6 v1_6 = &XBufferMainWindow::onError;
    QMetaData *slot_tbl = new QMetaData[7];
    slot_tbl[0].name = "stop()";
    slot_tbl[1].name = "run()";
    slot_tbl[2].name = "onOpenMenu()";
    slot_tbl[3].name = "onViewMenu()";
    slot_tbl[4].name = "onDelayMenu()";
    slot_tbl[5].name = "onAboutMenu()";
    slot_tbl[6].name = "onError(const QString&)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    metaObj = new QMetaObject( "XBufferMainWindow", "QDialog",
	slot_tbl, 7,
	0, 0 );
}
