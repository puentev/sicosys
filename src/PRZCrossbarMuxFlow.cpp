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
// File: ../ATCSimul/src/PRZCrossbarMuxFlow.cpp
//
// Clase      : PRZCrossbarMuxFlow
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMuxFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarMuxFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarMuxFlow.hpp>

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
 
IMPLEMENT_RTTI_DERIVED(PRZCrossbarMuxFlow,PRZCrossbarFlow);

//*************************************************************************
//:
//  f: PRZCrossbarMuxFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarMuxFlow :: PRZCrossbarMuxFlow( PRZComponent& component)
                    : PRZCrossbarFlow(component)
{

}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMuxFlow :: dispatchEvent(const PRZEvent& event)
{
   if( event.type() == _CrossbarRequest_ )
   {
/*      PRZMessage* msg;  
      m_MessageReceivedTable->valueAt(event.source(),&msg); 
      getMessagePool()->release(msg);
      m_MessageReceivedTable->setValueAt(event.source(),(PRZMessage *)0); */

      unsigned oPort = event.destiny();

      if( ! isOutputPortFree(oPort) )
      {
         // Si entre medias alguien ha pillado la salida, se contesta que no
         Inhereited::cleanConnectionRequestFor(oPort);
         return true;
      }
      
      uPAIR inp_channel = checkMultipleRequestFor(oPort);
      
      if( outputInterfaz(oPort)->isStopActive(inp_channel.right()) )
      {
         cleanConnectionRequestFor(oPort,inp_channel);
         return false;
      }
      
      unsigned iPort = inp_channel.left();
      
      establishConnection(iPort,oPort,inp_channel.right());

      cleanConnectionRequestFor(oPort,inp_channel);
      setLastInputForOutput(iPort,oPort);
      setStateForInput(iPort,Connect);
      
      removeAllRequestEventsForInput(iPort);
      removeAllRequestEventsForOutput(oPort);

      inputInterfaz(iPort)->clearStopRightNow();

   }

   else if( event.type() == _CrossbarPass_ ) 
   {
      unsigned iPort  = event.source();
      unsigned oPort  = getOutputForInput(iPort).left();
      unsigned channel= getOutputForInput(iPort).right();
 
      if( ! iPort )
      {
         PRZString err;
         err.sprintf( ERR_PRZCMFLO_001,
                      (char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         PRZString err;
         err.sprintf( ERR_PRZCMFLO_002,
                      (char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
 
      PRZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,&msg);
      
      /*
      
      Si la peticion que se hizo funciono correctamente es que no estaba el
      stop activo en ese momento, por lo que ahora no se le hace caso.
      
      if( outputInterfaz(oPort)->isStopActive(channel) && msg.isHeader() )
      {
         inputInterfaz(iPort)->sendStopRightNow();
         uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
         getEventQueue().enqueue(event,delayTime);
         return true;
      }
      */
      
      setStateForInput(iPort,Connect);      
    
      inputInterfaz(iPort)->clearStopRightNow();
      
      //$$Sepierden mensajes (preguntar prellezo)
#ifdef UNIROUTER
      updateMessageInfo(msg,oPort,1);    
      outputInterfaz(oPort)->sendData(msg,1);
#else
      updateMessageInfo(msg,oPort,channel);    
      outputInterfaz(oPort)->sendData(msg,channel);
#endif

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

Boolean PRZCrossbarMuxFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   PRZMessage* msg;
   PRZMessage* lastMessage;

   inputInterfaz(i)->getData(&msg);
   m_MessageReceivedTable->valueAt(i,&lastMessage);
      
   if(lastMessage)
      if( (*msg == *lastMessage) && (msg->type()!=PRZMessage::REQ) ) return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

   #ifndef NO_TRAZA
        PRZString texto = getComponent().asString() + " Flit Rx. TIME = ";
        texto += PRZString(delayTime) + " # " + msg->asString();
        PRZWRITE2LOG( texto );
  #endif
            
   if( msg->type()==PRZMessage::REQ )//getOutputForInput(i).left() == 0)
   {      
      // Esta pasando la peticion
      unsigned outPort    = extractOutputPortNumber(msg);            
      unsigned outChannel = (((PRZCrossbar&)getComponent()).isLocalNodeOutput(outPort)) ?
                            1 : extractOutputChannel(msg);

      #ifndef NO_TRAZA
            PRZString texto2 = getComponent().asString() + " Peticion desde i="+PRZString(i)+"  o="+PRZString(outPort);
            PRZWRITE2LOG( texto2 );
      #endif
	   
      getMessagePool().release(msg);
      m_MessageReceivedTable->setValueAt(i,0);

      if( isOutputPortFree(outPort,outChannel) && 
          ( ! outputInterfaz(outPort)->isStopActive(outChannel)) )
      {
         // Hay que asignar una salida
         delayTime += getHeaderDelay();
         if( ! isAnyRequestForOutput(outPort) )
         {
            PRZEvent requestEvent(_CrossbarRequest_,i,outPort,outChannel);
            getEventQueue().enqueue(requestEvent,delayTime);
         }
         addConnectionRequest(i,uPAIR(outPort,outChannel));
          
      }
      else
      {
         // El puerto de salida esta ocupado. No se encolan las
         // peticiones. Este crossbar solo atiende a las solicitudes
         // de puertos que son validas
         
           #ifndef NO_TRAZA
            PRZString texto = getComponent().asString() + " REQ rechazado." +msg->asString();
            PRZWRITE2LOG( texto );
   	   #endif
        
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

Boolean PRZCrossbarMuxFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   PRZWRITE2LOG(getComponent().asString()+ ": StopUp "+PRZString(interfaz)+" "+PRZString(cv));
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMuxFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return Inhereited::onStopDown(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual unsigned extractOutputPortNumber(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarMuxFlow :: extractOutputPortNumber(PRZMessage* msg)
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
//  f: virtual unsigned extractOutputPortNumber( przROUTINGTYPE dir,
//                                               unsigned channel );
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarMuxFlow :: extractOutputPortNumber( przROUTINGTYPE dir,
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
//:
//  f: virtual Boolean isOutputPortFree( unsigned outputPort,
//                                       unsigned channel=0) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMuxFlow :: isOutputPortFree( unsigned outputPort, 
                                                unsigned channel) const
{
   // Si esta ocupado alguno de los canales de la salida multiplexada,
   // entonces se retorna "false".  
   
   unsigned state=0;
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();
   unsigned canalesOcupados = 0;
   for( int i=1; i<=mux; i++ )
   {
      m_OutputStateTable->valueAt(i+(outputPort-1)*mux,state);
      if(state) return false;
   }
   
   return true;
   
}


//*************************************************************************
//:
//  f: void cleanInputInterfaces();
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMuxFlow :: cleanInputInterfaces()
{
   // Se sobrecarga para que no haga nada.
}


//*************************************************************************
//:
//  f: virtual void postRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMuxFlow :: postRun(uTIME time)
{
   setCleanInterfaces(false);
   run(time);
   setCleanInterfaces(true);
}


//*************************************************************************
//:
//  f: Boolean updateMessageInfo( PRZmessage** msg,
//                                unsigned oPort,
//                                unsigned channel );
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMuxFlow :: updateMessageInfo( PRZMessage* msg, 
                                              unsigned oPort,
                                              unsigned channel )
{
   // No hace nada sobre las cabeceras.
   return true;
}

//*************************************************************************

// fin del fichero
