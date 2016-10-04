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
// File: ../ATCSimul/src/PRZCrossbarCTFlowMux.cpp
//
// Clase      : PRZCrossbarCTFlowMux
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCrossbarCTFlowMux.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarCTFlowMux.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarCTFlowMux.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZCrossbarCTFlowMux,PRZCrossbarFlow);

//*************************************************************************
//:
//  f: PRZCrossbarCTFlowMux();
//
//  d:
//:
//*************************************************************************

PRZCrossbarCTFlowMux :: PRZCrossbarCTFlowMux( PRZComponent& component)
                   : PRZCrossbarFlow(component){
}


//*************************************************************************
//:
//  f: ~PRZCrossbarCTFlowMux();
//
//  d:
//:
//*************************************************************************

PRZCrossbarCTFlowMux :: ~PRZCrossbarCTFlowMux()

{	
   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarCTFlowMux :: dispatchEvent(const PRZEvent& event)
{        
   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      PRZCrossbar& crossbar = (PRZCrossbar&)(getComponent());
      unsigned iPort = event.source();
      unsigned oPort = getOutputForInput(iPort).left();
      unsigned channel = getOutputForInput(iPort).right();
     
      PRZMessage *msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      if(!msg) return true;
      if(!channel)
      {
         channel=1;
      }  
      else if( ! iPort )
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
      
      //Aqui esta la diferencia con la version normal
      if( msg->isHeader())
      {
         for(int i=1; i<=crossbar.numberOfInputs(); i++)
         {
            uPAIR temp;
            m_PortAsignamentTable->valueAt(i,temp);
            if(i!=iPort && temp.right()==oPort && msg->isHeader() )
            {
                inputInterfaz(iPort)->sendStop();
                uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
                getEventQueue().enqueue(event,delayTime); 
                return true;
            }
         }
      
         if( (outputInterfaz(oPort)->isStopActive(channel)))
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
      }

      establishConnection(iPort,oPort,channel);

      setStateForInput(iPort,Connect);      
    
      inputInterfaz(iPort)->clearStopRightNow();
     
      outputInterfaz(oPort)->sendData(msg,channel);
      m_MessageReceivedTable->setValueAt(iPort,(PRZMessage*)0);

   #ifndef NO_TRAZA 
      PRZString time = getOwnerRouter().getCurrentTime();       
      PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
      texto += time + msg->asString() + " i=" + PRZString(iPort) +
               ", o=" + PRZString(oPort);
            
      PRZWRITE2LOG( texto );
   #endif
   
      if( msg->isTail() )
      {
         clearConnection(iPort);
         iPort = getConnectionRequestFor(oPort);
         if( iPort != 0 )
         {
            establishConnection(iPort,oPort,channel);
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
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: 
//:
//*************************************************************************

Boolean PRZCrossbarCTFlowMux :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   PRZMessage* msg;
   PRZMessage* lastMessage;
   
   inputInterfaz(i)->getData(&msg);
   m_MessageReceivedTable->valueAt(i,&lastMessage);

   if(lastMessage)
      if( *msg == *lastMessage )
         return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

 	#ifndef NO_TRAZA
            PRZString texto = getComponent().asString() + " Flit Rx. TIME = ";
            texto += PRZString(delayTime) + " # " + msg->asString();
            PRZWRITE2LOG( texto );
   #endif
   if( getOutputForInput(i).left() == 0 )
   {
      // Esto es para que funcione el PRZCrossbarCTFlowMux
      if(!msg->isHeader())
      {
         m_MessageReceivedTable->setValueAt(i,(PRZMessage*)0);
	      return true;
	   } 
	   
      // Esta pasando la cabecera.
      unsigned outPort = extractOutputPortNumber(msg);
      unsigned outChannel = extractOutputChannel(msg);
                 
      PRZEvent requestEvent(_CrossbarRequest_,i,outPort,outChannel);
            
      setStateForInput(i,Waiting);

      if( isOutputPortFree(outPort) )
      {
         // Hay que asignar una salida
         
         delayTime += getHeaderDelay();
         
         if( ((PRZCrossbar&)getComponent()).isLocalNodeOutput(outPort) )
         {
            if( getHeaderDelay() > 1 )
               delayTime -= 1;
         }
         
         establishConnection(i,outPort);
         getEventQueue().enqueue(requestEvent,delayTime);
      }
      else
      {
         // El puerto de salida esta ocupado. Se encola una peticion
         // para el momento en el cual se libere.
               
    	   #ifndef NO_TRAZA
               PRZString texto = getComponent().asString() + " Esperando una SALIDA !";
               texto += PRZString(delayTime) + " iport= " + PRZString(i);
               PRZWRITE2LOG( texto );
   	   #endif
   	                  
         addConnectionRequest(i,uPAIR(outPort,outChannel));
      }

 	   #ifndef NO_TRAZA
 	      PRZString time = getOwnerRouter().getCurrentTime();       
         PRZString texto = getComponent().getName() + " Stop Tx. TIME = ";
         texto += time + " iport= " + PRZString(i);
         PRZWRITE2LOG( texto );
   	#endif
            
      inputInterfaz(i)->sendStopRightNow();
   }
   else
   {
      // Esta pasando el cuerpo del mensaje. Por tanto, existe un
      // camino asignado al Mensaje.
      delayTime += getDataDelay();
      PRZEvent passEvent(_CrossbarPass_,i);
      getEventQueue().enqueue(passEvent,delayTime);
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

Boolean PRZCrossbarCTFlowMux :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean PRZCrossbarCTFlowMux :: onStopDown(unsigned interfaz, unsigned cv)
{   
   //cerr<<"DOWN"<<getComponent().asString() <<"Time="<< getOwnerRouter().getCurrentTime()<<"  "<<interfaz<<endl;
   return Inhereited::onStopDown(interfaz,cv);
}

//*************************************************************************
//:
//  f: virtual unsigned extractOutputPortNumber(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarCTFlowMux :: extractOutputPortNumber(PRZMessage* msg)
{
   unsigned oPort = ((PRZCrossbar&)getComponent()).getOutputWith( msg->getRoutingPort(),1 );
   if( !oPort )
   {
      PRZString err;
      err.sprintf( ERR_PRZCMFLO_003,
                   (char*)getComponent().asString(),
                   (char*)msg->asString(),
                   (char*)_routingType2string(msg->getRoutingPort()) );
      EXIT_PROGRAM(err);
   }
   
   if( ((PRZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
      oPort += ( msg->getChannel() - 1 );
   return oPort;
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarCTFlowMux :: extractOutputPortNumber( przROUTINGTYPE dir,
                                                     unsigned channel )
{
   unsigned oPort = ((PRZCrossbar&)getComponent()).getOutputWith( dir,1 );
   if( !oPort )
   {
      PRZString err;
      err.sprintf( ERR_PRZCMFLO_003,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir) );
      EXIT_PROGRAM(err);
   }
   
   if( ((PRZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
      oPort += ( channel - 1 );
   return oPort;
}

//*************************************************************************


// fin del fichero
