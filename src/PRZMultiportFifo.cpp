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
// File: ../ATCSimul/src/PRZMultiportFifo.cpp
//
// Clase      : PRZMultiportFifo
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiportFifo.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiportFifo.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZMultiportFifo.hpp>

#ifndef __PRZMultiportFifoNoFragFlow_HPP__
#include <PRZMultiportFifoNoFragFlow.hpp>
#endif
/*
#ifndef __PRZCTMultiportFifoFlow_HPP__
#include <PRZCTMultiportFifoFlow.hpp>
#endif

#ifndef __PRZWHMultiportFifoFlow_HPP__
#include <PRZWHMultiportFifoFlow.hpp>
#endif
*/
#ifndef __PRZMultiportFifoFlow_HPP__
#include <PRZMultiportFifoFlow.hpp>
#endif


#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiportFifo,PRZRunnableComponent);

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
//  f: PRZMultiportFifo(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZMultiportFifo :: PRZMultiportFifo( const PRZComponentId& id,
                                unsigned bufferSize, unsigned numberPorts,
                                PRZBufferControl control )
               : PRZRunnableComponent(id),
                 m_BufferSize(bufferSize),
                 m_BufferControl(control),
                 m_inputs(numberPorts),
                 m_BufferSizeShort(0)
{
     setNumberOfInputs(numberPorts);
}


//*************************************************************************
//:
//  f: virtual ~PRZMultiportFifo();
//
//  d:
//:
//*************************************************************************

PRZMultiportFifo :: ~PRZMultiportFifo()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifo :: buildFlowControl()
{
   PRZString err;
   err.sprintf("PRZMultiportFifo :: %d is UNIMPLEMENTED", getBufferControl());
   if( getBufferControl() == PRZMultiportFifo::CT )
   {
      EXIT_PROGRAM(err);  
//      setFlowControl( new PRZCTMultiportFifoFlow(*this) );
   }
   else if  ( getBufferControl() == PRZMultiportFifo::WH )
   {
      EXIT_PROGRAM(err);  
//      setFlowControl( new PRZWHMultiportFifoFlow(*this) );
   } 
   else if  ( getBufferControl() == PRZMultiportFifo::CT_NF )
   {
      setFlowControl( new PRZMultiportFifoNoFragFlow(*this) );
   } 
   else
   {
      PRZString err;
      err.sprintf("Solo implemetado control de flujo CT y WH");
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

unsigned PRZMultiportFifo :: bufferHoles() const
{
   return ((PRZMultiportFifoFlow*)getFlowControl())->bufferHoles();
}

//*************************************************************************
//:
//  f: unsigned bufferOccupation() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZMultiportFifo :: bufferOccupation() const
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

void PRZMultiportFifo :: setBufferSize(unsigned size)
{
   m_BufferSize = size;
   if( getFlowControl() )
   {
      ((PRZMultiportFifoFlow*)getFlowControl()) -> setBufferSize(size);
   }
}
//*************************************************************************
//:
//  f: void setBufferSizeShort(unsigned size);
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifo :: setBufferSizeShort(unsigned size)
{
   m_BufferSizeShort = size;

}
//*************************************************************************
//:
//  f:  virtual Boolean bubbleReady() const;
//
//  d:  Return bubble information for Routing decision unit.
//:
//*************************************************************************

Boolean PRZMultiportFifo ::bubbleReady() const
{

      return ((PRZMultiportFifoFlow*)getFlowControl()) -> bubbleReady();

}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZMultiportFifo :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZMultiportFifo* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZMultiportFifo* PRZMultiportFifo :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idBuffer(*tag);
   PRZString bufferSize, bufferControl, dataDelay,inputs;
   PRZString bufferShort;

   unsigned size,sizeShort, delay=1;
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZE, bufferSize) )
   {
      size = bufferSize.asInteger();
   }
   else
   {
      size = ((PRZRouter*)owner)->valueForBufferSize();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZE_SHORT, bufferShort) )
   {
      sizeShort = bufferShort.asInteger();
   }
   else
   {
      sizeShort = ((PRZRouter*)owner)->valueForBufferSize();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }

   if( ! (tag->getAttributeValueWithName(PRZ_TAG_CONTROL, bufferControl)) )
   {
      bufferControl = ((PRZRouter*)owner)->valueForBufferControl();
   }
   //VPC Added (number of input ports)
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_INPUTS, inputs)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }

   PRZBufferControl control = PRZMultiportFifo::NIL;
   if( bufferControl == PRZ_TAG_CUTHROUGH )
   {
      control = PRZMultiportFifo::CT;
   }
   else if ( bufferControl == PRZ_TAG_WORMHOLE )
   {
      control = PRZMultiportFifo::WH;
   }
    else if ( bufferControl == PRZ_TAG_VCT_NOFRAG )
   {
      control = PRZMultiportFifo::CT_NF; 
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZFIFO_002, (char*)bufferControl );
      EXIT_PROGRAM(err);
   }
   
   //VPVAdded
   unsigned inp = inputs.asInteger();   
   PRZMultiportFifo* buffer = new PRZMultiportFifo(idBuffer,size,inp,control);
   if(sizeShort!=((PRZRouter*)owner)->valueForBufferSize())
      buffer->setBufferSizeShort(sizeShort);
   buffer->setOwner(owner);
   buffer->setDataDelay(delay-1);
   
   //((PRZRouter*)owner)->addBuffer(buffer);
   
   return buffer;
}

//*************************************************************************

// fin del fichero
