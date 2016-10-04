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
// File: ../ATCSimul/inc/PRZPort.hpp
//
// Description:  fichero de encabezado de la clase "PRZPort"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZPort.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZPort_HPP__
#define __PRZPort_HPP__

//************************************************************************

   #ifndef __PRZComponent_HPP__
   #include <PRZComponent.hpp>
   #endif
   
   #ifndef __PRZConnection_HPP__
   #include <PRZConnection.hpp>
   #endif

   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif
   
//************************************************************************

   class PRZGauge;

//************************************************************************

   class PRZPort : public PRZComponent
   {
      typedef PRZComponent Inhereited;

   public:
      PRZPort( const PRZComponentId& id);
      
      virtual Boolean sendDataRightNow(PRZMessage* msg) =0;
      virtual Boolean clearDataRightNow() =0;
     
      virtual Boolean sendStopRightNow() =0;
      virtual Boolean clearStopRightNow() =0;
      
      Boolean establishConnection(PRZConnection* connection);
      
      virtual void run(uTIME runTime);
      virtual PRZString asString() const;

      void setDataChanged(Boolean change);
      
      Boolean isDataChanged() const
      { return m_DataChanged; }

      Boolean isStopChanged() const
      { return m_StopChanged; }

      void setStopChanged(Boolean change)
      { m_StopChanged = change; }
           
      virtual Boolean sendData(PRZMessage* msg);
      
      virtual Boolean sendStop()
      {
         if( m_Stop ) return false;
         setStopChanged( ((!m_Stop)?true:false) );
         m_Stop = true;
         return true;      
      }

      virtual Boolean clearData()
      { 
         if( ! m_Ready ) return false;
         setDataChanged( ((m_Ready)?true:false) );
         m_Ready = false;
         return true; 
      }
      
      virtual Boolean clearStop()
      {
         if( ! m_Stop ) return false;
         setStopChanged( ((m_Stop)?true:false) );
         m_Stop = false;
         return true;
      }
      
      void setData(PRZMessage* msg)
      { m_MessageStored=msg; }
      
      PRZMessage* getData() const
      { return m_MessageStored; }

      Boolean isStopActive() const
      { return m_Stop; }

      Boolean isReadyActive() const
      { return m_Ready; }
      
      void setGauge(PRZGauge* gauge)
      { m_Gauge = gauge; }
      
      PRZGauge* getGauge() const
      { return m_Gauge; }
      
      // Run time information
      DEFINE_RTTI(PRZPort);

   protected:
      PRZMessage*       m_MessageStored;
      Boolean           m_Ready;
      Boolean           m_Stop;
      Boolean           m_DataChanged;
      Boolean           m_StopChanged;
      PRZGauge*         m_Gauge;
      unsigned          m_CurrentConnection;      

      unsigned getOwnerInterfazId() const;      
};

//************************************************************************


#endif

// fin del fichero
