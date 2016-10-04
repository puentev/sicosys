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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits
//
// Hereda de  : PRZRoutingAdaptativeBurbleFlowMRVirtual
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.hpp>

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

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZRouting_HPP__
#include <PRZRouting.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits,PRZRoutingAdaptativeBurbleFlowMRVirtual);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits :: PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits( PRZComponent& component)
                            : PRZRoutingAdaptativeBurbleFlowMRVirtual(component),
                              m_mpBufferXp(0),
                              m_mpBufferXm(0),
                              m_mpBufferYp(0),
                              m_mpBufferYm(0)
{
  // setState(Free);
}

   

//*************************************************************************
//:
//  f: void postInitialize();
//
//  d: facilta el accceso a los bufferes locales del router para aplicar bubble
//     aniado sobre elñ punto de partida (Routing VC)
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits :: postInitialize()
{  

   PRZComponentId idXp( PRZComponentId::MultiportFifo, _Xplus_,  "*", 1 );
   PRZComponentId idXm( PRZComponentId::MultiportFifo, _Xminus_, "*", 1 );
   PRZComponentId idYp( PRZComponentId::MultiportFifo, _Yplus_,  "*", 1 );
   PRZComponentId idYm( PRZComponentId::MultiportFifo, _Yminus_, "*", 1 );
   
   PRZComponent* compoXp = getOwnerRouter().componentWithIdLike(idXp); 
   PRZComponent* compoXm = getOwnerRouter().componentWithIdLike(idXm); 
   PRZComponent* compoYp = getOwnerRouter().componentWithIdLike(idYp);
   PRZComponent* compoYm = getOwnerRouter().componentWithIdLike(idYm);
   
   m_mpBufferXp = POINTERCAST(PRZMultiportFifo,compoXp);
   m_mpBufferXm = POINTERCAST(PRZMultiportFifo,compoXm);
   m_mpBufferYp = POINTERCAST(PRZMultiportFifo,compoYp);
   m_mpBufferYm = POINTERCAST(PRZMultiportFifo,compoYm);
   
   if( !(m_mpBufferXp && m_mpBufferXm && m_mpBufferYp && m_mpBufferYm) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
   //inicializo los punteros a los bufferes de entrada para aplicar la burbuja
   Inhereited::postInitialize();
}

//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits :: createMessageRequest(PRZMessage** pMsg)
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
   unsigned channelDor=2;
   if(m_FlitsRx.element(1)->isRequest()) channelDor=3;
   m_lpack= ((PRZSimulation*)getComponent().getSimulation())->getPacketLength((PRZNetwork*)getOwnerRouter().getOwner()); 

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
      PRZString err=PRZString((char*)getComponent().asString()) + " Este Componente no puede ser usado con missrouting\n";;
      EXIT_PROGRAM(err); 
      //testR=0;
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
   unsigned absDeltaX = headerMsg->deltaAbs(0);
   unsigned absDeltaY = headerMsg->deltaAbs(1);

   TPortList* portList = new TPortList();
   
   //Almacenare los canales de salida adaptativos en funcion 
   //del Grado de ocupacion de los bufffers en esta variable
   
   TPortList* selecFunc= new TPortList();
   
   msg->setPortList(portList);
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

   //Funcion de seleccion Max credits. La idea es ordenar el orden de las peticiones en funcion 
   //del grado de ocupacion de las multipuerto de salida. De esta forma puedo aplicar la
   //funcion de seleccion sin recurrir al router externo o emplear creditos en el control de flujo.
   
   //El planteamiento mas simple es precargar la ocupacion de todos los canales de salida y despues
   //ordenar en funcion de el grado de ocupacion antes de crear el portList que le voy a pasar al
   //crossbar. 
   int m_okupa[4];
   
   if(m_lpack>m_FlitsRx.element(1)->packetSize())
   {
      m_okupa[0]=m_mpBufferXp->bufferHolesShort();
      m_okupa[1]=m_mpBufferXm->bufferHolesShort();      
      m_okupa[2]=m_mpBufferYp->bufferHolesShort();
      m_okupa[3]=m_mpBufferYm->bufferHolesShort();
   }
   else
   {
      m_okupa[0]=m_mpBufferXp->bufferHoles();
      m_okupa[1]=m_mpBufferXm->bufferHoles();      
      m_okupa[2]=m_mpBufferYp->bufferHoles();
      m_okupa[3]=m_mpBufferYm->bufferHoles();
   }
    
 //Er fallo del deadlock esta-"ba" en este bloque 
 for(unsigned re=0;re<4;re++)
    {
       unsigned ocupacionMaxima=0;
       unsigned salidaMejor=0;
       for(unsigned indice=0;indice<4;indice++)
       { 
          if(m_okupa[indice]!=-1)
          {
             if(m_okupa[indice]>=(int)ocupacionMaxima)
             {          
                ocupacionMaxima=m_okupa[indice];
                salidaMejor=indice;
             }
           //ESte random me balanceara un poco mejor la carga
           /*  else if(m_okupa[indice]==(int)ocupacionMaxima && drand48()>0.5)
             {          
                ocupacionMaxima=m_okupa[indice];
                salidaMejor=indice;
             }*/
          }
       }
       m_okupa[salidaMejor]=-1;
       switch(salidaMejor)
       {
          case 0: selecFunc->addAsLast(TUPair(_Xplus_,1));
                  break;
          case 1: selecFunc->addAsLast(TUPair(_Xminus_,1));
                  break;
          case 2: selecFunc->addAsLast(TUPair(_Yplus_,1));
                  break;
          case 3: selecFunc->addAsLast(TUPair(_Yminus_,1));
                  break;
          default: PRZString err="Puerto en la funcion de seleccion incorrecto (ordenacion)";
          		    EXIT_PROGRAM(err);
       }
          
    }
    if(selecFunc->numberOfElements()==0)
    {
       PRZString err=PRZString((char*)getComponent().asString())+ "No hay ningun canal seleccionado por la FS ";
       EXIT_PROGRAM(err);
    }
//  selecFunc->addAsLast(TUPair(_Xplus_,1));
//  selecFunc->addAsLast(TUPair(_Xminus_,1));
//  selecFunc->addAsLast(TUPair(_Yplus_,1));
//  selecFunc->addAsLast(TUPair(_Yminus_,1));
    
    TPortList::Cursor cursor(*selecFunc);

    forCursor(cursor)
    {
       switch(cursor.element().left())
       {
          case _Xplus_ : if (absDeltaX > testR && deltaX>0)   
                         portList->addAsLast(TUPair(cursor.element().left(),cursor.element().right()));
                         break;
          case _Xminus_: if (absDeltaX > testR && !(deltaX>0)) 
                         portList->addAsLast(TUPair(cursor.element().left(),cursor.element().right()));
                         break;
          case _Yplus_ : if (absDeltaY > testR && deltaY>0)    
                         portList->addAsLast(TUPair(cursor.element().left(),cursor.element().right()));
                         break;
          case _Yminus_: if (absDeltaY > testR && !(deltaY>0)) 
                         portList->addAsLast(TUPair(cursor.element().left(),cursor.element().right()));
                         break;
           default: PRZString err="Puerto en la funcion de seleccion incorrecto (peticion)";
          		    EXIT_PROGRAM(err);
       }
    }
    delete selecFunc;
    
    //Canales determinsitas (igual)
           
      if( absDeltaX > testR )
      {
         if( controlAlgoritm(true,deltaX) ){
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),channelDor)); 
         }     
      }
      else if( (absDeltaX == testR)  && (absDeltaY > testR) )
      {
         if( controlAlgoritm(false,deltaY) ){ 
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),channelDor));
         }

      }
      
      //ME cepillo todo el codigo de misrouting
   
   
   
   

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


// fin del fichero
