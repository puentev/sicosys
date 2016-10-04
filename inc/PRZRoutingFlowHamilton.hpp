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
// File: ../ATCSimul/inc/PRZRoutingFlowHamilton.hpp
//
// Description:  Routing en redes irregulares UP/DOWN
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingFlowHamilton.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingFlowHamilton_HPP__
#define __PRZRoutingFlowHamilton_HPP__

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
   
   #ifndef __PRZFifoMemory_HPP__
   #include <PRZFifoMemory.hpp>
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
   
   #ifndef __PRZComponentId_HPP__
   #include <PRZComponentId.hpp>
   #endif
     
   #ifndef __PRZRouting_HPP__
   #include <PRZRouting.hpp>
   #endif
//*************************************************************************

   typedef PRZQueueTable<PRZMessage*> TMessagesQueue;
//*************************************************************************

   class PRZFifoMemory;
   
   
//*************************************************************************

   class PRZRoutingFlowHamilton : public PRZRoutingAdaptativeBurbleH1Flow
   {
      typedef PRZRoutingAdaptativeBurbleH1Flow Inhereited;

   public:
      PRZRoutingFlowHamilton( PRZComponent& component);
      ~PRZRoutingFlowHamilton();
      virtual void  postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);            
      // Run time information
      DEFINE_RTTI(PRZRoutingFlowHamilton);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      virtual Boolean createMessageRequest(PRZMessage** pMsg);
   private:
      int                 m_myPosition;
      ProfitableChannels  m_list;
      ProfitableChannels  m_listEscape;
      ProfitableChannels  m_listEscape2;
      int                 m_totalChannels;
      int                 m_cycle;
      int                 m_totalChannelsEscape;
      int                 m_totalChannelsEscape2;

      PRZFifoMemory**  m_BufferArray;
   };

//*************************************************************************


#endif


// fin del fichero
