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
// File: ../ATCSimul/inc/PRZInjectorFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZInjectorFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZInjectorFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZInjectorFlow_HPP__
#define __PRZInjectorFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>

   #ifndef __PRZQueue_HPP__
   #include <PRZQueue.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

   #ifndef __SFSPool_HPP__
   #include <SFSPool.hpp>
   #endif

//*************************************************************************

   class PRZComponent;
   class PRZInterfaz;
   class AlgoritmSendHeader;
   typedef PRZQueue<PRZMessage*> PRZMessageQueue;
   typedef SFSPool<PRZMessage> SFSMessagePool;
      
//*************************************************************************


   class PRZInjectorFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;
            
   public:
      PRZInjectorFlow( PRZComponent& component);
      virtual ~PRZInjectorFlow();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();
      
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      void sendMessage(PRZMessage* msg);
      unsigned getMessagesToSend() const;
      unsigned getAvaliableMessages() const;
      
      PRZMessageQueue& getMessageQueue() const
      { return (PRZMessageQueue&)m_MessageQueue; }

      // Run time information
      DEFINE_RTTI(PRZInjectorFlow);

      void setMissRouting(int short a)
      {m_MissRouting=a;}

      
      void makeMissRouting(PRZMessage msg);
      void makeMissRouting(PRZMessage msg,int short dim);
      
   protected:
      Boolean             isAnyMessageToSend() const;
      AlgoritmSendHeader* getAlgoritmToSend();
               
private: 
      int short           m_MissRouting;              
      PRZMessageQueue     m_MessageQueue;
      PRZMessage*         m_OutputData;
      Boolean             m_DataToSend;
      uTIME               m_PacketInjectionTime;
      AlgoritmSendHeader* m_AlgoritmSend;
};

//*************************************************************************

   class AlgoritmSendHeader
   {
      public:
         AlgoritmSendHeader(PRZInjectorFlow& flow) : m_InjectorFlow(flow),
	                                                  m_MessagePool(flow.getMessagePool())
         {  }
         virtual PRZMessage* getNextFlitToSend() = 0;
      
         Boolean isRequestNext() const;

	      SFSMessagePool& getMessagePool() const
         { return m_MessagePool; }
         
      protected:  
         PRZInjectorFlow& m_InjectorFlow;
         
         PRZMessageQueue& getMessageQueue() const
         { return m_InjectorFlow.getMessageQueue(); }
         
         void setPacketInjectionTime(uTIME time)
         { m_PacketInjectionTime = time; }
      
         uTIME getPacketInjectionTime() const
         { return m_PacketInjectionTime; }
                       
      private:
	      uTIME m_PacketInjectionTime;
	      SFSMessagePool& m_MessagePool;
   };
  
//*************************************************************************   
   
   class AlgoritmSend2Header : public AlgoritmSendHeader
   {
         public:
         AlgoritmSend2Header(PRZInjectorFlow& flow) : AlgoritmSendHeader(flow)
         {  }
         virtual PRZMessage* getNextFlitToSend();
   };

//*************************************************************************
   
   class AlgoritmSend1Header2D : public AlgoritmSendHeader
   {
      public:
         AlgoritmSend1Header2D(PRZInjectorFlow& flow) : AlgoritmSendHeader(flow)
         {  }
         virtual PRZMessage* getNextFlitToSend();   
   };

/* // *************************************************************************
   
   class AlgoritmSend1Header3D : public AlgoritmSendHeader
   {
      public:
         AlgoritmSend1Header3D(PRZInjectorFlow& flow) : AlgoritmSendHeader(flow)
         {  }
         virtual PRZMessage getNextFlitToSend();   
   };
   
// *************************************************************************   
   
   class AlgoritmSend2Header : public AlgoritmSendHeader
   {
         public:
         AlgoritmSend2Header(PRZInjectorFlow& flow) : AlgoritmSendHeader(flow)
         {  }
         virtual PRZMessage getNextFlitToSend();
   };

// *************************************************************************   
   
   class AlgoritmSend3Header : public AlgoritmSendHeader
   {
         public:
         AlgoritmSend3Header(PRZInjectorFlow& flow) : AlgoritmSendHeader(flow)
         {  }
         virtual PRZMessage getNextFlitToSend();
   };

// *************************************************************************
*/
#endif


// fin del fichero
