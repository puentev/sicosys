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
// File: ../ATCSimul/inc/PRZRoutingBurbleFlowH1Virtual.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingBurbleFlowH1Virtual"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingBurbleFlowH1Virtual.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingBurbleFlowH1Virtual_HPP__
#define __PRZRoutingBurbleFlowH1Virtual_HPP__

//*************************************************************************

   #include <PRZRoutingBurbleFlowH1.hpp>
   
   #ifndef __PRZFifoMemory_HPP__
   #include <PRZFifoMemory.hpp>
   #endif
//*************************************************************************

   class PRZRoutingBurbleFlowH1Virtual : public PRZRoutingBurbleFlowH1
   {
      typedef PRZRoutingBurbleFlowH1 Inhereited;
            
   public:
      PRZRoutingBurbleFlowH1Virtual( PRZComponent& component);
      void postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
       // Run time information
      DEFINE_RTTI(PRZRoutingBurbleFlowH1Virtual);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);
      //Canales de escape de peticiones
      PRZFifoMemory*  m_BufferXpReq;
      PRZFifoMemory*  m_BufferXmReq;
      PRZFifoMemory*  m_BufferYpReq;
      PRZFifoMemory*  m_BufferYmReq;
      //Canales de escape de respuestas
      PRZFifoMemory*  m_BufferXpRep;
      PRZFifoMemory*  m_BufferXmRep;
      PRZFifoMemory*  m_BufferYpRep;
      PRZFifoMemory*  m_BufferYmRep;
         
};

//*************************************************************************


#endif


// fin del fichero
