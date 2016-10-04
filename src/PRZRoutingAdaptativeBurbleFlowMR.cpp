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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMR.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleFlowMR
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMR.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMR.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleFlowMR.hpp>

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

#ifndef __PRZRouting_HPP__
#include <PRZRouting.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleFlowMR,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleFlowMR(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleFlowMR :: PRZRoutingAdaptativeBurbleFlowMR( PRZComponent& component)
                            : PRZRoutingFlow(component),
                              m_FlitsRx(1),
                              m_CurrentLocalPort(0),
                              m_FirstMessage(true)
{
   setState(Free);
}

//*************************************************************************
//:
//  f: void postInitialize();
//
//  d: facilta el accceso a los bufferes locales del router para aplicar bubble
//     aniado sobre elñ punto de partida (Routing VC)
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMR :: postInitialize()
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

void PRZRoutingAdaptativeBurbleFlowMR :: initializeRouting()
{  
   setState(Free);
}


//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: updateMessageInfo(PRZMessage* msg)
{
   // Para evitar la abstaccion de la clase. Como el routing no sabe el
   // puerto de salida por el que va a salir el paquete, no puede actua-
   // lizar la cabecera. Esta accion se llevara a cabo en el crossbar.
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: onStopUp(unsigned interfaz, unsigned cv)
{   
   #ifndef NO_TRAZA
            PRZString time = getOwnerRouter().getCurrentTime();       
            PRZString texto = getComponent().asString() + " Stop UP!!!(" +
                              " TIME = "+time;            
           PRZWRITE2LOG( texto );
  #endif
   inputInterfaz()->sendStopRightNow();
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( (getState() == DataTx) || (getState() == Free) )
      inputInterfaz()->clearStopRightNow();
      #ifndef NO_TRAZA
            PRZString time = getOwnerRouter().getCurrentTime();       
            PRZString texto = getComponent().asString() + " Stop DOWN!!!(" +
                              " TIME = "+time;            
           PRZWRITE2LOG( texto );
  #endif
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: onReadyUp(unsigned interfaz, unsigned cv)
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

Boolean PRZRoutingAdaptativeBurbleFlowMR :: onReadyDown(unsigned interfaz, unsigned cv)
{  
   return true;
}


//*************************************************************************
//:
//  f: void addHeaderReceived(const PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMR :: addHeaderReceived(PRZMessage* msg)
{
   m_FlitsRx.add(msg);
   
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      // No se permite recibir mas de 1 flit (cabeceras X e Y dentro de el)
      inputInterfaz()->sendStopRightNow();
      
      PRZMessage* msgTx; 
      if( createMessageRequest(&msgTx) )
      {
         outputInterfaz()->sendDataRightNow(msgTx);
         uTIME delay = getOwnerRouter().getCurrentTime() + getHeaderDelay();
         PRZEvent event(Routing_CheckCrossbarResponse,msg);
         getEventQueue().enqueue(event,delay);
         setState(WaitCrossbarRx);
         
         #ifndef NO_TRAZA
            uTIME time = getOwnerRouter().getCurrentTime();
            PRZString texto = getComponent().asString() + " REQ1 Tx. TIME=";
            texto += PRZString(time) + " # " + msgTx->asString();
            PRZWRITE2LOG( texto );
         #endif
      }
      else
      {
         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Routing_CheckCrossbarResponse,msg);
         getEventQueue().enqueue(event,delay);
         setState(WaitCrossbarRx);
      }
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRCAFL_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: outputWriting()
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

Boolean PRZRoutingAdaptativeBurbleFlowMR :: dispatchEvent(const PRZEvent& event)
{
   if( event.type() == Routing_CheckCrossbarResponse )
   {
      if( outputInterfaz() -> isStopActive() )
      {
         // Canal cerrado, hay que realizar otra solicitud.

         PRZMessage *msg;
         if( createMessageRequest(&msg) )
         {
            outputInterfaz()->sendData(msg);
            
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            
            PRZEvent event(Routing_CheckCrossbarResponse);
            getEventQueue().enqueue(event,delay);
         
            #ifndef NO_TRAZA
               PRZString texto = getComponent().asString() + " REQ2 : " + 
                                 msg->asString();
               PRZWRITE2LOG(texto);
            #endif
         }
         else
         {
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Routing_CheckCrossbarResponse);
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
   else if( event.type() == Routing_InputStopped )
   {
      inputInterfaz()->clearStop();
   }

   else 
   {
      return PRZFlow::dispatchEvent(event);
   }
   return true;
}


//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage** msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::MREQ);
   
   if( m_FlitsRx.numberOfElements() == 0 ) return false;
 
   PRZMessage *headerMsg = m_FlitsRx.element(1);
   
   
   //El criterio de consumo cambia en funcion de si hay miss
   //routing o no (1/1 es consumo sin missrouting y 0/0 con misrouting)
   PRZRouting& routing=(PRZRouting&)(getComponent());
   short missrouting=routing.getMissRouting();
   short testR;
   if(missrouting==0)
   {
      testR=1;
   }
   else
   {
      testR=0;
   }
    
   if( (headerMsg->deltaAbs(0)==testR) && (headerMsg->deltaAbs(1)==testR) ) 
   {
      // Va hacia el consumo. En el caso de multiples consumidores,
      // indicaremos en el canal el numero de consumidor al que debe
      // ir ( primero, ..., n-esimo).
         
      unsigned localPort = getCurrentLocalPort();
      TPortList* portList = new TPortList();
      portList->addAsLast(TUPair(_LocalNode_,localPort));
      msg->setPortList(portList);
      return true;
    }

   int deltaX = headerMsg->delta(0);
   int deltaY = headerMsg->delta(1);
   
   //
   //Miss routing Support
   //
   int missRoutingX = headerMsg->getMissRouting(0);
   int missRoutingY = headerMsg->getMissRouting(1);
   
   
         
   unsigned absDeltaX = headerMsg->deltaAbs(0);
   unsigned absDeltaY = headerMsg->deltaAbs(1);

   TPortList* portList = new TPortList();
   msg->setPortList(portList);
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

//    if( absDeltaX > 1 )
//    {
//       // Siempre se intenta sacar por el canal adaptativo X, independientemente
//       // de si nos encontramos en un canal X o Y.
//       portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
//    }
//    if( absDeltaY > 1 )
//    {
//       // La segunda prioridad es mandar el mensaje por el canal adaptativo Y.
//       portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
//    }   
//    funcion de seleccion X_Y de verdad

//Orden normal
  if(!msg->getLastMissRouted())
  {     
    if(myDirection==_Xminus_ || myDirection==_Xplus_ ||myDirection==_LocalNode_){
       if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
      }
      else{
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
        if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
      }
   }
   else
      //
      //Se da la vuelta el orden de las peticiones
      //si no la cagamos 
      //El paquete vuelve por el mismo camino
      //
   {
      if(myDirection==_Yminus_ || myDirection==_Yplus_){
       if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
      }
      else{
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
        if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
      }
      
   }
            
   
      if( absDeltaX > testR )
      {
         if( controlAlgoritm(true,deltaX) ){
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),2)); 
         }     
      }
      else if( (absDeltaX == testR)  && (absDeltaY > testR) )
      {
         if( controlAlgoritm(false,deltaY) ){ 
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),2));
         }

      }
   
   
   //------------------------------------------------------
   //--------------==|Miss routing block|==----------------
   //Only Adaptive channels accept miss Routed Messages
   //Last option in Selection Function.
   //------------------------------------------------------
   
    if(myDirection==_Xminus_ || myDirection==_Xplus_ || myDirection==_LocalNode_)
    { 
        //Primero se realiza el Missrouting en la dimension superior a la que nos encontramos
        if(missRoutingY!=0)
        {
           if( absDeltaY!=0)
           {
               //portList->addAsLast(TUPair(((deltaY>0)?_Yminus_:_Yplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Yplus_,1));
                     portList->addAsLast(TUPair(_Yminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Yminus_,1));
                     portList->addAsLast(TUPair(_Yplus_,1));

                 }
           }
        }
        
        //No hago missrouting hacia atras
        if(missRoutingX!=0 )
        {
           if( absDeltaX != 0  )
           {
               //portList->addAsLast(TUPair(((deltaX>0)?_Xminus_:_Xplus_),1));
           }
           else
           {
                 if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Xplus_,1));
                     portList->addAsLast(TUPair(_Xminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Xminus_,1));
                     portList->addAsLast(TUPair(_Xplus_,1));

                 }
           }  
        }
      }
      else
      {
         //Estamos en una Y=> primero missrouting por X y despues por Y
         
         if(missRoutingX!=0)
        {
           if( absDeltaX != 0 )
           {
               //portList->addAsLast(TUPair(((deltaX>0)?_Xminus_:_Xplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Xplus_,1));
                     portList->addAsLast(TUPair(_Xminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Xminus_,1));
                     portList->addAsLast(TUPair(_Xplus_,1));

                 }
           }  
        } 

        if(missRoutingY!=0 )
        {
           if( absDeltaY != 0 )
           {
               //portList->addAsLast(TUPair(((deltaY>0)?_Yminus_:_Yplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Yplus_,1));
                     portList->addAsLast(TUPair(_Yminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Yminus_,1));
                     portList->addAsLast(TUPair(_Yplus_,1));

                 }
           }
        }
        
        
      }
   
   //-------------------------------------------------------------------
   //-------------------------------------------------------------------   
   

   #ifndef NO_TRAZA
      if( portList->numberOfElements() != 0)
      {
         TPortList::Cursor cursor(*portList);
         PRZString text = getComponent().asString() + "REQ: ";
         forCursor(cursor)
         {
            text += _routingType2string(przROUTINGTYPE(cursor.element().left())) +
                    "(" + PRZString(cursor.element().right()) + "), ";
         }
         PRZWRITE2LOG(text);
      }
      else
      {
         PRZString text = getComponent().asString() + "REQ: Nothing";
         PRZWRITE2LOG(text);   
      }
   #endif
   
   return true;
}


//*************************************************************************
//:
//  f: void dataTransmit();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMR :: dataTransmit()
{
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      PRZMessage *msg = m_FlitsRx.element(1);
      
      outputInterfaz()->sendDataRightNow(msg);
      //inputInterfaz()->clearStop();
      uTIME time = getOwnerRouter().getCurrentTime();
               
      #ifndef NO_TRAZA

         PRZString texto = getComponent().asString() + " Flit Tx. TIME=";
         texto += PRZString(time) + " # " + msg->asString();
         PRZWRITE2LOG( texto );
      #endif
      
      m_FlitsRx.remove(1);
      
      PRZEvent event(Routing_InputStopped,msg);
      getEventQueue().enqueue(event,time+1);
   }
   else
   {
      PRZMessage* msg;
      inputInterfaz()->getData(&msg);
      outputInterfaz()->sendDataRightNow(msg);
      
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
//  f: virtual Boolean controlAlgoritm(Boolean salidaX, int delta);
//
//  d: Este metodo se utiliza para verificar la condicion de la burbuja. Se
//     le llama siempre que el canal de salida elegido sea del tipo DOR;
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMR :: controlAlgoritm( Boolean salidaX,
                                                           int delta )
{
   // El valor de "salidaX" es TRUE cuando se sale por un canal X, y FALSE en
   // caso de salir por un canal Y.

   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   przROUTINGTYPE newDirection;

   if(salidaX) newDirection = (delta>0) ? _Xplus_ : _Xminus_;
   else newDirection = (delta>0) ? _Yplus_ : _Yminus_;
   
   unsigned packet = ((PRZSimulation*)(getComponent().getSimulation()))->
                     getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());

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
//#ifndef CP
        case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+"Error de direccionamiento en el Routing DOR adaptativo");
        case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+"Error de direccionamiento en el Routing DOR adaptativo");
// #else
//          case _Xplus_  :fifo = m_BufferXp; break;
//          case _Xminus_ : fifo = m_BufferXp; break;
// #endif        
         case _Yplus_  : fifo = m_BufferYp; break;
         case _Yminus_ : fifo = m_BufferYm; break;
      }     
   }
    
   return fifo->bubbleReady();
   //return ( fifo->bufferHoles() >= 2*packet ) ? true : false;
}



//*************************************************************************
//:
//  f: unsigned getCurrentLocalPort();
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdaptativeBurbleFlowMR :: getCurrentLocalPort()
{
   unsigned lports = PRZSIMULATION->globalData((PRZNetwork*)getOwnerRouter().getOwner()).crossbarNumberOfLocalPorts();
   if( m_CurrentLocalPort >= lports ) m_CurrentLocalPort = 0;
   return ++m_CurrentLocalPort;
}


//*************************************************************************


// fin del fichero
