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
// File: ../ATCSimul/inc/PRZCrossbarCTFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbarCTFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarCTFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarCTFlow_HPP__
#define __PRZCrossbarCTFlow_HPP__

//*************************************************************************

   #include <PRZCrossbarFlow.hpp>

//*************************************************************************

   class PRZCrossbarCTFlow : public PRZCrossbarFlow
   {
      typedef PRZCrossbarFlow Inhereited;
      
   public:
      PRZCrossbarCTFlow( PRZComponent& component);
      virtual ~PRZCrossbarCTFlow();

      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);

      // Run time information
      DEFINE_RTTI(PRZCrossbarCTFlow);

   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);


};

//*************************************************************************


#endif


// fin del fichero
