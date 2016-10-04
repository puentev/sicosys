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
// File: ../ATCSimul/inc/PRZMultiplexorCV.hpp
//
// Description:  fichero de encabezado de la clase "PRZMultiplexorCV"
//
// Notes:  Esta clase representa la funcionalidad del multiplexor de
//              canales virtuales. La parte dinamica se delega sobre su 
//              flujo asociado. Un multiplexor puede tener capacidad de
//              almacenamiento para, al menos, 1 flit.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiplexorCV.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMultiplexorCV_HPP__
#define __PRZMultiplexorCV_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>


//************************************************************************


   class PRZMultiplexorCV : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;
      
   public:
      PRZMultiplexorCV(const PRZComponentId& id);
      virtual PRZString asString() const;

      void setBufferSize(unsigned size)
      { m_BufferSize = size; }

      unsigned getBufferSize() const
      { return m_BufferSize; }
         
      
      // Run time information
      DEFINE_RTTI(PRZMultiplexorCV);
   
   protected:      
      virtual void buildFlowControl();
      virtual void buildOutputInterfaz(unsigned number=1);
   private:
      static PRZMultiplexorCV* newFrom(const PRZTag* tag, PRZComponent* owner);
      unsigned m_BufferSize;
      unsigned m_ctmux;

   };

//*************************************************************************

#endif


// fin del fichero
