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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleH1Flow.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleH1Flow
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleH1Flow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleH1Flow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleH1Flow.hpp>

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

#ifndef __PRZCrossbar_HPP__
#include <PRZCrossbar.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleH1Flow,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleH1Flow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleH1Flow :: PRZRoutingAdaptativeBurbleH1Flow( PRZComponent& component)
                          : PRZRoutingFlow(component),
                            m_FlitsRx(1),
                            m_RequestState(1),
                            m_CurrentOut(0),
                            m_BufferXp(0),
                            m_BufferXm(0),
                            m_BufferYp(0),
                            m_BufferYm(0),
                            m_CurrentLocalPort(0),
                            m_FirstMessage(true)
{
   setState(Free);
}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleH1Flow :: postInitialize()
{  
   PRZComponentId idXp( PRZComponentId::FifoMemory, _Xplus_,  "*", 2 );
   PRZComponentId idXm( PRZComponentId::FifoMemory, _Xminus_, "*", 2 );
   PRZComponentId idYp( PRZComponentId::FifoMemory, _Yplus_,  "*", 2 );
   PRZComponentId idYm( PRZComponentId::FifoMemory, _Yminus_, "*", 2 );
   
   PRZComponent* compoXp = getOwnerRouter().componentWithIdLike(idXp); 
   PRZComponent* compoXm = getOwnerRouter().componentWithIdLike(idXm); 
   PRZComponent* compoYp = getOwnerRouter().componentWithIdLike(idYp);
   PRZComponent* compoYm = getOwnerRouter().componentWithIdLike(idYm);
   
   m_BufferXp = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXm = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYp = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYm = POINTERCAST(PRZFifoMemory,compoYm);
   
   if( !(m_BufferXp && m_BufferXm && m_BufferYp && m_BufferYm) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }

}


//*************************************************************************
//:
//  f: void initializeRouting();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleH1Flow :: initializeRouting()
{  
   setState(Free);
   m_RequestState = 1;
}


//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: updateMessageInfo(PRZMessage* msg)
{
   if( m_FirstMessage )
   {
      m_FirstMessage = false;   
      int delta = msg->delta(m_CurrentOut);
      if( delta > 0 )
      {
         msg->setDelta(delta-1,m_CurrentOut);
      }
      else if( delta < 0 )
      {
         msg->setDelta(delta+1,m_CurrentOut);
      }

   }
   
   if( msg->isTail()) m_FirstMessage = true;
     
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: onStopUp(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: onStopDown(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   inputInterfaz()->getData(&m_Message);    
   
   #ifndef NO_TRAZA
      uTIME delay = getOwnerRouter().getCurrentTime();
      PRZString texto = getComponent().asString() + " Flit Rx. TIME = ";
      texto += PRZString(delay) + " # " + m_Message->asString();
      PRZWRITE2LOG( texto );
   #endif
   
   if( getState() == Free )
   {
      addHeaderReceived(m_Message);
   }

   else if( getState() == WaitingHeader )
   {
      addHeaderReceived(m_Message);
   }

   else if( getState() == DataTx )
   {
      dataTransmit();
   }

   else if( getState() == WaitCrossbarRx )
   {
      addHeaderReceived(m_Message);
   }

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: onReadyDown(unsigned interfaz, unsigned cv)
{  
   return true;
}


//*************************************************************************
//:
//  f: void addHeaderReceived(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleH1Flow :: addHeaderReceived(PRZMessage* msg)
{ 
//   static PRZMessage msg;
//   msg = aMessage; 
   m_FlitsRx.add(msg);
   
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      inputInterfaz()->sendStopRightNow();
      PRZMessage* msgTx; 
      if( this->createMessageRequest(&msgTx) )
      {
         outputInterfaz()->sendDataRightNow(msgTx);

         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Routing_CheckCrossbarResponse,msg);
         getEventQueue().enqueue(event,delay);
       
         setState(WaitCrossbarRx);
         
         #ifndef NO_TRAZA
            uTIME time = getOwnerRouter().getCurrentTime();
            PRZString texto = getComponent().asString() + " REQ Tx. TIME=";
            texto += PRZString(time) + " Out =" +PRZString(msgTx->getRoutingPort()) +" # " + msgTx->asString();
            PRZWRITE2LOG( texto );
         #endif
      }
      else
      {
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Again_createMessageRequest);
            getEventQueue().enqueue(event,delay);
      }   
   }
   else
   {
               setState(WaitingHeader);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: outputWriting()
{     
   outputInterfaz()->clearData();
   
   uTIME timeStamp;
   uTIME currentTime = getOwnerRouter().getCurrentTime();
   while( getEventQueue().firstElementTimeStamp(timeStamp) )
   {
      if( timeStamp <= currentTime )
      {
         static PRZEvent event;
         getEventQueue().dequeue(event);
         dispatchEvent(event);
      }
      else break;
   }

   return true;
 
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: dispatchEvent(const PRZEvent& event)
{
   if(event.type() == Again_createMessageRequest)
   {
     PRZMessage* msg;
         if( this->createMessageRequest(&msg) )
         {
            outputInterfaz()->sendData(msg);
            //setRegisterTime();
                        
            uTIME delay = getOwnerRouter().getCurrentTime() + 2;
            
            
            PRZEvent event(Routing_CheckCrossbarResponse);
            getEventQueue().enqueue(event,delay);
         
            #ifndef NO_TRAZA
                uTIME time = getOwnerRouter().getCurrentTime();
                PRZString texto = getComponent().asString() + " REQ2 Tx. TIME=";
                texto += PRZString(time) + " Out =" + PRZString(msg->getRoutingPort()) +" # " + msg->asString();
                PRZWRITE2LOG( texto );
            #endif
         } 
         else
         {
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Again_createMessageRequest);
            getEventQueue().enqueue(event,delay);
         } 
   }
   else if( event.type() == Routing_CheckCrossbarResponse )
   {
      if( outputInterfaz() -> isStopActive() )
      {
         // Canal cerrado, hay que realizar otra solicitud.

         PRZMessage* msg;
         if( this->createMessageRequest(&msg) )
         {
            outputInterfaz()->sendData(msg);
            //setRegisterTime();
                        
            uTIME delay = getOwnerRouter().getCurrentTime() + 2;
            
            
            PRZEvent event(Routing_CheckCrossbarResponse);
            getEventQueue().enqueue(event,delay);
         
            #ifndef NO_TRAZA
                uTIME time = getOwnerRouter().getCurrentTime();
                PRZString texto = getComponent().asString() + " REQ2 Tx. TIME=";
                texto += PRZString(time) + " Out =" + PRZString(msg->getRoutingPort()) +" # " + msg->asString();
                PRZWRITE2LOG( texto );
            #endif
         } 
         else
         {
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Again_createMessageRequest);
            getEventQueue().enqueue(event,delay);
         }
      }
      else
      {
         #ifndef NO_TRAZA
            PRZString texto = getComponent().asString() + " REQ Accept";
            PRZWRITE2LOG(texto);
         #endif
         
         // Canal abierto, se puede transmitir.
         setState(DataTx);
         dataTransmit();
      }      
   }
   else if( event.type() == Routing_HeaderTx )
   {
      dataTransmit();
   }
   else if( event.type() == Routing_InputStopped )
   {
      PRZMessage* msg = event.message();
      updateMessageInfo(msg);
      outputInterfaz()->sendData(msg);
      inputInterfaz()->clearStop();
   }
   else
   {
      PRZFlow::dispatchEvent(event);
   }
    
   return true;
}


//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: createMessageRequest(PRZMessage** pMsg)
{
   
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);
   
   PRZMessage* header = m_FlitsRx.element(1);
   if( (header->deltaAbs(0)==1) && (header->deltaAbs(1)==1) ) 
   {
         // Va hacia el consumo. En el caso de multiples consumidores,
         // indicaremos en el canal el numero de consumidor al que debe
         // ir ( primero, ..., n-esimo).
         msg->setRoutingPort(_LocalNode_);
         msg->setChannel( getCurrentLocalPort() );
         return true;
   }

   do
   {
      if( m_RequestState == 1 )
      {
         m_CurrentOut = getHeaderX();
         m_RequestState = 2;
         int delta    = m_FlitsRx.element(1)->delta(getHeaderX());
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(getHeaderX());
         if( deltaAbs > 1 )
         {
            PRZWRITE2LOG(getComponent().asString() + " X ADAP");
            msg->setRoutingPort(getDirectRoutingType(delta));
            msg->setChannel(1);
            break;
         }
      }
   
      if( m_RequestState == 2 )
      {
         m_CurrentOut = getHeaderY();      
         m_RequestState = 3;
         int delta    = m_FlitsRx.element(1)->delta(getHeaderY());
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(getHeaderY());
         if( deltaAbs > 1 )
         {
            PRZWRITE2LOG(getComponent().asString() + " Y ADAP");         
            msg->setRoutingPort(getOppositeRoutingType(delta));
            msg->setChannel(1);
            break;
         }
      }
   
      if( m_RequestState == 3 )
      {
         // Solicitud DOR. Se intenta primero por la X si es que hay
         // "delta" en esa direccion. Si el routing esta en un canal
         // X, el primer flit que se ha recibido es la cabecera X, y
         // si esta en un canal Y, es la cabecera Y.
         m_CurrentOut = 0;         
         m_RequestState = 4;
         int delta    = m_FlitsRx.element(1)->delta(0);
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(0);
         if( deltaAbs > 1 ) 
         {
            // En el caso de realizar una peticion DOR, la siguiente
            // sera adaptativa, por lo que se pasa al estado 1.
            m_RequestState = 1;
            PRZWRITE2LOG(getComponent().asString() + " X "+ PRZString(delta) );
            if( controlAlgoritm(true,delta) )
            { 
               PRZWRITE2LOG(getComponent().asString() + " X DOR");
               msg->setRoutingPort( (delta>0) ? _Xplus_ : _Xminus_ );
               msg->setChannel(2);
               break;
            }
            else
            {
               PRZWRITE2LOG(getComponent().asString() + " Falla XDOR");            
               return false;
            }
         }
      }
   
      if( m_RequestState == 4 )
      {
         // DOR: hay que comprobar si existe la burbuja
         m_CurrentOut = 1;         
         m_RequestState = 1;
         int delta    = m_FlitsRx.element(1)->delta(1);
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(1);
         if( deltaAbs > 1 ) 
         {
            if( controlAlgoritm(false,delta) )
            { 
               PRZWRITE2LOG(getComponent().asString() + " Y DOR");
               msg->setRoutingPort( (delta>0) ? _Yplus_ : _Yminus_ );
               msg->setChannel(2);
               break;
            }
            else
            {
               PRZWRITE2LOG(getComponent().asString() + " Falla YDOR");
               return false;
            }
         }
      }
   } 
   while(1);


   return true;
}


//*************************************************************************
//:
//  f: void dataTransmit();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleH1Flow :: dataTransmit()
{
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      PRZMessage* msg;
      msg = m_FlitsRx.element(1);
      updateMessageInfo(msg);
      outputInterfaz()->sendDataRightNow(msg);
      setRegisterTime();
      
      #ifndef NO_TRAZA
         uTIME delay = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + " Flit Tx. TIME=";
         texto += PRZString(delay) + " # " + msg->asString();
         PRZWRITE2LOG( texto );
      #endif

      inputInterfaz()->clearStop();
      
      #ifndef NO_TRAZA
         texto = getComponent().asString() + " Clear Stop Tx. TIME=";
         texto += PRZString(delay) ;
         PRZWRITE2LOG( texto );
      #endif

      setState(DataTx);
      m_FlitsRx.remove(1);
   }
   else
   {
      PRZMessage* msg;
      inputInterfaz()->getData(&msg);
      uTIME time = getOwnerRouter().getCurrentTime();
      if( getRegisterTime() == time )
      {
         PRZEvent event(Routing_InputStopped,msg);
         getEventQueue().enqueue(event,time+1);
         inputInterfaz()->sendStopRightNow();
         return;
      }
      updateMessageInfo(msg);
      outputInterfaz()->sendDataRightNow(msg);
      setRegisterTime();
      
      #ifndef NO_TRAZA
         uTIME delay = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + " Flit Tx. TIME=";
         texto += PRZString(delay) + " # " + msg->asString();
         PRZWRITE2LOG( texto );
      #endif
            
      if( msg->isTail() ) initializeRouting();
   }
}


//*************************************************************************
//:
//  f: przROUTINGTYPE getOppositeRoutingType(int delta) const;
//
//  d:
//:
//*************************************************************************

przROUTINGTYPE PRZRoutingAdaptativeBurbleH1Flow :: getOppositeRoutingType(int delta) const
{
   przROUTINGTYPE rType = getComponent().getRoutingDirection();
   przROUTINGTYPE rc;
   
   switch(rType)
   {
      case _Xplus_ : 
      case _Xminus_ :
      case _LocalNode_ :
           if( delta > 0 ) rc = _Yplus_;
           else rc = _Yminus_;
           break;
      case _Yplus_  :
      case _Yminus_ :
           if( delta > 0 ) rc = _Xplus_;
           else rc = _Xminus_;
           break;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: przROUTINGTYPE getDirectRoutingType(int delta) const;
//
//  d:
//:
//*************************************************************************

przROUTINGTYPE PRZRoutingAdaptativeBurbleH1Flow :: getDirectRoutingType(int delta) const
{
   przROUTINGTYPE rType = getComponent().getRoutingDirection();
   przROUTINGTYPE rc;
   
   switch(rType)
   {
      case _Xplus_  : 
      case _Xminus_ :
      case _Yplus_  :
      case _Yminus_ :
           rc = rType;
           break;
           
      case _LocalNode_ :
           if( delta > 0 ) rc = _Xplus_;
           else rc = _Xminus_;
           break;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean salidaX, int delta);
//
//  d: Este metodo se utiliza para verificar la condicion de la burbuja. Se
//     le llama siempre que el canal de salida elegido sea del tipo DOR;
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1Flow :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
   // El valor de "salidaX" es TRUE cuando se sale por un canal X, y FALSE en
   // caso de salir por un canal Y.

   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   przROUTINGTYPE newDirection;

   if(salidaX) newDirection = (delta>0) ? _Xplus_ : _Xminus_;
   else newDirection = (delta>0) ? _Yplus_ : _Yminus_;
   
   if( myDirection == _LocalNode_ )
   {                  
      // El Routing es de un canal de inyeccion, por lo que hay que verificar
      // que existe la condicion de la burbuja en cualquier caso.
      
      switch( newDirection )
      {
         case _Xplus_  : fifo = m_BufferXp; break;
         case _Xminus_ : fifo = m_BufferXm; break;
         case _Yplus_  : fifo = m_BufferYp; break;
         case _Yminus_ : fifo = m_BufferYm; break;         
      }
      
   }
   
   else if( getComponent().getChannel() == 1 )  // Canal adaptativo -> DOR
   {
      // En este caso, se esta pasando de un canal Adaptativo a un canal DOR, por
      // lo que sebemos verificar que se cumple la condicion de la burbuja.
      
      switch( newDirection )
      {
         case _Xplus_  : fifo = m_BufferXp; break;
         case _Xminus_ : fifo = m_BufferXm; break;
         case _Yplus_  : fifo = m_BufferYp; break;
         case _Yminus_ : fifo = m_BufferYm; break;         
      }
   }
   
   else   // Canal DOR -> DOR.
   {
      if( myDirection == newDirection ) return true;
      
      // Cuando estamos pasando de un canal DOR a otro DOR, solo debe verificarse la
      // condicion de la burbuja en el caso de que realicemos un cambio de dimension,
      // esto es, "myDirection" difiere de "newDirection".
      
      switch( newDirection )
      {
         case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Yplus_  : fifo = m_BufferYp; break;
         case _Yminus_ : fifo = m_BufferYm; break;
      }      
   }

   return fifo->bubbleReady();
}


//*************************************************************************
//:
//  f: unsigned getCurrentLocalPort();
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdaptativeBurbleH1Flow :: getCurrentLocalPort()
{
   unsigned lports = ((PRZSimulation*)(getComponent().getSimulation()))->
                     globalData((PRZNetwork*)getOwnerRouter().getOwner()).crossbarNumberOfLocalPorts();
                     
   if( m_CurrentLocalPort >= lports )
      m_CurrentLocalPort = 0;
      
   return ++m_CurrentLocalPort;
}


//*************************************************************************
//:
//  f: unsigned getHeaderX() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdaptativeBurbleH1Flow :: getHeaderX() const
{
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   if( (myDirection == _Xplus_)  ||
       (myDirection == _Xminus_) ||
       (myDirection == _LocalNode_) )
   {
      return 0;
   }

   return 1;
}


//*************************************************************************
//:
//  f: unsigned getHeaderY() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdaptativeBurbleH1Flow :: getHeaderY() const
{
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   if( (myDirection == _Yplus_)  ||
       (myDirection == _Yminus_) )
   {
      return 0;
   }

   return 1;
}


//*************************************************************************
//:
//  f: PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRoutingAdaptativeBurbleH1Flow :: getStatus() const
{
   PRZString rs = getComponent().asString();

   if( inputInterfaz()->isStopActive() ) rs += PRZString("\t IN stop\t");
   else rs += PRZString("\t IN clear\t");

   if( inputInterfaz()->isStopActive() ) rs += PRZString("OUT stop");
   else rs += PRZString("OUT clear");
   
   rs += PRZString("\t State=") + PRZString(getState()) + "\t Head=" + 
         PRZString(m_FlitsRx.numberOfElements());

   TEventQueue::Cursor cursor(getEventQueue());
   forCursor(cursor)
      rs += PRZString(". Event=") + PRZString(cursor.element().element().type());
   return rs;
}

//*************************************************************************


// fin del fichero
