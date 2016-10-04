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
// File: ../ATCSimul/inc/PRZFifoMuxedFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZFifoMemoryFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFifoMuxedFlow.hpp,v $
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
   
   #ifndef __PRZQueue_HPP__ 
   #include <PRZQueue.hpp>
   #endif 
   
//*************************************************************************

typedef PRZQueue<PRZMessage*> QueueFlits;
//*************************************************************************

   class PRZFifoMuxedFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;

   public:
      PRZFifoMuxedFlow( PRZComponent& component);
      ~PRZFifoMuxedFlow();

      virtual void initialize();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      //VPV Added  
      virtual Boolean bubbleReady() const;  
      //virtual Boolean controlAlgoritm(Boolean info, int delta) const;
 
      void setBufferSize(unsigned size)
      { m_Size = size; }
  
      // Run time information
      DEFINE_RTTI(PRZFifoMuxedFlow);

      unsigned bufferSize() const
      { return m_Size; }
      
      unsigned bufferElements() const;

      unsigned bufferElements(unsigned interfaz) const;
      
      unsigned bufferHoles() const     
      { return m_Size+m_outputs - bufferElements(); }  
 
      unsigned bufferHoles(unsigned interfaz) const
      { return m_Size/m_outputs+1 - bufferElements(interfaz); }  
 
   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);
      virtual void    sendFlit(PRZMessage* msg, unsigned interfaz);
                 
   private:
      virtual Boolean propagateStop();
      QueueFlits *m_memory;
      unsigned  m_Size;
      Boolean   m_SendStop;
      unsigned  m_outputs;
      unsigned  m_lastOutput;
      unsigned  m_partialStoring;
};

//*************************************************************************


#endif


// fin del fichero
