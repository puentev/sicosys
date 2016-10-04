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
// File: ../ATCSimul/inc/PRZRoutingCVFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingCVFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingCVFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingCVFlow_HPP__
#define __PRZRoutingCVFlow_HPP__

//*************************************************************************

   #include <PRZRoutingFlow.hpp>
   
   #ifndef __PRZPosition_HPP__
   #include <PRZPosition.hpp>
   #endif
   
//*************************************************************************

   class PRZRoutingCVFlow : public PRZRoutingFlow
   {
      typedef PRZRoutingFlow Inhereited;
            
   public:
      PRZRoutingCVFlow( PRZComponent& component);

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
            
      // Run time information
      DEFINE_RTTI(PRZRoutingCVFlow);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);


   };

//*************************************************************************


#endif


// fin del fichero
