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
// File: ../ATCSimul/src/PRZRoutingBurbleFlowH1.cpp
//
// Clase      : PRZRoutingBurbleFlowH1
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingBurbleFlowH1.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZRoutingBurbleFlowH1.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingBurbleFlowH1.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZRoutingBurbleFlowH1,PRZRoutingBurbleFlow);


//*************************************************************************
//:
//  f: PRZRoutingBurbleFlowH1(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingBurbleFlowH1 :: PRZRoutingBurbleFlowH1( PRZComponent& component)
                      : PRZRoutingBurbleFlow(component)
{

}



//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingBurbleFlowH1 :: updateMessageInfo(PRZMessage* msg)
{
  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
  


     if( msg->isHeader() )
     {  
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
      }
   } 
   return changeDirection(msg);
}
//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingBurbleFlowH1 :: onReadyUp(unsigned interfaz, unsigned cv)
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
     // if( cutCurrentHeader(m_Message) ) return true;
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


// fin del fichero
