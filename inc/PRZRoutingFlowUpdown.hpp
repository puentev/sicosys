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
// File: ../ATCSimul/inc/PRZRoutingFlowUpdown.hpp
//
// Description:  Routing en redes irregulares UP/DOWN
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingFlowUpdown.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingFlowUpdown_HPP__
#define __PRZRoutingFlowUpdown_HPP__

//*************************************************************************

   #include <PRZRoutingAdaptativeBurbleH1Flow.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZRouter_HPP__
   #include <PRZRouter.hpp>
   #endif

   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
   #ifndef __PRZGlobalData_HPP__
   #include <PRZGlobalData.hpp>
   #endif
   
   #ifndef __PRZSimulation_HPP__
   #include <PRZSimulation.hpp>
   #endif
   
   #ifndef __PRZNetwork_HPP__
   #include <PRZNetwork.hpp>
   #endif
   
   #ifndef __PRZComponent_HPP__
   #include <PRZComponent.hpp>
   #endif
   
   #ifndef __PRZRouting_HPP__
   #include <PRZRouting.hpp>
   #endif
//*************************************************************************

   typedef PRZQueueTable<PRZMessage*> TMessagesQueue;

   
//*************************************************************************

   class PRZRoutingFlowUpdown : public PRZRoutingAdaptativeBurbleH1Flow
   {
      typedef PRZRoutingAdaptativeBurbleH1Flow Inhereited;

   public:
      PRZRoutingFlowUpdown( PRZComponent& component);
      
      virtual void    postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);            
      // Run time information
      DEFINE_RTTI(PRZRoutingFlowUpdown);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      virtual Boolean createMessageRequest(PRZMessage** pMsg);
   private:
      int                 m_myPosition;
      ProfitableChannels  m_list;
      int                 m_totalChannels;
   };

//*************************************************************************


#endif


// fin del fichero
