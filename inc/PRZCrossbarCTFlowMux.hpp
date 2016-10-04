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
// File: ../ATCSimul/inc/PRZCrossbarCTFlowMux.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbarCTFlowMux"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarCTFlowMux.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarCTFlowMux_HPP__
#define __PRZCrossbarCTFlowMux_HPP__

//*************************************************************************

   #include <PRZCrossbarFlow.hpp>

//*************************************************************************

   class PRZCrossbarCTFlowMux : public PRZCrossbarFlow
   {
      typedef PRZCrossbarFlow Inhereited;
      
   public:
      PRZCrossbarCTFlowMux( PRZComponent& component);
      virtual ~PRZCrossbarCTFlowMux();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      // Run time information
      DEFINE_RTTI(PRZCrossbarCTFlowMux);

   protected:
     virtual Boolean dispatchEvent(const PRZEvent& event);
     virtual unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
     virtual unsigned extractOutputPortNumber(PRZMessage* msg);
};

//*************************************************************************


#endif


// fin del fichero
