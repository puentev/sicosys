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
// File: ../ATCSimul/src/PRZCrossbarMRequestSingleWHFlow.cpp
//
// Clase      : PRZCrossbarMRequestSingleWHFlow
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  Esta es la nueva clase para el crossbar secuencial. Atiende
//              las peticiones una por una;
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestSingleWHFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarMRequestSingleWHFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarMRequestSingleWHFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZCrossbarMRequestSingleWHFlow,PRZCrossbarFlow);

//*************************************************************************
//
//  f: PRZCrossbarMRequestSingleWHFlow();
//
//  d: Constructor;
//
//*************************************************************************

PRZCrossbarMRequestSingleWHFlow :: PRZCrossbarMRequestSingleWHFlow( PRZComponent& component)
                         			: PRZCrossbarFlow(component),
                                   m_Token(0)
{
   unsigned numberOfInputs = ((PRZCrossbar&)getComponent()).numberOfInputs();
   m_RequestInput = new TPortList*[numberOfInputs]; 
   for(int i=0; i<numberOfInputs; i++ ) 
   {
      m_RequestInput[i] = (TPortList*)0;
   }
}


//*************************************************************************
//:
//  f: ~PRZCrossbarMRequestSingleWHFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarMRequestSingleWHFlow :: ~PRZCrossbarMRequestSingleWHFlow()
{
   delete[] m_RequestInput;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: Este metodo se activa por cada linea de conexion que nos envie un flit;
//:
//*************************************************************************

Boolean PRZCrossbarMRequestSingleWHFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   PRZMessage* msg;
   PRZMessage* lastMessage;

   inputInterfaz(i)->getData(&msg);
   
   // El mismo flit puede llegar 2 veces, y se comprueba que no se ha procesado aun.
   m_MessageReceivedTable->valueAt(i,&lastMessage);
   if(lastMessage)
      if( (*msg==*lastMessage) && (msg->type()!=PRZMessage::MREQ) ) 
         return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   
   // Tiempo actual.
   uTIME currentTime = getOwnerRouter().getCurrentTime();

   #ifndef NO_TRAZA
   		// p.e., getComponent().asString() retorna "BUF11(1,2,3)"
        PRZString texto = getComponent().asString() + " Flit Rx. TIME = ";
        texto += PRZString(currentTime) + " # " + msg->asString();
        PRZWRITE2LOG( texto );
   #endif
   
    // getOutputForInput(i) devuelve una clase PRZPair<unsigned,unsigned>. Dice el puerto
    // y el canal de salida asociados a esa entrada. Tiene 2 mettodos: right() y left().
    // left() devuelve el puerto y right() el CV. Si el puerto=0, no hay salida asignada.
    
   if( getOutputForInput(i).left() == 0 )
   {
      if (msg->type()!=PRZMessage::MREQ) return true;
      // Se trata del flit de solicitud de puerto(s)
      #ifndef NO_TRAZa
         PRZString texto = getComponent().asString() + " Header Rx. Input port=";
         texto += PRZString(interfaz);
         PRZWRITE2LOG( texto );
      #endif
		
      TPortList* listPort = msg->getPortList();
      if( ! listPort )
      {
         PRZString err;
         err.sprintf( ERR_PRZCMRFL_003,
                      (char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }

      if( m_RequestInput[interfaz-1] != 0 ) return true;
      m_RequestInput[interfaz-1] = listPort;
      // Se define un iterador sobre la lista de solicitudes

            
      inputInterfaz(i)->sendStopRightNow();

      getMessagePool().release(msg);
      m_MessageReceivedTable->setValueAt(i,0);  
   }
   else
   {
      // Esta pasando el cuerpo del mensaje. Por tanto, existe un
      // camino asignado al Mensaje.
      PRZEvent passEvent(_CrossbarPass_,i);
      getEventQueue().enqueue(passEvent,currentTime+getDataDelay()); 
   }

   //msg.destroyPortList();
      
   return true;      
  
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestSingleWHFlow :: outputWriting()
{
   TPortList* listPort = 0;
   unsigned index = 0;
   unsigned numberOfInputs = ((PRZCrossbar&)getComponent()).numberOfInputs();
   for(int i=0; i<numberOfInputs; i++ ) 
   {
      index = (i+m_Token) % numberOfInputs;
      listPort = m_RequestInput[index];
      if(listPort) break;
   }
   
   if( ! listPort ) 
   {
      return Inhereited::outputWriting();
   }
   //m_Token = index;
   m_Token = (m_Token+1) % numberOfInputs;

   TPortList::Cursor cursor(*listPort);

   forCursor(cursor)
   {
         // La forma de acceder a cada elemento de la lista es: "cursor.element()"
         // "przROUTINGTYPE" es un enum de X+, X-, Y+,...
         przROUTINGTYPE dir = przROUTINGTYPE(cursor.element().left());
         unsigned channel   = cursor.element().right();
         
         // Obtiene el indice a partir de la direccion y del cv
         unsigned outPort   = extractOutputPortNumber( dir, channel );            
         if( outPort == 0 ) 
         {
            PRZString err;
            err.sprintf(ERR_PRZCMRFL_002,(char*)getComponent().asString() );
            EXIT_PROGRAM(err);
         }
         if( ! isOutputPortFree(outPort) )
         {
            continue;
         }
         if( outputInterfaz(outPort)->isStopActive() )
         {
            continue;
         }

         delete m_RequestInput[index];
         m_RequestInput[index] = 0;
         
         establishConnection(index+1,outPort,1);
         inputInterfaz(index+1)->clearStopRightNow();
         return Inhereited::outputWriting();
   } // forCursor
   	
   // Se llama al metodo de la clase padre para que realice su tarea por defecto: despachar los eventos
   // de la cola. Estos eventos se corresponden con el paso de los flits por el crossbar.
   
   return Inhereited::outputWriting();
}

//*************************************************************************
//:
//  f: virtual Boolean isOutputPortFree( unsigned outputPort,
//                                       unsigned channel=0) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestSingleWHFlow :: isOutputPortFree( unsigned outputPort, 
                                                     unsigned channel) const
{
   // Si esta ocupado alguno de los canales de la salida multiplexada,
   // entonces se retorna "false".
   
   unsigned state=0;
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();
   
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

void PRZCrossbarMRequestSingleWHFlow :: cleanInputInterfaces()
{
   // Se sobrecarga para que no haga nada.
}


//*************************************************************************
//:
//  f: unsigned cleanConnectionRequestFor(unsigned port, const uPAIR& pair);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMRequestSingleWHFlow :: cleanConnectionRequestFor( unsigned port,
                                                           const uPAIR& pair )
{
   Inhereited::cleanConnectionRequestFor(port);
   
   PRZCrossbar& crossbar = (PRZCrossbar&) getComponent();
   
   for( int i=1; i<=crossbar.numberOfOutputs(); i++ )
   {
      if( i == port ) continue;
      uPAIR temp;
      PRZRequestQueue::Cursor cursor(m_RequestQueue[i-1]);
      forCursor(cursor)
      {
         if( cursor.element().left() == pair.left() )
         {
            cursor.remove();
            break;
         }
      }
   }
}


//*************************************************************************
//:
//  f: Boolean updateMessageInfo( PRZmessage& msg,
//                                unsigned oPort,
//                                unsigned channel );
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestSingleWHFlow :: updateMessageInfo( PRZMessage* msg, 
                                              unsigned oPort,
                                              unsigned channel )
{
   if( msg->type() != PRZMessage::HEADER ) return false;
   przROUTINGTYPE type = ((PRZCrossbar&)getComponent()).getTypeForOutput(oPort,channel);
   int header;
   switch(type)
   {
      case _Xplus_ :
      case _Xminus_: header=0;
                     break;
      case _Yplus_ : 
      case _Yminus_: header=1;
                     break;
      case _Zplus_ : 
      case _Zminus_: header=2;
                     break;
                     
      case _LocalNode_: return true;
   }
   
   int delta = msg->delta(header);
   if( delta>0 )
      msg->setDelta(delta-1,header);
   else
      msg->setDelta(delta+1,header);
      
   return true;
}

//*************************************************************************

// fin del fichero
