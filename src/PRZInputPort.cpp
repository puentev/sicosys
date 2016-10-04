//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZInputPort.cpp
//
// Clase      : PRZInputPort
//
// Hereda de  : PRZPort
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZInputPort.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZInputPort.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZInputPort.hpp>

#ifndef __PRZMessage_HPP__
#include <PRZMessage.hpp>
#endif

#ifndef __PRZGauge_HPP__
#include <PRZGauge.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZInputPort, PRZPort);

//************************************************************************
//:
//  f: PRZInputPort(const PRZComponentId& id);
//
//  d:
//:
//************************************************************************

PRZInputPort :: PRZInputPort( const PRZComponentId& id )
              : PRZPort(id)
{

}

//************************************************************************
//:
//  f: virtual ~PRZInputPort();
//
//  d:
//:
//************************************************************************

PRZInputPort :: ~PRZInputPort()
{

}


//************************************************************************
//:
//  f: virtual Boolean sendDataRightNow(const PRZMessage& msg);
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: sendDataRightNow(PRZMessage* msg)
{
   // Un puerto de entrada no puede enviar datos.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean clearDataRightNow();
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: clearDataRightNow()
{
   // Un puerto de entrada no puede manipular las señales de entrada.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean sendStopRightNow();
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: sendStopRightNow()
{
	if( sendStop() )
	{
      PRZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      {
         cursor.element()->sendStop();
      }
      return true;
   }
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean clearStopRightNow();
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: clearStopRightNow()
{
	if( clearStop() )
	{
      PRZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      {
         cursor.element()->clearStop();
      }
      return true;
   }
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean onDataUp(unsigned interfaz, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: onDataUp(unsigned interfaz, unsigned channel)
{
   if(getGauge()) getGauge()->onDataRx((PRZInterfaz*)getOwner());
   return getOwner() -> onDataUp(getOwnerInterfazId(),getChannel());
}


//************************************************************************
//:
//  f: virtual Boolean onDataDown(unsigned interfaz, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: onDataDown(unsigned interfaz, unsigned channel)
{
   return getOwner()->onDataDown(getOwnerInterfazId(),getChannel());
}


//************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: onStopUp(unsigned interfaz, unsigned channel)
{
   // Un puerto de entrada no recibe la señal de stop.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean PRZInputPort :: onStopDown(unsigned interfaz, unsigned channel)
{
   // Un puerto de entrada no recibe la señal de stop.
   return false;
}


//************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//************************************************************************

PRZString PRZInputPort :: asString() const
{
   PRZString texto ="IPort:";
   texto += getOwner()->asString();
   return texto;
}


//************************************************************************


// fin del fichero
