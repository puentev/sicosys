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
// File: ../ATCSimul/src/PRZRoutingAdapCV1RequestFlow.cpp
//
// Clase      : PRZRoutingAdapCV1RequestFlow
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdapCV1RequestFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdapCV1RequestFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdapCV1RequestFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdapCV1RequestFlow,PRZRoutingFlow);


//*************************************************************************
//:
//  f: PRZRoutingAdapCV1RequestFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdapCV1RequestFlow :: PRZRoutingAdapCV1RequestFlow( PRZComponent& component)
                            : PRZRoutingFlow(component),
                              m_FlitsRx(1),
                              m_CurrentLocalPort(0)
{
   setState(Free);
}


//*************************************************************************
//:
//  f: void initializeRouting();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdapCV1RequestFlow :: initializeRouting()
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

Boolean PRZRoutingAdapCV1RequestFlow :: updateMessageInfo(PRZMessage* msg)
{
   // Para evitar la abstaccion de la clase. Como el routing no sabe el
   // puerto de salida por el que va a salir el paquete, no puede actua-
   // lizar la cabecera. Esta accion se llevara a cabo en el crossbar.
   if( getState() != WaitCrossbarRx ) outputInterfaz()->clearDataRightNow();
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdapCV1RequestFlow :: onStopUp(unsigned interfaz, unsigned cv)
{   
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

Boolean PRZRoutingAdapCV1RequestFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( (getState() == DataTx) || (getState() == Free) )
   {
      inputInterfaz()->clearStopRightNow();
   }
   else if (getState() == WaitCrossbarRx )
   {
   	PRZWRITE2LOG(getComponent().asString() + " El CROSSBAR concede paso");
      dataTransmit();
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//
//*************************************************************************

Boolean PRZRoutingAdapCV1RequestFlow ::  onReadyDown(unsigned interfaz, unsigned cv)
{
   if( getState() != WaitCrossbarRx ) outputInterfaz()->clearDataRightNow();
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdapCV1RequestFlow :: onReadyUp(unsigned interfaz, unsigned cv)
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
//  f: void addHeaderReceived(const PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdapCV1RequestFlow :: addHeaderReceived(PRZMessage* msg)
{
   m_FlitsRx.add(msg);
   
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      PRZMessage* msgTx; 
      if( createMessageRequest(&msgTx) )
      {
         outputInterfaz()->clearData();
         outputInterfaz()->sendData(msgTx);
         setState(WaitCrossbarRx);
         
         #ifndef NO_TRAZA
            uTIME time = getOwnerRouter().getCurrentTime();
            PRZString texto = getComponent().asString() + " REQ1 Tx. TIME=";
            texto += PRZString(time) + " # " + msgTx->asString();
            PRZWRITE2LOG( texto );
         #endif
      }
      
      // No se permite recibir mas de 1 flit (cabeceras X e Y dentro de el)
      inputInterfaz()->sendStopRightNow();

      /* Este Routing seimpre va a pedir los puertos una sola vez, por lo que es seguro
         que existen solicitudes y este "else" no es necesario
      else
      {
         uTIME delay = getOwnerRouter().getCurrentTime() + 1;
         PRZEvent event(Routing_CheckCrossbarResponse,msg);
         getEventQueue().enqueue(event,delay);
         setState(WaitCrossbarRx);
      }
      */
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
//  f: Boolean createMessageRequest(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdapCV1RequestFlow :: createMessageRequest(PRZMessage** pMsg)
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
 
   PRZMessage* headerMsg = m_FlitsRx.element(1);
   
   if( (headerMsg->deltaAbs(0)==1) && (headerMsg->deltaAbs(1)==1) ) 
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
   
   unsigned absDeltaX = headerMsg->deltaAbs(0);
   unsigned absDeltaY = headerMsg->deltaAbs(1);
   
   
   TPortList* portList = new TPortList();
   msg->setPortList(portList);
   
   if( absDeltaX > 1 )
   {
      // Siempre se intenta sacar por el canal adaptativo X, independientemente
      // de si nos encontramos en un canal X o Y.
      portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
   }
   if( absDeltaY > 1 )
   {
      // La segunda prioridad es mandar el mensaje por el canal adaptativo Y.
      portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
   }
            
   if( getComponent().getChannel() == 1 )   // Canal adaptativo
   {
      // Cambios de canal adaptativo a canal H o L : Debe cumplirse siempre
      // la condicion del encaminamiento DOR, esto es, solo se puede avanzar 
      // por el eje de las Y's cuando absDeltaX == 1 (componente X nula).
      
      if( absDeltaX > 1 )
      {
         portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),2));
         if( controlAlgoritm(true,deltaX) )
         {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),3));
         }
      }
      else if( (absDeltaX == 1)  && (absDeltaY > 1) )
      {
         portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),2));
         if( controlAlgoritm(false,deltaY) )
         {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),3));
         }
      }
   }
   else if( getComponent().getChannel() == 2 )   // Canal H
   {
      // Pasos de canal H a canal H o L.
      if( absDeltaX > 1 )
      {
         portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),2));      
         if( controlAlgoritm(true,deltaX) )
         {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),3));
         }
      }
      else if( (absDeltaX == 1)  && (absDeltaY > 1) )
      {
         portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),2));
         if( controlAlgoritm(false,deltaY) )
         {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),3));
         }
      }
   }
   else if( getComponent().getChannel() == 3 )   // Canal L
   {
      // pasos de canal L a L o H. Es mas prioritario seguir por un canal L
      // en lugar de cambiarse al canal H. Esto es asi debido a que los canales
      // L van a estar siempre mas vacios que los canales H, y de esta forma
      // se contribuye a que se usen mas.
      
      if( absDeltaX > 1 )
      {
         portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),3));
         portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),2));
      }
      else if( (absDeltaX == 1) && (absDeltaY > 1) )
      {
         portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),2));
         if( controlAlgoritm(false,deltaY) )
         {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),3));
         }
      }
   }

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

void PRZRoutingAdapCV1RequestFlow :: dataTransmit()
{
   if( m_FlitsRx.numberOfElements() == 1 )
   {
      PRZMessage* msg = m_FlitsRx.element(1);
      outputInterfaz()->sendDataRightNow(msg);
      uTIME time = getOwnerRouter().getCurrentTime();
               
      #ifndef NO_TRAZA

         PRZString texto = getComponent().asString() + " Flit Tx. TIME=";
         texto += PRZString(time) + " # " + msg->asString();
         PRZWRITE2LOG( texto );
      #endif
      
      m_FlitsRx.remove(1);
      inputInterfaz()->clearStop();
      setState(DataTx);      
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
//  f: virtual Boolean controlAlgoritm(Boolean canalX, int delta=0);
//
//  d: El control en esta clase comprueba si se puede pasar de un canal H
//     a un canal L. Para ello, basta con mirar si el mensaje atravesara
//     los enlaces perifericos, lo cual se determina sabiendo por que 
//     dimension se esta avanzando (X o Y), asi como la delta de la cabeza
//     del mensaje en esa dimension;
//:
//*************************************************************************

Boolean PRZRoutingAdapCV1RequestFlow :: controlAlgoritm(Boolean canalX, int delta)
{
   int dest = delta + 
              getOwnerRouter().getPosition().
              valueForCoordinate(((canalX)?(PRZPosition::X):(PRZPosition::Y)));
   
   if( (dest > PRZSIMULATION->getRadius()) ||
       (dest < 0 ) )
   {
      // Si se cumple una de estas 2 condiciones, el mensaje atraviesa un
      // enlace de la periferia, por lo que retorna "false".
      return false;
   }
   
   return true; 
   
}


//*************************************************************************
//:
//  f: unsigned getCurrentLocalPort();
//
//  d:
//:
//*************************************************************************

unsigned PRZRoutingAdapCV1RequestFlow :: getCurrentLocalPort()
{
   unsigned lports = PRZSIMULATION->globalData((PRZNetwork*)getOwnerRouter().getOwner()).crossbarNumberOfLocalPorts();
   if( m_CurrentLocalPort >= lports ) m_CurrentLocalPort = 0;
   return ++m_CurrentLocalPort;
}


//*************************************************************************


// fin del fichero
