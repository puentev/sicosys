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
// File: ../ATCSimul/inc/PRZWHFifoMemoryFlow.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZWHFifoMemoryFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZWHFifoMemoryFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZWHFifoMemoryFlow_HPP__
#define __PRZWHFifoMemoryFlow_HPP__

//*************************************************************************

   #include <PRZFifoMemoryFlow.hpp>

//*************************************************************************

   class PRZWHFifoMemoryFlow : public PRZFifoMemoryFlow
   {
   public:
      PRZWHFifoMemoryFlow(PRZComponent& component);

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);

      // Run time information
      DEFINE_RTTI(PRZWHFifoMemoryFlow);

};

//*************************************************************************


#endif


// fin del fichero
