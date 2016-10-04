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
// File: ../ATCSimul/src/SFSSimpleRouter.cpp
//
// Clase      : PRZCrossbar
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: SFSSimpleRouter.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/SFSSimpleRouter.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <SFSSimpleRouter.hpp>

#ifndef __SFSSimpleRouterFlow_HPP__
#include <SFSSimpleRouterFlow.hpp>
#endif

#ifndef __SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__
#include <SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZRouterBuilder_HPP__
#include <PRZRouterBuilder.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(SFSSimpleRouter,PRZRunnableComponent);

//*************************************************************************

//*************************************************************************
//:
//  f: inline CrossbarType getCrossbarType() const;
//  d:
//
//  f: inline void setCrossbarType(CrossbarType type);
//  d:
//:
//*************************************************************************



//*************************************************************************
//:
//  f: SFSSimpleRouter( const PRZComponentId& id,
//                  unsigned inputs,
//                  unsigned outputs );
//
//  d:
//:
//*************************************************************************

SFSSimpleRouter :: SFSSimpleRouter( const PRZComponentId& id,
                            unsigned inputs,
                            unsigned outputs,
			                   SFSRoutingControl routingControl,
					             SFSBufferControl bufferControl )
             : PRZRunnableComponent(id),
	          m_RoutingControl(routingControl),
	          m_BufferControl(bufferControl)
{
   setNumberOfInputs(inputs);
   setNumberOfOutputs(outputs);   
}


//*************************************************************************
//:
//  f: virtual ~SFSSimpleRouter();
//
//  d:
//:
//*************************************************************************

SFSSimpleRouter :: ~SFSSimpleRouter()
{
 
}

void SFSSimpleRouter :: terminate()
{
   getFlowControl()->terminate(); 
}


//*************************************************************************
//:
//  f:  unsigned numberOfLocalPorts() const;
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouter :: numberOfLocalPorts() const
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   return gData.simpleRouterNumberOfLocalPorts();
}


//*************************************************************************
//:
//  f:  void setNumberOfLocalPorts(unsigned number);
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouter :: setNumberOfLocalPorts(unsigned number)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.simpleRouterSetNumberOfLocalPorts(number);
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void SFSSimpleRouter :: buildFlowControl()
{
   if( getRoutingControl() == SFSSimpleRouter::DOR_BURBLE_H1 && 
       getBufferControl() == SFSSimpleRouter::BC_CT)
   {
      setFlowControl( new SFSSimpleRouterFlow(*this) ); 
   }
   else    if( getRoutingControl() == SFSSimpleRouter::BURBLE_ADAPT_MR_MAXCREDITS_REQ_REPLY && 
               getBufferControl() == SFSSimpleRouter::BC_CT)
   {
      setFlowControl( new SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits(*this) ); 
   }

   if(getFlowControl())
   {
      getFlowControl() -> initialize();
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_001, getRoutingControl() );
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

PRZString SFSSimpleRouter :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static unsigned getInputWith(przROUTINGTYPE type, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouter :: getInputWith(przROUTINGTYPE type, unsigned channel)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   if( ! gData.simpleRouterInputsType() ) return 0;
   
   PRZTypeChannel elementToFind(type,channel);
   PRZTypeChannel tempType;

   for( int i=1; i<=gData.simpleRouterInputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterInputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}


//*************************************************************************
//:
//  f: unsigned getOutputWith(przROUTINGTYPE type, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouter :: getOutputWith(przROUTINGTYPE type, unsigned channel)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));

   if( ! gData.simpleRouterOutputsType() ) return 0;
   
   PRZTypeChannel elementToFind(type,channel);
   PRZTypeChannel tempType;

   for( int i=1; i<=gData.simpleRouterOutputsType()->numberOfElements(); i++ )
   {
      gData.simpleRouterOutputsType()->valueAt(i,tempType);
      if( elementToFind == tempType ) return i;
   }

   return 0;
}


//*************************************************************************
//:
//  f: przROUTINGTYPE getTypeForOutput(unsigned output, unsigned channel);//
//  d:
//:
//*************************************************************************

przROUTINGTYPE SFSSimpleRouter :: getTypeForOutput(unsigned output)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   if( ! gData.simpleRouterOutputsType() )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   PRZTypeChannel type;
   gData.simpleRouterOutputsType()->valueAt(output,type);

   return type.type();
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput(unsigned oPort);
//
//  d:
//:
//*************************************************************************

Boolean SFSSimpleRouter :: isLocalNodeOutput(unsigned oPort)
{
   PRZTypeChannel tempType;
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.simpleRouterOutputsType()->valueAt(oPort,tempType);
   return (tempType.type()==_LocalNode_) ? true : false;
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput(unsigned oPort);
//
//  d:
//:
//*************************************************************************

unsigned SFSSimpleRouter :: getBufferSize() const
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   return gData.routerBufferSize();
}

//*************************************************************************
//:
//  f: static SFSSimpleRouter* newFrom( const PRZTag* tag, 
//                                  PRZComponent* owner,
//                                  INDEX& index );
//  d:
//:
//*************************************************************************

SFSSimpleRouter* SFSSimpleRouter :: newFrom( const PRZTag* tag, 
                                     PRZComponent* owner,
                                     INDEX& index)
{
   PRZComponentId idSimpleRouter(*tag);
   PRZString inputs, outputs; 
   PRZString dataDelay, headerDelay;
 
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_INPUTS, inputs)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_INPUTS );
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_OUTPUTS, outputs)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_003, (char*)PRZ_TAG_OUTPUTS );
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_HEADER_DELAY, headerDelay)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_HEADER_DELAY);
      EXIT_PROGRAM(err);
   }
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_DATA_DELAY);
      EXIT_PROGRAM(err);
   }
   
   unsigned inp = inputs.asInteger();
   unsigned out = outputs.asInteger();

   PRZString routingControlString = ((PRZRouter*)owner)->valueForRoutingControl();
   SFSRoutingControl routingControl = SFSSimpleRouter::NIL;
   if( routingControlString == PRZ_TAG_DORBURBLE_H1 )
   {
      routingControl = SFSSimpleRouter::DOR_BURBLE_H1;
   }
   else if( routingControlString == PRZ_TAG_BURBLE_ADAP_MAXCREDITS_RAND_REQ_REPLY )
   {
      routingControl = SFSSimpleRouter::BURBLE_ADAPT_MR_MAXCREDITS_REQ_REPLY;
   }
   
   PRZString bufferControlString = ((PRZRouter*)owner)->valueForBufferControl();
   SFSBufferControl bufferControl = SFSSimpleRouter::BC_NIL;
   if( bufferControlString == PRZ_TAG_CUTHROUGH )
   {
      bufferControl = SFSSimpleRouter::BC_CT;
   }
   
   
   SFSSimpleRouter* simpleRouter = new SFSSimpleRouter( idSimpleRouter, inp, out, 
                                                        routingControl, bufferControl );
   simpleRouter->setOwner(owner);
   simpleRouter->setSimulation(owner->getSimulation());
   simpleRouter->setDataDelay(dataDelay.asInteger());
   simpleRouter->setHeaderDelay(headerDelay.asInteger());
   PRZGlobalData& gData = ((PRZSimulation*)(owner->getSimulation()))->globalData((PRZNetwork*)(owner->getOwner()));
      
   PRZString endTag = PRZString("/") + tag->tagName();
   PRZTag* nextTag = PRZComponent::routerBuilder->getTagWithIndex(++index);
   
   if( ! gData.simpleRouterIsInitializated() )
   {
      gData.simpleRouterSetInputsType( new PRZRoutingTypeArray(inp) );
      gData.simpleRouterSetOutputsType( new PRZRoutingTypeArray(out) );
   }  
         
   while( nextTag && nextTag->tagName()!=endTag )
   {
      if( nextTag->tagName() == PRZ_TAG_INPUT )
      {
         // Se trata de una entrada del simpleRouter
         unsigned number = nextTag->tagId().asInteger();
         PRZString type;
	      PRZString channel;
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(PRZ_TAG_CHANNEL, channel);
         if(type == "")
         {
            PRZString err;
            err.sprintf(ERR_PRZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel == "")
         {
	         channel = "1";
	      }
         
         if( ! gData.simpleRouterIsInitializated() )
         {         
            gData.simpleRouterInputsType()->setValueAt(number,PRZTypeChannel(_string2routingType(type),channel.asInteger()));
//            gData.simpleRouterInputsType()->setValueAt(number,_string2routingType(type));
         }
      }
      else if( nextTag->tagName() == PRZ_TAG_OUTPUT )
      {
         // Se trata de una salida del simpleRouter
         unsigned number = nextTag->tagId().asInteger();
         PRZString type;
	      PRZString channel;
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(PRZ_TAG_CHANNEL, channel);
         if(type == "")
         {
            PRZString err;
            err.sprintf(ERR_PRZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel == "")
         {
	         channel = "1";
	      }
         if( ! gData.simpleRouterIsInitializated() )
         {         
            gData.simpleRouterOutputsType()->setValueAt(number,PRZTypeChannel(_string2routingType(type),channel.asInteger()));
//            gData.simpleRouterOutputsType()->setValueAt(number,_string2routingType(type));      
         }
      }
      else
      {
         PRZString err;
         err.sprintf(ERR_PRZCROSS_004, (char*)(nextTag->tagName()) );
         EXIT_PROGRAM(err);
      }
      
      nextTag = PRZComponent::routerBuilder->getTagWithIndex(++index);
   
   } // while



/*   if( type == PRZ_TAG_WORMHOLE )
   {
      simpleRouter->setSimpleRouterType(SFSSimpleRouter::WH);
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_003, (char*)type );
      EXIT_PROGRAM(err);
   } */

   if( ! gData.simpleRouterIsInitializated() )
   {
      for( int i=1; i<=simpleRouter->numberOfOutputs(); i++ )
      {
         if( simpleRouter->isLocalNodeOutput(i) )
         {
            simpleRouter->setNumberOfLocalPorts( simpleRouter->numberOfLocalPorts() + 1 );
         }
      }
   }
   
   gData.simpleRouterSetInitializated();
   owner->addPostRunComponent(simpleRouter);
   owner->addPreRunComponent(simpleRouter);

   // 5 canales con un buffer y un routing
   ((PRZRouter*)owner)->addToPoolSize(5*(gData.routerBufferSize()+1));
      
   return simpleRouter;
}

//*************************************************************************


// fin del fichero
