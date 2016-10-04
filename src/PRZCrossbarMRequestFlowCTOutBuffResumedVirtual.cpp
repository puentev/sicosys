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
// File: ../ATCSimul/src/PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.cpp
//
// Clase      : PRZCrossbarMRequestFlowCTOutBuffResumedVirtual
//
// Hereda de  : PRZCrossbarFlow
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZCrossbarMRequestFlowCTOutBuffResumedVirtual,PRZCrossbarMRequestFlowCTOutBuffResumedVirtual);

//*************************************************************************
//:
//  f: PRZCrossbarMRequestFlowCTOutBuffResumedVirtual();
//
//  d:
//:
//*************************************************************************

PRZCrossbarMRequestFlowCTOutBuffResumedVirtual :: PRZCrossbarMRequestFlowCTOutBuffResumedVirtual( PRZComponent& component)
                         : PRZCrossbarMRequestFlowCTOutBuffResumed(component)
{
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d: 
//:
//*************************************************************************

Boolean PRZCrossbarMRequestFlowCTOutBuffResumedVirtual :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   unsigned outPort;
   
   PRZMessage* msg;
   PRZMessage* lastMessage;
   inputInterfaz(i)->getData(&msg);
   
   m_MessageReceivedTable->valueAt(i,lastMessage);
   if(lastMessage)
      if( (*msg == *lastMessage) && (msg->type() != PRZMessage::MREQ) ) return true;
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
         if( dir == _LocalNode_ ) channel=1;    

         
         unsigned nOi = ((PRZCrossbar&)getComponent()).numberOfInputs(); 
         unsigned cvi;
               if(i<=(nOi-1)/3)
               {
                  cvi=1; 
               }
               else if(i<=(nOi-1)*2/3)
               {
                   cvi=2;
               }
               else
               {   
                   cvi=3;
               }
                                       
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
            #ifndef NO_TRAZA
               PRZString texto = getComponent().getName() + " REQ acceptado." +
                                 PRZString(i) + "=>" + PRZString(outPort) +
                                 " direccion "+  PRZString(channel) +", "+ PRZString(dir);
               PRZWRITE2LOG( texto );
   	      #endif    
         
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
      //msg->destroyPortList();
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
//  f: unsigned extractOutputPortNumber(przROUTINGTYPE type, unsigned channel,unsigned input);
//
//  d: Metodo especifico para este tipo de crossbar (outbuffer adaptativo resumido)
//:
//*************************************************************************

unsigned PRZCrossbarMRequestFlowCTOutBuffResumedVirtual :: extractOutputPortNumberOBRE( przROUTINGTYPE type,
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
