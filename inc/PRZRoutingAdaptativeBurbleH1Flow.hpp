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
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleH1Flow.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingAdaptativeBurbleH1Flow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleH1Flow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleH1Flow_HPP__
#define __PRZRoutingAdaptativeBurbleH1Flow_HPP__

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

   class PRZRoutingAdaptativeBurbleH1Flow : public PRZRoutingFlow
   {
      typedef PRZRoutingFlow Inhereited;
      enum { Free, WaitingHeader, DataTx, WaitCrossbarRx };

   public:
      PRZRoutingAdaptativeBurbleH1Flow( PRZComponent& component);
      
      virtual void    postInitialize();
      virtual Boolean outputWriting();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);

      virtual PRZString getStatus() const;
            
      // Run time information
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleH1Flow);
      
   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      przROUTINGTYPE  getOppositeRoutingType(int delta) const;
      przROUTINGTYPE  getDirectRoutingType(int delta) const;
      virtual Boolean createMessageRequest(PRZMessage** msg);
      virtual void    addHeaderReceived(PRZMessage* msg);
      void            dataTransmit();
      virtual void    initializeRouting();
      unsigned        getCurrentLocalPort();
      unsigned        getHeaderX() const;
      unsigned        getHeaderY() const;
      

   protected:
      PRZFifoMemory*  m_BufferXp;
      PRZFifoMemory*  m_BufferXm;
      PRZFifoMemory*  m_BufferYp;
      PRZFifoMemory*  m_BufferYm;
      TMessagesQueue  m_FlitsRx;
      unsigned  m_CurrentOut;
      unsigned  m_RequestState;
      unsigned  m_CurrentLocalPort;
      Boolean   m_InputStopped;      
      Boolean   m_FirstMessage;
   };

//*************************************************************************


#endif


// fin del fichero
