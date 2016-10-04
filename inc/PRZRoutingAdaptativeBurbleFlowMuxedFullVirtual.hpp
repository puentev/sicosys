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
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual_HPP__
#define __PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual_HPP__

//*************************************************************************

   #include <PRZRoutingAdaptativeBurbleFlowMuxed.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif   
   
//*************************************************************************

   class PRZFifoMemory;
   
//*************************************************************************

   class PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual : public PRZRoutingAdaptativeBurbleFlowMuxed
   {
      typedef PRZRoutingAdaptativeBurbleFlowMuxed PRZRoutingFlow;

   public:
      PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual( PRZComponent& component);
      ~PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual();

      virtual void    postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
            
      // Run time information
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual);
      
   protected:

      virtual Boolean createMessageRequest(PRZMessage** msg, unsigned interfaz);

   protected:
      PRZFifoMemory*  m_BufferXpReq;
      PRZFifoMemory*  m_BufferXmReq;
      PRZFifoMemory*  m_BufferYpReq;
      PRZFifoMemory*  m_BufferYmReq;
      Boolean m_nowIsRequest;
   };

//*************************************************************************


#endif


// fin del fichero
