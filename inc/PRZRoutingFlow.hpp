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
// File: ../ATCSimul/inc/PRZRoutingFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingFlow_HPP__
#define __PRZRoutingFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>
   
   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//*************************************************************************

   class PRZRoutingFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;

   public:
      PRZRoutingFlow( PRZComponent& component);

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      

      // Run time information
      DEFINE_RTTI(PRZRoutingFlow);
      
   protected:
      virtual Boolean updateMessageInfo(PRZMessage* msg) = 0;
      Boolean         cutCurrentHeader(const PRZMessage* msg);
      Boolean         changeDirection(const PRZMessage* msg);
      unsigned        getDirection() const;
      virtual Boolean dispatchEvent(const PRZEvent& event);

      void setHeader(Boolean val)
      { m_NextHeader = val; }
      
      Boolean isHeader() const
      { return m_NextHeader; }

      void setWaiting(Boolean val)
      { m_Waiting = val; }
      
      Boolean isWaiting() const
      { return m_Waiting; }
      
      

      
      PRZMessage* m_Message;
      
   private:
      Boolean     m_NextHeader;
      Boolean     m_Waiting;
};

//*************************************************************************


#endif


// fin del fichero
