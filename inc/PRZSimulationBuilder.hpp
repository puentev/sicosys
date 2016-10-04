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
// File: ../ATCSimul/inc/PRZSimulationBuilder.hpp
//
// Description:  fichero de encabezado de la clase "PRZSimulationBuilder"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZSimulationBuilder.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZSimulationBuilder_HPP__
#define __PRZSimulationBuilder_HPP__

//*************************************************************************

   #ifndef __PRZBuilder_HPP__
   #include <PRZBuilder.hpp>
   #endif
   
//*************************************************************************

   class PRZSimulationBuilder : public PRZBuilder
   {
     
   public:
      PRZSimulationBuilder(const PRZString& sgmlFile);

      virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
                                                      PRZComponent* owner,
                                                      INDEX& index );
      // Define RTTI
      DEFINE_RTTI(PRZSimulationBuilder);      
};

//****************************************************


#endif


// fin del fichero
