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
// File: ../ATCSimul/src/PRZOutputPort.cpp
//
// Clase      : PRZOutputPort
//
// Hereda de  : PRZPort
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZOutputPort.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZOutputPort.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZOutputPort.hpp>

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZOutputPort, PRZPort);


//*************************************************************************
//:
//  f: PRZOutputPort(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZOutputPort :: PRZOutputPort( const PRZComponentId& id)
               : PRZPort(id)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZOutputPort();
//
//  d:
//:
//*************************************************************************

PRZOutputPort :: ~PRZOutputPort()
{
 
}


//*************************************************************************
//:
//  f: virtual Boolean sendDataRightNow(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: sendDataRightNow(PRZMessage* msg)
{
	// Se debe enviar el mensaje ahora mismo, sin esperar a que lo haga 
	// el run de la conexion.
	
//	sendData(msg);
	
   PRZConnectionSet::Cursor cursor(m_ConnectionSet);
   forCursor(cursor)
   {
      cursor.element()->sendData(msg);
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean clearDataRightNow();
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: clearDataRightNow()
{
	// Se debe limpiar el mensaje ahora mismo, sin esperar a que lo haga 
	// el run de la conexion.
	
	clearData();
	
   PRZConnectionSet::Cursor cursor(m_ConnectionSet);
   forCursor(cursor)
   {
      cursor.element()->clearData();
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean sendStopRightNow();
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: sendStopRightNow()
{
   // Un puerto de salida no puede enviar la señal de stop, unicamente
   // datos y ready.
   
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean clearStopRightNow();
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: clearStopRightNow()
{
   // Un puerto de salida no puede enviar la señal de stop, unicamente
   // datos y ready.
   
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataUp(unsigned interfaz, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: onDataUp(unsigned interfaz, unsigned channel)
{
   // Un puerto de salida no puede nunca recibir datos
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataDown(unsigned interfaz, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: onDataDown(unsigned interfaz, unsigned channel)
{
   // Un puerto de salida no puede nunca recibir datos
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: onStopUp(unsigned interfaz, unsigned channel)
{
   return getOwner()->onStopUp(getOwnerInterfazId(),getChannel());
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean PRZOutputPort :: onStopDown(unsigned interfaz, unsigned channel)
{
   return getOwner()->onStopDown(getOwnerInterfazId(),getChannel());
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZOutputPort :: asString() const
{
   PRZString texto ="OPort:";
   texto += getOwner()->asString();
   return texto;
}

//*************************************************************************


// fin del fichero
