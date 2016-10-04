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
// File: ../ATCSimul/src/PRZFifoMuxed.cpp
//
// Clase      : PRZFifoMuxed
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFifoMuxed.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZFifoMuxed.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZFifoMuxed.hpp>

#ifndef __PRZCTFifoMuxedFlow_HPP__
#include <PRZCTFifoMuxedFlow.hpp>
#endif

#ifndef __PRZFifoMuxedFlow_HPP__
#include <PRZFifoMuxedFlow.hpp>
#endif


#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZFifoMuxed,PRZRunnableComponent);

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
//  f: PRZFifoMuxed(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZFifoMuxed :: PRZFifoMuxed( const PRZComponentId& id,
                                unsigned bufferSize, unsigned numberPorts,
                                PRZBufferControl control )
               : PRZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control),
                 m_outputs(numberPorts)
{
     setNumberOfOutputs(numberPorts);
}


//*************************************************************************
//:
//  f: virtual ~PRZFifoMuxed();
//
//  d:
//:
//*************************************************************************

PRZFifoMuxed :: ~PRZFifoMuxed()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZFifoMuxed :: buildFlowControl()
{
   if( getBufferControl() == PRZFifoMuxed::CT )
   {
      setFlowControl( new PRZCTFifoMuxedFlow(*this) );
   }
   else  
   {
      PRZString err;
      err.sprintf("Only CutThrough implemented");
      EXIT_PROGRAM(err);  
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

unsigned PRZFifoMuxed :: bufferHoles() const
{
   return ((PRZFifoMuxedFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: unsigned bufferOccupation() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZFifoMuxed :: bufferOccupation() const
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

void PRZFifoMuxed :: setBufferSize(unsigned size)
{
   m_BufferSize = size+2;
   if( getFlowControl() )
   {
      ((PRZFifoMuxedFlow*)getFlowControl()) -> setBufferSize(size);
   }
}

//*************************************************************************
//:
//  f:  virtual Boolean bubbleReady() const;
//
//  d:  Return bubble information for Routing decision unit.
//:
//*************************************************************************

Boolean PRZFifoMuxed ::bubbleReady() const
{

      return ((PRZFifoMuxedFlow*)getFlowControl()) -> bubbleReady();

}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZFifoMuxed :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZFifoMuxed* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZFifoMuxed* PRZFifoMuxed :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idBuffer(*tag);
   PRZString bufferSize, bufferControl, dataDelay, outputs, acksGranularity;

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
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_OUTPUTS, outputs)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }

   PRZBufferControl control = PRZFifoMuxed::NIL;
   if( bufferControl == PRZ_TAG_CUTHROUGH )
   {
      control = PRZFifoMuxed::CT;
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   unsigned outs = outputs.asInteger();   
   PRZFifoMuxed* buffer = new PRZFifoMuxed(idBuffer,size,outs,control);
   buffer->setDataDelay(delay-1);
   buffer->setGranularity(granularity);
  
   return buffer;
}

//*************************************************************************

// fin del fichero
