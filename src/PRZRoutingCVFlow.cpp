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
// File: ../ATCSimul/src/PRZRoutingCVFlow.cpp
//
// Clase      : PRZRoutingCVFlow
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingCVFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingCVFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingCVFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingCVFlow,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingCVFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingCVFlow :: PRZRoutingCVFlow( PRZComponent& component)
                  : PRZRoutingFlow(component)
{

}



//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingCVFlow :: updateMessageInfo(PRZMessage* msg)
{   
   int delta       = msg->delta();
   PRZPosition pos = getOwnerRouter().getPosition();

   msg->setChannel(getComponent().componentId().channel());
   
   if( msg->flitNumber() == 1 )
   {
      if( delta > 0 )
      {
         msg->setDelta(delta-1);
         msg->setRoutingPort(_Xplus_);
      }
      else if( delta < 0 )
      {
         msg->setDelta(delta+1);
         msg->setRoutingPort(_Xminus_);
      }
      else
      {
         PRZString err;
         err.sprintf(ERR_PRZRCFLO_001, (char*)getComponent().asString() );
         EXIT_PROGRAM("Delta=0");
      }

      if( pos.valueForCoordinate(PRZPosition::X) == 0 )
         msg->setChannel(2);

   }
   else if( msg->flitNumber() == 2 )
   {
   
      msg->setType(PRZMessage::HEADER);
      
      if( delta > 0 )
      {
         msg->setDelta(delta-1);
         msg->setRoutingPort(_Yplus_);         
      }
      else if(delta < 0)
      {
         msg->setDelta(delta+1);
         msg->setRoutingPort(_Yminus_);
      }
      else
      {
         PRZString err;
         err.sprintf(ERR_PRZRCFLO_001, (char*)getComponent().asString() );
         EXIT_PROGRAM("Delta=0");
      }

      if( changeDirection(msg) )
      {
         // El mensaje cambia de dimension. Ahora debe salir por el
         // canal "1", aunque haya llegado por el canal "2".
         msg->setChannel(1);      
      }

      if( pos.valueForCoordinate(PRZPosition::Y) == 0 )
         msg->setChannel(2);

   }
   else if( msg->flitNumber() == 3 )
   {
      msg->setRoutingPort(_LocalNode_);
      msg->setChannel(1);
   }
   
   #ifndef NO_TRAZA
      PRZString texto= getComponent().asString() + ": ";
      texto += PRZString("Rout=") + PRZString(msg->getRoutingPort()) +
               PRZString(". Channel=") + PRZString(msg->getChannel());
      PRZWRITE2LOG(texto);
   #endif
   
   return changeDirection(msg);
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingCVFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{      
   inputInterfaz()->getData(&m_Message);

   #ifndef NO_TRAZA
      PRZString delayTime1 = PRZString(getOwnerRouter().getCurrentTime());
      PRZString texto1 = getComponent().asString() + " Flit Rx. TIME = ";
      texto1 += delayTime1 + " # " + m_Message->asString();
      PRZWRITE2LOG( texto1 );
   #endif

         
   if( isHeader() )
   {
      outputInterfaz()->clearDataRightNow();
   
      //Este flit se elimina. Asumiendo 2 cabeceras cuando agota una
      //de las dimensiones se tira el mensaje
      if( cutCurrentHeader(m_Message) )
      {
         getMessagePool().release(m_Message);
         return true;            
      }
      
      setWaiting(true);
      updateMessageInfo(m_Message);   
      uTIME delay = getOwnerRouter().getCurrentTime() + 1;         
      PRZEvent event(_BufferData_,m_Message);
      getEventQueue().enqueue(event,delay);
  
      inputInterfaz()->sendStopRightNow();
      
      return true;
   }
      
   if( m_Message->isTail() )
   {
      setHeader(true);
   }

   outputInterfaz()->sendDataRightNow(m_Message);
      
   #ifndef NO_TRAZA
      PRZString delayTime = getOwnerRouter().getCurrentTime();
      PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += delayTime + " # " + m_Message->asString();
      PRZWRITE2LOG( texto );
   #endif
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingCVFlow :: controlAlgoritm(Boolean info, int delta)
{
   return true;
}


//*************************************************************************


// fin del fichero
