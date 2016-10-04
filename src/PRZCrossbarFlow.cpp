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
// File: ../ATCSimul/src/PRZCrossbarFlow.cpp
//
// Clase      : PRZCrossbarFlow
//
// Hereda de  : PRZFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZCrossbarFlow,PRZFlow);

//*************************************************************************
//:
//  f: PRZCrossbarFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarFlow :: PRZCrossbarFlow( PRZComponent& component)
                 : PRZFlow(component),
                   m_PortAsignamentTable(0),
                   m_OutputStateTable(0),
                   m_MessageReceivedTable(0),
                   m_InputStateTable(0),
                   m_OutputLastUseTable(0),
                   m_CleanInterfaces(true)
{
   enableEvent(_CrossbarRequest_);
   enableEvent(_CrossbarPass_);
}


//*************************************************************************
//:
//  f: ~PRZCrossbarFlow();
//
//  d:
//:
//*************************************************************************

PRZCrossbarFlow :: ~PRZCrossbarFlow()
{

   delete[] m_RequestQueue;   
   delete m_PortAsignamentTable;
   delete m_OutputStateTable;
   delete m_MessageReceivedTable;
   delete m_InputStateTable;
   delete m_OutputLastUseTable;

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: initialize()
{
   Inhereited :: initialize();

   PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();
   m_PortAsignamentTable = new PRZCrossbarTable(crossbar.numberOfInputs());
   m_OutputStateTable    = new PRZArray<unsigned>(MAXIMO(1,crossbar.getOutputMux())*
                                                  crossbar.numberOfOutputs() );
   
   m_MessageReceivedTable= new PRZMessageTable(crossbar.numberOfInputs());
   m_InputStateTable     = new PRZStateTable(crossbar.numberOfInputs());
   m_OutputLastUseTable  = new PRZArray<unsigned>(crossbar.numberOfOutputs());
   m_RequestQueue        = new PRZRequestQueue[crossbar.numberOfOutputs()];

   // Se inicializan las tablas para que todos los recursos esten libres.
	
   m_PortAsignamentTable->initialize(uPAIR(0,0));
   m_OutputStateTable->initialize(0);
   m_MessageReceivedTable->initialize(NULL);
   m_OutputLastUseTable->initialize(0);
   m_InputStateTable->initialize(Free);   
}


//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: inputReading()
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

Boolean PRZCrossbarFlow :: stateChange()
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

Boolean PRZCrossbarFlow :: outputWriting()
{
   uTIME timeStamp;
	PRZSequence<PRZEvent> tempList;
	cleanOutputInterfaces();
   cleanInputInterfaces();
     
   if( getEventQueue().firstElementTimeStamp(timeStamp) )
   {
      while( timeStamp <= getOwnerRouter().getCurrentTime() )
      {
         static PRZEvent event;
         getEventQueue().dequeue(event);
         if( ! dispatchEvent(event) )
         {
            tempList.addAsLast(event);
         }
         if( !getEventQueue().firstElementTimeStamp(timeStamp) ) break;
      }
   }
   
   PRZSequence<PRZEvent>::Cursor cursor(tempList);
   forCursor(cursor)
      getEventQueue().enqueue(cursor.element());
   
   tempList.removeAllElements();
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: propagateStop()
{
    return true;
}


//*************************************************************************
//:
//  f: Boolean addConnectionRequest(unsigned src, uPAIR dst);
//
//  d: Encola en la salida correspondiente a "dst.left()" una pareja de datos
//     que son la entrada que la solicita "src" con el canal de salida que
//     figura en "dst.right()";
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: addConnectionRequest(unsigned src, uPAIR dst)
{
   PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();

   if( (dst.left() > 0) && (dst.left() <= crossbar.numberOfOutputs()) )
   {
      // Los puertos van del 1 al N.
      m_RequestQueue[dst.left()-1].enqueue(uPAIR(src,dst.right()));
      return true;
   #ifndef NO_TRAZA 
      PRZString texto = getComponent().asString() + ". Conexion Request ";
      texto += PRZString(dst.left())+ " => " + PRZString(dst.right());
      PRZWRITE2LOG(texto);
   #endif
   }
   #ifndef NO_TRAZA 
      PRZString texto2 = getComponent().asString() + ". Conexion Request Out of Bounds";
      texto2 += PRZString(dst.left())+ " => " + PRZString(dst.right());
      PRZWRITE2LOG(texto2);
   #endif
   return false;
}


//*************************************************************************
//:
//  f: unsigned getConnectionRequestFor(unsigned port);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarFlow :: getConnectionRequestFor(unsigned port)
{
   PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();

   if( (port > 0) && (port <= crossbar.numberOfOutputs()) )
   {
      uPAIR rc(0,0);
      if( m_RequestQueue[port-1].dequeue(rc) )
         return rc.left();
   }
   return 0;
}


//*************************************************************************
//:
//  f: unsigned cleanConnectionRequestFor(unsigned port, const uPAIR& pair);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: cleanConnectionRequestFor( unsigned port,
                                                   const uPAIR& pair )
{
   PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();

   if( (port > 0) && (port <= crossbar.numberOfOutputs()) )
   {
      m_RequestQueue[port-1].removeAllElements();
   }
}


//*************************************************************************
//:
//  f: uPAIR getOutputForInput(unsigned input) const;
//
//  d:
//:
//*************************************************************************

uPAIR PRZCrossbarFlow :: getOutputForInput(unsigned input) const
{
   uPAIR result(0,0);
   m_PortAsignamentTable->valueAt(input,result);
      
   return result;
}


//*************************************************************************
//:
//  f: virtual Boolean isOutputPortFree( unsigned outputPort,
//                                       unsigned channel=0 ) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: isOutputPortFree( unsigned outputPort, 
                                             unsigned channel ) const
{
   unsigned state=0;
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();
   
   m_OutputStateTable->valueAt(MAXIMO(1,channel)+(outputPort-1)*MAXIMO(1,mux),state);
  
   return (state == 0) ? true : false;
}


//*************************************************************************
//:
//  f: void establishConnection( unsigned input, 
//                               unsigned output, 
//                               unsigned channnel =0 );
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: establishConnection( unsigned input, 
                                             unsigned output,
                                             unsigned channel )
{
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();
   m_PortAsignamentTable->setValueAt(input,uPAIR(output,channel));
   m_OutputStateTable->setValueAt(MAXIMO(1,channel)+(output-1)*MAXIMO(1,mux),input);
   
   #ifndef NO_TRAZA 
      PRZString texto = getComponent().asString() + ". Conexion ";
      texto += PRZString(input) + " => " + PRZString(output) + "," + 
               PRZString(channel);
      PRZWRITE2LOG(texto);
   #endif
}


//*************************************************************************
//:
//  f: void clearConnection(unsigned input);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: clearConnection(unsigned input)
{
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();
   uPAIR out(0,0);
   m_PortAsignamentTable->valueAt(input,out);
   m_PortAsignamentTable->setValueAt(input,uPAIR(0,0));
   m_OutputStateTable->setValueAt(MAXIMO(1,out.right())+(out.left()-1)*MAXIMO(1,mux),0);

   setStateForInput(input,Free);
   inputInterfaz(input)->clearStop();
   
   #ifndef NO_TRAZA
      PRZString text = getComponent().asString() + " CLEAR: " + 
                       PRZString(input) + " -> " + PRZString(out.left());
      PRZWRITE2LOG(text);
   #endif
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarFlow :: extractOutputPortNumber(PRZMessage* msg)
{
   return extractOutputPortNumber( msg->getRoutingPort(), 
                                   msg->getChannel() );
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarFlow :: extractOutputPortNumber( przROUTINGTYPE dir,
                                                     unsigned channel )
{
   unsigned oPort = ((PRZCrossbar&)getComponent()).getOutputWith( dir, channel );
   
   if( !oPort )
   {
      PRZString err;
      err.sprintf( ERR_PRZCFLOW_001,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir),
                   channel );
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

unsigned PRZCrossbarFlow :: extractOutputChannel(PRZMessage* msg) const
{
   return msg->getChannel();
}

//*************************************************************************
//:
//  f: void setStateForInput(unsigned i, PRZCrossbarState state);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: setStateForInput(unsigned i, PRZCrossbarState state)
{
   m_InputStateTable->setValueAt(i,state);
}


//*************************************************************************
//:
//  f: PRZCrossbarState getStateForInput(unsigned i);
//
//  d:
//:
//*************************************************************************

PRZCrossbarFlow::PRZCrossbarState PRZCrossbarFlow :: 
                                  getStateForInput(unsigned i) const
{
   PRZCrossbarState state = Free;
   m_InputStateTable->valueAt(i,state);
   return state;
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: dispatchEvent(const PRZEvent& event)
{

   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      PRZMessage* msg;
      unsigned iPort = event.source();
      unsigned oPort = getOutputForInput(iPort).left();
      if( ! iPort )
      {
         PRZString err;
         err.sprintf(ERR_PRZCFLOW_002,(char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         PRZString err;
         m_MessageReceivedTable->valueAt(iPort,&msg);

         // Si ya he mandado el mensaje, paso de todo
         if(!msg) return true;

         cerr << "Message=" << msg->asString() << " Input=" << iPort << endl;
         err.sprintf(ERR_PRZCFLOW_003,(char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
      
      if( outputInterfaz(oPort)->isStopActive() )
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
         getEventQueue().enqueue(event,delayTime); 
         return true;
      }

      setStateForInput(iPort,Connect);      
      m_MessageReceivedTable->valueAt(iPort,&msg);

      // Si ya he mandado el mensaje, paso de todo
      if(!msg) return true;

      inputInterfaz(iPort)->clearStopRightNow();
      // inputInterfaz(iPort)->clearStop();
      updateMessageInfo(msg,oPort,1);
      outputInterfaz(oPort)->sendData(msg);
      
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
         //PRZWRITE2LOG(getComponent().asString()+"libero paso"+PRZString(iPort)+","+PRZString(oPort));

         if( iPort != 0 )
         {
            establishConnection(iPort,oPort);
            PRZEvent requestEvent(_CrossbarRequest_,iPort,oPort);
            uTIME delayTime = getOwnerRouter().getCurrentTime() +
                              MAXIMO(1,getHeaderDelay());
            
            if( ((PRZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
            {
               if( getHeaderDelay() > 1 )  // El retraso debe ser, al menos 1 ciclo
                  delayTime -= 1;
            }
            getEventQueue().enqueue(requestEvent,delayTime);
         
         }
      }
      // Ya he enviado el mensaje lo puedo eliminar de la tabla de mensajes.
      m_MessageReceivedTable->setValueAt(iPort,(PRZMessage *)0);

   }
   else
   {
      Inhereited::dispatchEvent(event);
   }
   return true;
}


//*************************************************************************
//:
//  f: void cleanOutputInterfaces();
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: cleanOutputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i,j;
      PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();
   
      forInterfaz(i,crossbar.numberOfOutputs())
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

void PRZCrossbarFlow :: cleanInputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i;
      PRZCrossbar& crossbar = (PRZCrossbar&)getComponent();
   
      forInterfaz(i,crossbar.numberOfInputs())
      {
         if( getStateForInput(i) == PRZCrossbarFlow::Free )
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

Boolean PRZCrossbarFlow :: controlAlgoritm(Boolean info, int delta)
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

Boolean PRZCrossbarFlow :: onReadyUp(unsigned interfaz, unsigned cv)
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

Boolean PRZCrossbarFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   unsigned iPort=0;
   
   //m_OutputStateTable->valueAt(1+(interfaz-1)*MAXIMO(1,cv),iPort);
   
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();   
   m_OutputStateTable->valueAt( MAXIMO(1,cv)+(interfaz-1)*MAXIMO(1,mux), iPort );
   
   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString() + " Stop Rx. iport=";
      texto += PRZString(iPort) + " oport=" + PRZString(interfaz);
      PRZWRITE2LOG( texto );
   #endif

   if( iPort && propagateStop() )
   {
      #ifndef NO_TRAZA
         PRZString texto = getComponent().asString() + " Stop Tx. oport=";
         texto += PRZString(interfaz) + " iport=" + PRZString(iPort);
         PRZWRITE2LOG( texto );
   	#endif

	   inputInterfaz(iPort)->sendStopRightNow();
   }
   setStopped(true);
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   unsigned iPort=0;

   // m_OutputStateTable->valueAt(1+(interfaz-1)*MAXIMO(1,cv),iPort);
   
   unsigned mux = ((PRZCrossbar&)(getComponent())).getOutputMux();   
   m_OutputStateTable->valueAt( MAXIMO(1,cv)+(interfaz-1)*MAXIMO(1,mux), iPort );

   
   if( iPort && (getStateForInput(iPort) == Connect) )
   {
      #ifndef NO_TRAZA
         PRZCrossbar& crossbar = (PRZCrossbar&)(getComponent());
         PRZString texto = getComponent().asString() + " Stop Down. O=";
         texto += PRZString(interfaz) + "," + PRZString(cv) + 
                  "=>" + PRZString(iPort);
         for(int i=1; i<=crossbar.numberOfInputs(); i++)
         {
            uPAIR temp;
            m_PortAsignamentTable->valueAt(i,temp);
            if( temp.left() != 0 )
               texto += PRZString("; ") + PRZString(i) + "=>" + 
                        PRZString(temp.left()) + "," + PRZString(temp.right());
         }
         PRZWRITE2LOG(texto);
   	#endif

      inputInterfaz(iPort)->clearStopRightNow();
      setStopped(false);
   }
   
   return true;
}


//*************************************************************************
//:
//  f: unsigned getLastInputForOutput(unsigned output) const;
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbarFlow :: getLastInputForOutput(unsigned output) const
{
   unsigned rc = 0;
   m_OutputLastUseTable->valueAt(output,rc);
   return rc;
}


//*************************************************************************
//:
//  f: void setLastInputForOutput(unsigned input,unsigned output);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: setLastInputForOutput(unsigned input,unsigned output)
{
   m_OutputLastUseTable->setValueAt(output,input);
}


//*************************************************************************
//:
//  f: Boolean isAnyRequestForOutput(unsigned output);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: isAnyRequestForOutput(unsigned output)
{
   // Los puertos van del 1 al N.
   return m_RequestQueue[output-1].numberOfElements();

}


//*************************************************************************
//:
//  f: uPAIR checkMultipleRequestFor(unsigned output);
//
//  d: Retorna un par de numeros, que representan la conexion. "left()" da
//     el indice del puerto de entrada, y "right()" el canal asociado;
//:
//*************************************************************************
 
uPAIR PRZCrossbarFlow :: checkMultipleRequestFor(unsigned output)
{ 
   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString()  + 
                        " Numero peticiones (out=" +
                        PRZString(output) + ") : ";
      texto += PRZString(m_RequestQueue[output-1].numberOfElements());
      PRZWRITE2LOG(texto);
   #endif
   
   uPAIR rc(0,0);
      
   if( m_RequestQueue[output-1].numberOfElements() == 0 )
   {
      PRZString err;
      err.sprintf((char*)getComponent().asString());
      EXIT_PROGRAM(err);
   }
   
   unsigned lastInput = getLastInputForOutput(output);
   
   PRZRequestQueue::Cursor cursor(m_RequestQueue[output-1]);

   forCursor(cursor)
   {
      uPAIR temp = cursor.element();
      if( rc == uPAIR(0,0) ) rc = temp;
      if( (temp.left()!=lastInput) && (rc.left()==lastInput)) rc = temp; 
   }
   return rc;
}


//*************************************************************************
//:
//  f: void removeAllRequestEventsForInput(unsigned iPort);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: removeAllRequestEventsForInput(unsigned iPort)
{
   Boolean eventRemove;
   do
   {
      eventRemove = false;
      TEventQueue::Cursor cursor(getEventQueue());
      forCursor(cursor)
      {
         PRZEvent event(cursor.element().element());
         if( (event.type() == _CrossbarRequest_) &&
             (event.source() == iPort) )
         {
            #ifndef NO_TRAZA
               PRZString text = getComponent().asString() + " Borrando ie: " +
                                PRZString(iPort) + "=>" + 
                                PRZString(event.destiny()) + "," +
                                PRZString(event.channel());
               PRZWRITE2LOG(text);
            #endif
            cursor.remove();
            eventRemove = true;
            break;
         }
      }
   }
   while(eventRemove);
}



//*************************************************************************
//:
//  f: void removeAllRequestEventsForOutput(unsigned oPort);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: removeAllRequestEventsForOutput(unsigned oPort)
{
   Boolean eventRemove;
   do
   {
      eventRemove = false;
      TEventQueue::Cursor cursor(getEventQueue());
      forCursor(cursor)
      {
         PRZEvent event(cursor.element().element());
         if( (event.type() == _CrossbarRequest_) &&
             (event.destiny() == oPort) )
         {
            #ifndef NO_TRAZA
               PRZString text = getComponent().asString() + " Borrando oe: " +
                                PRZString(event.source()) + "=>" +
                                PRZString(oPort) + "," +
                                PRZString(event.channel());
               PRZWRITE2LOG(text);
            #endif
            cursor.remove();
            eventRemove = true;
            break;
         }
      }
   }
   while(eventRemove);
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

Boolean PRZCrossbarFlow :: updateMessageInfo( PRZMessage* msg, 
                                              unsigned oPort,
                                              unsigned channel )
{
   // por defecto, no hace nada sobre la cabecera.  
   return true;
}


//*************************************************************************
//:
//  f: void enableEvent(int event);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: enableEvent(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              m_EventsActive[0] = true;
              return;      

      case _CrossbarPass_:
              m_EventsActive[1] = true;
              return;      
   }

   PRZString err;
   err.sprintf( ERR_PRZCFLOW_005,
                (char*)getComponent().asString(),
                event );
   EXIT_PROGRAM(err);
}


//*************************************************************************
//:
//  f: void disableEvent(int event);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarFlow :: disableEvent(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              m_EventsActive[0] = false;
              return;      

      case _CrossbarPass_:
              m_EventsActive[1] = false;
              return;      
   }

   PRZString err;
   err.sprintf( ERR_PRZCFLOW_005,
                (char*)getComponent().asString(),
                event );
   EXIT_PROGRAM(err);
}


//*************************************************************************
//:
//  f: Boolean isEventEnable(int event);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarFlow :: isEventEnable(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              return m_EventsActive[0];

      case _CrossbarPass_:
              return m_EventsActive[1];     
   }
   
   return false;
}


//*************************************************************************
//:
//  f: PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZCrossbarFlow :: getStatus() const
{
   PRZCrossbar& crb = (PRZCrossbar&)getComponent();
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

      else for(channel=1; channel<=crb.getOutputMux(); channel++ )
      {
         if( outputInterfaz(i)->isStopActive(channel) )
            rs+= PRZString("O") + PRZString(i) + PRZString(channel) + " ";
      }
   }

   return rs;
}

//*************************************************************************


// fin del fichero
