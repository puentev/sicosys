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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMuxed.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleFlowMuxed
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMuxed.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMuxed.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleFlowMuxed.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZRoutingMuxed_HPP__
#include <PRZRoutingMuxed.hpp>
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

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleFlowMuxed,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleFlowMuxed(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleFlowMuxed :: PRZRoutingAdaptativeBurbleFlowMuxed( PRZComponent& component)
                          : PRZRoutingFlow(component),
                            m_CurrentOut(0),
                            m_BufferXp(0),
                            m_BufferXm(0),
                            m_BufferYp(0),
                            m_BufferYm(0),
                            m_CurrentLocalPort(0),
                            m_FirstMessage(true)
{  
   m_inputs= ((PRZRoutingMuxed&)getComponent()).numberOfInputs();
   m_FlitsRx =new TMessagesQueue[m_inputs];   
   m_listRequest_boolean=new Boolean[m_inputs];
   m_RequestState=new unsigned[m_inputs];
   unsigned i;
   for(i=1;i<=m_inputs;i++)
       m_RequestState[i-1] = 1;
      
   for(i=1;i<=m_inputs;i++)
      m_listRequest_boolean[i-1]=false;
   
   setState(Free);
}

PRZRoutingAdaptativeBurbleFlowMuxed :: ~PRZRoutingAdaptativeBurbleFlowMuxed()
{
   delete[] m_FlitsRx;
   delete[] m_listRequest_boolean;
   delete[] m_RequestState;
}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMuxed :: postInitialize()
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

void PRZRoutingAdaptativeBurbleFlowMuxed :: initializeRouting()
{  
   unsigned i;
   setState(Free);
   for(i=1;i<=m_inputs;i++)
   m_RequestState[i-1] = 1;
   for(i=1;i<=m_inputs;i++)
      m_listRequest_boolean[i-1]=false;
}


//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: updateMessageInfo(PRZMessage* msg)
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: onStopDown(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: Ya adaptada a multiples entradas
//		 Tengo dudas sobre a que se refiere el estado (salida asumo)
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   inputInterfaz(interfaz)->getData(&m_Message);    
   
   #ifndef NO_TRAZA
      uTIME delay = getOwnerRouter().getCurrentTime();
      PRZString texto = getComponent().asString() + " Flit Rx. TIME = ";
      texto += PRZString(delay) + " # " + m_Message->asString() + " Interfaz =" + PRZString(interfaz);
      PRZWRITE2LOG( texto );
   #endif
   
   if( getState() == Free )
   {
      addHeaderReceived(m_Message,interfaz);
   }

   else if( getState() == WaitingHeader && interfaz == m_interfazActual)
   {
      addHeaderReceived(m_Message,interfaz);
   }

   else if( getState() == DataTx && interfaz == m_interfazActual)
   {
      dataTransmit();
   }

   else if( getState() == WaitCrossbarRx && interfaz == m_interfazActual )
   {
      //Cada uno de los canales es manejado de forma independiente
      addHeaderReceived(m_Message,interfaz);
   }
   else     //Tengo dudas al respecto: se supone que el mensaje correponde a alguna de las cabezas de 
   {
      if(m_listRequest_boolean[interfaz-1]==false)
      {
         //cerr<<getComponent().asString()<<m_listRequest.numberOfElements()<<" "<<interfaz<<endl;
         m_listRequest.addAsLast(interfaz);
         addHeaderReceived(m_Message,interfaz);
         m_listRequest_boolean[interfaz-1]=true;
      }
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: onReadyDown(unsigned interfaz, unsigned cv)
{  
   return true;
}


//*************************************************************************
//:
//  f: void addHeaderReceived(const PRZMessage& msg);
//
//  d: comtemplado varios interfaces de entrada
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMuxed :: addHeaderReceived(
                                                PRZMessage* msg, 
      													   unsigned interfaz)
{
   m_FlitsRx[interfaz-1].add(msg);
   
   if( m_FlitsRx[interfaz-1].numberOfElements() == 1 )
   {
      inputInterfaz(interfaz)->sendStopRightNow();    
      PRZMessage *msgTx;
      if(getState()==Free && m_listRequest.numberOfElements()==0)
      { 
         m_interfazActual=interfaz;       
         if( createMessageRequest(&msgTx,interfaz) )
         {
            outputInterfaz()->sendDataRightNow(msgTx); //sfs

            uTIME delay = getOwnerRouter().getCurrentTime() + 3;
            PRZEvent event(Routing_CheckCrossbarResponse,msg);
            getEventQueue().enqueue(event,delay);

            setState(WaitCrossbarRx);

            #ifndef NO_TRAZA
               uTIME time = getOwnerRouter().getCurrentTime();
               PRZString texto = getComponent().asString() + " REQ1 Tx. TIME=";
               texto += PRZString(time) + " # " + msgTx->asString() + " Interfaz ="+PRZString(interfaz);
               PRZWRITE2LOG( texto );
            #endif
         }
          else
         {
            setState(WaitingHeader);
         }
      }
   
   }
   
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d: 
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: outputWriting()
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: dispatchEvent(const PRZEvent& event)
{
   if( event.type() == Routing_CheckCrossbarResponse )
   {
      if( outputInterfaz() -> isStopActive() )
      {
         // Canal cerrado, hay que realizar otra solicitud.

         //
         //   Tengo que aplicar el round robin aki. En teoria chequeo hasta el ultimo estado del interfaz 
         //   de entrada y de no encontrar respuesta satisfactoria por parte del crossbar paso al siguiente

         if(m_RequestState[m_interfazActual-1] ==5
               && m_listRequest.numberOfElements()!=0) //ME he saltado todos los estados en el interfaz actual
            												  //luego tengo que saltar al siguiente valido
         {
            m_listRequest.addAsLast(m_interfazActual);
            m_listRequest.firstElement(m_interfazActual);
            m_listRequest.removeAt(0);
            m_RequestState[m_interfazActual-1]=1;
            //uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            //PRZEvent event(Routing_SendNextRequest);
            //getEventQueue().enqueue(event,delay);
            //return true;
         }
        
         
         PRZMessage *msg;
         if(createMessageRequest(&msg,m_interfazActual) )
         {
            #ifndef NO_TRAZA
               PRZString texto = getComponent().asString() + " REQ2 : " + 
                                 msg->asString();
               PRZWRITE2LOG(texto);
            #endif
            outputInterfaz()->sendData(msg);
            //setRegisterTime();
                        
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            
            
            PRZEvent event(Routing_CheckCrossbarResponse);
            getEventQueue().enqueue(event,delay);
         

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
            PRZString texto = getComponent().asString() + " REQ1 Accept";
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
      PRZMessage *msg = event.message();
      updateMessageInfo(msg);
      outputInterfaz()->sendData(msg);
      inputInterfaz(m_interfazActual)->clearStop();
   }
   else if( event.type() == Routing_SendNextRequest)
   {
         PRZMessage *msgTx;
         if( createMessageRequest(&msgTx,m_interfazActual) )
         {
            outputInterfaz()->sendDataRightNow(msgTx); //sfs
            #ifndef NO_TRAZA
               uTIME time = getOwnerRouter().getCurrentTime();
               PRZString texto = getComponent().asString() + " REQ3 Tx. TIME=";
               texto += PRZString(time) + " # " + msgTx->asString() + " DTX Interfaz ="+PRZString(m_interfazActual);
               PRZWRITE2LOG( texto );
            #endif

            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Routing_CheckCrossbarResponse,msgTx);
            getEventQueue().enqueue(event,delay);
            setState(WaitCrossbarRx);


         }
         else
         {
            uTIME delay = getOwnerRouter().getCurrentTime() + 1;
            PRZEvent event(Routing_CheckCrossbarResponse);
            getEventQueue().enqueue(event,delay);
         }
    }
    else if( event.type() == Free_routing)
    {
       //ME cagon Dioz!
       if(m_listRequest.numberOfElements()==0)
       {
         initializeRouting();
       }
       else
       {
         m_listRequest_boolean[m_interfazActual-1]=false;
         m_listRequest.firstElement(m_interfazActual);
         m_listRequest.removeAt(0);
         m_RequestState[m_interfazActual-1]=1;
         
         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Routing_SendNextRequest);
         getEventQueue().enqueue(event,delay);
         #ifndef NO_TRAZA
         PRZString texto = getComponent().asString() + "Go Next Request =" + PRZString(m_interfazActual);
         PRZWRITE2LOG( texto );
         #endif
       }
    }
    else
    {
      PRZFlow::dispatchEvent(event);
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: createMessageRequest(PRZMessage** pMsg, unsigned interfaz)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);
   PRZMessage *header = m_FlitsRx[interfaz-1].element(1);
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
      
      
       if( m_RequestState[interfaz-1] == 1 ||m_RequestState[interfaz-1] == 5)
       {
          m_CurrentOut = getHeaderX();
          m_RequestState[interfaz-1] = 2;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(getHeaderX());
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(getHeaderX());
          if( deltaAbs > 1 )
          {
             PRZWRITE2LOG(getComponent().asString() + " X ADAP");
             msg->setRoutingPort(getDirectRoutingType(delta));
             msg->setChannel(1);
             break;
          }
       }

       if( m_RequestState[interfaz-1] == 2 )
       {
          m_CurrentOut = getHeaderY();      
          m_RequestState[interfaz-1] = 3;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(getHeaderY());
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(getHeaderY());
          
          if( deltaAbs > 1 )
          {
             PRZWRITE2LOG(getComponent().asString() + " Y ADAP");         
             msg->setRoutingPort(getOppositeRoutingType(delta));
             msg->setChannel(1);
             break;
          }
       }

       if( m_RequestState[interfaz-1] == 3 )
       {
          // Solicitud DOR. Se intenta primero por la X si es que hay
          // "delta" en esa direccion. Si el routing esta en un canal
          // X, el primer flit que se ha recibido es la cabecera X, y
          // si esta en un canal Y, es la cabecera Y.
          m_CurrentOut = 0;         
          m_RequestState[interfaz-1] = 4;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(0);
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(0);
          if( deltaAbs > 1 ) 
          {
             // En el caso de realizar una peticion DOR, la siguiente
             // sera adaptativa, por lo que se pasa al estado 1.
             
             //Este se me habia despistao el cabron
             //produce un deadlock al emplear multiplexacion compl
             
             m_RequestState[interfaz-1] = 5;
             
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

       if( m_RequestState[interfaz-1] == 4 )
       {
          // DOR: hay que comprobar si existe la burbuja
          m_CurrentOut = 1;   
          if(m_inputs!=1)      
             m_RequestState[interfaz-1] = 5;  //Condicion de finalizacion en para el dispachevent
          else
             m_RequestState[interfaz-1] = 1;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(1);
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(1);
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
   } while(1);


   return true;
}


//*************************************************************************
//:
//  f: void dataTransmit();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMuxed :: dataTransmit()
{
   if( m_FlitsRx[m_interfazActual-1].numberOfElements() == 1 )
   {
      PRZMessage* msg;
         
      msg = m_FlitsRx[m_interfazActual-1].element(1);
      updateMessageInfo(msg);
      
      #ifndef NO_TRAZA
         uTIME delay = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + " Flit Tx1. TIME=";
         texto += PRZString(delay) + " # " + msg->asString()  + "Interfaz =" + PRZString(m_interfazActual);
         PRZWRITE2LOG( texto );
      #endif
         
      outputInterfaz()->sendDataRightNow(msg);
      setRegisterTime();
      
      

      inputInterfaz(m_interfazActual)->clearStop();
      
      #ifndef NO_TRAZA
         texto = getComponent().asString() + " Clear Stop Tx. TIME=";
         texto += PRZString(delay) ;
         PRZWRITE2LOG( texto );
      #endif

      setState(DataTx);
      m_FlitsRx[m_interfazActual-1].remove(1);
   }
   else
   {
      //Burbujas en el paquete
      PRZMessage *msg;
      
      inputInterfaz(m_interfazActual)->getData(&msg);
      uTIME time = getOwnerRouter().getCurrentTime();
      if( getRegisterTime() == time )
      {
         PRZEvent event(Routing_InputStopped,msg);
         getEventQueue().enqueue(event,time+1);
         inputInterfaz(m_interfazActual)->sendStopRightNow();
         return;
      }
      updateMessageInfo(msg);
      setRegisterTime();
      
      #ifndef NO_TRAZA
         uTIME delay = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + " Flit Tx2. TIME=";
         texto += PRZString(delay) + " # " + msg->asString()+ "Interfaz =" + PRZString(m_interfazActual);
         PRZWRITE2LOG( texto );
      #endif
       outputInterfaz()->sendDataRightNow(msg);

         
      //Hay que liberar recursos del mensajes en la unidad de routing
      //if(m_inputs==1)
     // {
     //    initializeRouting();
     //    return;
     // }   
      if( msg->isTail() && m_listRequest.numberOfElements()!=0)
      {

         //TEngo que reprecesar el resto de cabezas que tengo almacenadas
         //Tardara un ciclo mas
         m_listRequest_boolean[m_interfazActual-1]=false;
         m_listRequest.firstElement(m_interfazActual);
         m_listRequest.removeAt(0);
         m_RequestState[m_interfazActual-1]=1;
         
         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Routing_SendNextRequest);
         getEventQueue().enqueue(event,delay);
         #ifndef NO_TRAZA
         PRZString texto = getComponent().asString() + "Go Next Request =" + PRZString(m_interfazActual);
         PRZWRITE2LOG( texto );
         #endif
            
      }     
      else if( msg->isTail() && m_listRequest.numberOfElements()==0) 
      {
         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Free_routing);
         getEventQueue().enqueue(event,delay);
         
         //initializeRouting();
      }
   }
}


//*************************************************************************
//:
//  f: przROUTINGTYPE getOppositeRoutingType(int delta) const;
//
//  d:
//:
//*************************************************************************

przROUTINGTYPE PRZRoutingAdaptativeBurbleFlowMuxed :: getOppositeRoutingType(int delta) const
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

przROUTINGTYPE PRZRoutingAdaptativeBurbleFlowMuxed :: getDirectRoutingType(int delta) const
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxed :: controlAlgoritm( Boolean salidaX,
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
         case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Yplus_  : fifo = m_BufferYp; break;
         case _Yminus_ : fifo = m_BufferYm; break;
      }      
   }

   return fifo->bubbleReady();
   //return ( fifo->bufferHoles() >= 2*packet ) ? true : false;}
}


//*************************************************************************
//:
//  f: unsigned getCurrentLocalPort();
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdaptativeBurbleFlowMuxed :: getCurrentLocalPort()
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

unsigned PRZRoutingAdaptativeBurbleFlowMuxed :: getHeaderX() const
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

unsigned PRZRoutingAdaptativeBurbleFlowMuxed :: getHeaderY() const
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

PRZString PRZRoutingAdaptativeBurbleFlowMuxed :: getStatus() const
{
   PRZString rs = getComponent().asString();

   if( inputInterfaz()->isStopActive() ) rs += PRZString("\t IN stop\t");
   else rs += PRZString("\t IN clear\t");

   if( inputInterfaz()->isStopActive() ) rs += PRZString("OUT stop");
   else rs += PRZString("OUT clear");
   
   rs += PRZString("\t State=") + PRZString(getState()) + "\t Head=" + 
         PRZString(m_FlitsRx[m_interfazActual-1].numberOfElements());

   TEventQueue::Cursor cursor(getEventQueue());
   forCursor(cursor)
      rs += PRZString(". Event=") + PRZString(cursor.element().element().type());
   return rs;
}

//*************************************************************************


// fin del fichero
