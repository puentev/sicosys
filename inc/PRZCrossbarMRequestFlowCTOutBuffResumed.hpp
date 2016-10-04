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
// File: ../ATCSimul/inc/PRZCrossbarMRequestFlowCTOutBuffResumed.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbarMRequestFlowCTOutBuffResumed"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestFlowCTOutBuffResumed.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarMRequestFlowCTOutBuffResumed_HPP__
#define __PRZCrossbarMRequestFlowCTOutBuffResumed_HPP__

//*************************************************************************

   #include <PRZCrossbarFlow.hpp>

//*************************************************************************

   class PRZCrossbarMRequestFlowCTOutBuffResumed : public PRZCrossbarFlow
   {
      typedef PRZCrossbarFlow Inhereited;
      
   public:
      
      PRZCrossbarMRequestFlowCTOutBuffResumed( PRZComponent& component);
      virtual ~PRZCrossbarMRequestFlowCTOutBuffResumed();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual void    run(uTIME time);
      virtual void    preRun(uTIME time);
      void removeAllPassEventsForInput(unsigned iPort);
      // Run time information
      DEFINE_RTTI(PRZCrossbarMRequestFlowCTOutBuffResumed);

   protected:
      virtual void     cleanInputInterfaces();
      virtual Boolean  dispatchEvent(const PRZEvent& event);
      virtual Boolean  isOutputPortFree(unsigned outputPort, unsigned channel=0) const;
      virtual void     cleanConnectionRequestFor(unsigned port, const uPAIR& pair);
      virtual Boolean  updateMessageInfo(PRZMessage* msg, unsigned oPort, unsigned channel);  
      unsigned extractOutputPortNumberOBRE( przROUTINGTYPE type,unsigned channel,unsigned input,unsigned channel_input );  
};

//*************************************************************************


#endif


// fin del fichero
