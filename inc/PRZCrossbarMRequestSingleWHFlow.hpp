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
// File: ../ATCSimul/inc/PRZCrossbarMRequestSingleWHFlow.hpp
//
// Description:  fichero de encabezado de la clase 
//              "PRZCrossbarMRequestSingleWHFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarMRequestSingleWHFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarMRequestSingleWHFlow_HPP__
#define __PRZCrossbarMRequestSingleWHFlow_HPP__

//*************************************************************************
   
   #include <PRZCrossbarFlow.hpp>

//*************************************************************************

   class PRZCrossbarMRequestSingleWHFlow : public PRZCrossbarFlow
   {
      typedef PRZCrossbarFlow Inhereited;
      
   public:
      
      PRZCrossbarMRequestSingleWHFlow( PRZComponent& component);
      virtual ~PRZCrossbarMRequestSingleWHFlow();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean outputWriting();
      
      // Run time information
      DEFINE_RTTI(PRZCrossbarMRequestSingleWHFlow);

   protected:
      virtual void     cleanInputInterfaces();
      virtual Boolean  isOutputPortFree(unsigned outputPort, unsigned channel=0) const;
      virtual void     cleanConnectionRequestFor(unsigned port, const uPAIR& pair);
      virtual Boolean  updateMessageInfo(PRZMessage* msg, unsigned oPort, unsigned channel);
            
   private:
      unsigned m_Token;
      TPortList** m_RequestInput;
};

//*************************************************************************


#endif


// fin del fichero
