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
// File: ../ATCSimul/src/SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.cpp
//
// Clase      : SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits
//
// Hereda de  : PRZFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.hpp>

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

//#define TRACE 1
//#define COUNT 1

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits,PRZFlow);

//*************************************************************************
//:
//  f: SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits();
//
//  d:
//:
//*************************************************************************

SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits( PRZComponent& component)
                 : PRZFlow(component)
{
}


//*************************************************************************
//:
//  f: ~SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits();
//
//  d:
//:
//*************************************************************************

SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: ~SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits()
{
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: initialize()
{
   Inhereited :: initialize();
   m_token = 1;
   int i;      
   for(i=0; i<6; i++)
   {
      m_detSync[i] = 0;
      m_detRouting[i] = 0;
      m_detRoutingtime[i] = 0;
      m_adaSync[i] = 0;
      m_adaRouting[i] = 0;
      m_adaRoutingtime[i] = 0;
   }
   for(i=0; i<10; i++)
   {
      m_crossbarInputs[i].msg = i%2 ? &m_detRouting[(i+1)/2] : 0;
      m_crossbarInputs[i].changeDirection = false;
      m_crossbarLatches[i] = 0;
      m_detConnections[i] = 0;
   }
   
   for(i=0; i<6; i++)
   {
      m_preOutBufferSelect[i] = 0;
      m_preOutBufferBusy[i] = 0;
      for(int j=0; j<5; j++)
      {
         m_preOutBufferInputs[i][j] = 0;
         m_preOut[i][j] = 0;
         m_preOut2[i][j] = 0;
      }
   }

/*   for(i=0; i<10; i++)
   {
      m_flitCount[i]=0;
      m_holeCount[i]=0;
   } */

   SFSSimpleRouter& simpleRouter = (SFSSimpleRouter&)getComponent();
   unsigned packetSize = 10;
   m_detBufferSize = 4 * packetSize;
   m_adaBufferSize = 1 * packetSize;
   m_outBufferSize = 4 * packetSize; //simpleRouter.getBufferSize();
   setCleanInterfaces(true);
}


//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: terminate()
{
//   cout << getComponent().asString() << endl;
/*
   int total=m_holeCount[1]+m_flitCount[1];
   cout << getComponent().asString() << "  ";
   for(int i=1; i<10; i++)
   {
      PRZString out;
      out.sprintf(" % 5d (% 3d%%)",m_flitCount[i], (int)m_flitCount[i]*100/total);
      cout << out;
   }
   cout << " // " << total << endl;
   */
/*   int i;
   for(i = 1; i <= 5; i++)
   {
      cout << "   m_detFifos[" << i << "] " <<
    m_detFifos[i].numberOfElements() << '(' <<
    m_detBufferSize << ") m_detRouting[" << i << "] " << 
    (m_detRouting[i] ? 1 : 0) << endl;
   }
   for(i = 1; i <= 5; i++)
   {
      cout << "   m_adaFifos[" << i << "] " <<
    m_adaFifos[i].numberOfElements() << '(' <<
    m_adaBufferSize << ") m_adaRouting[" << i << "] " << 
    (m_adaRouting[i] ? 1 : 0) << endl;
   }
   for(i = 1; i <= 4; i++)
   {
      if(m_detConnections[i*2])
      {
    if(m_detConnections[i*2]%2)
       cout << "   m_detRouting[" << (i+1)/2<< "] -> ";
    else
       cout << "   m_adaRouting[" << i/2<< "] -> ";
      }
      else cout << "   *-> ";
      cout << "m_crossbarFifos[" << i << "] " <<
    m_crossbarFifos[i].numberOfElements() << endl;
   }
   for(i = 1; i <= 5; i++)
   {
      if(m_detConnections[i*2-1])
      {
    if(m_detConnections[i*2-1]%2)
       cout << "   m_detRouting[" << (i+1)/2<< "] -> ";
    else
       cout << "   m_adaRouting[" << i/2<< "] -> ";
      }
      else cout << "   *-> ";
      cout << "m_crossbarLatches[" << i << "] " <<
    (m_crossbarLatches[i] ? 1 : 0) << endl;
   }
   for(i = 1; i <= 5; i++)
   {
      cout << "   m_preOutBuffer[" << i << "][] {";
      int j=0;
       cout << m_preOutBuffer[i][j++].numberOfElements();
      for(;j<=3;j++)
      {
    cout << ',' << m_preOutBuffer[i][j].numberOfElements();
      }
      cout << '}' << endl;
   }
   for(i = 1; i <= 5; i++)
   {
      cout << "   m_outFifos[" << i << "] " <<
    m_outFifos[i].numberOfElements() << '(' <<
    m_outBufferSize << ')' << endl;
   }*/
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************
Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: inputReading()
{ 
   cleanOutputInterfaces();
   doMultiportFifoTwo();
   doMultiportFifoOne();
   doCrossbar();
   doRouting();
   doDemuxSelect();
   doCrossbarSelect();
   doSync();    
   return true;   
}
void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doMultiportFifoTwo()
{
   unsigned outPort;
   unsigned inPort;

   // Recorremos todos los bufferes de salida
   for( outPort = 1; outPort <= 5; outPort++)
   {
      // Recorremos cada puerto
      for( inPort = 0; inPort < 5; inPort++)
      {
	 // Si no hay mensaje, pasa las siguiente
	 if( ! m_preOut2[outPort][inPort] ) continue;
	 // Mete el mensaje en la cola
         m_outBuffer[outPort][inPort].enqueue(m_preOut2[outPort][inPort]);
	 // Libera el registro de entrada
         m_preOut2[outPort][inPort]=0;
      }
      // Recorremos cada puerto
      for( inPort = 0; inPort < 5; inPort++)
      {
	 // Si no hay mensaje o no se puede pasar, pasa las siguiente
	 if( ! m_preOut[outPort][inPort] || m_preOut2[outPort][inPort] ) continue;
	 // Mete el mensaje en el siguiente registro
         m_preOut2[outPort][inPort] = m_preOut[outPort][inPort];
	 // Libera el registro de entrada
         m_preOut[outPort][inPort]=0;
      }
   }
}

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doMultiportFifoOne()
{
   unsigned outPort;
   unsigned inPort;

   // Recorremos todos los bufferes de salida
   for( outPort = 1; outPort <= 5; outPort++)
   {
      // Recorremos cada puerto
      for( inPort = 0; inPort < 5; inPort++)
      {
         // Si el puerto no esta libre pasa al siguiente
         if( m_preOut[outPort][inPort] ) continue;
         // Si no hay conexion de un multiplexor pasa al siguiente
         if( ! m_preOutBufferInputs[outPort][inPort] ) continue;
         PRZMessage **routing = m_preOutBufferInputs[outPort][inPort];
         // Si no hay mensaje en el routing pasa al siguiente
         if( ! *routing ) continue;
         // Si es una cabecera y no hay sitio para otro paquete, pasa al siguiente
         if( (*routing)->isHeader() && ! outputReady(outPort) ) continue;
         // Mete el mensaje del routing en el puerto
         m_preOut[outPort][inPort] = *routing;
         #ifdef COUNT
         cerr << getComponent().asString() << " PRE_FIFO  " << outPort << inPort << " Rx" << endl;
         #endif
         #ifdef TRACE
         cerr << getComponent().asString() << " PRE_FIFO  " << outPort << inPort << ' ' 
         << (int)(*routing) << ' ' << (*routing)->getIdentifier() << ' ' << (*routing)->flitNumber() << endl;
         #endif
         // Si el mensaje es de cola
         if( (*routing)->isTail() )
         {
            // Desconectar el demultiplexor.
            m_preOutBufferInputs[outPort][inPort] = 0;
         }
         // Vacia el routing
         *routing=0;
      }
   }
}


void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doCrossbar()
{
   unsigned outPort;
   unsigned inPort;
   // Recorrer todos los puertos del crossbar
   for( outPort = 1; outPort <= 9; outPort++)
   {      
      // Comprueba que si la salida esta latcheada, el latch este vacio, 
      if( outPort%2 && m_crossbarLatches[(outPort+1)/2] ) continue;
      // Busca el puerto de entrada asignado a outPort.
      // Si no esta asignado, pasa al siguiente.
      if( ! (inPort = m_detConnections[outPort] ) ) continue;
      // Si no hay conexion de un multiplexor pasa al siguiente (no deberia ocurrir)
      if( ! m_crossbarInputs[inPort].msg ) continue;
      PRZMessage **routing = m_crossbarInputs[inPort].msg;
      // Si no hay mensaje en el routing pasa al siguiente
      if( ! *routing ) continue;
      // Si la salida esta buffereada, comprobar que tiene sitio para un paquete entero
      if( ! outPort%2 && (*routing)->isHeader() && ! outputReady(outPort/2) ) continue;
      // Si el puerto de salida esta latcheado
      if( outPort % 2 )
      {         
         // Lo meto en el latch.
         m_crossbarLatches[(outPort+1)/2]=*routing;
         #ifdef TRACE
         cerr << getComponent().asString() << " LATCH     " << (outPort+1)/2 << "  " 
              << (int)(*routing) << ' ' << (*routing)->getIdentifier() << ' ' << (*routing)->flitNumber() << endl;
         #endif
      }
      else
      {
         // Si no hay sitio en el buffer de salida espero al siguiente ciclo
         if( (*routing)->isHeader() && ! outputReady(outPort/2) ) continue;
         // Si no se puede pasa al siguiente
	 if( m_preOut[outPort/2][4] ) continue;
         // Lo meto en la fifo.
	 m_preOut[outPort/2][4]=*routing;
         #ifdef COUNT
         cerr << getComponent().asString() << " CB_FIFO   " << outPort/2 << "  Rx" << endl;
         #endif
         #ifdef TRACE
         cerr << getComponent().asString() << " CB_FIFO   " << outPort/2 << "  " 
              << (int)(*routing) << ' ' << (*routing)->getIdentifier() << ' ' << (*routing)->flitNumber() << endl;
         #endif
      }
      // Si es el final del mensaje
      if( (*routing)->isTail() )
      {
         // libero la conexion
         m_detConnections[outPort] = 0;
         // si el puerto de entrada era adaptativo
         if( ! ( inPort % 2 ) )
         {
            // Desconecto la entrada del demultiplexor
            m_crossbarInputs[inPort].msg = 0;
         }
         #ifdef TRACE
         cerr << getComponent().asString() << " CRSSBR->  " << outPort << "  "
         << (int)(*routing) << ' ' << (*routing)->getIdentifier() << ' ' << (*routing)->flitNumber() << endl;
         for(int zz = 1; zz <= 9; zz++)
            if(m_detConnections[zz]) cerr << m_detConnections[zz] << "->" << zz << endl;
      
         #endif
      }
      // Quito el mensaje del routing para que vengan mas
      (*routing) = 0;    
   } 
}

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doRouting()
{
   unsigned inPort;
   // Recorrer todos los routing deterministas
   for(inPort = 1; inPort <= 5; inPort++)
   {
      // Si el routing determinista esta vacio
      if( ! m_detRouting[inPort] )
      {
         // Coger el siguiente mensaje de la fifo correspondiente
         if(m_detFifos[inPort].dequeue(m_detRouting[inPort]))
         {
            #ifdef COUNT
               cerr << getComponent().asString() << " DET_FIFO  " << inPort  << "  Tx" << endl;
            #endif
            #ifdef TRACE
               cerr << getComponent().asString() << " DET_RTG   " << inPort << "  " 
               << (int)m_detRouting[inPort] << ' ' << m_detRouting[inPort]->getIdentifier() << ' ' << m_detRouting[inPort]->flitNumber()  << endl;
            #endif
            // Asignar el retraso en el routing
            m_detRoutingtime[inPort] = m_detRouting[inPort]->isHeader() ? 1 : 0;
            // Si el mensaje es de cabecera
            if( m_detRouting[inPort]->isHeader() )
            {
               // Calcular el puerto de salida
               m_crossbarInputs[inPort*2-1].changeDirection = updateMessageInfo(m_detRouting[inPort], inPort);
            }
         }
      }
   }
   // Recorrer todos los routing adaptativos
   for(inPort = 1; inPort <= 4; inPort++)
   {
      // Si el routing adaptativo esta vacio
      if( ! m_adaRouting[inPort] )
      {
         // Coger el siguiente mensaje de la fifo correspondiente
         if(m_adaFifos[inPort].dequeue(m_adaRouting[inPort]))
         {
            #ifdef COUNT
                cerr << getComponent().asString() << " ADA_FIFO  " << inPort  << "  Tx" << endl;
            #endif
//   if(m_adaRouting[inPort]->isHeader()) cerr << m_adaRouting[inPort]->getIdentifier() << ' ' << getComponent().asString() << endl;
            #ifdef TRACE
               cerr << getComponent().asString() << " ADA_RTG   " << inPort << "  " 
               << (int)m_adaRouting[inPort] << ' ' << m_adaRouting[inPort]->getIdentifier() << ' ' << m_adaRouting[inPort]->flitNumber()  << endl;
            #endif
            // Asignar el retraso en el routing
            m_adaRoutingtime[inPort] = m_adaRouting[inPort]->isHeader() ? 1 : 0;
            // Si el mensaje es de cabecera
            if( m_adaRouting[inPort]->isHeader() )
            {
               // Calcular el puerto de salida
               updateMessageInfo(m_adaRouting[inPort], inPort);
            }
         }
      }
   }
}

// Como nunca se van a hacer conexiones entre puertos de entrada y salida iguales,
// esto permite utilizar un elemento menos en los arrays.
#define sfsOPOSITE(a) (((a)-1)>1 ? 2+((a)-2)%2 : !((a)-1))
#define sfsFUNNYCOUNT(a,b) ((a) > sfsOPOSITE(b) ? (a-2) : (a-1))

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doDemuxSelect()
{
   unsigned outPort;
   unsigned inPort;
   // Recorremos todos los routing adaptativos.
   for(inPort = 1; inPort <= 4; inPort++)
   {
      // Si no tiene mensaje, pasamos al siguiente
      if( ! m_adaRouting[inPort] ) continue;
      // Si no es una cabecera, pasar al siguiente
      if( ! m_adaRouting[inPort]->isHeader() ) continue;
      // Sacar el puerto de salida
      outPort = extractOutputPortNumber(m_adaRouting[inPort]); 
      // Si el puerto es determinista
      if( outPort % 2 )
      {   
         // Conectamos el routing a la entrada correspondiente del crossbar.
         m_crossbarInputs[inPort*2].msg = &m_adaRouting[inPort];
         // Hay cambio de direccion porque se pasa al canal determinista
         m_crossbarInputs[inPort*2].changeDirection = true;
         #ifdef TRACE
         cerr << getComponent().asString() << " DEMUX->CB " << inPort << "  " 
              << (int)m_adaRouting[inPort] << ' ' << m_adaRouting[inPort]->getIdentifier() << ' ' << m_adaRouting[inPort]->flitNumber()  << endl;
         #endif
      }
      else
      {
         // Conectamos el routing al buffer de la salida correspondiente.
         if(outPort != 10)
         {
            m_preOutBufferInputs[outPort/2][sfsFUNNYCOUNT(inPort,outPort/2)] = &m_adaRouting[inPort];
         }
         else
         {
            m_preOutBufferInputs[outPort/2][inPort-1] = &m_adaRouting[inPort];
         }
         #ifdef TRACE
         cerr << getComponent().asString() << " DEMUX->BF " << inPort << ' ' << outPort/2 << sfsFUNNYCOUNT(inPort,outPort/2) <<  ' ' 
              << (int)m_adaRouting[inPort] << ' ' << m_adaRouting[inPort]->getIdentifier() << ' ' << m_adaRouting[inPort]->flitNumber()  << endl;
         #endif
      }
   }
}

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doCrossbarSelect()
{
   unsigned outPort;
   unsigned inPort;
   inPort = m_token;
   m_token = 0;
   int i;
   for(i = 0; i < 9; i++, inPort = inPort % 9 + 1)
   {
      // Seleccionar el routing 
      if( ! m_crossbarInputs[inPort].msg ) continue;
      PRZMessage *routing = *(m_crossbarInputs[inPort].msg);
      // Si el routing esta vacio, pasar al siguiente
      if( ! routing ) continue;
      // Si no es una cabecera, pasar al siguiente
      if( ! routing->isHeader() ) continue;
      // Sacar el puerto de salida
      outPort = extractOutputPortNumber(routing);
      if(outPort == 10) cerr << "Problemen\n";
      // Esta el puerto libre? 
      // Si hay un cambio de direccion, garantizar que hay burbuja
      if( ! m_detConnections[outPort] &&
          ( ! m_crossbarInputs[inPort].changeDirection || bubbleReady(outPort) ) )
      {
         // Ocupar el puerto
         m_detConnections[outPort] = inPort;
         #ifdef TRACE
         cerr << getComponent().asString() << " ->CRSSBR  " << inPort << ' ' << outPort << "  "
              << (int)routing << ' ' << routing->getIdentifier() << ' ' << routing->flitNumber()  << endl;
         for(int zz = 1; zz <= 9; zz++)
            if(m_detConnections[zz]) cerr << m_detConnections[zz] << "->" << zz << endl;
         #endif
      }
      else
      {
         #ifdef TRACE
         cerr << getComponent().asString() << " -|CRSSBR  " << inPort << ' ' << outPort << "  "
              << (int)routing << ' ' << routing->getIdentifier() << ' ' << routing->flitNumber()  << endl;
         for(int zz = 1; zz <= 9; zz++)
            if(m_detConnections[zz]) cerr << m_detConnections[zz] << "->" << zz << endl;
         #endif
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
}

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::doSync()
{
   unsigned inPort;
   // Recorrer todos los sync.
   for( inPort = 1; inPort <= 5; inPort++)
   {
      // Si hay un mensaje en el sicronizador determinista,
      if(m_detSync[inPort])
      {
//         m_flitCount[inPort*2-1]++;
         #ifdef TRACE
         cerr << getComponent().asString() << " DET_SYNC  " << inPort << "  " 
              << (int)m_detSync[inPort] << ' ' << m_detSync[inPort]->getIdentifier() << ' ' << m_detSync[inPort]->flitNumber()  << endl;
         #endif
         // Ponerlo en el Buffer correspondiente
         m_detFifos[inPort].enqueue(m_detSync[inPort]);
         #ifdef COUNT
         cerr << getComponent().asString() << " DET_FIFO  " << inPort << "  Rx" << endl;
         #endif
         // y quitarlo de syncronizador
         m_detSync[inPort]=0;
      }
//      else m_holeCount[inPort*2-1]++;
   }
   for( inPort = 1; inPort <= 4; inPort++)
   {
      // Si hay un mensaje en el sicronizador adaptativo,
      if(m_adaSync[inPort])
      {
//         m_flitCount[inPort*2]++;
         #ifdef TRACE
         cerr << getComponent().asString() << " ADA_SYNC  " << inPort << "  " 
              << (int)m_adaSync[inPort] << ' ' << m_adaSync[inPort]->getIdentifier() << ' ' << m_adaSync[inPort]->flitNumber()  << endl;
         #endif
         // Ponerlo en el Buffer correspondiente
         m_adaFifos[inPort].enqueue(m_adaSync[inPort]);
         #ifdef COUNT
         cerr << getComponent().asString() << " ADA_FIFO  " << inPort << "  Rx" << endl;
         #endif
         // y quitarlo de syncronizador
         m_adaSync[inPort]=0;
      }
//      else m_holeCount[inPort*2]++;
   }
}

//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: stateChange()
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: outputWriting()
{
   unsigned outPort;
   // Recorrer todos los puertos de salida (ya veremos que pasa con el de consumo).
   for( outPort = 1; outPort <= 5; outPort++)
   {
      PRZMessage* msg;
      Boolean enviado=false;
      // Si hay un mensaje en el latch que no es de cabecera,
      if( m_crossbarLatches[outPort] && !m_crossbarLatches[outPort]->isHeader() )
      {
         // hay que mandar el paquete entero (de flit en flit, claro...)
         outputInterfaz(outPort*2-1)->sendData( m_crossbarLatches[outPort] );
         enviado = true;
         #ifdef TRACE
         cerr << getComponent().asString() << " OUTPORT   " << outPort << "  " 
              << (int)m_crossbarLatches[outPort] << ' ' << m_crossbarLatches[outPort]->getIdentifier() << ' ' << m_crossbarLatches[outPort]->flitNumber()  << endl;
         #endif
         // y liberar el latch
         m_crossbarLatches[outPort] = 0;
      }
      for( int atempt = m_preOutBufferBusy[outPort] ? 4 : 0; atempt < 5; atempt++ )
      {
         // Si el puerto aun esta libre 
         if( enviado == false )
         {
            // y hay un mensaje en la fifo de salida,
            if( m_outBuffer[outPort][m_preOutBufferSelect[outPort]].firstElement(msg) )
            {
               // Si no es cabecera o hay sitio para otro paquete.
               if( ! msg->isHeader() || ! outputInterfaz(outPort < 5 ? outPort*2 : 9)->isStopActive() )
               {
                  // Sacarlo de la fifo y
                  m_outBuffer[outPort][m_preOutBufferSelect[outPort]].dequeue( msg );
                  #ifdef COUNT
                  cerr << getComponent().asString() << " OUT_FIFO  " << outPort  << "  Tx" << endl;
                  #endif
                  // Enviarlo
                  outputInterfaz(outPort < 5 ? outPort*2 : 9)->sendData( msg );
                  enviado = true;
		  m_preOutBufferBusy[outPort] = true;
                  #ifdef TRACE
                  cerr << getComponent().asString() << " OUTPORT   " << outPort << (outPort < 5 ? outPort*2 : 9) << " " 
                       << (int)msg << ' ' << msg->getIdentifier() << ' ' << msg->flitNumber()  << endl;
                  #endif
                  // Si el flit es de cola 
                  if( msg->isTail() )
		  {
                     // pasarle el turno a otro buffer
                     ++m_preOutBufferSelect[outPort] %= 5; 
		     m_preOutBufferBusy[outPort] = false;
		  }
               }
            }
            else
	    {
	       // Si no se esta mandando ningun paquete
	       if(! m_preOutBufferBusy[outPort] )
                  // pasarle el turno a otro buffer
                  ++m_preOutBufferSelect[outPort] %= 5;
            }
	 }
      }
      // Si el puerto aun esta libre y hay un mensaje en el latch,
      if( enviado == false && m_crossbarLatches[outPort] )
      {
         // Si no es cabecera o hay sitio para otro paquete.
 /*        if( ! m_crossbarLatches[outPort]->isHeader() || ! outputInterfaz(outPort*2-1)->isStopActive() )
         {
            // Enviarlo*/
            outputInterfaz(outPort*2-1)->sendData( m_crossbarLatches[outPort] );
            enviado = true;
            #ifdef TRACE
            cerr << getComponent().asString() << " OUTPORT   " << outPort << "  " 
                 << (int)m_crossbarLatches[outPort] << ' ' << m_crossbarLatches[outPort]->getIdentifier() << ' ' << m_crossbarLatches[outPort]->flitNumber()  << endl;
            #endif
            // y liberar el latch
            m_crossbarLatches[outPort] = 0;
      //   }
      }
/*      if(((PRZRouter *)getComponent().getOwner())->getPosition() == PRZPosition(0,0,0) && outPort==1)
      if(!enviado && m_outFifos[outPort].numberOfElements()) cerr << '|'; else cerr << '-';*/
   } 
   // y ahora el consumo.
   // Si hay un mensaje en el latch,
/*   if( m_crossbarLatches[5] )
   {
      // Enviarlo
      outputInterfaz(9)->sendData( m_crossbarLatches[5] );
      // y liberar el latch
      m_crossbarLatches[5] = 0;
   } */
   unsigned inPort;
   unsigned sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                        getPacketLength((PRZNetwork*)getOwnerRouter().getOwner())-1;

   // Recorrer todos los bufferes deterministas.
   for( inPort = 1; inPort <= 5; inPort++)
   {
      // Si no hay sitio para otro paquete, mandar señal de stop.
      if((m_detBufferSize - m_detFifos[inPort].numberOfElements()) < sizeForCT)
      {
         inputInterfaz(inPort*2-1)->sendStopRightNow();
      }
      else
      {
         inputInterfaz(inPort*2-1)->clearStopRightNow();
      }
   }
   // Recorrer todos los bufferes adaptativos.
   for( inPort = 1; inPort <= 4; inPort++)
   {
      // Si no hay sitio para otro paquete, mandar señal de stop.
      if((m_adaBufferSize - m_adaFifos[inPort].numberOfElements()) < sizeForCT)
      {
         inputInterfaz(inPort*2)->sendStopRightNow();
      }
      else
      {
         inputInterfaz(inPort*2)->clearStopRightNow();
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: bubbleReady(unsigned inPort) const
{
   unsigned bubble = ((PRZSimulation*)getComponent().getSimulation())->
                              getPacketLength((PRZNetwork*)getOwnerRouter().getOwner()) * 2; 
   // Si el puerto es de consumo, siempre hay burbuja
   if(inPort==9)
   {
      return true;
   }
   // Si el puerto es determinista, 
   else if(inPort%2)
   {
      // hay que mirar el buffer de la entrada determinista
      if(m_detBufferSize - m_detFifos[(inPort+1)/2].numberOfElements() < bubble) return false;
      return true;
   }
   // Si el puerto es adaptativo
   else
   {
      // no hay problema
      return true;
   }
}

#define sfsPACKETROUND(x) (x+(packetLength-x%packetLength)*(x%packetLength ? 1: 0))

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: outputReady(unsigned inPort) const
{
   unsigned packetLength = ((PRZSimulation*)getComponent().getSimulation())->
                        getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());    
   int sizeForCT = packetLength;   
   int size = sfsPACKETROUND(m_outBuffer[inPort][0].numberOfElements() + (m_preOut[inPort][0] ? 1 : 0) + (m_preOut2[inPort][0] ? 1 : 0)) +
              sfsPACKETROUND(m_outBuffer[inPort][1].numberOfElements() + (m_preOut[inPort][1] ? 1 : 0) + (m_preOut2[inPort][1] ? 1 : 0)) +
              sfsPACKETROUND(m_outBuffer[inPort][2].numberOfElements() + (m_preOut[inPort][2] ? 1 : 0) + (m_preOut2[inPort][2] ? 1 : 0)) +
              sfsPACKETROUND(m_outBuffer[inPort][3].numberOfElements() + (m_preOut[inPort][3] ? 1 : 0) + (m_preOut2[inPort][3] ? 1 : 0));
   if(inPort != 5) size += sfsPACKETROUND(m_outBuffer[inPort][4].numberOfElements() + (m_preOut[inPort][4] ? 1 : 0) + (m_preOut2[inPort][4] ? 1 : 0));
   // Si no hay sitio para otro paquete entero, no hay sitio.
   if(m_outBufferSize - size < sizeForCT) return false;
   return true;
}

unsigned SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: numberOfElements(unsigned inPort) const
{
   int size = m_outBuffer[inPort][0].numberOfElements() + (m_preOut[inPort][0] ? 1 : 0) +(m_preOut2[inPort][0] ? 1 : 0) +
              m_outBuffer[inPort][1].numberOfElements() + (m_preOut[inPort][1] ? 1 : 0) +(m_preOut2[inPort][1] ? 1 : 0) +
              m_outBuffer[inPort][2].numberOfElements() + (m_preOut[inPort][2] ? 1 : 0) +(m_preOut2[inPort][2] ? 1 : 0) +
              m_outBuffer[inPort][3].numberOfElements() + (m_preOut[inPort][3] ? 1 : 0) +(m_preOut2[inPort][3] ? 1 : 0);
   if(inPort != 5)  size += m_outBuffer[inPort][4].numberOfElements() + (m_preOut[inPort][4] ? 1 : 0) +(m_preOut2[inPort][4] ? 1 : 0);
   return size;
}

//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: propagateStop()
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: updateMessageInfo(PRZMessage* msg, unsigned pref)
{  
   if( msg->isHeader() )
   {  
      int deltaX = msg->delta(0);
      int deltaY = msg->delta(1);
//      cerr << deltaX << ' ' << deltaY << endl;
      
      int buff[5] = { 0, 0, 0, 0, 0 };
      
      // Para cada direccion valida medimos la ocupacion de los bufferes
      if( deltaX > 1 )
      {
         buff[1] = m_outBufferSize - numberOfElements(1);
         if(buff[1] < 0)
         {
//            cerr << m_outBufferSize <<'-'<< numberOfElements(1)<< endl;
            buff[1] = 0;
         }
      }
      if( deltaX < -1 )
      {
         buff[2] = m_outBufferSize - numberOfElements(2);
         if(buff[2] < 0)
         {
//            cerr << m_outBufferSize <<'-'<< numberOfElements(2)<< endl;
            buff[2] = 0;
         }
      }
      if( deltaY > 1 )
      {
         buff[3] = m_outBufferSize - numberOfElements(3);
         if(buff[3] < 0)
         {
//            cerr << m_outBufferSize <<'-'<< numberOfElements(3)<< endl;
            buff[3] = 0;
         }
      }
      if( deltaY < -1 )
      {
         buff[4] = m_outBufferSize - numberOfElements(4);
         if(buff[4] < 0)
         {
//            cerr << m_outBufferSize <<'-'<< numberOfElements(4)<< endl;
            buff[4] = 0;
         }
      }
      
      
      // de los puertos posibles cogemos el que mas sitio libre tenga
      int max=buff[1];
      int select=1;
//      cerr << buff[1] << ' ';
      for(int i=2; i<=4; i++)
      {
//         cerr << buff[i] << ' ';
         if(buff[i] > max || buff[i] == max && i==pref)
         {
            max = buff[i];
            select = i;
         }
      }
//      cerr << "- " << max << ' ' << select << endl;
      // Si hay algun buffer con sitio
      if(max >0)
      {
         msg->setChannel(2);
         switch(select)
         {
            case 1:
               msg->setDelta(deltaX-1,0);
               msg->setRoutingPort(_Xplus_);
//               cerr << "X+" << endl;
               return true;
            case 2:
               msg->setDelta(deltaX+1,0);
               msg->setRoutingPort(_Xminus_);
//               cerr << "X-" << endl;
               return true;
            case 3:
               msg->setDelta(deltaY-1,1);
               msg->setRoutingPort(_Yplus_);
//               cerr << "Y+" << endl;
               return true;
            case 4:
               msg->setDelta(deltaY+1,1);
               msg->setRoutingPort(_Yminus_);
//               cerr << "Y-" << endl;
               return true;
          }
      }
      else if( deltaX > 1 )
      {
//         cerr << "x+" << endl;
         msg->setDelta(deltaX-1,0);
         if(msg->getRoutingPort() == _Xplus_ && msg->getChannel() == 1) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Xplus_);
         msg->setChannel(1);
      }
      else if( deltaX < -1 )
      {
//         cerr << "x-" << endl;
         msg->setDelta(deltaX+1,0);
         if(msg->getRoutingPort() == _Xminus_ && msg->getChannel() == 1) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Xminus_);
         msg->setChannel(1);
      }
      else if( deltaY > +1 )
      {
//         cerr << "y+" << endl;
         msg->setDelta(deltaY-1,1);
         if(msg->getRoutingPort() == _Yplus_ && msg->getChannel() == 1) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Yplus_);         
         msg->setChannel(1);
      }
      else if(deltaY < -1)
      {
//         cerr << "y-" << endl;
         msg->setDelta(deltaY+1,1);
         if(msg->getRoutingPort() == _Yminus_ && msg->getChannel() == 1) return false; // no hay cambio de direccion
         msg->setRoutingPort(_Yminus_);
         msg->setChannel(1);
      }
      else
      {
//         cerr << "nd" << endl;
         msg->setRoutingPort(_LocalNode_);
         // No actualizamos el canal para que si el mensaje venia por el canal adaptativo 
         // y tenia que ir al consumo, que vaya tambien por el canal adaptativo.
//         msg->setChannel(1);
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

unsigned SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: extractOutputPortNumber(PRZMessage* msg)
{
   return extractOutputPortNumber( msg->getRoutingPort(), msg->getChannel() );
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: extractOutputPortNumber( przROUTINGTYPE dir, unsigned channel )
{
   unsigned oPort = ((SFSSimpleRouter&)getComponent()).getOutputWith( dir, channel );
   
   // Esto es un apaño sucio y vil... perdon
   if( dir == _LocalNode_ && channel == 2 ) oPort = 10;

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

unsigned SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: extractOutputChannel(PRZMessage* msg) const
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: dispatchEvent(const PRZEvent& event)
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

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: cleanOutputInterfaces()
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

void SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: cleanInputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i;
      SFSSimpleRouter& simpleRouter = (SFSSimpleRouter&)getComponent();
      forInterfaz(i,simpleRouter.numberOfInputs())
      {
//         if( getStateForInput(i) == SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits::Free )
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: controlAlgoritm(Boolean info, int delta)
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: onReadyUp(unsigned interfaz, unsigned cv)
{
   PRZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg);
   if(interfaz % 2)
   {
      interfaz = (interfaz+1)/2;
      if(m_detSync[interfaz]) cerr << "Problemen det\n";
      m_detSync[interfaz]=msg;
//      if(msg->isHeader()) cerr << interfaz << ' ' << getComponent().asString() << endl;
   }
   else
   {
      interfaz /= 2;
      if(m_adaSync[interfaz]) cerr << "Problemen ada\n";
      m_adaSync[interfaz]=msg;
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: onStopUp(unsigned interfaz, unsigned cv)
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

Boolean SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: onStopDown(unsigned interfaz, unsigned cv)
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

PRZString SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits :: getStatus() const
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
