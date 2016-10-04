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
// File: ../ATCSimul/src/PRZInjector.cpp
//
// Clase      : PRZInjector
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZInjector.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZInjector.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZInjector.hpp>

#ifndef __PRZInjectorFlow_HPP__
#include <PRZInjectorFlow.hpp>
#endif

#ifndef __PRZInjectorCTFlow_HPP__
#include <PRZInjectorCTFlow.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZInjector,PRZRunnableComponent);

unsigned PRZInjector::sm_NumberOfHeaders = 2;

//*************************************************************************
//:
//  f: PRZInjector(const PRZComponentId& id, PRZInjectorType type);
//
//  d:
//:
//*************************************************************************

PRZInjector :: PRZInjector(const PRZComponentId& id, PRZInjectorType type)
             : PRZRunnableComponent(id),
               m_InjectorType(type),
               m_MissRouting(0)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZInjector();
//
//  d:
//:
//*************************************************************************

PRZInjector :: ~PRZInjector()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZInjector :: initialize()
{
   if( isInitializated() ) return;
   setNumberOfInputs(0);
   Inhereited::initialize();
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZInjector :: buildFlowControl()
{
   if( m_InjectorType == PRZInjector::WH )
      setFlowControl( new PRZInjectorFlow(*this) );
      
   else if( m_InjectorType == PRZInjector::CT )
      setFlowControl( new PRZInjectorCTFlow(*this) );
   
   getFlowControl()->initialize();
}


//*************************************************************************
//:
//  f: void sendMessage(const PRZMessage& aMessage);
//
//  d:
//:
//*************************************************************************

void PRZInjector :: sendMessage(PRZMessage* aMessage)
{
   ((PRZInjectorFlow*)getFlowControl())->sendMessage(aMessage);
}

//*************************************************************************
//:
//  f: unsigned getAvaliableMessages() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZInjector :: getAvaliableMessages() const
{
   return (((PRZInjectorFlow*)getFlowControl())->getAvaliableMessages());
}

//*************************************************************************
//:
//  f: unsigned getMessagesToSend() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZInjector :: getMessagesToSend() const
{
   return (((PRZInjectorFlow*)getFlowControl())->getMessagesToSend());
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZInjector :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string+=((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZInjector* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZInjector* PRZInjector :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idInjector(*tag);
   PRZString type;
   PRZString numberOfHeaders;
   PRZString numberOfMisses;
   PRZString isolateTraffic;
   
   tag->getAttributeValueWithName(PRZ_TAG_TYPE_INJECTOR, type);
   tag->getAttributeValueWithName(PRZ_TAG_NUM_HEADERS, numberOfHeaders);
   tag->getAttributeValueWithName(PRZ_TAG_MISS_ROUTING, numberOfMisses);
   tag->getAttributeValueWithName(PRZ_TAG_ISOLATED, isolateTraffic);
   
   PRZInjector* injector = 0;
   type = (type=="") ? ((PRZRouter*)owner)->valueForBufferControl() : type;
   
   if( type == PRZ_TAG_WORMHOLE )
   {
      injector = new PRZInjector(idInjector,PRZInjector::WH);
   }
   else if( type == PRZ_TAG_CUTHROUGH )
   {
      injector = new PRZInjector(idInjector,PRZInjector::CT);
   }
   else if( type == PRZ_TAG_VIRTUAL_CT )
   {
      injector = new PRZInjector(idInjector,PRZInjector::WH);
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINJEC_001, (char*)type );
      EXIT_PROGRAM(err);
   }

   if( numberOfHeaders == "" )
   {
      injector->setNumberOfHeaders(2);
   }
   else
   {
      int headers = numberOfHeaders.asInteger();
      injector->setNumberOfHeaders(headers);
      PRZGlobalData& gData = ((PRZSimulation*)(owner->getSimulation()))->globalData((PRZNetwork*)(owner->getOwner()));
      gData.setNumberOfHeaders(headers);
   }
   
  // DEsistop de hacerlo asi.
  // if(isolateTraffic == "")
  // {
  //    injector->setIsolate(false);
  // }
  // else
  // {
  //    injector->setNumberOfOutputs(3);
  //    injector->setIsolate(true);
  // }
   
   if( numberOfMisses == "" )
   {
      injector->setMissRouting(0);
   }
   else
   {
      int missesalowed = numberOfMisses.asInteger();
      injector->setMissRouting(missesalowed);
   }

   ((PRZRouter*)owner)->addInjector(injector);
   return injector;
}


//*************************************************************************

// fin del fichero
