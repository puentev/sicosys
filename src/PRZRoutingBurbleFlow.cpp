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
// File: ../ATCSimul/src/PRZRoutingBurbleFlow.cpp
//
// Clase      : PRZRoutingBurbleFlow
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingBurbleFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingBurbleFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingBurbleFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingBurbleFlow,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingBurbleFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingBurbleFlow :: PRZRoutingBurbleFlow( PRZComponent& component)
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

Boolean PRZRoutingBurbleFlow :: updateMessageInfo(PRZMessage* msg)
{
   // Aqui nos llega la cabecera. En las clases hijas se sobrecargara este
   // metodo para la adaptatividad, o lo que sea.
   // De momento, vamos a suponer que es DOR.
   
   int delta = msg->delta();

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
         err.sprintf(ERR_PRZRBFLO_001, (char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
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
         err.sprintf(ERR_PRZRBFLO_001, (char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
   }
   else if( msg->flitNumber() == 3 )
   {
      msg->setRoutingPort(_LocalNode_);
   }

   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString() + " UPDATE out=";
      texto += PRZString(msg->getRoutingPort()) + "  " +
               msg->source().asString() + " -> " +
               msg->destiny().asString() +
               " DELTA= " + PRZString(msg->delta()) +
               " ORDER= " + PRZString(msg->flitNumber()) ;
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

Boolean PRZRoutingBurbleFlow :: onReadyUp(unsigned interfaz, unsigned cv)
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
      uTIME delay = getOwnerRouter().getCurrentTime() + 
                    getHeaderDelay();         
      PRZEvent event(_BufferData_,m_Message);
      getEventQueue().enqueue(event,delay);
        
      inputInterfaz()->sendStopRightNow();
      
      return true;
   }
      
   if( m_Message->isTail() ) setHeader(true);

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

Boolean PRZRoutingBurbleFlow :: controlAlgoritm(Boolean info,int delta)
{
   if( ! changeDirection(m_Message) ) return true;

   przROUTINGTYPE newDirection = m_Message->getRoutingPort();
         
   PRZComponentId id( PRZComponentId::FifoMemory, newDirection, "*" );
   PRZComponent* component = getOwnerRouter().componentWithIdLike(id); 
   PRZFifoMemory* fifo = POINTERCAST(PRZFifoMemory,component);
   if( ! fifo )
   {
//      cerr << newDirection << " $$ " << m_Message->asString() << endl;
      //return true;
      PRZString err;
     
      err.sprintf( ERR_PRZRBFLO_002,
                   (char*)getComponent().asString(),
                   (char*)id.asString() );
      EXIT_PROGRAM(err);  
   }
   
   unsigned packet = ((PRZSimulation*)(getComponent().getSimulation()))->
                     getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());

   #ifndef NO_TRAZA
      PRZString texto = PRZString("holes=") + PRZString(fifo->bufferHoles());
      texto += PRZString(" direction=") + PRZString(newDirection) + " yo=" +
               PRZString(getComponent().componentId().componentType());
      PRZWRITE2LOG(texto);
   #endif
   
  return fifo->bubbleReady();
   //return ( fifo->bufferHoles() >= 2*packet ) ? true : false;
}

//*************************************************************************


// fin del fichero
