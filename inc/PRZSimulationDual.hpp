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
// File: ../ATCSimul/inc/PRZSimulationDual.hpp
//
// Description:  fichero de encabezado de la clase "PRZSimulationDual"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZSimulationDual.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZSimulationDual_HPP__
#define __PRZSimulationDual_HPP__

//************************************************************************

   #ifndef __PRZSIMUL_HPP__
   #include "PRZSimulation.hpp"       // PRZSimulation
   #endif

//************************************************************************


   class PRZSimulationDual : public PRZSimulation
   {
      typedef PRZSimulation Inhereited;
      friend class PRZSimulationBuilder;
      
   public:
      PRZSimulationDual(const PRZComponentId& id);
      virtual ~PRZSimulationDual();

      virtual PRZString asString() const;
      virtual void run(uTIME time=0);
      virtual void initialize();
      void writeSimulationStatus(ostream& os);
      virtual void writeResults();      
      
      virtual void setPacketLength(unsigned length,int net=0)
      { if (net==0)Inhereited::setPacketLength(length); else m_PacketLength = length; }

      virtual unsigned getPacketLength(PRZNetwork *net=0) const
      { if(net==m_Network) return m_PacketLength; else return Inhereited::getPacketLength();
       }
                             
      virtual void* getExternalInfoAt(PRZPosition pos, int id=0)
      { return getNetwork(id)->getExternalInfoAt(pos); }

      void setNetwork(int netId, PRZNetwork* net)
      { if(netId==0) Inhereited::setNetwork(net); else m_Network=net; }
                  
      virtual PRZNetwork* getNetwork(int netId=0) const
      { if(netId==0) return Inhereited::getNetwork(); return m_Network; }

      PRZGlobalData& globalData(PRZNetwork* net=0) const
      { if(net==m_Network) return (PRZGlobalData&)m_GlobalData1; else return Inhereited::globalData();}
      
      // Run time information
      DEFINE_RTTI(PRZSimulationDual);
   
   private:   
      static PRZSimulationDual* newFrom( const PRZTag* tag, 
                                         INDEX& i);

      void      writeCurrentData();
      unsigned           m_PacketLength;
      PRZNetwork*        m_Network;
      PRZGlobalData      m_GlobalData1;
   };

//*************************************************************************

#endif


// fin del fichero
