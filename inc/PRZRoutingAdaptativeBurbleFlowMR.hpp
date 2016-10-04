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
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleFlowMR.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZRoutingAdaptativeBurbleFlowMR" Algoritmo de la butbuja con
//               multiples peticiones.
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMR.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleFlowMR_HPP__
#define __PRZRoutingAdaptativeBurbleFlowMR_HPP__

//*************************************************************************

   #include <PRZRoutingFlow.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//*************************************************************************

   typedef PRZQueueTable<PRZMessage*> TMessagesQueue;

//*************************************************************************
   class PRZFifoMemory;
//*************************************************************************

   class PRZRoutingAdaptativeBurbleFlowMR : public PRZRoutingFlow
   {
      typedef PRZRoutingFlow Inhereited;
   protected:
      enum { Free,WaitingHeader,DataTx,WaitCrossbarRx };

   public:
      PRZRoutingAdaptativeBurbleFlowMR( PRZComponent& component);
      
      //VPV
      virtual void    postInitialize();
      virtual Boolean outputWriting();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
            
      // Run time information
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleFlowMR);
      
   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      virtual Boolean createMessageRequest(PRZMessage** msg);
      void            addHeaderReceived(PRZMessage* msg);
      void            dataTransmit();
      void            initializeRouting();
      unsigned        getCurrentLocalPort();
      
      TMessagesQueue m_FlitsRx;
      unsigned       m_CurrentLocalPort;
      Boolean        m_FirstMessage;
      PRZFifoMemory*  m_BufferXp;
      PRZFifoMemory*  m_BufferXm;
      PRZFifoMemory*  m_BufferYp;
      PRZFifoMemory*  m_BufferYm;
   
   };

//*************************************************************************


#endif


// fin del fichero
