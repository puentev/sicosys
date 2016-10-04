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
// File: ../ATCSimul/src/PRZRoutingFlow.cpp
//
// Clase      : PRZRoutingFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingFlow,PRZFlow);

//*************************************************************************
//:
//  f: inline void setHeader(Boolean val);
//  d:
//
//  f: inline Boolean isHeader() const;
//  d:
//
//  f: inline void setWaiting(Boolean val);
//  d:
//
//  f: inline Boolean isWaiting() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZRoutingFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingFlow :: PRZRoutingFlow( PRZComponent& component)
                : PRZFlow(component),
                  m_NextHeader(true),
                  m_Waiting(false)
{
   m_Message=0;
}



//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: inputReading()
{
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: stateChange()
{ 
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: outputWriting()
{   
   if( ! outputInterfaz()->isStopActive() )
   {
      uTIME timeStamp;
      if( getEventQueue().firstElementTimeStamp(timeStamp) )
      {
         if( timeStamp <= getOwnerRouter().getCurrentTime() )
         {
            PRZEvent event;
            getEventQueue().dequeue(event);
            dispatchEvent(event);
         }
         else
         {
            outputInterfaz()->clearData();
         }
      }
   }      

   return true;
}


//*************************************************************************
//:
//  f: Boolean cutCurrentHeader(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: cutCurrentHeader(const PRZMessage* msg)
{
   PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());

   if( msg->flitNumber() > gData.getNumberOfHeaders() )
      return false;

   int delta = msg->delta();
   
   if( ! delta )
   {
      PRZString err;
      err.sprintf(ERR_PRZRFLOW_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);
   }
   
   if( (delta==1) || (delta==-1) )
   {
      
      return true;
   }
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: dispatchEvent(const PRZEvent& event)
{
   m_Message = event.message();
   if( ! controlAlgoritm() )
   {
      #ifndef NO_TRAZA
         PRZString texto = getComponent().asString();
         texto += " Esperando por el control del flujo ";
         PRZWRITE2LOG(texto);
      #endif
      
      uTIME delay = getOwnerRouter().getCurrentTime() + 1;         
      PRZEvent event(_BufferData_,m_Message);
      getEventQueue().enqueue(event,delay);
      return true;   
   }
   
   setWaiting(false);
   setHeader(false);
   outputInterfaz()->sendData(m_Message);
      
   #ifndef NO_TRAZA
      PRZString delayTime = PRZString(getOwnerRouter().getCurrentTime());
      PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += delayTime + " # " + m_Message->asString();
      PRZWRITE2LOG( texto );
   #endif
      
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//
//*************************************************************************

Boolean PRZRoutingFlow :: onReadyDown(unsigned interfaz, unsigned cv)
{
   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString() + " READY DOWN";
      PRZWRITE2LOG( texto );
   #endif
   
   if( !isWaiting() )
     outputInterfaz()->clearDataRightNow();

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString() + " STOP DOWN";
      PRZWRITE2LOG( texto );
   #endif
   
   if(  !isWaiting() )
     inputInterfaz()->clearStopRightNow();

   return true;
}

//*************************************************************************
//:
//  f: Boolean changeDirection(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlow :: changeDirection(const PRZMessage* msg)
{
   if( msg->getRoutingPort() ==_LocalNode_) return false;

   return ( msg->getRoutingPort()!=getComponent().getRoutingDirection() );
}


//*************************************************************************
//:
//  f: unsigned getDirection() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingFlow :: getDirection() const
{
   unsigned rc = 0;
   przROUTINGTYPE dir = getComponent().getRoutingDirection();
   switch(dir)
   {
      case _Xplus_ : case _Xminus_ : rc = CHANNEL_X; break;
      case _Yplus_ : case _Yminus_ : rc = CHANNEL_Y; break;
      case _Zplus_ : case _Zminus_ : rc = CHANNEL_Z; break;
      default : rc = CHANNEL_N;
   }
   return rc;
}


//*************************************************************************


// fin del fichero
