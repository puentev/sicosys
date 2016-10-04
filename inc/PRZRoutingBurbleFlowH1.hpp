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
// File: ../ATCSimul/inc/PRZRoutingBurbleFlowH1.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingBurbleFlowH1"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingBurbleFlowH1.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingBurbleFlowH1_HPP__
#define __PRZRoutingBurbleFlowH1_HPP__

//*************************************************************************

   #include <PRZRoutingBurbleFlow.hpp>
   
//*************************************************************************

   class PRZRoutingBurbleFlowH1 : public PRZRoutingBurbleFlow
   {
      typedef PRZRoutingBurbleFlow Inhereited;
            
   public:
      PRZRoutingBurbleFlowH1( PRZComponent& component);
      Boolean cutCurrentHeader(const PRZMessage& msg)
      {
         return false;
      }
            virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);

      // Run time information
      DEFINE_RTTI(PRZRoutingBurbleFlowH1);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg);
   };

//*************************************************************************


#endif


// fin del fichero
