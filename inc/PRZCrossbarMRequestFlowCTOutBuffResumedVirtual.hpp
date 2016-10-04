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
// File: ../ATCSimul/inc/PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.hpp
//
// Description:  Crossbar para buffer hibrido con redes de escape en 
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarMRequestFlowCTOutBuffResumedVirtual_HPP__
#define __PRZCrossbarMRequestFlowCTOutBuffResumedVirtual_HPP__

//*************************************************************************

   #include <PRZCrossbarMRequestFlowCTOutBuffResumed.hpp>

//*************************************************************************

   class PRZCrossbarMRequestFlowCTOutBuffResumedVirtual : public PRZCrossbarMRequestFlowCTOutBuffResumed
   {
      typedef PRZCrossbarMRequestFlowCTOutBuffResumed Inhereited;
      
   public:
      
      PRZCrossbarMRequestFlowCTOutBuffResumedVirtual( PRZComponent& component);
     
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
     
      // Run time information
      DEFINE_RTTI(PRZCrossbarMRequestFlowCTOutBuffResumedVirtual);

   protected:
      unsigned extractOutputPortNumberOBRE( przROUTINGTYPE type,unsigned channel,unsigned input,unsigned channel_input );  
};

//*************************************************************************


#endif


// fin del fichero
