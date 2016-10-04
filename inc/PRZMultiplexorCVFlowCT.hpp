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
// File: ../ATCSimul/inc/PRZMultiplexorCVFlowCT.hpp
//
// Description: Multiplexor a nivel de paquete. Le hago nuevo... el de prelle
//             es super ineficiente (mas elegante pero mas lento que el caballodel malo. 
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiplexorCVFlowCT.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMultiplexorCVFlowCT_HPP__
#define __PRZMultiplexorCVFlowCT_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

   
   #ifndef __PRZQueue_HPP__ 
   #include <PRZQueue.hpp>
   #endif 
   
//*************************************************************************

typedef PRZQueue<PRZMessage*> QueueFlits;

   class PRZMultiplexorCVFlowCT : public PRZFlow
   {
      typedef PRZFlow Inhereited;
      enum { Free, Multiplexor };

   public:
      PRZMultiplexorCVFlowCT( PRZComponent& component);
      ~PRZMultiplexorCVFlowCT();
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();
      virtual void    postRun(uTIME time);
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);    
      void updateCurrentOutput();
      
      // Run time information
      DEFINE_RTTI(PRZMultiplexorCVFlowCT);
      
   protected:
      void cleanOutputInterfaz();
      
   private:
      QueueFlits            *m_memoryRcv;
      unsigned               m_payload;
      unsigned               m_current;
      unsigned               m_lpack;
      unsigned               m_inputs;
   };

//*************************************************************************


#endif


// fin del fichero
