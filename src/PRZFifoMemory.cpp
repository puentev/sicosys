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
// File: ../ATCSimul/src/PRZFifoMemory.cpp
//
// Clase      : PRZFifoMemory
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFifoMemory.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZFifoMemory.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZFifoMemory.hpp>

#ifndef __PRZWHFifoMemoryFlow_HPP__
#include <PRZWHFifoMemoryFlow.hpp>
#endif

#ifndef __PRZCTFifoMemoryFlow_HPP__
#include <PRZCTFifoMemoryFlow.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZFifoMemory,PRZRunnableComponent);

//*************************************************************************
//:
//  f: inline PRZBufferControl getBufferControl() const;
//  d:
//
//  f: inline unsigned getBufferSize() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZFifoMemory(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZFifoMemory :: PRZFifoMemory( const PRZComponentId& id,
                                unsigned bufferSize,
                                PRZBufferControl control )
               : PRZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZFifoMemory();
//
//  d:
//:
//*************************************************************************

PRZFifoMemory :: ~PRZFifoMemory()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZFifoMemory :: buildFlowControl()
{
   if( getBufferControl() == PRZFifoMemory::WH )
   {
      setFlowControl( new PRZWHFifoMemoryFlow(*this) );
   }
   else if( getBufferControl() == PRZFifoMemory::CT )
   {
      setFlowControl( new PRZCTFifoMemoryFlow(*this) );
   }
   
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZFIFO_001,getBufferControl());
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: unsigned bufferHoles() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZFifoMemory :: bufferHoles() const
{
   return ((PRZFifoMemoryFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: unsigned bufferOccupation() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZFifoMemory :: bufferOccupation() const
{
   return (getBufferSize()-bufferHoles());
}


//*************************************************************************
//:
//  f: void setBufferSize(unsigned size);
//
//  d:
//:
//*************************************************************************

void PRZFifoMemory :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((PRZFifoMemoryFlow*)getFlowControl()) -> setBufferSize(size);
   }
}

//*************************************************************************
//:
//  f:  virtual Boolean bubbleReady() const;
//
//  d:  Return bubble information for Routing decision unit.
//:
//*************************************************************************

Boolean PRZFifoMemory ::bubbleReady() const
{

      return ((PRZFifoMemoryFlow*)getFlowControl()) -> bubbleReady();

}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZFifoMemory :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZFifoMemory* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZFifoMemory* PRZFifoMemory :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idBuffer(*tag);
   PRZString bufferSize, bufferControl, dataDelay, acksGranularity;

   unsigned size, granularity=0, delay=1;
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZE, bufferSize) )
   {
      size = bufferSize.asInteger();
   }
   else
   {
      size = ((PRZRouter*)owner)->valueForBufferSize();
   }

   if( tag->getAttributeValueWithName(PRZ_TAG_ACKGRANUL, acksGranularity) )
   {
      granularity = acksGranularity.asInteger();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(PRZ_TAG_CONTROL, bufferControl)) )
   {
      bufferControl = ((PRZRouter*)owner)->valueForBufferControl();
   }


   PRZBufferControl control = PRZFifoMemory::NIL;
   if( bufferControl == PRZ_TAG_WORMHOLE )
   {
      control = PRZFifoMemory::WH;
   }
   else if( bufferControl == PRZ_TAG_CUTHROUGH )
   {
      control = PRZFifoMemory::CT;
   }
   else if( bufferControl == PRZ_TAG_VIRTUAL_CT )
   {
      control = PRZFifoMemory::VCT;
   }
   else if( bufferControl == PRZ_TAG_STOREFOR )
   {
      control = PRZFifoMemory::SF;
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   PRZFifoMemory* buffer = new PRZFifoMemory(idBuffer,size,control);
   buffer->setDataDelay(delay-1);
   
   ((PRZRouter*)owner)->addBuffer(buffer);
   buffer->setGranularity(granularity);
   return buffer;
}

//*************************************************************************

// fin del fichero
