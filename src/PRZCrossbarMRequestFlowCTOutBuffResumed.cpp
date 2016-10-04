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
// File: ../ATCSimul/src/PRZCrossbarMRequestFlowCTOutBuffResumed.cpp
//
// Clase      : PRZCrossbarMRequestFlowCTOutBuffResumed
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestFlowCTOutBuffResumed.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarMRequestFlowCTOutBuffResumed.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarMRequestFlowCTOutBuffResumed.hpp>

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

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZCrossbarMRequestFlowCTOutBuffResumed,PRZCrossbarFlow);

//*************************************************************************
//:
//  f: PRZCrossbarMRequestFlowCTOutBuffResumed();
//
//  d:
//:
//*************************************************************************

PRZCrossbarMRequestFlowCTOutBuffResumed :: PRZCrossbarMRequestFlowCTOutBuffResumed( PRZComponent& component)
                         : PRZCrossbarFlow(component)
{
}


//*************************************************************************
//:
//  f: ~PRZCrossbarMRequestFlowCTOutBuffResumed();
//
//  d:
//:
//*************************************************************************

PRZCrossbarMRequestFlowCTOutBuffResumed :: ~PRZCrossbarMRequestFlowCTOutBuffResumed()

{	

}


//*************************************************************************
//:
//  f: virtual void preRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMRequestFlowCTOutBuffResumed :: preRun(uTIME time)
{
   // Para evitar que las senales del multiplexor se cuelen hacia el
   // routing haciendole confundir entre camino aceptado o stop temporal,
   // se hace que todos los eventos de request sean tratados justo antes
   // de que los mire el routing
  
   enableEvent(_CrossbarRequest_);
   disableEvent(_CrossbarPass_);
   Inhereited::run(time);
}


//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMRequestFlowCTOutBuffResumed :: run(uTIME runTime)
{
   disableEvent(_CrossbarRequest_);
   enableEvent(_CrossbarPass_);
   Inhereited::run(runTime);
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d: 
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: dispatchEvent(const PRZEvent& event)
{
   if( event.type() == _CrossbarRequest_ )
   {
      if( ! isEventEnable(_CrossbarRequest_) ) return false;  
      unsigned oPort = event.destiny();
      uPAIR inp_channel = checkMultipleRequestFor(oPort);
      unsigned iPort    = inp_channel.left();

      if( !isOutputPortFree(oPort) )
      {
         // Si entre medias alguien ha pillado la salida, se contesta que no
       #ifndef NO_TRAZA
         PRZWRITE2LOG(getComponent().asString() + "ocupado" + PRZString(iPort)+ "=>"+ PRZString(oPort));
       #endif
         Inhereited::cleanConnectionRequestFor(oPort);
         return true;
      }
      if( outputInterfaz(oPort)->isStopActive() )
      {
         // El multiplexor puede parar la salida en cualquier momento, por
         // lo que si sucede en el momento de dar la conexion, se anula la
         // solicitud sobre este puerto.
       #ifndef NO_TRAZA
         PRZWRITE2LOG(getComponent().asString() + "stopRechaza" + PRZString(iPort)+ "=>"+ PRZString(oPort));
       #endif
         Inhereited::cleanConnectionRequestFor(oPort);
         return true;
      }
      

      establishConnection(iPort,oPort,inp_channel.right());
      cleanConnectionRequestFor(oPort, inp_channel);
      setLastInputForOutput(iPort,oPort);
      setStateForInput(iPort,Connect);
      removeAllRequestEventsForInput(iPort);
      removeAllRequestEventsForOutput(oPort);
        
      inputInterfaz(iPort)->clearStopRightNow();
       #ifndef NO_TRAZA
         PRZWRITE2LOG(getComponent().asString() + "enlace" + PRZString(iPort)+ "=>"+ PRZString(oPort));
       #endif
      }

   else if( event.type() == _CrossbarPass_ ) 
   {
      if( ! isEventEnable(_CrossbarPass_) ) return false;  

      unsigned iPort  = event.source();
      unsigned oPort  = getOutputForInput(iPort).left();
      unsigned channel= getOutputForInput(iPort).right();
  	   
      //
      //Este cambio de estado impide que se forme una condicion de carrera 
      //con el routing. Un stop desde adelante se propaga hacia el
      //routing de manera incorrecta, entendiendole este como 
      //un ack en las peticiones cuando realmente es un stop
      // --Solo esta corregido en este Crossbar (probablemente 
      // -- se puede volver a producir en el resto WH mrequests.. pero
      //--hasta ahora no se ha manifestado. Estan por tanto sin tocar. 
       
      setStateForInput(iPort,PassMessage);
      
      #ifndef NO_TRAZA
         PRZWRITE2LOG(getComponent().asString() + "setState->PassMessage:" + PRZString(iPort));
      #endif
            
      if( ! iPort )
      {
         PRZString err;
         err.sprintf(ERR_PRZCMRFL_001,(char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         PRZString err;
         err.sprintf(ERR_PRZCMRFL_002,(char*)getComponent().asString() );
         EXIT_PROGRAM(err);
      }
 
      PRZMessage* msg;
      m_MessageReceivedTable->valueAt(iPort,msg);

	   // Si ya he mandado el mensaje, paso de todo
//      if(!msg) return true;
 
//Eliminado. 
//      setStateForInput(iPort,Connect);      
    
      inputInterfaz(iPort)->clearStopRightNow();
      updateMessageInfo(msg,oPort,1);    
      outputInterfaz(oPort)->sendData(msg);
      
      #ifndef NO_TRAZA
            PRZString time = getOwnerRouter().getCurrentTime();       
            PRZString texto = getComponent().asString() + " Flit Tx(" +
                              PRZString(oPort) + " TIME = ";
            texto += time + msg->asString() + " i=" + PRZString(iPort) +
                     ", o=" + PRZString(oPort);
            
           PRZWRITE2LOG( texto );
   	#endif
      // Ya he enviado el mensaje lo puedo eliminar de la tabla de mensajes.
//      m_MessageReceivedTable->setValueAt(iPort,(PRZMessage *)0);

      if( msg->isTail() )
      {
         clearConnection(iPort);
         removeAllPassEventsForInput(iPort);
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

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   unsigned outPort;
   
   PRZMessage* msg;
   PRZMessage* lastMessage;
   inputInterfaz(i)->getData(&msg);
   
   m_MessageReceivedTable->valueAt(i,lastMessage);
   if(lastMessage)
      if( (*msg==*lastMessage) && (msg->type()!=PRZMessage::MREQ) ) return true;
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
      #ifndef NO_TRAZA
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
      
      TPortList::Cursor cursor(*listPort);

      forCursor(cursor)
      {
         przROUTINGTYPE dir = przROUTINGTYPE(cursor.element().left());
         unsigned channel   = cursor.element().right();
         
         // unsigned nOi = ((PRZCrossbar&)getComponent()).numberOfInputs();
//          
//           //=========================================
//           //CAmbio con respecto a la version anterior
//           //necesito un metodo pero lo dejo asi temporalmente
//           //=========================================
//           
//          
//           nOi=nOi/2;
//           if(channel==1 && i<=nOi/2)
//           {
//              //CAnales Adaptativos de salida y entrada
//              //Tengo que reordenar las entradas (adaptativas 4 primeras).
//              
//              outPort=dir-1;
//              
//              if(dir==_LocalNode_)outPort=(nOi/2);
//              
//              outPort=(i+(outPort)*(nOi/2+1));
//           }
//           else  if (channel==1)
//           {
//              outPort=dir-1;
//              if(dir==_LocalNode_)outPort=(nOi/2);
//              
//              outPort=(i-nOi/2+(outPort)*(nOi/2+1))+1;
//              
//           }//Channel==2 o entrada no adaptativa
//           else
//           {
//              //CAnales DOR de entrada y adaptativos hacia DOR
//              outPort=i-nOi/2+(nOi/2+1)*(nOi/2+1);
//              if(dir==_LocalNode_)outPort=(nOi/2+1)*(nOi/2+1)+nOi/2;
//           }
          
         //Por alguna "extraña razon" aparece un channel=2 cuando va a consumo
         if( dir == _LocalNode_ ) channel=1;    
         //El canal virtual de entrada hace cosas raras
         //lo pongo bien.
         
         unsigned nOi = ((PRZCrossbar&)getComponent()).numberOfInputs(); 
         unsigned cvi;
               if(i<=nOi/2)cvi=1; else cvi=2;   
                                       
         unsigned outPort=extractOutputPortNumberOBRE(dir,channel,i,cvi);
                                    
         if( outPort == 0 ) 
         {
            PRZString err;
            err.sprintf(ERR_PRZCMRFL_002,(char*)getComponent().asString() );
            EXIT_PROGRAM(err);
         }
         
        

         if( isOutputPortFree(outPort) && ( ! outputInterfaz(outPort)->isStopActive()) )
         {
            // Hay que asignar una salida
            PRZEvent requestEvent(_CrossbarRequest_,i,outPort);
            getEventQueue().enqueue(requestEvent,delayTime+getHeaderDelay());
            addConnectionRequest(i,uPAIR(outPort,1));
         
         }
         else
         {
            // El puerto de salida esta ocupado. No se encolan las
            // peticiones. Este crossbar solo atiende a las solicitudes
            // de puertos que son validas
         
            #ifndef NO_TRAZA
               PRZString texto = getComponent().getName() + " REQ rechazado." +
                                 PRZString(i) + "=>" + PRZString(outPort);
               PRZWRITE2LOG( texto );
   	      #endif       
         }
        
      } // forCursor

      #ifndef NO_TRAZA
          PRZString time = getOwnerRouter().getCurrentTime();       
          PRZString text = getComponent().getName() + " Stop Tx. TIME = ";
          text += time + " iport= " + PRZString(i);
          PRZWRITE2LOG( text );
      #endif
            
      inputInterfaz(i)->sendStopRightNow();

      getMessagePool().release(msg);
      m_MessageReceivedTable->setValueAt(i,0);  
    
      //msg.destroyPortList();
   }
   else
   {
      // Esta pasando el cuerpo del mensaje. Por tanto, existe un
      // camino asignado al Mensaje.
      PRZEvent passEvent(_CrossbarPass_,i);
      getEventQueue().enqueue(passEvent,delayTime+getDataDelay()); 
   }
   //Por que esta esto aqui?. DEberia limpiarse solo en el caso anterior no?
   msg->destroyPortList();
      
   return true;      
  
}

   	

//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}



//*************************************************************************
//:
//  f: virtual Boolean isOutputPortFree( unsigned outputPort,
//                                       unsigned channel=0) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: isOutputPortFree( unsigned outputPort, 
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
//  f: void removeAllRequestEventsForInput(unsigned iPort);
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMRequestFlowCTOutBuffResumed :: removeAllPassEventsForInput(unsigned iPort)
{
   Boolean eventRemove;
   do
   {
      eventRemove = false;
      TEventQueue::Cursor cursor(getEventQueue());
      forCursor(cursor)
      {
         PRZEvent event(cursor.element().element());
         if( (event.type() == _CrossbarPass_) &&
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
//  f: void cleanInputInterfaces();
//
//  d:
//:
//*************************************************************************

void PRZCrossbarMRequestFlowCTOutBuffResumed :: cleanInputInterfaces()
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

void PRZCrossbarMRequestFlowCTOutBuffResumed :: cleanConnectionRequestFor( unsigned port,
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
//  f: Boolean updateMessageInfo( PRZmessage** msg,
//                                unsigned oPort,
//                                unsigned channel );
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: updateMessageInfo( PRZMessage* msg, 
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
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumed :: onStopUp(unsigned interfaz, unsigned cv)
{
   
   
   return true;
}

//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE type, unsigned channel,unsigned input);
//
//  d: Metodo especifico para este tipo de crossbar (outbuffer adaptativo resumido)
//:
//*************************************************************************

unsigned PRZCrossbarMRequestFlowCTOutBuffResumed :: extractOutputPortNumberOBRE( przROUTINGTYPE type,
                                                     unsigned channel,unsigned input,unsigned channel_input )
{
    PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->
                     globalData((PRZNetwork*)(getOwnerRouter().getOwner()));
    
   if( ! gData.crossbarOutputsType() ) return 0;
   
   PRZTypeChannel elementToFind(type,channel);
   PRZTypeChannel tempType;

   if(channel!=1 )
   //CAso salidas DOR . Evaluo el puerto de forma normal
   {
      for( int i=1; i<=gData.crossbarOutputsType()->numberOfElements(); i++ )
      {
         gData.crossbarOutputsType()->valueAt(i,tempType);
         if( elementToFind == tempType ) return i;
      }
   }
   else
   {
      //SAlidas adaptativas. Tengo que distinguir entre entradas 
      //(DOR+inyeccion) y ADAPTATIVAS

      if(channel_input==1 && input!=((PRZCrossbar&)getComponent()).numberOfInputs())
      //Entradas adaptativas. No problemo
      {
         //Las entradas tienen que estar ordenadas de otra forma
         //Primero TODAS LAS entradas ADAPTATIVAS!
         //
          unsigned counter=0;
          for( int i=1; i<=gData.crossbarOutputsType()->numberOfElements(); i++ )
          {
            gData.crossbarOutputsType()->valueAt(i,tempType);
            if( elementToFind == tempType ) counter++;
            if( counter == input ) return i;
          }
         
      }
      else
      {
         // Entradas DOR e inyecion hacia salida adaptativa
         // selecionan la ultima entrada disponible.
          unsigned last=0;
          for( int i=1; i<=gData.crossbarOutputsType()->numberOfElements(); i++ )
          {
            gData.crossbarOutputsType()->valueAt(i,tempType);
            if( elementToFind == tempType )
            {
               last=1;
            }
            else
            { //Si el ultimo puerto es adecuado y el actual no era el bueno.
               if( last == 1 ) return i-1;
            }
            
          }
      }

   }
   cerr<<input<<"  "<<channel_input<<" Puerto o:"<<type<<" O Channel"<<channel<<endl;
   return 0;
}
//*************************************************************************

// fin del fichero
