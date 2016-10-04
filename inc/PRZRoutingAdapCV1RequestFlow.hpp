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
// File: ../ATCSimul/inc/PRZRoutingAdapCV1RequestFlow.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZRoutingAdapCV1RequestFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdapCV1RequestFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdapCV1RequestFlow_HPP__
#define __PRZRoutingAdapCV1RequestFlow_HPP__

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

   class PRZRoutingAdapCV1RequestFlow : public PRZRoutingFlow
   {
      typedef PRZRoutingFlow Inhereited;
      enum { Free,WaitingHeader,DataTx,WaitCrossbarRx };

   public:
      PRZRoutingAdapCV1RequestFlow( PRZComponent& component);
      
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
            
      // Run time information
      DEFINE_RTTI(PRZRoutingAdapCV1RequestFlow);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      virtual Boolean createMessageRequest(PRZMessage** msg);
      void            addHeaderReceived(PRZMessage* msg);
      void            dataTransmit();
      void            initializeRouting();
      unsigned        getCurrentLocalPort();
      
      TMessagesQueue m_FlitsRx;
      unsigned       m_CurrentLocalPort;
   };

//*************************************************************************


#endif


// fin del fichero
