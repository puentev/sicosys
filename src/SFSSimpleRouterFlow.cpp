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
// File: ../ATCSimul/src/SFSSimpleRouterFlow.cpp
//
// Clase      : SFSSimpleRouterFlow
//
// Hereda de  : PRZFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: SFSSimpleRouterFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/SFSSimpleRouterFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <SFSSimpleRouterFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __SFSSimpleRouter_HPP__
#include <SFSSimpleRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(SFSSimpleRouterFlow,PRZFlow);

//*************************************************************************
//:
//  f: SFSSimpleRouterFlow();
//
//  d:
//:
//*************************************************************************

SFSSimpleRouterFlow :: SFSSimpleRouterFlow( PRZComponent& component)
                 : PRZFlow(component)
{
}


//*************************************************************************
//:
//  f: ~SFSSimpleRouterFlow();
//
//  d:
//:
//*************************************************************************

SFSSimpleRouterFlow :: ~SFSSimpleRouterFlow()
{
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterFlow :: initialize()
{
   Inhereited :: initialize();
   m_token = 1;
   for(int i=0; i<6; i++)
   {
      m_connections[i] = 0;
      m_routing[i] = 0;
      m_sync[i] = 0;
      m_routingtime[i] = 0;
      m_changeDirection[i] = false;
   }
   SFSSimpleRouter& simpleRouter = (SFSSimpleRouter&)getComponent();
   m_bufferSize = simpleRouter.getBufferSize();
   setCleanInterfaces(true);
}


//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************
//#define PPP for(j=1; j<=5; j++) cerr << "   " << (int)m_routing[j] << endl; cerr << endl;
Boolean SFSSimpleRouterFlow :: inputReading()
{   
/*int j;
   cerr << getComponent().asString() << endl;
   cerr << "1 " << endl; PPP; */
   unsigned outPort;
   unsigned inPort;
   cleanOutputInterfaces();
   // Recorrer todos los puertos de salida.
   for( outPort = 1; outPort <= 5; outPort++)
   {      
      // Buscar el puerto de entrada asignado a outPort.
      // Si no esta asignado, pasa al siguiente.
      if( ! (inPort = m_connections[outPort] ) ) continue;
      // Si no hay mensaje en el routing pasa al siguiente
      if( ! m_routing[inPort] ) continue;
      // Si el puerto esta ocupado y es una cabecera, pasa a la siguiente
      if( outputInterfaz(outPort)->isStopActive() && m_routing[inPort]->isHeader() ) continue;
      // Si hay un mensaje en el routing
      if( ! ( m_routingtime[inPort]-- ) )
      {
         // Lo mando a la salida correspondiente
         outputInterfaz(outPort)->sendData(m_routing[inPort]);
         // Si es el final del mensaje
         if( m_routing[inPort]->isTail() )
         {
            // libero la conexion
            m_connections[outPort] = 0;
         }
         // Quito el mensaje del routing para que vengan mas
         m_routing[inPort] = 0;	 
      }
   }

   // Recorrer todos los routing
//   cerr << "11 " << endl; PPP;
   for(inPort = 1; inPort <= 5; inPort++)
   {
      Boolean nuevo = false;
      // Si el routing esta vacio
      if( ! m_routing[inPort] )
      {
         // Coger el siguiente mensaje de la fifo correspondiente
         if(m_fifos[inPort].dequeue(m_routing[inPort]))
	      {
	         // Asignar el retraso en el routing
            m_routingtime[inPort] = m_routing[inPort]->isHeader() ? 1 : 0;
            // Si el mensaje es de cabecera
            if( m_routing[inPort]->isHeader() )
            {
               // Calcular el puerto de salida
               m_changeDirection[inPort] = updateMessageInfo(m_routing[inPort]);
	         }
		   }
//		   else
//		      m_routing[inPort] = 0;
      }
   }
if((int)(m_routing[5]) == 0x03) cerr << "La cagamos 6 " << inPort << endl;
   inPort = m_token;
   m_token = 0;
   int i;
   for(i = 0; i < 5; i++, inPort = inPort % 5 + 1)
   {
      // Si el routing esta vacio, pasar al siguiente
      if( ! m_routing[inPort] ) continue;
      // Si no es una cabecera, pasar al siguiente
      if( ! m_routing[inPort]->isHeader() ) continue;
      // Sacar el puerto de salida
      outPort = extractOutputPortNumber(m_routing[inPort]); 
      // Esta el puerto libre? 
	   // Si hay un cambio de direccion, garantizar que hay burbuja
      if( ! m_connections[outPort] && ( ! m_changeDirection[inPort] || bubbleReady(outPort) ) )
      {

         // Ocupar el puerto
         m_connections[outPort] = inPort;
      }
      else
      {
         // Si es el primer puerto que queda desatendido en esta ronda
         // lo guardo para atenderle el primero en la siguiente
	      if( ! m_token )
	      {
            m_token = inPort;
	      }
      }
      // Si no, esperar al proximo ciclo.
   }
   // No se ha desatendido ningun puerto, empezar donde antes.
   if( ! m_token )
       m_token = inPort; 
   // Recorrer todos los sync.
   for( inPort = 1; inPort <= 5; inPort++)
   {
      // Si hay un mensaje en el sicronizador,
      if(m_sync[inPort])
      {
         // Ponerlo en el Buffer correspondiente
         m_fifos[inPort].enqueue(m_sync[inPort]);
	      // y quitarlo de syncronizador
         m_sync[inPort]=0;
	   }
   }
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: stateChange()
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: outputWriting()
{
   unsigned inPort;
   unsigned sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                        getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());   

   // Recorrer todos los bufferes.
   for( inPort = 1; inPort <= 5; inPort++)
   {
      // Si no hay sitio para otro paquete, mandar señal de stop.
      if((m_bufferSize - m_fifos[inPort].numberOfElements()) <= sizeForCT)
      {
         inputInterfaz(inPort)->sendStopRightNow();
	   }
	   else
	   {
         inputInterfaz(inPort)->clearStopRightNow();
	   }
   }
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean bubbleReady(unsigned inPort);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: bubbleReady(unsigned inPort) const
{
   unsigned bubble = ((PRZSimulation*)getComponent().getSimulation())->
                        getPacketLength((PRZNetwork*)getOwnerRouter().getOwner()) * 2;   

   // Si no hay sitio para dos paquetes, no hay burbuja.
   if(m_bufferSize - m_fifos[inPort].numberOfElements() < bubble)
   {
      return false;
   }
   else
   {
      return true;
   }
}

//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: propagateStop()
{
    return true;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: updateMessageInfo(PRZMessage* msg)
{  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);

   if( msg->isHeader() )
   {  
      if( deltaX > 1 )
      {
         msg->setDelta(deltaX-1,0);
	      if(msg->getRoutingPort() == _Xplus_) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Xplus_);         
      }
      else if( deltaX < -1 )
      {
         msg->setDelta(deltaX+1,0);
	      if(msg->getRoutingPort() == _Xminus_) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Xminus_);
      }
      else if( deltaY > +1 )
      {
         msg->setDelta(deltaY-1,1);
	      if(msg->getRoutingPort() == _Yplus_) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Yplus_);         
      }
      else if(deltaY < -1)
      {
         msg->setDelta(deltaY+1,1);
	      if(msg->getRoutingPort() == _Yminus_) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Yminus_);
      }
      else
      {
         msg->setRoutingPort(_LocalNode_);
         return false; // No hay que hacer control de flujo.
      }
   } 
   return true; // Hay cambio de direccion
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouterFlow :: extractOutputPortNumber(PRZMessage* msg)
{
   return extractOutputPortNumber( msg->getRoutingPort() );
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouterFlow :: extractOutputPortNumber( przROUTINGTYPE dir )
{
   unsigned oPort = ((SFSSimpleRouter&)getComponent()).getOutputWith( dir );
   
   if( !oPort )
   {
      PRZString err;
      err.sprintf( ERR_PRZCFLOW_001,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir),
                   0 );
      EXIT_PROGRAM(err);
   }
   
   return oPort;
}

//*************************************************************************
//:
//  f: unsigned extractOutputChannel(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouterFlow :: extractOutputChannel(PRZMessage* msg) const
{
   return msg->getChannel();
}

//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: dispatchEvent(const PRZEvent& event)
{
   return true; 
}


//*************************************************************************
//:
//  f: void cleanOutputInterfaces();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterFlow :: cleanOutputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i,j;
      SFSSimpleRouter& simpleRouter = (SFSSimpleRouter&)getComponent();
   
      forInterfaz(i,simpleRouter.numberOfOutputs())
      {
         forInterfaz(j,outputInterfaz(i)->numberOfCV() )
            outputInterfaz(i)->clearData(j);
      }
   }
}

//*************************************************************************
//:
//  f: void cleanInputInterfaces();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterFlow :: cleanInputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i;
      SFSSimpleRouter& simpleRouter = (SFSSimpleRouter&)getComponent();
      forInterfaz(i,simpleRouter.numberOfInputs())
      {
//         if( getStateForInput(i) == SFSSimpleRouterFlow::Free )
         inputInterfaz(i)->clearStop();
      }
   }
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: controlAlgoritm(Boolean info, int delta)
{
   // Para evitar que sea una clase abstracta.
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: 
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   PRZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg);
   if(m_sync[interfaz]) cerr << "Problemen\n";
   m_sync[interfaz]=msg;
   return true;      
}

   	
//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterFlow :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean SFSSimpleRouterFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}

//*************************************************************************
//:
//  f: PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString SFSSimpleRouterFlow :: getStatus() const
{
   SFSSimpleRouter& crb = (SFSSimpleRouter&)getComponent();
   PRZString rs = crb.asString() + PRZString(":\tIn stop= ");

   int i, channel;
   for( i=1; i<=crb.numberOfInputs(); i++ )
   {
      if( inputInterfaz(i)->isStopActive() )
         rs+= PRZString("I") + PRZString(i) + " ";
   }
   rs += PRZString(".\tOut stop= ");
   for( i=1; i<=crb.numberOfOutputs(); i++ )
   {
      if( crb.isLocalNodeOutput(i) )
      {
         if( outputInterfaz(i)->isStopActive() )
            rs+= PRZString("O") + PRZString(i) + " ";
      }

/*      else for(channel=1; channel<=crb.getOutputMux(); channel++ )
      {
         if( outputInterfaz(i)->isStopActive(channel) )
            rs+= PRZString("O") + PRZString(i) + PRZString(channel) + " ";
      } */
      else 
      {
         channel=1;
         if( outputInterfaz(i)->isStopActive(channel) )
            rs+= PRZString("O") + PRZString(i) + PRZString(channel) + " ";
      } 
   }

   return rs;
}

//*************************************************************************


// fin del fichero
