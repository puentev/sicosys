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
// File: ../ATCSimul/inc/PRZFifoMemory.hpp
//
// Description:  fichero de encabezado de la clase "PRZFifoMemory"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFifoMemory.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZFifoMemory_HPP__
#define __PRZFifoMemory_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;

//************************************************************************

   class PRZFifoMemory : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;

   public:   
      typedef enum { NIL, WH, CT, VCT, SF } PRZBufferControl;
         
      PRZFifoMemory( const PRZComponentId& id, 
                     unsigned size,
                     PRZBufferControl control );
                     
      virtual ~PRZFifoMemory();
   
      virtual PRZString asString() const;
      
      unsigned bufferHoles() const;
      unsigned bufferOccupation() const;
      
      PRZBufferControl getBufferControl() const
      { return m_BufferControl; }
      
      unsigned getBufferSize() const
      { return m_BufferSize; }

      void setBufferSize(unsigned size);
      //VPV Added  
      virtual Boolean bubbleReady() const;
      
      void setGranularity(unsigned a)
      {
         m_granularity=a;
      } 
      
      unsigned getGranularity() const
      {
         return m_granularity;
      }        
      
      // Run time information
      DEFINE_RTTI(PRZFifoMemory);
   
   protected:      
      virtual void buildFlowControl();

   private:
      static PRZFifoMemory* newFrom(const PRZTag* tag, PRZComponent* owner);

      unsigned m_BufferSize;
      PRZBufferControl m_BufferControl;
      unsigned m_granularity;
   };

//*************************************************************************

#endif


// fin del fichero
