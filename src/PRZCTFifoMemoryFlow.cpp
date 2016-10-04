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
// File: ../ATCSimul/src/PRZCTFifoMemoryFlow.cpp
//
// Clase      : PRZCTFifoMemoryFlow
//
// Hereda de  : PRZFifoMemoryFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCTFifoMemoryFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCTFifoMemoryFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCTFifoMemoryFlow.hpp>

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZCTFifoMemoryFlow, PRZFifoMemoryFlow);


//*************************************************************************
//:
//  f: PRZCTFifoMemoryFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZCTFifoMemoryFlow :: PRZCTFifoMemoryFlow(PRZComponent& component)
                     : PRZFifoMemoryFlow(component)
{

}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZCTFifoMemoryFlow :: controlAlgoritm(Boolean info, int delta)
{
   unsigned sizeForCT;
      
   //Por el retraso del cable quito el igual       
   //cerr<<sizeForCT<<endl;  
   if(((PRZFifoMemory&)getComponent()).getGranularity()==0)
   {
       sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                               getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());   
   }
   else
   {
       sizeForCT = ((PRZFifoMemory&)getComponent()).getGranularity();
   }  
   return ( bufferHoles() > sizeForCT+1 ) ? true : false;
}

//*************************************************************************
//:
//  f:       virtual Boolean bubbleReady() const;   
//  d:       semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean PRZCTFifoMemoryFlow ::bubbleReady()const
 {
   unsigned sizeForCT; 
   if(((PRZFifoMemory&)getComponent()).getGranularity()==0)
   {
       sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                               getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());   
   }
   else
   {
       sizeForCT = ((PRZFifoMemory&)getComponent()).getGranularity();
   }     
     return ( bufferHoles() >= 2*sizeForCT ) ? true : false;
 }
//*************************************************************************


// fin del fichero
