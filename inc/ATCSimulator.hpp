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
// File: ../ATCSimul/inc/ATCSimulator.hpp
//
// Description:  fichero de encabezado de la clase "ATCSimulator"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: ATCSimulator.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __ATCSimulator_HPP__
#define __ATCSimulator_HPP__

//************************************************************************

   #include <PRZNotifier.hpp>

   #ifndef __PRZSimulation_HPP__
   #include <PRZSimulation.hpp>
   #endif

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif

//************************************************************************

   class ATCSimulator : public PRZNotifier
   {
      typedef PRZNotifier Inhereited;
      typedef PRZArray<PRZSimulation*> TSimulationArray;
      
   public:
      ~ATCSimulator();
      static ATCSimulator* instance();
      static void destroyInstance();

      unsigned       createSimulation(int argc, char* argv[]);
      unsigned       createSimulation(const PRZString& param);
      PRZSimulation* getSimulation(unsigned index) const;
      void           deleteSimulation(unsigned index);
      void           deleteAllSimulations();

      // Run time information
      DEFINE_RTTI(ATCSimulator);

   private:
      ATCSimulator();
      void           installBuilders(const PRZString& name);
      void           uninstallBuilders();
      PRZSimulation* initialize(const PRZString& param);
      
      static ATCSimulator* m_Instance;
      TSimulationArray*    m_SimulationArray;
      void (*m_OldHandler)(int i);
   };

//*************************************************************************

   #define ATCSIMULATOR()  ATCSimulator::instance()

//*************************************************************************

#endif

// fin del fichero
