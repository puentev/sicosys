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
// File: ../ATCSimul/inc/PRZMultiportFifo.hpp
//
// Description:  fichero de encabezado de la clase "PRZFifoMemory"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiportFifo.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************
#ifndef __PRZMultiportFifo_HPP__
#define __PRZMultiportFifo_HPP__
//************************************************************************
 #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;
   
   class PRZMultiportFifo : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;

   public:   
      typedef enum {  CT, WH, CT_NF, NIL } PRZBufferControl; //PRZQueueControl;
         
      PRZMultiportFifo ( const PRZComponentId& id,
                                unsigned bufferSize, unsigned numberPorts,
                                PRZBufferControl control );
                     
      virtual ~PRZMultiportFifo();
   
      virtual PRZString asString() const;
      
      unsigned bufferHoles() const;
      
      virtual unsigned bufferHolesShort() const
      {return 0;}

      unsigned numberOfInputs () const
      {return m_inputs;}
      
      unsigned bufferOccupation() const;
      
      PRZBufferControl getBufferControl() const
      { return m_BufferControl; }
      
      unsigned getBufferSize() const
      { return m_BufferSize; }

      unsigned getBufferSizeShort() const
      { return m_BufferSizeShort; }
            
      void setBufferSize(unsigned size);
      
      void setBufferSizeShort(unsigned size);

      //VPV Added  
      virtual Boolean bubbleReady() const;
      
      // Run time information
      DEFINE_RTTI(PRZMultiportFifo);
   
   protected:      
      virtual void buildFlowControl();

   private:
      static PRZMultiportFifo* newFrom(const PRZTag* tag, PRZComponent* owner);

      unsigned m_BufferSize;
      unsigned m_BufferSizeShort;
      unsigned m_inputs;
      PRZBufferControl m_BufferControl;
   };

//*************************************************************************

#endif
