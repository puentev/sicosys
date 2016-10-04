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
// File: ../ATCSimul/inc/PRZFifoMemoryFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZFifoMemoryFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFifoMemoryFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZFifoMemoryFlow_HPP__
#define __PRZFifoMemoryFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//*************************************************************************

   class PRZFifoMemoryFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;

   public:
      PRZFifoMemoryFlow( PRZComponent& component);

      virtual void initialize();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      //VPV Added  
      virtual Boolean bubbleReady() const;   
      void setBufferSize(unsigned size)
      { m_Size = size+2; }
  
      // Run time information
      DEFINE_RTTI(PRZFifoMemoryFlow);

      unsigned bufferSize() const
      { return m_Size; }
      
      unsigned bufferElements() const
      { return getEventQueue().numberOfElements(); }

      unsigned bufferHoles() const
      { return m_Size - bufferElements(); }       

   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);
      virtual void    sendFlit(PRZMessage* msg);
                 
   private:
      virtual Boolean propagateStop();

      unsigned  m_Size;
      Boolean   m_SendStop;
};

//*************************************************************************


#endif


// fin del fichero
