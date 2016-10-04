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
// File: ../ATCSimul/inc/SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.hpp
//
// Description:  fichero de encabezado de la clase "SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__
#define __SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>

   #ifndef __PRZQueue_HPP__
   #include <PRZQueue.hpp>
   #endif

//*************************************************************************

   class PRZMessage;
   
//*************************************************************************
   
   typedef PRZQueue<PRZMessage*>      PRZMessageQueue;

//*************************************************************************

   class SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits : public PRZFlow
   {
      typedef PRZFlow Inhereited;
      typedef struct 
      {
         PRZMessage** msg;
         Boolean changeDirection;
      } SFSCrossbarInput;
   
   public:
      SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits( PRZComponent& component);
      virtual ~SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits();

      virtual void initialize();
      virtual void terminate();

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual PRZString getStatus() const;
      
      // Run time information
      DEFINE_RTTI(SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits);

   protected:
      virtual  void    cleanOutputInterfaces();
      virtual  void    cleanInputInterfaces();
      virtual  Boolean propagateStop();
      virtual  Boolean updateMessageInfo(PRZMessage* msg, unsigned pref);
      virtual  Boolean dispatchEvent(const PRZEvent& event);
      
      virtual  unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
      virtual  unsigned extractOutputPortNumber(PRZMessage* msg);
      
      unsigned extractOutputChannel(PRZMessage* msg) const;

   protected:
      Boolean bubbleReady(unsigned inPort) const;
      Boolean outputReady(unsigned inPort) const;
      Boolean cleanInterfaces() const
      { return m_CleanInterfaces; }
      
      void setCleanInterfaces(Boolean value)
      { m_CleanInterfaces = value; }
      
      Boolean m_CleanInterfaces;
   private:
      void doCrossbar();
      void doRouting();
      void doMultiportFifoOne();
      void doMultiportFifoTwo();
      void doDemuxSelect();
      void doCrossbarSelect();
      void doSync();    
      unsigned numberOfElements(unsigned inPort) const;

      // Parte determinista
      PRZMessage* m_detSync[6];
      
      unsigned m_detBufferSize;
      PRZMessageQueue m_detFifos[6];            

      PRZMessage* m_detRouting[6];
      int m_detRoutingtime[6];
//      Boolean m_detChangeDirection[6];

//      PRZMessage** m_crossbarInputs[10];
      SFSCrossbarInput m_crossbarInputs[10];
      Boolean m_crossbarDirection[10];
      unsigned m_detConnections[10];
      unsigned m_token;

      // Parte adaptativa
      PRZMessage* m_adaSync[6];
      
      unsigned m_adaBufferSize;
      PRZMessageQueue m_adaFifos[6];            

      PRZMessage* m_adaRouting[6];
      int m_adaRoutingtime[6];
//      Boolean m_adaChangeDirection[6];

      // Parte de salida
//      PRZMessageQueue m_crossbarFifos[5];            
      PRZMessage* m_crossbarLatches[6];            
      PRZMessage** m_preOutBufferInputs[6][5];
      unsigned m_preOutBufferSelect[6];
      Boolean m_preOutBufferBusy[6];
      PRZMessage* m_preOut[6][5];
      PRZMessage* m_preOut2[6][5];
      PRZMessageQueue m_outBuffer[6][5];
      unsigned m_outBufferSize;
//      PRZMessageQueue m_outFifos[6];            
      unsigned m_flitCount[10];
      unsigned m_holeCount[10];
};

//*************************************************************************


#endif


// fin del fichero
