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
// File: ../ATCSimul/src/PRZWHFifoMemoryFlow.cpp
//
// Clase      : PRZWHFifoMemoryFlow
//
// Hereda de  : PRZFifoMemoryFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZWHFifoMemoryFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZWHFifoMemoryFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZWHFifoMemoryFlow.hpp>

#ifndef __PRZNetwork_HPP__
#include <PRZNetwork.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZWHFifoMemoryFlow, PRZFifoMemoryFlow);


//*************************************************************************
//:
//  f: PRZWHFifoMemoryFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZWHFifoMemoryFlow :: PRZWHFifoMemoryFlow(PRZComponent& component)
                     : PRZFifoMemoryFlow(component)
{

}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info = false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZWHFifoMemoryFlow :: controlAlgoritm(Boolean info, int delta)
{
   unsigned delay = PRZNETWORK->getConnectionDelay() + 1;

   return ( bufferHoles() > delay ) ? true : false;
}


//*************************************************************************


// fin del fichero
