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
// File: ../ATCSimul/src/PRZConsumer.cpp
//
// Clase      : PRZConsumer
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZConsumer.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZConsumer.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZConsumer.hpp>

#ifndef __PRZConsumerFlow_HPP__
#include <PRZConsumerFlow.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZConsumer,PRZRunnableComponent);


//*************************************************************************
//:
//  f: PRZCosumer(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZConsumer :: PRZConsumer(const PRZComponentId& id)
             : PRZRunnableComponent(id)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZConsumer();
//
//  d:
//:
//*************************************************************************

PRZConsumer :: ~PRZConsumer()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZConsumer :: initialize()
{
   setNumberOfOutputs(0);
   Inhereited::initialize();
}

//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZConsumer :: buildFlowControl()
{
   PRZConsumerFlow* flow = new PRZConsumerFlow(*this);
   flow -> initialize();
   setFlowControl(flow);
}


//*************************************************************************
//:
//  f: virtual void buildInputInterfaz(unsigned number =1 );
//
//  d:
//:
//*************************************************************************

void PRZConsumer :: buildInputInterfaz(unsigned number)
{
   // Es necesario sobrecargar el metodo, ya que el interfaz de salida del
   // multiplexor debe llevar N senales de control, siendo N el numero de
   // canales virtuales.

   for( int i=1; i<=number; i++ )   
   {
      PRZInterfaz* interfaz = 
                   PRZInterfaz::createInputInterfaz(i,numberOfInputs());
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZConsumer :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZConsumer* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZConsumer* PRZConsumer :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idConsumer(*tag);
   PRZString dataDelay, consumerInputs;
   uTIME delay = 1;
   
   PRZConsumer* consumer = new PRZConsumer(idConsumer);

   if( tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay) )
   {
      delay = dataDelay.asInteger();
   }
      
   consumer->setDataDelay(delay);

   if( tag->getAttributeValueWithName(PRZ_TAG_INPUTS, consumerInputs) )
   {
      unsigned size = consumerInputs.asInteger();
      consumer->setNumberOfInputs(size);
   } 
      
   ((PRZRouter*)owner)->addConsumer(consumer);
   return consumer;
}


//*************************************************************************



// fin del fichero
