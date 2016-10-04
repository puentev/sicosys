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
// File: ../ATCSimul/inc/PRZRunnableComponent.hpp
//
// Description:  fichero de encabezado de la clase "PRZRunnableComponent"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRunnableComponent.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRunnableComponent_HPP__
#define __PRZRunnableComponent_HPP__

//************************************************************************

   #include <PRZCompositeComponent.hpp>

   #ifndef __PRZFlow_HPP__
   #include <PRZFlow.hpp>
   #endif

//************************************************************************

   class PRZRunnableComponent : public PRZCompositeComponent
   {
      typedef PRZCompositeComponent Inhereited;

   public:
      PRZRunnableComponent(const PRZComponentId& id);
      virtual ~PRZRunnableComponent();

      virtual void      run(uTIME time=0);
      virtual void      preRun(uTIME time);
      virtual void      postRun(uTIME time);
      virtual void      initialize();
      virtual void      postInitialize();
      
      virtual Boolean   onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean   onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean   onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean   onStopDown(unsigned interfaz, unsigned cv);

      virtual PRZString getStatus() const;
      virtual PRZString asString() const;

      void setNumberOfInputs(unsigned inputs)
      { m_Inputs = inputs; }

      void setNumberOfOutputs(unsigned outputs)
      { m_Outputs = outputs; }

      unsigned numberOfInputs()  const
      { return m_Inputs; }
      
      unsigned numberOfOutputs() const
      { return m_Outputs; }

      uTIME getHeaderDelay() const
      { return m_HeaderDelay; }

      uTIME getDataDelay() const
      { return m_DataDelay; }
      
      PRZFlow* getFlowControl() const
      { return m_Flow; }
      
      // Run time information
      DEFINE_RTTI(PRZRunnableComponent);
   
   protected:
      virtual void buildInputInterfaz(unsigned number=1);
      virtual void buildOutputInterfaz(unsigned number=1);
      virtual void buildFlowControl() = 0;

      void setFlowControl(PRZFlow* flow)
      { m_Flow = flow; }
   

      
      void setHeaderDelay(uTIME delay)
      { m_HeaderDelay = delay; }

      void setDataDelay(uTIME delay)
      { m_DataDelay = delay; }
               
   private:
      PRZFlow*      m_Flow;
      unsigned      m_Inputs;
      unsigned      m_Outputs;
      uTIME         m_HeaderDelay;
      uTIME         m_DataDelay;
   };

//*************************************************************************

#endif


// fin del fichero
