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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleH1VirtualFlow.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleH1VirtualFlow
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleH1VirtualFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleH1VirtualFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleH1VirtualFlow.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleH1VirtualFlow,PRZRoutingAdaptativeBurbleH1Flow);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleH1VirtualFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleH1VirtualFlow :: PRZRoutingAdaptativeBurbleH1VirtualFlow( PRZComponent& component)
                          : PRZRoutingAdaptativeBurbleH1Flow(component),
                            m_BufferXpReq(0),
                            m_BufferXmReq(0),
                            m_BufferYpReq(0),
                            m_BufferYmReq(0),
                            m_BufferXpRep(0),
                            m_BufferXmRep(0),
                            m_BufferYpRep(0),
                            m_BufferYmRep(0)

{
   //setState(Free);
}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleH1VirtualFlow :: postInitialize()
{  
   PRZComponentId idXp( PRZComponentId::FifoMemory, _Xplus_,  "BUF13", 3 );
   PRZComponentId idXm( PRZComponentId::FifoMemory, _Xminus_, "BUF23", 3 );
   PRZComponentId idYp( PRZComponentId::FifoMemory, _Yplus_,  "BUF33", 3 );
   PRZComponentId idYm( PRZComponentId::FifoMemory, _Yminus_, "BUF43", 3 );

   PRZComponent* compoXp = getOwnerRouter().componentWithIdLike(idXp); 
   PRZComponent* compoXm = getOwnerRouter().componentWithIdLike(idXm); 
   PRZComponent* compoYp = getOwnerRouter().componentWithIdLike(idYp);
   PRZComponent* compoYm = getOwnerRouter().componentWithIdLike(idYm);
   
   m_BufferXpReq = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXmReq = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYpReq = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYmReq = POINTERCAST(PRZFifoMemory,compoYm);
   
   if( !(m_BufferXpReq && m_BufferXmReq && m_BufferYpReq && m_BufferYmReq) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
	
   PRZComponentId idXpRep( PRZComponentId::FifoMemory, _Xplus_,  "BUF12", 2 );
   PRZComponentId idXmRep( PRZComponentId::FifoMemory, _Xminus_, "BUF22", 2 );
   PRZComponentId idYpRep( PRZComponentId::FifoMemory, _Yplus_,  "BUF32", 2 );
   PRZComponentId idYmRep( PRZComponentId::FifoMemory, _Yminus_, "BUF42", 2 );
   
   compoXp = getOwnerRouter().componentWithIdLike(idXpRep); 
   compoXm = getOwnerRouter().componentWithIdLike(idXmRep); 
   compoYp = getOwnerRouter().componentWithIdLike(idYpRep);
   compoYm = getOwnerRouter().componentWithIdLike(idYmRep);
    
   m_BufferXpRep = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXmRep = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYpRep = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYmRep = POINTERCAST(PRZFifoMemory,compoYm);
   
   if( !(m_BufferXpRep && m_BufferXmRep && m_BufferYpRep && m_BufferYmRep) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
}

//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage** pMsg);
//
//  d: Las solucitides a los canales dor van en el virutal 2 para reply y en el 3 para request
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1VirtualFlow :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);

   PRZMessage* header = m_FlitsRx.element(1);   
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
      if( m_RequestState == 1 )
      {
         m_CurrentOut = getHeaderX();
         m_RequestState = 2;
         int delta    = m_FlitsRx.element(1)->delta(getHeaderX());
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(getHeaderX());
         if( deltaAbs > 1 )
         {
            PRZWRITE2LOG(getComponent().asString() + " X ADAP");
            msg->setRoutingPort(getDirectRoutingType(delta));
            msg->setChannel(1);
            break;
         }
      }
   
      if( m_RequestState == 2 )
      {
         m_CurrentOut = getHeaderY();      
         m_RequestState = 3;
         int delta    = m_FlitsRx.element(1)->delta(getHeaderY());
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(getHeaderY());
         if( deltaAbs > 1 )
         {
            PRZWRITE2LOG(getComponent().asString() + " Y ADAP");         
            msg->setRoutingPort(getOppositeRoutingType(delta));
            msg->setChannel(1);
            break;
         }
      }
   
      if( m_RequestState == 3 )
      {
         // Solicitud DOR. Se intenta primero por la X si es que hay
         // "delta" en esa direccion. Si el routing esta en un canal
         // X, el primer flit que se ha recibido es la cabecera X, y
         // si esta en un canal Y, es la cabecera Y.
         m_CurrentOut = 0;         
         m_RequestState = 4;
         int delta    = m_FlitsRx.element(1)->delta(0);
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(0);
         if( deltaAbs > 1 ) 
         {
            // En el caso de realizar una peticion DOR, la siguiente
            // sera adaptativa, por lo que se pasa al estado 1.
            m_RequestState = 1;
            PRZWRITE2LOG(getComponent().asString() + " X "+ PRZString(delta) );
            if( controlAlgoritm(true,delta) )
            { 
               PRZWRITE2LOG(getComponent().asString() + " X DOR");
               msg->setRoutingPort( (delta>0) ? _Xplus_ : _Xminus_ );
               if(header->isRequest())
               {
                  msg->setChannel(3);
               }
               else
               {
                  msg->setChannel(2);
               }
               break;
            }
            else
            {
               PRZWRITE2LOG(getComponent().asString() + " Falla XDOR");            
               return false;
            }
         }
      }
   
      if( m_RequestState == 4 )
      {
         // DOR: hay que comprobar si existe la burbuja
         m_CurrentOut = 1;         
         m_RequestState = 1;
         int delta    = m_FlitsRx.element(1)->delta(1);
         int deltaAbs = m_FlitsRx.element(1)->deltaAbs(1);
         if( deltaAbs > 1 ) 
         {
            if( controlAlgoritm(false,delta) )
            { 
               PRZWRITE2LOG(getComponent().asString() + " Y DOR");
               msg->setRoutingPort( (delta>0) ? _Yplus_ : _Yminus_ );
              if(header->isRequest())
                  msg->setChannel(3);
               else
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
//  f: virtual Boolean controlAlgoritm(Boolean salidaX, int delta);
//
//  d: Este metodo se utiliza para verificar la condicion de la burbuja. Se
//     le llama siempre que el canal de salida elegido sea del tipo DOR;
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleH1VirtualFlow :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
   // El valor de "salidaX" es TRUE cuando se sale por un canal X, y FALSE en
   // caso de salir por un canal Y.
	
   unsigned fifoID;
   Boolean isRequest=(m_FlitsRx.element(1))->isRequest();
            
   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   przROUTINGTYPE newDirection;

   if(salidaX) newDirection = (delta>0) ? _Xplus_ : _Xminus_;
   else newDirection = (delta>0) ? _Yplus_ : _Yminus_;
   
   if( myDirection == _LocalNode_ )
   {                  
      // El Routing es de un canal de inyeccion, por lo que hay que verificar
      // que existe la condicion de la burbuja en cualquier caso.
      
      switch( newDirection )
      {
         case _Xplus_  : fifoID = 1; break;
         case _Xminus_ : fifoID = 2; break;
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;         
      }
      
   }
   
   else if( getComponent().getChannel() == 1 )  // Canal adaptativo -> DOR
   {
      // En este caso, se esta pasando de un canal Adaptativo a un canal DOR, por
      // lo que sebemos verificar que se cumple la condicion de la burbuja.
      
      switch( newDirection )
      {
         case _Xplus_  : fifoID = 1; break;
         case _Xminus_ : fifoID = 2; break;
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;         
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
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;
      }      
   }
   if(isRequest)
   {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpReq; break;
         case 2: fifo = m_BufferXmReq; break;
         case 3: fifo = m_BufferYpReq; break;
         case 4: fifo = m_BufferYmReq; break;
       }
    }
    
    else
    {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpRep; break;
         case 2: fifo = m_BufferXmRep; break;
         case 3: fifo = m_BufferYpRep; break;
         case 4: fifo = m_BufferYmRep; break;
      }
   }
   return fifo->bubbleReady();
}

//*************************************************************************


// fin del fichero
