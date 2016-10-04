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
// File: ../ATCSimul/inc/PRZMultiplexorCVFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZMultiplexorCVFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMultiplexorCVFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMultiplexorCVFlow_HPP__
#define __PRZMultiplexorCVFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif
   
//*************************************************************************

   class PRZMultiplexorCVFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;
      typedef PRZArray<PRZMessage*> PRZMessageArray;
      enum { Free, Multiplexor };

   public:
      PRZMultiplexorCVFlow( PRZComponent& component);
      ~PRZMultiplexorCVFlow();
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();
      virtual void    postRun(uTIME time);
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      
      unsigned getCurrentOutput() const
      { return m_CurrentOutput; }
      
      void updateCurrentOutput();
      
      // Run time information
      DEFINE_RTTI(PRZMultiplexorCVFlow);
      
   protected:
      void cleanOutputInterfaz();
      
   private:
      PRZMessageArray* m_Messages;
      unsigned         m_CurrentOutput;      
      unsigned         m_MessageToSend;
   };

//*************************************************************************


#endif


// fin del fichero
