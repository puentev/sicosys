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
// File: ../ATCSimul/src/PRZInjectorFlow.cpp
//
// Clase      : PRZInjectorFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZInjectorFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZInjectorFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZInjectorFlow.hpp>

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZInjector_HPP__
#include <PRZInjector.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#include <math.h>

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZInjectorFlow,PRZFlow);

//*************************************************************************
//:
//  f: PRZInjectorFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZInjectorFlow :: PRZInjectorFlow( PRZComponent& component)
                 : PRZFlow(component),
                   m_DataToSend(false),
                   m_PacketInjectionTime(0),
                   m_MissRouting(0),
                   m_AlgoritmSend(0)
{
   m_OutputData=0;
}


//*************************************************************************
//:
//  f: virtual ~PRZInjectorFlow();
//
//  d:
//:
//*************************************************************************

PRZInjectorFlow :: ~PRZInjectorFlow()
{
   PRZMessageQueue::Cursor cursor(m_MessageQueue);
   forCursor(cursor)
   {
      delete cursor.element();
   }

   delete m_OutputData;
   delete m_AlgoritmSend;
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZInjectorFlow :: inputReading()
{
	return true;      
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean PRZInjectorFlow :: stateChange()
{      
   return true;
}


//*************************************************************************
//:
//  f: AlgoritmSendHeader* getAlgoritmToSend();
//
//  d:
//:
//*************************************************************************

AlgoritmSendHeader* PRZInjectorFlow :: getAlgoritmToSend()
{   
   if( ! m_AlgoritmSend )
   {
//Esto es problematico (hay que meter z=0 en las descripciones 2-D)   
#ifdef  UNIROUTER
      if( PRZNETWORK->getSizeZ() == 1 ) 
      {
         switch( PRZInjector::numberOfHeaders() )
         {
            case 1: m_AlgoritmSend = new AlgoritmSend1Header3D(*this);
               break;
            case 2: EXIT_PROGRAM("Si Z>1, numero de Cabeceras debe ser !=2");
               break;
            case 3: m_AlgoritmSend = new AlgoritmSend3Header(*this);
               break;
         }
      }
      else
      {
         switch( PRZInjector::numberOfHeaders() )
          {
             case 1: m_AlgoritmSend = new AlgoritmSend1Header2D(*this);
                break;
             case 2: m_AlgoritmSend = new AlgoritmSend2Header(*this);
                break;         
             case 3: EXIT_PROGRAM("Si Z=1, numero de Cabeceras debe ser !=3");
                break;
          }
       }
         
#else

       if( PRZNETWORK->getSizeZ() > 1 )   // Estamos en una red 3D
       {
          switch( PRZInjector::numberOfHeaders() )
          {
//              case 1: m_AlgoritmSend = new AlgoritmSend1Header3D(*this);
//                 break;
              case 1: EXIT_PROGRAM("DESHABILITADO");
                 break;
              case 2: EXIT_PROGRAM("Si Z>1, numero de Cabeceras debe ser !=2");
                 break;
//              case 3: m_AlgoritmSend = new AlgoritmSend3Header(*this);
//                 break;
              case 3: EXIT_PROGRAM("DESHABILITADO");
                 break;
          }
       }
       else
       {
          switch( PRZInjector::numberOfHeaders() )
          {
              case 1: m_AlgoritmSend = new AlgoritmSend1Header2D(*this);
                 break;
              case 2: m_AlgoritmSend = new AlgoritmSend2Header(*this);
                 break;         
              case 3: EXIT_PROGRAM("Si Z=1, numero de Cabeceras debe ser !=3");
                 break;
          }
       }
#endif
    }   
    return m_AlgoritmSend;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZInjectorFlow :: outputWriting()
{
   
   outputInterfaz() -> clearData();
   
	if( isAnyMessageToSend() && (!outputInterfaz()->isStopActive()) )
   {
      PRZMessage *msg;
      if(msg = getAlgoritmToSend()->getNextFlitToSend())
         outputInterfaz() -> sendData(msg);
     
        
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
         texto += PRZString(delayTime) + " # " + msg->asString();
         if( msg->isHeader() )
         {
            texto += PRZString("  CABECERA: ") + 
                     msg->source().asString() +
                     " => " + msg->destiny().asString();
         }
         PRZWRITE2LOG( texto );
      #endif
      
   }
	
   return true;
}


//*************************************************************************
//:
//  f: void sendMessage(const PRZMessage& aMessage);
//
//  d:
//:
//*************************************************************************

void PRZInjectorFlow :: sendMessage(PRZMessage* msg)
{
#ifdef UNIROUTER
   unsigned x, y, z;
   //Sola valido para probar traficos sobre un solo router
   switch( getComponent().getRoutingDirection() )
   {
      case _Xplus_ :
         x = msg->destiny().valueForCoordinate(PRZPosition::X);
         y = msg->destiny().valueForCoordinate(PRZPosition::Y);
         z = msg->destiny().valueForCoordinate(PRZPosition::Z);              
         msg->setDestiny(PRZPosition(abs(x),y,z));
         break;      
      case _Xminus_:  
         x = msg->destiny().valueForCoordinate(PRZPosition::X);
         y = msg->destiny().valueForCoordinate(PRZPosition::Y);
         z = msg->destiny().valueForCoordinate(PRZPosition::Z);              
         msg->setDestiny(PRZPosition(-abs(x),y,z));
         break; 
      case _Yplus_: 
         y = msg->destiny().valueForCoordinate(PRZPosition::Y);
         z = msg->destiny().valueForCoordinate(PRZPosition::Z);              
         //msg->setDestiny(PRZPosition(abs(y),0,z));
         switch( PRZInjector::numberOfHeaders() )
         {
         case 1: msg->setDestiny(PRZPosition(0,abs(y),z));
                 break;

         case 2: msg->setDestiny(PRZPosition(abs(y),0,z));
                 break;         

         case 3: msg->setDestiny(PRZPosition(abs(y),0,z));
                 break;
         }
         break;
      case _Yminus_:
         y = msg->destiny().valueForCoordinate(PRZPosition::Y);
         z = msg->destiny().valueForCoordinate(PRZPosition::Z);              
         //msg->setDestiny(PRZPosition(-abs(y),0,z));
          switch( PRZInjector::numberOfHeaders() )
         {
         case 1: msg->setDestiny(PRZPosition(0,-abs(y),z));
                 break;

         case 2: msg->setDestiny(PRZPosition(-abs(y),0,z));
                 break;         

         case 3: msg->setDestiny(PRZPosition(-abs(y),0,z));
                 break;
         }
         break;
         //3DSupport for unirouter Networks
      case _Zplus_: //X and Y nulas (respetar Dimensional order routing)
         z = msg->destiny().valueForCoordinate(PRZPosition::Z); 
         switch( PRZInjector::numberOfHeaders() )
         {
         case 1: msg->setDestiny(PRZPosition(0,0,abs(z)));
                 break;

         case 2: msg->setDestiny(PRZPosition(abs(z),0,0));
                 break;         

         case 3: msg->setDestiny(PRZPosition(abs(z),0,0));
                 break;
         }
         break;
      case _Zminus_:
         z = msg->destiny().valueForCoordinate(PRZPosition::Z); 
         switch( PRZInjector::numberOfHeaders() )
         {
         case 1: msg->setDestiny(PRZPosition(0,0,-abs(z)));
                 break;

         case 2: msg->setDestiny(PRZPosition(-abs(z),0,0));
                 break;         

         case 3: msg->setDestiny(PRZPosition(-abs(z),0,0));
                 break;
         }
         break;
      case _LocalNode_: break;
   }
   
#endif 
   m_MessageQueue.enqueue(msg);  

}


//*************************************************************************
//:
//  f: Boolean isAnyMessageToSend() const;
//
//  d:
//:
//*************************************************************************

Boolean PRZInjectorFlow :: isAnyMessageToSend() const
{
   return (m_MessageQueue.numberOfElements() > 0) ? true : false;
}


//*************************************************************************
//:
//  f: unsigned getMessagesToSend() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZInjectorFlow :: getMessagesToSend() const
{
   return m_MessageQueue.numberOfElements(); 
}

//*************************************************************************
//:
//  f: unsigned getAvaliableMessages() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZInjectorFlow :: getAvaliableMessages() const
{
   return getMessagePool().free();
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZInjectorFlow :: controlAlgoritm(Boolean info, int delta)
{
   // Para evitar la abstraccion. No se utiliza en esta clase.
   return true;
}

//*************************************************************************
//*************************************************************************
//*************************************************************************
//:
//  f: Boolean isRequestNext() const;
//  d:
//:
//*************************************************************************

Boolean AlgoritmSendHeader::isRequestNext() const
{   
    PRZMessage* topMessage;
    getMessageQueue().firstElement(topMessage);
    return topMessage->isRequest();
}

//*************************************************************************
//:
//  f: PRZMessage getNextFlitToSend();
//
//  d:
//:
//*************************************************************************

PRZMessage* AlgoritmSend2Header :: getNextFlitToSend()
{
   PRZMessage* topMessage = 0;
   PRZMessage* returnMsg = 0;
   PRZNetwork* net = ((PRZSimulation*)(m_InjectorFlow.getComponent().getSimulation()))->getNetwork();
   unsigned radioX = net->getSizeX();
   unsigned radioY = net->getSizeY();
   unsigned radioZ = net->getSizeZ();
   int xdelta,ydelta,zdelta;

   
   if( getMessageQueue().firstElement(topMessage) )
   {
      if((returnMsg = getMessagePool().allocate())==0)
      {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);
      }

      //si preveo que el mensaje no lo voy a poder sacar entero lo recoloco
      //otra vez en la cola de mensajes listos para enviar...nor!
      //los mensajes solo se borran cuando sale la cola. Con liberar en el pool
      //es suficiente.
       
      if(topMessage->flitNumber() == 1 && 
         getMessagePool().free() < (radioX*radioY*radioZ*topMessage->getMessageLength()*topMessage->getPacketLength()))
      {
         getMessagePool().release(returnMsg);
	      return 0;
      }

      *returnMsg = *topMessage;
               
      if( topMessage->flitNumber() < topMessage->packetSize() )
      {
         // La forma de llevar la cuenta de los flits es la siguiente:
         // Al principio, flitNumber=1 y totalFlit=N. Se va incremen-
         // tando orderNumber a medida que se van solicitando los flits
         // para el envio. Cuando ambas variables son iguales, se saca
         // el mensaje de la cola.
         topMessage->setFlitNumber( (topMessage->flitNumber() +1 ) );
      }
      else
      {
         // Se ha enviado el ultimo flit del paquete, por lo que hay que
         // comprobar si el paquete es el ultimo del mensaje
         
         if( topMessage->packetNumber() < topMessage->messageSize() )
         {
            // Se reestablece la cuenta de flits a 1.
            topMessage->setPacketNumber(( topMessage->packetNumber() +1 ));
            topMessage->setFlitNumber(1);
         }
         else
         {
            getMessageQueue().dequeue(topMessage);
            delete topMessage;
         }
      }
   }
 
	if( returnMsg->flitNumber() == 1 )   // Componente X
	{   
	
      
      // En el primer flit del paquete que sale por el injector, 
      // se debe actualizar esta medida de tiempo para escribirlo
      // correctamente en todos los flits del paquete
      returnMsg->setType(PRZMessage::HEADER);

      net->routingRecord(returnMsg->source(),returnMsg->destiny(),xdelta, ydelta,zdelta);
		returnMsg->setDelta(xdelta);
      setPacketInjectionTime( MAXIMO(m_InjectorFlow.getOwnerRouter().getCurrentTime(),
                                     returnMsg->generationTime()) );
	}
	else if( returnMsg->flitNumber() == 2 )   // Componente Y
	{
	   net->routingRecord(returnMsg->source(),returnMsg->destiny(),xdelta, ydelta,zdelta);
		returnMsg->setDelta(ydelta);
   }
	else if( returnMsg->flitNumber() == returnMsg->packetSize() )
	{
		returnMsg->setType(PRZMessage::TAIL);
	}
	else
	{
		returnMsg->setType(PRZMessage::INFO);
	}
	
	returnMsg->setPacketInjectionTime(getPacketInjectionTime());
   
   return returnMsg;
}

//*************************************************************************
//:
//  f: PRZMessage getNextFlitToSend();
//
//  d:
//:
//*************************************************************************

PRZMessage* AlgoritmSend1Header2D :: getNextFlitToSend()
{
   PRZMessage* topMessage=0;
   PRZMessage* returnMsg=0;
   PRZNetwork* net = ((PRZSimulation*)(m_InjectorFlow.getComponent().getSimulation()))->getNetwork();
   unsigned radioX = net->getSizeX();
   unsigned radioY = net->getSizeY();

   if( getMessageQueue().firstElement(topMessage) )
   {
      //si preveo que el mensaje no lo voy a poder sacar entero paso de todo
      if(topMessage->flitNumber() == 1 &&
         getMessagePool().free() < (radioX*radioY*topMessage->getMessageLength()*topMessage->getPacketLength()))
	   {
	      return 0;
      } 
      if((returnMsg = getMessagePool().allocate())==0)
      {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);
      }
       
      *returnMsg = *topMessage;
      
      if( topMessage->flitNumber() < topMessage->packetSize() )
      {
         // La forma de llevar la cuenta de los flits es la siguiente:
         // Al principio, flitNumber=1 y totalFlit=N. Se va incremen-
         // tando orderNumber a medida que se van solicitando los flits
         // para el envio. Cuando ambas variables son iguales, se saca
         // el mensaje de la cola.
         topMessage->setFlitNumber( (topMessage->flitNumber() +1 ) );
      }
      else
      {
         // Se ha enviado el ultimo flit del paquete, por lo que hay que
         // comprobar si el paquete es el ultimo del mensaje
         
         if( topMessage->packetNumber() < topMessage->messageSize() )
         {
            // Se reestablece la cuenta de flits a 1.
            topMessage->setPacketNumber(( topMessage->packetNumber() +1 ));
            topMessage->setFlitNumber(1);
         }
         else
         {
            getMessageQueue().dequeue(topMessage);
            delete topMessage;
         }
      }
   }

	if( returnMsg->flitNumber() == 1 )  // La informacion de las 2 cabezas va en 1 flit
	{   
      int xdelta,ydelta,zdelta;
      net->routingRecord(returnMsg->source(),returnMsg->destiny(),xdelta, ydelta,zdelta);
      returnMsg->setDelta(xdelta,0);
      returnMsg->setDelta(ydelta,1);
      
      // En el primer flit del paquete que sale por el injector, 
      // se debe actualizar esta medida de tiempo para escribirlo
      // correctamente en todos los flits del paquete
      
      setPacketInjectionTime( MAXIMO(m_InjectorFlow.getOwnerRouter().getCurrentTime(),
                                     returnMsg->generationTime()) );
      
      //Inicializo miss Routing
      PRZInjector& injector=(PRZInjector&)(m_InjectorFlow.getComponent());
      short missrouting=injector.getMissRouting();     
      returnMsg->setMissRouting(missrouting,0);
      returnMsg->setMissRouting(missrouting,1);
      if(missrouting!=0)
      {
         short deltax=returnMsg->delta(0);
         short deltay=returnMsg->delta(1);
         //cout<<returnMsg.delta(0)<<"  "<<returnMsg.delta(1)<<endl;
         returnMsg->setDelta(((deltax>=0)?1:-1)*(abs(deltax)-1),0);
         returnMsg->setDelta(((deltay>=0)?1:-1)*(abs(deltay)-1),1);
         
         //cout<<returnMsg.delta(0)<<"  "<<returnMsg.delta(1)<<endl<<endl;
      }
      returnMsg->setType(PRZMessage::HEADER); //Me faltaba esto!!!!
	}
	else if( returnMsg->flitNumber() == returnMsg->packetSize() )
	{
		returnMsg->setType(PRZMessage::TAIL);
	}
	else
	{
		returnMsg->setType(PRZMessage::INFO);
	}
	
	returnMsg->setPacketInjectionTime(getPacketInjectionTime());
   return returnMsg;
}

//*************************************************************************

// fin del fichero
