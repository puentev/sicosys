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
// File: ../ATCSimul/src/PRZRoutingBurbleFlowH1Virtual.cpp
//
// Clase      : PRZRoutingBurbleFlowH1Virtual
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingBurbleFlowH1Virtual.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZRoutingBurbleFlowH1Virtual.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingBurbleFlowH1Virtual.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingBurbleFlowH1Virtual,PRZRoutingBurbleFlow);


//*************************************************************************
//:
//  f: PRZRoutingBurbleFlowH1Virtual(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingBurbleFlowH1Virtual :: PRZRoutingBurbleFlowH1Virtual( PRZComponent& component)
                      : PRZRoutingBurbleFlowH1(component),
                        m_BufferXpReq(0),
                        m_BufferXmReq(0),
                        m_BufferYpReq(0),
                        m_BufferYmReq(0),
                        m_BufferXpRep(0),
                        m_BufferXmRep(0),
                        m_BufferYpRep(0),
                        m_BufferYmRep(0)

{

}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingBurbleFlowH1Virtual :: postInitialize()
{  
   
   PRZComponentId idXpRep( PRZComponentId::FifoMemory, _Xplus_,  "BUF11" );
   PRZComponentId idXmRep( PRZComponentId::FifoMemory, _Xminus_, "BUF21" );
   PRZComponentId idYpRep( PRZComponentId::FifoMemory, _Yplus_,  "BUF31" );
   PRZComponentId idYmRep( PRZComponentId::FifoMemory, _Yminus_, "BUF41" );
   
   PRZComponentId idXpReq( PRZComponentId::FifoMemory, _Xplus_,  "BUF12" );
   PRZComponentId idXmReq( PRZComponentId::FifoMemory, _Xminus_, "BUF22" );
   PRZComponentId idYpReq( PRZComponentId::FifoMemory, _Yplus_,  "BUF32" );
   PRZComponentId idYmReq( PRZComponentId::FifoMemory, _Yminus_, "BUF42" );
   
   
   PRZComponent* compoXpRep = getOwnerRouter().componentWithIdLike(idXpRep); 
   PRZComponent* compoXmRep = getOwnerRouter().componentWithIdLike(idXmRep); 
   PRZComponent* compoYpRep = getOwnerRouter().componentWithIdLike(idYpRep);
   PRZComponent* compoYmRep = getOwnerRouter().componentWithIdLike(idYmRep);
   
   PRZComponent* compoXpReq = getOwnerRouter().componentWithIdLike(idXpReq); 
   PRZComponent* compoXmReq = getOwnerRouter().componentWithIdLike(idXmReq); 
   PRZComponent* compoYpReq = getOwnerRouter().componentWithIdLike(idYpReq);
   PRZComponent* compoYmReq = getOwnerRouter().componentWithIdLike(idYmReq); 
     
   m_BufferXpRep = POINTERCAST(PRZFifoMemory,compoXpRep);
   m_BufferXmRep = POINTERCAST(PRZFifoMemory,compoXmRep);
   m_BufferYpRep = POINTERCAST(PRZFifoMemory,compoYpRep);
   m_BufferYmRep = POINTERCAST(PRZFifoMemory,compoYmRep);

   m_BufferXpReq = POINTERCAST(PRZFifoMemory,compoXpReq);
   m_BufferXmReq = POINTERCAST(PRZFifoMemory,compoXmReq);
   m_BufferYpReq = POINTERCAST(PRZFifoMemory,compoYpReq);
   m_BufferYmReq = POINTERCAST(PRZFifoMemory,compoYmReq);
      
   if( !(m_BufferXpRep && m_BufferXmRep && m_BufferYpRep && m_BufferYmRep) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
   if( !(m_BufferXpReq && m_BufferXmReq && m_BufferYpReq && m_BufferYmReq) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }

}
//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingBurbleFlowH1Virtual :: updateMessageInfo(PRZMessage* msg)
{
  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
  
     if( msg->isHeader() )
     {  
        
      if(msg->isRequest())
        msg->setChannel(2);
      else
        msg->setChannel(1); 
      
      if( deltaX > 1 )
      {
         msg->setDelta(deltaX-1,0);
         msg->setRoutingPort(_Xplus_);         
      }
      else if( deltaX < -1 )
      {
         msg->setDelta(deltaX+1,0);
         msg->setRoutingPort(_Xminus_);
      }
      else if( deltaY > +1 )
      {
         msg->setDelta(deltaY-1,1);
         msg->setRoutingPort(_Yplus_);         
      }
      else if(deltaY < -1)
      {
         msg->setDelta(deltaY+1,1);
         msg->setRoutingPort(_Yminus_);
      }
      else
      {
         msg->setRoutingPort(_LocalNode_);
         msg->setChannel(1); 
      }
   } 
   return changeDirection(msg);
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d: si sere zokete... sin esto se bloquea echando leches!
//:
//*************************************************************************

Boolean PRZRoutingBurbleFlowH1Virtual :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
   // El valor de "salidaX" es TRUE cuando se sale por un canal X, y FALSE en
   // caso de salir por un canal Y.
	
   unsigned fifoID;
   Boolean isRequest=m_Message->isRequest();
            
   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   przROUTINGTYPE newDirection = m_Message->getRoutingPort();

   if( myDirection == newDirection || newDirection == _LocalNode_) return true;
   switch( newDirection )
   {
      case _Xplus_  : fifoID = 1; break;
      case _Xminus_ : fifoID = 2; break;
      case _Yplus_  : fifoID = 3; break;
      case _Yminus_ : fifoID = 4; break;         
   }
      
   if(isRequest)
   {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpReq; break;
         case 2: fifo = m_BufferXmReq; break;
         case 3: fifo = m_BufferYpReq; break;
         case 4: fifo = m_BufferYmReq; break;
         default:
                 fifo = 0;
       }
       if(!fifo)
       {
           EXIT_PROGRAM("Puntero a fifo sin inicializar");
       }
    }
    
    else
    {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpRep;break;
         case 2: fifo = m_BufferXmRep;break;
         case 3: fifo = m_BufferYpRep;break;
         case 4: fifo = m_BufferYmRep;break;
      }
   }
   return fifo->bubbleReady();
}
//*************************************************************************


// fin del fichero
