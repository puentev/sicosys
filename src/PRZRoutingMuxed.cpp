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
// File: ../ATCSimul/src/PRZRoutingMuxed.cpp
//
// Clase      : PRZRoutingMuxed
//
// Hereda de  : PRZRouting
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingMuxed.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingMuxed.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZRoutingMuxed.hpp>
/*
#ifndef __PRZRoutingAdaptativeBurbleFlowMuxed_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMuxed.hpp>
#endif

#ifndef __PRZRoutingAdaptativeBurbleFlowMuxedFull_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMuxedFull.hpp>
#endif
*/
#ifndef __PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZRunnableComponent_HPP__
#include <PRZRunnableComponent.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingMuxed,PRZRunnableComponent);

//*************************************************************************
//:
//  f: inline PRZRoutingControl getRoutingControl() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZRoutingMuxed(const PRZComponentId& id, PRZRoutingControl control);
//
//  d:
//:
//*************************************************************************

PRZRoutingMuxed :: PRZRoutingMuxed( 
      						const PRZComponentId& id,
                        PRZRoutingMuxedControl control,
      						unsigned inputs 
      						)
            : PRZRunnableComponent(id),
              m_RoutingControl(control)
{
		setNumberOfInputs(inputs);
}


//*************************************************************************
//:
//  f: virtual ~PRZRoutingMuxed();
//
//  d:
//:
//*************************************************************************

PRZRoutingMuxed :: ~PRZRoutingMuxed()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZRoutingMuxed :: buildFlowControl()
{
   PRZString err;
   err.sprintf("PRZRoutingMuxed :: %d is UNIMPLEMENTED", getRoutingMuxedControl() );
   
   if( getRoutingMuxedControl() == PRZRoutingMuxed::MUXED_BUBBLE_ADAP )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMuxed(*this) );
   }
   else if( getRoutingMuxedControl() == PRZRoutingMuxed::MUXED_BUBBLE_ADAP_FULL )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMuxedFull(*this) );
   }
   else if( getRoutingMuxedControl() == PRZRoutingMuxed::MUXED_BUBBLE_ADAP_FULL_REQ_REP )
   {
      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual(*this) );
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_001, getRoutingMuxedControl() );
      EXIT_PROGRAM(err);
   }
   
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_001, getRoutingMuxedControl() );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRoutingMuxed :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}

//*************************************************************************
//:
//  f: static PRZRoutingMuxed* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZRoutingMuxed* PRZRoutingMuxed :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idRouting(*tag);
   PRZString routingControl, headerDelay, dataDelay;
   PRZString inputs;
   
   //
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_INPUTS, inputs)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   //
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_CONTROL, routingControl)) )
   {
      routingControl = ((PRZRouter*)owner)->valueForRoutingControl();
   }
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_HEADER_DELAY, headerDelay)) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRTING_002, (char*)PRZ_TAG_HEADER_DELAY );
      EXIT_PROGRAM(err);
   }
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay)) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRTING_002, (char*)PRZ_TAG_DATA_DELAY );
      EXIT_PROGRAM(err);
   }

   PRZRoutingMuxedControl control = PRZRoutingMuxed::NIL;
   
   if( routingControl == PRZ_TAG_MUXED_BUBBLE_ADAP )
   {
      control = PRZRoutingMuxed::MUXED_BUBBLE_ADAP;
   }
   else if( routingControl == PRZ_TAG_MUXED_BUBBLE_ADAP_FULL )
   {
      control = PRZRoutingMuxed::MUXED_BUBBLE_ADAP_FULL;
   }   
   else if( routingControl == PRZ_TAG_MUXED_BUBBLE_ADAP_FULL_REQ_REP )
   {
      control = PRZRoutingMuxed::MUXED_BUBBLE_ADAP_FULL_REQ_REP;
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_003, (char*)routingControl );
      EXIT_PROGRAM(err);
   }
   
   unsigned inp = inputs.asInteger();
   PRZRoutingMuxed* routing = new PRZRoutingMuxed(idRouting,control,inp);
   
   routing->setHeaderDelay(headerDelay.asInteger());
   routing->setDataDelay(dataDelay.asInteger());
   

   return routing;
}
      
 
//*************************************************************************

// fin del fichero
