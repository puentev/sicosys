//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
/****************************************************************************
** XSimulationView meta object code from reading C++ file 'xsimul.hpp'
**
** Created: Sun May 13 18:52:18 2001
**      by: The Qt MOC ($Id: mxsimul.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_XSimulationView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "../inc/xsimul.hpp"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *XSimulationView::className() const
{
    return "XSimulationView";
}

QMetaObject *XSimulationView::metaObj = 0;

void XSimulationView::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("XSimulationView","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString XSimulationView::tr(const char* s)
{
    return qApp->translate( "XSimulationView", s, 0 );
}

QString XSimulationView::tr(const char* s, const char * c)
{
    return qApp->translate( "XSimulationView", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* XSimulationView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(XSimulationView::*m1_t0)();
    typedef void(XSimulationView::*m1_t1)();
    typedef void(XSimulationView::*m1_t2)();
    typedef void(XSimulationView::*m1_t3)();
    typedef void(XSimulationView::*m1_t4)();
    typedef void(XSimulationView::*m1_t5)();
    typedef void(XSimulationView::*m1_t6)();
    typedef void(XSimulationView::*m1_t7)();
    typedef void(XSimulationView::*m1_t8)(double,double,double,double);
    typedef void(XSimulationView::*m1_t9)(const PRZString&);
    m1_t0 v1_0 = Q_AMPERSAND XSimulationView::onButtonStopClick;
    m1_t1 v1_1 = Q_AMPERSAND XSimulationView::onButtonRunClick;
    m1_t2 v1_2 = Q_AMPERSAND XSimulationView::onButtonPrintClick;
    m1_t3 v1_3 = Q_AMPERSAND XSimulationView::onButtonResultClick;
    m1_t4 v1_4 = Q_AMPERSAND XSimulationView::onButtonQuitClick;
    m1_t5 v1_5 = Q_AMPERSAND XSimulationView::onOpenMenu;
    m1_t6 v1_6 = Q_AMPERSAND XSimulationView::onSettingsMenu;
    m1_t7 v1_7 = Q_AMPERSAND XSimulationView::onAboutMenu;
    m1_t8 v1_8 = Q_AMPERSAND XSimulationView::onDataReceived;
    m1_t9 v1_9 = Q_AMPERSAND XSimulationView::onResultReceived;
    QMetaData *slot_tbl = QMetaObject::new_metadata(10);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(10);
    slot_tbl[0].name = "onButtonStopClick()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "onButtonRunClick()";
    slot_tbl[1].ptr = (QMember)v1_1;
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "onButtonPrintClick()";
    slot_tbl[2].ptr = (QMember)v1_2;
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "onButtonResultClick()";
    slot_tbl[3].ptr = (QMember)v1_3;
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "onButtonQuitClick()";
    slot_tbl[4].ptr = (QMember)v1_4;
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "onOpenMenu()";
    slot_tbl[5].ptr = (QMember)v1_5;
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "onSettingsMenu()";
    slot_tbl[6].ptr = (QMember)v1_6;
    slot_tbl_access[6] = QMetaData::Public;
    slot_tbl[7].name = "onAboutMenu()";
    slot_tbl[7].ptr = (QMember)v1_7;
    slot_tbl_access[7] = QMetaData::Public;
    slot_tbl[8].name = "onDataReceived(double,double,double,double)";
    slot_tbl[8].ptr = (QMember)v1_8;
    slot_tbl_access[8] = QMetaData::Public;
    slot_tbl[9].name = "onResultReceived(const PRZString&)";
    slot_tbl[9].ptr = (QMember)v1_9;
    slot_tbl_access[9] = QMetaData::Public;
    typedef void(XSimulationView::*m2_t0)(double,double,double,double);
    typedef void(XSimulationView::*m2_t1)(const PRZString&);
    m2_t0 v2_0 = Q_AMPERSAND XSimulationView::signalDataReceived;
    m2_t1 v2_1 = Q_AMPERSAND XSimulationView::signalResultReceived;
    QMetaData *signal_tbl = QMetaObject::new_metadata(2);
    signal_tbl[0].name = "signalDataReceived(double,double,double,double)";
    signal_tbl[0].ptr = (QMember)v2_0;
    signal_tbl[1].name = "signalResultReceived(const PRZString&)";
    signal_tbl[1].ptr = (QMember)v2_1;
    metaObj = QMetaObject::new_metaobject(
	"XSimulationView", "QDialog",
	slot_tbl, 10,
	signal_tbl, 2,
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

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL signalDataReceived
void XSimulationView::signalDataReceived( double t0, double t1, double t2, double t3 )
{
    // No builtin function for signal parameter type double,double,double,double
    QConnectionList *clist = receivers("signalDataReceived(double,double,double,double)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef void (QObject::*RT1)(double);
    typedef void (QObject::*RT2)(double,double);
    typedef void (QObject::*RT3)(double,double,double);
    typedef void (QObject::*RT4)(double,double,double,double);
    RT0 r0;
    RT1 r1;
    RT2 r2;
    RT3 r3;
    RT4 r4;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
#ifdef Q_FP_CCAST_BROKEN
		r0 = reinterpret_cast<RT0>(*(c->member()));
#else
		r0 = (RT0)*(c->member());
#endif
		(object->*r0)();
		break;
	    case 1:
#ifdef Q_FP_CCAST_BROKEN
		r1 = reinterpret_cast<RT1>(*(c->member()));
#else
		r1 = (RT1)*(c->member());
#endif
		(object->*r1)(t0);
		break;
	    case 2:
#ifdef Q_FP_CCAST_BROKEN
		r2 = reinterpret_cast<RT2>(*(c->member()));
#else
		r2 = (RT2)*(c->member());
#endif
		(object->*r2)(t0, t1);
		break;
	    case 3:
#ifdef Q_FP_CCAST_BROKEN
		r3 = reinterpret_cast<RT3>(*(c->member()));
#else
		r3 = (RT3)*(c->member());
#endif
		(object->*r3)(t0, t1, t2);
		break;
	    case 4:
#ifdef Q_FP_CCAST_BROKEN
		r4 = reinterpret_cast<RT4>(*(c->member()));
#else
		r4 = (RT4)*(c->member());
#endif
		(object->*r4)(t0, t1, t2, t3);
		break;
	}
    }
}

// SIGNAL signalResultReceived
void XSimulationView::signalResultReceived( const PRZString& t0 )
{
    // No builtin function for signal parameter type const PRZString&
    QConnectionList *clist = receivers("signalResultReceived(const PRZString&)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef void (QObject::*RT1)(const PRZString&);
    RT0 r0;
    RT1 r1;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
#ifdef Q_FP_CCAST_BROKEN
		r0 = reinterpret_cast<RT0>(*(c->member()));
#else
		r0 = (RT0)*(c->member());
#endif
		(object->*r0)();
		break;
	    case 1:
#ifdef Q_FP_CCAST_BROKEN
		r1 = reinterpret_cast<RT1>(*(c->member()));
#else
		r1 = (RT1)*(c->member());
#endif
		(object->*r1)(t0);
		break;
	}
    }
}
