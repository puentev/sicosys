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
// File: ../ATCSimul/inc/PRZFifoMuxed.hpp
//
// Description:  fichero de encabezado de la clase "PRZFifoMemory"
//
// Notes:  Fifos multiplexadas para crossbar multiplexados
//              incorporan el manejo de canales virtuales para
//              la evitacion del HLB. En realidad no son mas que fifos en 
//              horizontal. 
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFifoMuxed.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************
#ifndef __PRZFifoMuxed_HPP__
#define __PRZFifoMuxed_HPP__
//************************************************************************
 #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;
   
   class PRZFifoMuxed : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;

   public:   
      typedef enum {  CT, WH,NIL } PRZBufferControl; //PRZQueueControl;
         
      PRZFifoMuxed ( const PRZComponentId& id,
                                unsigned bufferSize, unsigned numberPorts,
                                PRZBufferControl control );
                     
      virtual ~PRZFifoMuxed();
   
      virtual PRZString asString() const;
      
      unsigned bufferHoles() const;
      
      unsigned numberOfOutputs () const
      {return m_outputs;}
      
      unsigned bufferOccupation() const;
      
      PRZBufferControl getBufferControl() const
      { return m_BufferControl; }
      
      unsigned getBufferSize() const
      { return m_BufferSize; }

      void setBufferSize(unsigned size);

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
      DEFINE_RTTI(PRZFifoMuxed);
   
   protected:      
      virtual void buildFlowControl();

   private:
      static PRZFifoMuxed* newFrom(const PRZTag* tag, PRZComponent* owner);

      unsigned m_BufferSize;
      unsigned m_outputs;
      PRZBufferControl m_BufferControl;
      unsigned m_granularity;
   };

//*************************************************************************

#endif
