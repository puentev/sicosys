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
// File: ../ATCSimul/inc/PRZCrossbarMuxFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbarMuxFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMuxFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarMuxFlow_HPP__
#define __PRZCrossbarMuxFlow_HPP__

//*************************************************************************

   #include <PRZCrossbarFlow.hpp>

//*************************************************************************

   class PRZCrossbarMuxFlow : public PRZCrossbarFlow
   {
      typedef PRZCrossbarFlow Inhereited;
      
   public:
      
      PRZCrossbarMuxFlow( PRZComponent& component);

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual void    postRun(uTIME time);
      
      // Run time information
      DEFINE_RTTI(PRZCrossbarFlow);

   protected:
      virtual void     cleanInputInterfaces();
      virtual Boolean  dispatchEvent(const PRZEvent& event);
      virtual unsigned extractOutputPortNumber(PRZMessage* msg);
      virtual unsigned extractOutputPortNumber( przROUTINGTYPE dir,
                                                unsigned channel );
      virtual Boolean  isOutputPortFree(unsigned outputPort, unsigned channel=0) const;         
      virtual Boolean  updateMessageInfo(PRZMessage* msg, unsigned oPort, unsigned channel);
};

//*************************************************************************


#endif


// fin del fichero
