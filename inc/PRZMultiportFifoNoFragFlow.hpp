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
// File: ../ATCSimul/inc/PRZMultiportFifoNoFragFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZMultiportFifoNoFragFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiportFifoNoFragFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMultiportFifoNoFragFlow_HPP__
#define __PRZMultiportFifoNoFragFlow_HPP__

//*************************************************************************
   
   #include <PRZMultiportFifoFlow.hpp>
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif  
   
   #ifndef __PRZQueue_HPP__ 
   #include <PRZQueue.hpp>
   #endif 
   
//*************************************************************************

typedef PRZQueue<PRZMessage*> QueueFlits;

   class PRZMultiportFifoNoFragFlow : public PRZMultiportFifoFlow 
   {
      typedef PRZMultiportFifoFlow  Inhereited;

   public:
      PRZMultiportFifoNoFragFlow( PRZComponent& component);
      virtual ~PRZMultiportFifoNoFragFlow();
      
      virtual void initialize();

      virtual Boolean inputReading();
      virtual Boolean outputWriting();
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0)
      {
         return true;
      }
      unsigned bufferElementsShort() const;
      unsigned bufferHolesShort()
      {
         return m_shortcap-bufferElementsShort();
      }
 
      // Run time information
      DEFINE_RTTI(PRZMultiportFifoNoFragFlow);
   protected:
      virtual void    sendFlit(PRZMessage* msg);
      QueueFlits *m_registers;
      unsigned m_shortcap;
      QueueFlits *m_shortMemory;
      Boolean    *m_isShort;
      Boolean   m_lastIsShort;
      

      
};

//*************************************************************************


#endif


// fin del fichero
