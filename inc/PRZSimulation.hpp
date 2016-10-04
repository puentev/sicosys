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
// File: ../ATCSimul/inc/PRZSimulation.hpp
//
// Description:  fichero de encabezado de la clase "PRZSimulation"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZSimulation.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZSIMUL_HPP__
#define __PRZSIMUL_HPP__

//************************************************************************

   #ifndef __PRZCOMPO_HPP__
   #include "PRZComponent.hpp"       // PRZComponent
   #endif

   #ifndef __PRZNET_HPP__
   #include "PRZNetwork.hpp"         // PRZNetwork
   #endif

   #ifndef __PRZGLOBA_HPP__
   #include "PRZGlobalData.hpp"       // PRZGlobalData
   #endif

   #include <time.h>
   #include <unistd.h>
   #include <signal.h>

//************************************************************************

   class PRZSimulationBuilder;
   class PRZTrafficPattern;
   class PRZGauge;
         
//************************************************************************


   class PRZSimulation : public PRZComponent
   {
      typedef PRZComponent Inhereited;
      friend class PRZSimulationBuilder;
      
   public:
      PRZSimulation(const PRZComponentId& id);
      virtual ~PRZSimulation();

      virtual PRZString asString() const;
      virtual void run(uTIME time=0);
      virtual void initialize();
      virtual void writeResults();
      void writeSimulationStatus(ostream& os);
      char* writeSimulationStatus();
      void setTrafficPattern(const PRZString& name);
      void setBimodalTraffic(const PRZString& params);
      void setTrafficPatternFile(const PRZString& name);

      void* getExternalInfoAt(unsigned x, unsigned y, unsigned z=0)
      { return getNetwork()->getExternalInfoAt(x,y,z); }
            
      virtual void* getExternalInfoAt(PRZPosition pos,int id=0)
      { return getNetwork()->getExternalInfoAt(pos); }
            
      uTIME getCurrentTime() const
      { return m_Clock; }
      
      void setCurrentTime(uTIME time) 
      { m_Clock = time; }
      
      void setNetwork(PRZNetwork* net)
      { m_Network = net; }
      
      virtual PRZNetwork* getNetwork(int a=0) const
      { return m_Network; }
      
      void setSimulationCycles(unsigned long cycles)
      { m_SimulationCycles = cycles; }

      unsigned long getSimulationCycles() const
      { return m_SimulationCycles; }

      void setStopSimulation(unsigned long cycles)
      { m_StopSimulation = cycles; }

      void setSeed(unsigned seed)
      { m_Seed = seed; }
      
      void setMessageLength(unsigned length)
      { m_MessageLength = length; }

      unsigned getMessageLength() const
      { return m_MessageLength; }

      virtual void setPacketLength(unsigned length,int net=0)
      { m_PacketLength = length; }

      virtual unsigned getPacketLength(PRZNetwork *net=0) const
      { return m_PacketLength; }

      void setRadius(unsigned radius)
      { m_Radius = radius; }

      unsigned getRadius() const
      { return m_Radius; }

      double defaultLoadSupply() const
      { return m_defaultLoad; }

      void setDefaultLoadSupply(double value)
      { m_defaultLoad = value; }
      
      void setLoadSupply(double value);
      
      double defaultLoadProb() const
      { return m_defaultProb; }

      void setDefaultLoadProb(double value)
      { m_defaultProb = value; }
      
      void setLoadProb(double value);

      void setShowState(Boolean value)
      { m_ShowState = value; }

      void setTrafficPattern(PRZTrafficPattern* pattern)
      { m_TrafficPattern = pattern; }

      void setShowBufferState(Boolean value)
      { m_ShowBufferState = value; }
      
      unsigned getFlitSize( )const
      { return m_flitSize; }
                  
      void setClock(float value)
      { m_clockTR = (unsigned long)value; }
      
      float getClock( )const
      { return m_clockTR; }
                  
      void setFlitSize(unsigned value)
      { m_flitSize = value; }
                  
      void setBuffersFileName(const PRZString& name)
      { m_BuffersFileName = name; }

      void setBufferSize(unsigned size)
      { 
        if(m_Network)
        { 
	   m_Network->setBufferSize(size); 
	   m_bufferSize=size;
	}
     }
     
     unsigned getBufferSize()const
     {return m_bufferSize;}

      PRZGlobalData& globalData(PRZNetwork* net=0) const
      { return (PRZGlobalData&)m_GlobalData; }
      
       void setNumberOfMsgs(unsigned long a)
      {
         m_mensajestraza=a;
      }
      // Run time information
      DEFINE_RTTI(PRZSimulation);
   
   protected:
      PRZTrafficPattern* getTrafficPattern() const
      { return m_TrafficPattern; }

      PRZString buffersFileName() const
      { return m_BuffersFileName; }

      Boolean showBufferState() const
      { return m_ShowBufferState; }
      
      void      writeBufferStatus();
      PRZString convertSeconds(double secs);
      void      writeCurrentData();
            
   private:   
      static PRZSimulation* newFrom( const PRZTag* tag, 
                                     INDEX& i);


        
      PRZNetwork*        m_Network;
      PRZGlobalData      m_GlobalData;

   protected:
      PRZTrafficPattern* m_TrafficPattern;
  
      unsigned long      m_SimulationCycles;
      unsigned long      m_StopSimulation;
      unsigned           m_Seed;
      unsigned           m_MessageLength;
      unsigned           m_PacketLength;
      unsigned           m_Radius;
      unsigned           m_flitSize;
      double             m_q;
      double             m_defaultLoad;
      double             m_defaultProb;
      Boolean            m_ShowState;
      time_t             m_StartHour;
      uTIME              m_Clock;
      uTIME              m_simulatedClock;
      ofstream*          m_BufferOutput;
      Boolean            m_ShowBufferState;
      PRZString          m_BuffersFileName;
      PRZGauge*          m_Gauge;
      unsigned long      m_mensajestraza;
      unsigned long      m_clockTR;
      unsigned           m_bufferSize;
   };

//*************************************************************************

#endif


// fin del fichero
