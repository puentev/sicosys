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
// File: ../ATCSimul/inc/PRZConsumerFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZConsumerFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZConsumerFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZConsumerFlow_HPP__
#define __PRZConsumerFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>

   #ifndef __PRZQueue_HPP__
   #include <PRZQueue.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif
   
//*************************************************************************

   class PRZComponent;
   class PRZInterfaz;

//*************************************************************************

   class PRZConsumerFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;
   
   public:
      enum { Header, Information };
      PRZConsumerFlow( PRZComponent& component);
      virtual ~PRZConsumerFlow();

      virtual void terminate();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual void run(uTIME runTime);
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      // Run time information
      DEFINE_RTTI(PRZConsumerFlow);

   private: 
      Boolean           m_ReceivingMessage;
      PRZMessage*       m_InputData;
      Boolean           m_DataReceived;
      int               m_State;
};

//*************************************************************************


#endif


// fin del fichero
