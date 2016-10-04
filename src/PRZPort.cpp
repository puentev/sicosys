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
// File: ../ATCSimul/src/PRZPort.cpp
//
// Clase      : PRZPort
//
// Hereda de  : PRZComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZPort.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

//Borja
static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZPort.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZPort.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZGauge_HPP__
#include <PRZGauge.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZPort,PRZComponent);

//************************************************************************
//:
//  f: virtual Boolean sendDataInmediately(const PRZMessage& msg) = 0;
//  d:
//
//  f: virtual Boolean receiveData(const PRZMessage& msg) = 0;
//  d:
//
//  f: inline virtual void clearData();
//  d:
//
//  f: inline virtual void clearDataInmediately();
//  d:
//
//  f: inline PRZMessage getData() const;
//  d:
//
//  f: inline Boolean isMessageValid() const;
//  d:
//
//:
//************************************************************************

//************************************************************************
//:
//  f: PRZPort(const PRZComponentId& id);
//
//  d:
//:
//************************************************************************

PRZPort :: PRZPort(const PRZComponentId& id)
         : PRZComponent(id),
           m_Ready(false),
           m_Stop(false),
           m_DataChanged(false),
           m_StopChanged(false),
           m_Gauge(0),
           m_CurrentConnection(0)
{
   setAutoDeleteConnectionSet(false);
   m_MessageStored=0;
}

   
//************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//************************************************************************

void PRZPort :: run(uTIME runTime)
{
   // por defecto, no hace nada.
}


//************************************************************************
//:
//  f: unsigned getOwnerInterfazId() const
//
//  d:
//:
//************************************************************************

unsigned PRZPort :: getOwnerInterfazId() const
{
   PRZInterfaz* interfaz = (PRZInterfaz*)getOwner();
   return interfaz->getNumberId();
}


//*************************************************************************
//:
//  f: Boolean establishConnection(PRZConnection* connection);
//
//  d:
//:
//*************************************************************************

Boolean PRZPort :: establishConnection(PRZConnection* connection)
{
 //    if ( m_ConnectionSet.numberOfElements() == 1 ) cerr << "SI\n";
//   if( ! m_ConnectionSet.containsElement(connection) )
      return m_ConnectionSet.add(connection);

//   return false;
}


//*************************************************************************
//:
//  f: Boolean sendData(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZPort :: sendData(PRZMessage* msg)
{
   setDataChanged(((!m_Ready) ? true:false));
   m_Ready = true;
   setData(msg);
   if(getGauge()) getGauge()->onDataTx((PRZInterfaz*)getOwner());
   return true;
}


//************************************************************************
//:
//  f: void setDataChanged(Boolean change);
//
//  d:
//:
//************************************************************************

void PRZPort :: setDataChanged(Boolean change)
{ 
   if ( m_DataChanged != change )
   {
      unsigned conns = m_ConnectionSet.numberOfElements();
      if( conns > 1 )
      {
         if( (++m_CurrentConnection) == conns ) 
         {
            m_DataChanged = change;
            m_CurrentConnection = 0;
         }
      }
      else
      {
         m_DataChanged = change;
      }
   } 
}

//************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//************************************************************************

PRZString PRZPort :: asString() const
{
   return "Port";
}

//************************************************************************


// fin del fichero
