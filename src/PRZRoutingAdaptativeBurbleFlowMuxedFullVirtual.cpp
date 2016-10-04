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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZRoutingMuxed_HPP__
#include <PRZRoutingMuxed.hpp>
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

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual,PRZRoutingAdaptativeBurbleFlowMuxed);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual :: PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual( PRZComponent& component)
                          : PRZRoutingAdaptativeBurbleFlowMuxed(component),
                            m_BufferXpReq(0),
                            m_BufferXmReq(0),
                            m_BufferYpReq(0),
                            m_BufferYmReq(0),
                            m_nowIsRequest(false)
{  
   
}

PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual :: ~PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual()
{

}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual :: postInitialize()
{  
   PRZComponentId idXp( PRZComponentId::FifoMemory, _Xplus_,  "BUF12", 2 );
   PRZComponentId idXm( PRZComponentId::FifoMemory, _Xminus_, "BUF22", 2 );
   PRZComponentId idYp( PRZComponentId::FifoMemory, _Yplus_,  "BUF32", 2 );
   PRZComponentId idYm( PRZComponentId::FifoMemory, _Yminus_, "BUF42", 2 );
   
   PRZComponentId idXpReq( PRZComponentId::FifoMemory, _Xplus_,  "BUF14", 4 );
   PRZComponentId idXmReq( PRZComponentId::FifoMemory, _Xminus_, "BUF24", 4 );
   PRZComponentId idYpReq( PRZComponentId::FifoMemory, _Yplus_,  "BUF34", 4 );
   PRZComponentId idYmReq( PRZComponentId::FifoMemory, _Yminus_, "BUF44", 4 );

   PRZComponent* compoXp = getOwnerRouter().componentWithIdLike(idXp); 
   PRZComponent* compoXm = getOwnerRouter().componentWithIdLike(idXm); 
   PRZComponent* compoYp = getOwnerRouter().componentWithIdLike(idYp);
   PRZComponent* compoYm = getOwnerRouter().componentWithIdLike(idYm);

   PRZComponent* compoXpReq = getOwnerRouter().componentWithIdLike(idXpReq); 
   PRZComponent* compoXmReq = getOwnerRouter().componentWithIdLike(idXmReq); 
   PRZComponent* compoYpReq = getOwnerRouter().componentWithIdLike(idYpReq);
   PRZComponent* compoYmReq = getOwnerRouter().componentWithIdLike(idYmReq);  
 
   m_BufferXp = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXm = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYp = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYm = POINTERCAST(PRZFifoMemory,compoYm);

   m_BufferXpReq = POINTERCAST(PRZFifoMemory,compoXpReq);
   m_BufferXmReq = POINTERCAST(PRZFifoMemory,compoXmReq);
   m_BufferYpReq = POINTERCAST(PRZFifoMemory,compoYpReq);
   m_BufferYmReq = POINTERCAST(PRZFifoMemory,compoYmReq);
    
   if( !(m_BufferXp && m_BufferXm && m_BufferYp && m_BufferYm) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }

   if( !(m_BufferXpReq && m_BufferXmReq && m_BufferYpReq && m_BufferYmReq) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }

}

//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage** msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual :: createMessageRequest(PRZMessage** pMsg, unsigned interfaz)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);
   PRZMessage* header = m_FlitsRx[interfaz-1].element(1);
   
   unsigned adapChannel=(header->isRequest() ) ? 3 : 1;
   unsigned deterChannel=(header->isRequest() ) ? 4 : 2;
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
      m_nowIsRequest=header->isRequest();
      
       if( m_RequestState[interfaz-1] == 1 ||m_RequestState[interfaz-1] == 5)
       {
          m_CurrentOut = getHeaderX();
          m_RequestState[interfaz-1] = 2;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(getHeaderX());
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(getHeaderX());
          if( deltaAbs > 1 )
          {
             PRZWRITE2LOG(getComponent().asString() + " X ADAP");
             msg->setRoutingPort(getDirectRoutingType(delta));
             msg->setChannel(adapChannel);
             break;
          }
       }

       if( m_RequestState[interfaz-1] == 2 )
       {
          m_CurrentOut = getHeaderY();      
          m_RequestState[interfaz-1] = 3;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(getHeaderY());
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(getHeaderY());
          
          if( deltaAbs > 1 )
          {
             PRZWRITE2LOG(getComponent().asString() + " Y ADAP");         
             msg->setRoutingPort(getOppositeRoutingType(delta));
             msg->setChannel(adapChannel);
             break;
          }
       }

       if( m_RequestState[interfaz-1] == 3 )
       {
          // Solicitud DOR. Se intenta primero por la X si es que hay
          // "delta" en esa direccion. Si el routing esta en un canal
          // X, el primer flit que se ha recibido es la cabecera X, y
          // si esta en un canal Y, es la cabecera Y.
          m_CurrentOut = 0;         
          m_RequestState[interfaz-1] = 4;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(0);
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(0);
          if( deltaAbs > 1 ) 
          {
             // En el caso de realizar una peticion DOR, la siguiente
             // sera adaptativa, por lo que se pasa al estado 1.
             
             //Este se me habia despistao el cabron
             //produce un deadlock al emplear multiplexacion compl
             
             m_RequestState[interfaz-1] = 5;
             
             PRZWRITE2LOG(getComponent().asString() + " X "+ PRZString(delta) );
             if( controlAlgoritm(true,delta) )
             { 
                PRZWRITE2LOG(getComponent().asString() + " X DOR");
                msg->setRoutingPort( (delta>0) ? _Xplus_ : _Xminus_ );
                msg->setChannel(deterChannel);
                break;
             }
             else
             {
                PRZWRITE2LOG(getComponent().asString() + " Falla XDOR");            
                return false;
             }
          }
       }

       if( m_RequestState[interfaz-1] == 4 )
       {
          // DOR: hay que comprobar si existe la burbuja
          m_CurrentOut = 1;   
          if(m_inputs!=1)      
             m_RequestState[interfaz-1] = 5;  //Condicion de finalizacion en para el dispachevent
          else
             m_RequestState[interfaz-1] = 1;
          int delta    = m_FlitsRx[interfaz-1].element(1)->delta(1);
          int deltaAbs = m_FlitsRx[interfaz-1].element(1)->deltaAbs(1);
          if( deltaAbs > 1 ) 
          {
             if( controlAlgoritm(false,delta) )
             { 
                PRZWRITE2LOG(getComponent().asString() + " Y DOR");
                msg->setRoutingPort( (delta>0) ? _Yplus_ : _Yminus_ );
                msg->setChannel(deterChannel);
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

Boolean PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{


   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   unsigned myChannel=getComponent().getChannel();
   przROUTINGTYPE newDirection;

   if(salidaX) newDirection = (delta>0) ? _Xplus_ : _Xminus_;
   else newDirection = (delta>0) ? _Yplus_ : _Yminus_;
   
   if( myDirection == _LocalNode_ )
   {                  
     if(!m_nowIsRequest)
     {
          switch( newDirection )
          {
             case _Xplus_  : fifo = m_BufferXp; break;
             case _Xminus_ : fifo = m_BufferXm; break;
             case _Yplus_  : fifo = m_BufferYp; break;
             case _Yminus_ : fifo = m_BufferYm; break;  
          }       
      }
      else
      {
	       switch( newDirection )
          {
             case _Xplus_  : fifo = m_BufferXpReq; break;
             case _Xminus_ : fifo = m_BufferXmReq; break;
             case _Yplus_  : fifo = m_BufferYpReq; break;
             case _Yminus_ : fifo = m_BufferYmReq; break;  
          }       
      }
      
   }
   //
   //Se asume que siempre el ultimo canal corresponde a los determinsitas
   //
   
   else if( m_interfazActual!=m_inputs)  // Canal adaptativo -> DOR
   {
      
     if(!m_nowIsRequest)
     {
         switch( newDirection )
         {
            case _Xplus_  : fifo = m_BufferXp; break;
            case _Xminus_ : fifo = m_BufferXm; break;
            case _Yplus_  : fifo = m_BufferYp; break;
            case _Yminus_ : fifo = m_BufferYm; break;         
         }
      }
      else
      {
          switch( newDirection )
          {
             case _Xplus_  : fifo = m_BufferXpReq; break;
             case _Xminus_ : fifo = m_BufferXmReq; break;
             case _Yplus_  : fifo = m_BufferYpReq; break;
             case _Yminus_ : fifo = m_BufferYmReq; break;  
          }       
      }
    }
    else   // Canal DOR -> DOR.
    {
      if( myDirection == newDirection )
      {
          return true;
      }
      
      // Cuando estamos pasando de un canal DOR a otro DOR, solo debe verificarse la
      // condicion de la burbuja en el caso de que realicemos un cambio de dimension,
      // esto es, "myDirection" difiere de "newDirection".
 
      if(!m_nowIsRequest)
      {
         switch( newDirection )
         {
            case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
            case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
            case _Yplus_  : fifo = m_BufferYp; break;
            case _Yminus_ : fifo = m_BufferYm; break;
         }
      }
      else
      {
         switch( newDirection )
         {
            case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
            case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
            case _Yplus_  : fifo = m_BufferYpReq; break;
            case _Yminus_ : fifo = m_BufferYmReq; break;
         }
      }      
   }

   return fifo->bubbleReady();
}

//*************************************************************************


// fin del fichero
