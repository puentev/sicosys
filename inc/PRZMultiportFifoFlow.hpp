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
// File: ../ATCSimul/inc/PRZMultiportFifoFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZMultiportFifoFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiportFifoFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMultiportFifoFlow_HPP__
#define __PRZMultiportFifoFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>
   
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

   class PRZMultiportFifoFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;

   public:
      PRZMultiportFifoFlow( PRZComponent& component);
      virtual ~PRZMultiportFifoFlow();
      
      virtual void initialize();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      //VPV Added  
      virtual Boolean bubbleReady() const;   
      void setBufferSize(unsigned size)
      { m_Size = size; }
  
      // Run time information
      DEFINE_RTTI(PRZMultiportFifoFlow);
      
      unsigned bufferElements() const;
      
      unsigned bufferSize() const
      { return m_Size; }
      


      unsigned bufferHoles() const
      { return m_Size - bufferElements(); }   
      
                   

   protected:
      virtual void    sendFlit(PRZMessage* msg);
      unsigned    m_Size;
      unsigned    m_inputs;
      Boolean     m_SendStop;
      QueueFlits *m_memory;
      unsigned    m_outLast;
      unsigned    m_payload;
      unsigned    m_lpack;           
   private:
      virtual Boolean propagateStop();
      

      
};

//*************************************************************************


#endif


// fin del fichero
