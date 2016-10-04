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
// File: ../ATCSimul/inc/PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits" Algoritmo de la butbuja con
//               multiples peticiones. Pensado para router hibrido con redes de 
//               escape virtuales separadas para request y reply. 
//               
//               Mejora en la funcion de seleccion
// 
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__
#define __PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__

//*************************************************************************

   #include <PRZRoutingAdaptativeBurbleFlowMRVirtual.hpp>

   #ifndef __PRZTable_HPP__
   #include <PRZTable.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
   #ifndef __PRZMultiportFifo_HPP__
   #include <PRZMultiportFifo.hpp>
   #endif


   
//*************************************************************************
   class PRZFifoMemory;
//*************************************************************************

   class PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits : public PRZRoutingAdaptativeBurbleFlowMRVirtual
   {
      typedef PRZRoutingAdaptativeBurbleFlowMRVirtual Inhereited;

   public:
      PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits( PRZComponent& component);
      
      //VPV
      virtual void    postInitialize();
      DEFINE_RTTI(PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits);
      
   protected:
      virtual Boolean createMessageRequest(PRZMessage** msg); 
      
      PRZMultiportFifo*  m_mpBufferXp;
      PRZMultiportFifo*  m_mpBufferXm;
      PRZMultiportFifo*  m_mpBufferYp;
      PRZMultiportFifo*  m_mpBufferYm;
      unsigned           m_lpack;      
      
   };

//*************************************************************************


#endif


// fin del fichero
