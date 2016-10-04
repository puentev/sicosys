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
// File: ../ATCSimul/inc/PRZCTFifoMuxedFlow.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZCTFifoMuxedFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCTFifoMuxedFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCTFifoMuxedFlow_HPP__
#define __PRZCTFifoMuxedFlow_HPP__

//*************************************************************************

   #include <PRZFifoMuxedFlow.hpp>

//*************************************************************************

   class PRZCTFifoMuxedFlow : public PRZFifoMuxedFlow
   {
      typedef PRZFifoMuxedFlow Inhereited;
      
   public:
      PRZCTFifoMuxedFlow(PRZComponent& component);
      virtual void initialize();
      virtual Boolean outputWriting();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean controlAlgoritm(unsigned interfaz);

      //VPV Added  
      virtual Boolean bubbleReady() const;  
      // Run time information
      DEFINE_RTTI(PRZCTFifoMuxedFlow);
};

//*************************************************************************


#endif


// fin del fichero
