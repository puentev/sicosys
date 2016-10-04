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
// File: ../ATCSimul/src/PRZCrossbarCTFlow.cpp
//
// Clase      : PRZCrossbarCTFlow
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCrossbarCTFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarCTFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarCTFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZCrossbar_HPP__
#include <PRZCrossbar.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZCrossbarCTFlow,PRZCrossbarFlow);

//*************************************************************************
//:
//  f: PRZCrossbarCTFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarCTFlow :: PRZCrossbarCTFlow( PRZComponent& component)
                   : PRZCrossbarFlow(component){
}


//*************************************************************************
//:
//  f: ~PRZCrossbarCTFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarCTFlow :: ~PRZCrossbarCTFlow()

{	
   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarCTFlow :: dispatchEvent(const PRZEvent& event)
{
   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      unsigned iPort = event.source();
      unsigned oPort = getOutputForInput(iPort).left();
      PRZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      // Si ya he mandado el mensaje, paso de todo
      if(!msg) return true;
      
      if( ! iPort )
      {
         PRZString err;
         err.sprintf( ERR_PRZCFLOC_001, 
                      (char*)getComponent().asString(),
                      (char*)msg->asString() );
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         PRZString err;
         err.sprintf( ERR_PRZCFLOC_002, 
                      (char*)getComponent().asString(),
                      (char*)msg->asString() );
         EXIT_PROGRAM(err);
      }

      if( (outputInterfaz(oPort)->isStopActive())&&  msg->isHeader() )
      {
         inputInterfaz(iPort)->sendStop();
         uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
         getEventQueue().enqueue(event,delayTime);

   #ifndef NO_TRAZA 
      PRZString time = getOwnerRouter().getCurrentTime();       
      PRZString texto = getComponent().asString() + " STOP TIME = ";
      texto += time + msg->asString() + " i=" + PRZString(iPort) +
               ", o=" + PRZString(oPort);
            
      PRZWRITE2LOG( texto );
   #endif
               return true;
      }

      setStateForInput(iPort,Connect);      
    
      inputInterfaz(iPort)->clearStopRightNow();
     
      outputInterfaz(oPort)->sendData(msg);

   #ifndef NO_TRAZA 
      PRZString time = getOwnerRouter().getCurrentTime();       
      PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += time + msg->asString() + " i=" + PRZString(iPort) +
               ", o=" + PRZString(oPort);
            
      PRZWRITE2LOG( texto );
   #endif

      // Ya he enviado el mensaje lo puedo eliminar de la tabla de mensajes.
      m_MessageReceivedTable->setValueAt(iPort,(PRZMessage *)0);
   
      if( msg->isTail() )
      {
         clearConnection(iPort);
         iPort = getConnectionRequestFor(oPort);
         if( iPort != 0 )
         {
            establishConnection(iPort,oPort);
            PRZEvent requestEvent(_CrossbarRequest_,iPort,oPort);
            uTIME delayTime = getOwnerRouter().getCurrentTime() +
                              MAXIMO(1,getHeaderDelay());
            
            if( ((PRZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
            {
               if( getHeaderDelay() > 1 )
                  delayTime -= 1;
            }
            
            getEventQueue().enqueue(requestEvent,delayTime);
         }
      }
   }
   return true;
}

   	
//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarCTFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
  // cerr<<"UP"<<getComponent().asString() <<"Time="<< getOwnerRouter().getCurrentTime()<<"  "<<interfaz<<""<<cv<<endl;
   
   return true;//Inhereited::onStopUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarCTFlow :: onStopDown(unsigned interfaz, unsigned cv)
{   
   //cerr<<"DOWN"<<getComponent().asString() <<"Time="<< getOwnerRouter().getCurrentTime()<<"  "<<interfaz<<endl;
   return Inhereited::onStopDown(interfaz,cv);
}
//*************************************************************************


// fin del fichero
