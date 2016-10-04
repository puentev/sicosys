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
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleFlowMRVirtual.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZRoutingAdaptativeBurbleFlowMRVirtual" Algoritmo de la butbuja con
//               multiples peticiones. Pensado para router hibrido con redes de 
//               escape virtuales separadas para request y reply. 
// 
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMRVirtual.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleFlowMRVirtual_HPP__
#define __PRZRoutingAdaptativeBurbleFlowMRVirtual_HPP__

//*************************************************************************

   #include <PRZRoutingAdaptativeBurbleFlowMR.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//*************************************************************************
   class PRZFifoMemory;
//*************************************************************************

   class PRZRoutingAdaptativeBurbleFlowMRVirtual : public PRZRoutingAdaptativeBurbleFlowMR
   {
      typedef PRZRoutingAdaptativeBurbleFlowMR Inhereited;

   public:
      PRZRoutingAdaptativeBurbleFlowMRVirtual( PRZComponent& component);
      
      //VPV
      virtual void    postInitialize();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
            
      // Run time information
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleFlowMRVirtual);
      
   protected:
      virtual Boolean createMessageRequest(PRZMessage** msg);
     
   
      PRZFifoMemory*  m_BufferXpReq;
      PRZFifoMemory*  m_BufferXmReq;
      PRZFifoMemory*  m_BufferYpReq;
      PRZFifoMemory*  m_BufferYmReq;
      
      PRZFifoMemory*  m_BufferXpRep;
      PRZFifoMemory*  m_BufferXmRep;
      PRZFifoMemory*  m_BufferYpRep;
      PRZFifoMemory*  m_BufferYmRep;
   
   };

//*************************************************************************


#endif


// fin del fichero
