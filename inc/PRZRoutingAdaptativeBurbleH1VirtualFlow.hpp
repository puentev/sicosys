//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleH1VirtualFlow.hpp
//
// Description:  Clase de Routing para soporte de redes virtuales de peticiones
//              y datos en una ccNUMA. Esta clase esta pensada para utilizar con
//				    el mecanismo de la burbuja empleando canales de escapa separados 
//              para estos dos tipos de trafico pero un canal adaptativo unico
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleH1VirtualFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleH1VirtualFlow_HPP__
#define __PRZRoutingAdaptativeBurbleH1VirtualFlow_HPP__

//*************************************************************************

   #include <PRZRoutingAdaptativeBurbleH1Flow.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//*************************************************************************

//   typedef PRZQueueTable<PRZMessage*> TMessagesQueue;

//*************************************************************************

   class PRZFifoMemory;
   
//*************************************************************************

   class PRZRoutingAdaptativeBurbleH1VirtualFlow : public PRZRoutingAdaptativeBurbleH1Flow
   {
   public:
      PRZRoutingAdaptativeBurbleH1VirtualFlow( PRZComponent& component);
      
      virtual void    postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      // Run time information
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleH1VirtualFlow);
      
   protected:
      virtual Boolean         createMessageRequest(PRZMessage** pMsg);
      

   protected:
      //Canales de escape de peticiones
      PRZFifoMemory*  m_BufferXpReq;
      PRZFifoMemory*  m_BufferXmReq;
      PRZFifoMemory*  m_BufferYpReq;
      PRZFifoMemory*  m_BufferYmReq;
      //CAnales de escape de respuestas
      PRZFifoMemory*  m_BufferXpRep;
      PRZFifoMemory*  m_BufferXmRep;
      PRZFifoMemory*  m_BufferYpRep;
      PRZFifoMemory*  m_BufferYmRep;
   };

//*************************************************************************


#endif


// fin del fichero
