//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZSimulationBuilder.cpp
//
// Clase      : PRZSimulationBuilder
//
// Hereda de  : PRZBuilder
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZSimulationBuilder.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#include <PRZSimulationBuilder.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZSimulationDual_hpp__
#include <PRZSimulationDual.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZSimulationBuilder,PRZBuilder);

//*************************************************************************
//:
//  f: PRZSimulationBuilder( const PRZString& sgmlFile);
//
//  d:
//:
//*************************************************************************

PRZSimulationBuilder :: PRZSimulationBuilder(const PRZString& sgmlFile)
                      : PRZBuilder(sgmlFile)
{

}


//*************************************************************************
//:
//  f: PRZComponent* parseComponentDefinition( const PRZTag* tag,
//                                             PRZComponent* owner,
//                                             INDEX& index );
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZSimulationBuilder::parseComponentDefinition( const PRZTag* tag,
                                                              PRZComponent* owner,
                                                              INDEX& index )
{
   PRZString tagName = tag->tagName();
   PRZComponent* rComponent = 0;
   
   if( tagName == PRZ_TAG_SIMULATION )
   {
      rComponent = PRZSimulation::newFrom(tag,index);
   }
   else if(tagName == PRZ_TAG_SIMULATION_DUAL)
   {                  
      rComponent = PRZSimulationDual::newFrom(tag,index);
   }

   return rComponent;
}


//*************************************************************************


// fin del fichero
