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
// File: ../ATCSimul/inc/PRZCTFifoMemoryFlow.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZCTFifoMemoryFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCTFifoMemoryFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCTFifoMemoryFlow_HPP__
#define __PRZCTFifoMemoryFlow_HPP__

//*************************************************************************

   #include <PRZFifoMemoryFlow.hpp>

//*************************************************************************

   class PRZCTFifoMemoryFlow : public PRZFifoMemoryFlow
   {
   public:
      PRZCTFifoMemoryFlow(PRZComponent& component);

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      //VPV Added  
      virtual Boolean bubbleReady() const;  
      // Run time information
      DEFINE_RTTI(PRZCTFifoMemoryFlow);
};

//*************************************************************************


#endif


// fin del fichero
