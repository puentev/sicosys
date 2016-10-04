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
// File: ../ATCSimul/src/PRZRouterBuilder.cpp
//
// Clase      : PRZRouterBuilder
//
// Hereda de  : PRZBuilder
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRouterBuilder.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#include <PRZRouterBuilder.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
#endif

#ifndef __PRZMultiportFifo_HPP__
#include <PRZMultiportFifo.hpp>
#endif

#ifndef __PRZFifoMuxed_HPP__
#include <PRZFifoMuxed.hpp>
#endif

#ifndef __PRZRouting_HPP__
#include <PRZRouting.hpp>
#endif

#ifndef __PRZRoutingMuxed_HPP__
#include <PRZRoutingMuxed.hpp>
#endif

#ifndef __PRZCrossbar_HPP__
#include <PRZCrossbar.hpp>
#endif

#ifndef __PRZMultiplexorCV_HPP__
#include <PRZMultiplexorCV.hpp>
#endif
/*
#ifndef __PRZDemultiplexor_HPP__
#include <PRZDemultiplexor.hpp>
#endif
*/
#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZInjector_HPP__
#include <PRZInjector.hpp>
#endif

#ifndef __PRZConsumer_HPP__
#include <PRZConsumer.hpp>
#endif

#ifndef __SFSSimpleRouter_HPP__
#include <SFSSimpleRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRouterBuilder,PRZBuilder);

//*************************************************************************
//:
//  f: PRZRouterBuilder(const PRZString& sgmlFile);
//
//  d:
//:
//*************************************************************************

PRZRouterBuilder :: PRZRouterBuilder(const PRZString& sgmlFile)
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

PRZComponent* PRZRouterBuilder::parseComponentDefinition( const PRZTag* tag,
                                                          PRZComponent* owner,
                                                          INDEX& index )
{
   PRZString tagName = tag->tagName();
   PRZComponent* rComponent = 0;

   PRZString err("PRZRouterBuilder :: ");
   err += tagName + " is unimplemented\n";
   
   if( tagName == PRZ_TAG_ROUTER )
   {
      rComponent = PRZRouter::newFrom(tag,owner,index);
   }
   else if( tagName == PRZ_TAG_SIMPLE_ROUTER )
   {
      rComponent = SFSSimpleRouter::newFrom(tag,owner,index);
   }
   else if( tagName == PRZ_TAG_BUFFER )
   {
      rComponent = PRZFifoMemory::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_MPBUFFER )
   {
      rComponent = PRZMultiportFifo ::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_FIFOMUXED )
   {
      rComponent = PRZFifoMuxed ::newFrom(tag,owner);
   } 
   else if( tagName == PRZ_TAG_ROUTING )
   {
      rComponent = PRZRouting::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_ROUTING_MUXED )
   {
      rComponent = PRZRoutingMuxed::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_CROSSBAR )
   {
      rComponent = PRZCrossbar::newFrom(tag,owner,index);
   }
   else if( tagName == PRZ_TAG_MUXCV )
   {
      rComponent = PRZMultiplexorCV::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_DEMUX )
   {
      EXIT_PROGRAM(err);
//      rComponent = PRZDemultiplexor::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_CONNECTION )
   {
      PRZConnection::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_INJECTOR )
   {
      rComponent = PRZInjector::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_CONSUMER )
   {
      rComponent = PRZConsumer::newFrom(tag,owner);
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZRBLDR_001, (char*)tagName );
      EXIT_PROGRAM(err);
   }

   return rComponent;
}


//*************************************************************************


// fin del fichero
