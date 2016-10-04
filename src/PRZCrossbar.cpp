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
// File: ../ATCSimul/src/PRZCrossbar.cpp
//
// Clase      : PRZCrossbar
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCrossbar.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCrossbar.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZCrossbar.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZCrossbarFlow_HPP__
#include <PRZCrossbarFlow.hpp>
#endif

#ifndef __PRZCrossbarCTFlow_HPP__
#include <PRZCrossbarCTFlow.hpp>
#endif

#ifndef __PRZCrossbarCTFlowMux_HPP__
#include <PRZCrossbarCTFlowMux.hpp>
#endif
/*
#ifndef __PRZCrossbarWHFlowOutBuff_HPP__
#include <PRZCrossbarWHFlowOutBuff.hpp>
#endif

#ifndef __PRZCrossbarCTFlowOutBuff_HPP__
#include <PRZCrossbarCTFlowOutBuff.hpp>
#endif
*/
#ifndef __PRZCrossbarMuxFlow_HPP__
#include <PRZCrossbarMuxFlow.hpp>
#endif
/*
#ifndef __PRZCrossbarMuxFlowStarFree_HPP__
#include <PRZCrossbarMuxFlowStarFree.hpp>
#endif

#ifndef __PRZCrossbarNoMuxSeqFlow_HPP__
#include <PRZCrossbarNoMuxSeqFlow.hpp>
#endif

#ifndef __PRZCrossbarMuxFlowOutBuff_HPP__
#include <PRZCrossbarMuxFlowOutBuff.hpp>
#endif

#ifndef __PRZCrossbarMRequestFlow_HPP__
#include <PRZCrossbarMRequestFlow.hpp>
#endif

#ifndef __PRZCrossbarMRequestFlowOutBuff_HPP__
#include <PRZCrossbarMRequestFlowOutBuff.hpp>
#endif

#ifndef __PRZCrossbarMRequestFlowCTOutBuff_HPP__
#include <PRZCrossbarMRequestFlowCTOutBuff.hpp>
#endif

#ifndef __PRZCrossbarMRequestFlowCTOutBuffResumed_HPP__
#include <PRZCrossbarMRequestFlowCTOutBuffResumed.hpp>
#endif
*/
#ifndef __PRZCrossbarMRequestFlowCTOutBuffResumedVirtual_HPP__
#include <PRZCrossbarMRequestFlowCTOutBuffResumedVirtual.hpp>
#endif
/*
#ifndef __PRZCrossbarMRequestFlowCT_HPP__
#include <PRZCrossbarMRequestFlowCT.hpp>
#endif
*/
#ifndef __PRZCrossbarMRequestSingleWHFlow_HPP__
#include <PRZCrossbarMRequestSingleWHFlow.hpp>
#endif
/*
#ifndef __PRZCrossbarMRequestSingleCTFlow_HPP__
#include <PRZCrossbarMRequestSingleCTFlow.hpp>
#endif

#ifndef __PRZCrossbar3DMRequestSingleCTFlow_HPP__
#include <PRZCrossbar3DMRequestSingleCTFlow.hpp>
#endif
*/
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

IMPLEMENT_RTTI_DERIVED(PRZCrossbar,PRZRunnableComponent);

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
//  f: PRZCrossbar( const PRZComponentId& id,
//                  unsigned inputs,
//                  unsigned outputs );
//
//  d:
//:
//*************************************************************************

PRZCrossbar :: PRZCrossbar( const PRZComponentId& id,
                            unsigned inputs,
                            unsigned outputs )
             : PRZRunnableComponent(id)
{
   setNumberOfInputs(inputs);
   setNumberOfOutputs(outputs);   
}


//*************************************************************************
//:
//  f: virtual ~PRZCrossbar();
//
//  d:
//:
//*************************************************************************

PRZCrossbar :: ~PRZCrossbar()
{
 
}

//*************************************************************************
//:
//  f:  unsigned getThrSf() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbar :: getThrSf() const
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   return gData.getThrStarvationFree();
}


//*************************************************************************
//:
//  f:  void setThrSf(unsigned number);
//
//  d:
//:
//*************************************************************************

void PRZCrossbar :: setThrSf(unsigned thrSF)
{
      
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.setThrStarvationFree(thrSF);
}
//*************************************************************************
//:
//  f:  unsigned getOutputMux() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbar :: getOutputMux() const
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   return gData.crossbarOutputMux();
}


//*************************************************************************
//:
//  f:  void setOutputMux(unsigned number);
//
//  d:
//:
//*************************************************************************

void PRZCrossbar :: setOutputMux(unsigned number)
{
      
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.crossbarSetOutputMux(number);
}

//*************************************************************************
//:
//  f:  unsigned numberOfLocalPorts() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZCrossbar :: numberOfLocalPorts() const
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   return gData.crossbarNumberOfLocalPorts();
}


//*************************************************************************
//:
//  f:  void setNumberOfLocalPorts(unsigned number);
//
//  d:
//:
//*************************************************************************

void PRZCrossbar :: setNumberOfLocalPorts(unsigned number)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.crossbarSetNumberOfLocalPorts(number);
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZCrossbar :: buildFlowControl()
{
   PRZString err;
   err.sprintf("PRZCrossbar :: %d is unimplemented\n", getCrossbarType() );

   if( getCrossbarType() == PRZCrossbar::WH )
      setFlowControl( new PRZCrossbarFlow(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::WH_OB )
      EXIT_PROGRAM(err)
//      setFlowControl( new PRZCrossbarWHFlowOutBuff(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT )
      setFlowControl( new PRZCrossbarCTFlow(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT_MUX )
      setFlowControl( new PRZCrossbarCTFlowMux(*this) );
 
   else if( getCrossbarType() == PRZCrossbar::CT_OB )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarCTFlowOutBuff(*this) );   

   else if( getCrossbarType() == PRZCrossbar::MUX_ADAPT )
      setFlowControl( new PRZCrossbarMuxFlow(*this) );
      
   else if( getCrossbarType() == PRZCrossbar::MUX_ADAPT_SF )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMuxFlowStarFree(*this) );
         
   else if( getCrossbarType() == PRZCrossbar::MUX_ADAPT_OB )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMuxFlowOutBuff(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::WH_OAC )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarNoMuxSeqFlow(*this) );   
   
   else if( getCrossbarType() == PRZCrossbar::WH_MULTIPLE_REQ )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlow(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::WH_MULTIPLE_REQ_OB )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlowOutBuff(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlowCT(*this) );

   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlowCT(*this) );

   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ_OB )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlowCTOutBuff(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ_OB_RE )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestFlowCTOutBuffResumed(*this) ); 
        
  else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ_OB_RE_VIRTUAL )
      setFlowControl( new PRZCrossbarMRequestFlowCTOutBuffResumedVirtual(*this) );  
     
   else if( getCrossbarType() == PRZCrossbar::WH_MULTIPLE_REQ_SIMPLE )
      setFlowControl( new PRZCrossbarMRequestSingleWHFlow(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ_SIMPLE )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbarMRequestSingleCTFlow(*this) );
   
   else if( getCrossbarType() == PRZCrossbar::CT_MULTIPLE_REQ_SIMPLE_3D )
      EXIT_PROGRAM(err)
//setFlowControl( new PRZCrossbar3DMRequestSingleCTFlow(*this) );
   
   getFlowControl() -> initialize();
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz(unsigned number=1);
//
//  d:
//:
//*************************************************************************

void PRZCrossbar :: buildOutputInterfaz(unsigned number)
{
   if( getOutputMux() == 0 ) 
   {
      Inhereited::buildOutputInterfaz(number);
      return;
   }

   // En este caso, las salidas del crossbar estan multiplexadas por un
   // factor que se ha establecido en "getOutputMux()", que generalmente
   // vale 2 en caso de multiplexacion. Las salidas que van hacia el nodo
   // local no se multiplexan.

   for( int i=1; i<=number; i++ )   
   {
      PRZInterfaz* interfaz = ( PRZCrossbar::isLocalNodeOutput(i) ) ?
                   PRZInterfaz::createOutputInterfaz(i,1) :
                   PRZInterfaz::createOutputInterfaz(i,getOutputMux());
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

PRZString PRZCrossbar :: asString() const
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

unsigned PRZCrossbar :: getInputWith(przROUTINGTYPE type, unsigned channel)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   if( ! gData.crossbarInputsType() ) return 0;
   
   PRZTypeChannel elementToFind(type,channel);
   PRZTypeChannel tempType;

   for( int i=1; i<=gData.crossbarInputsType()->numberOfElements(); i++ )
   {
      gData.crossbarInputsType()->valueAt(i,tempType);
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

unsigned PRZCrossbar :: getOutputWith(przROUTINGTYPE type, unsigned channel)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));

   if( ! gData.crossbarOutputsType() ) return 0;
   
   PRZTypeChannel elementToFind(type,channel);
   PRZTypeChannel tempType;

   for( int i=1; i<=gData.crossbarOutputsType()->numberOfElements(); i++ )
   {
      gData.crossbarOutputsType()->valueAt(i,tempType);
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

przROUTINGTYPE PRZCrossbar :: getTypeForOutput(unsigned output, unsigned channel)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   if( ! gData.crossbarOutputsType() )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_001,(char*)asString());
      EXIT_PROGRAM(err);
   }
   PRZTypeChannel tempType;
   gData.crossbarOutputsType()->valueAt(output,tempType);

   return tempType.type();
}

//*************************************************************************
//:
//  f: Boolean isLocalNodeOutput(unsigned oPort);
//
//  d:
//:
//*************************************************************************

Boolean PRZCrossbar :: isLocalNodeOutput(unsigned oPort)
{
   PRZTypeChannel tempType;
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)(PRZROUTER->getOwner()));
   gData.crossbarOutputsType()->valueAt(oPort,tempType);
   return (tempType.type()==_LocalNode_) ? true : false;
}


//*************************************************************************
//:
//  f: static PRZCrossbar* newFrom( const PRZTag* tag, 
//                                  PRZComponent* owner,
//                                  INDEX& index );
//  d:
//:
//*************************************************************************

PRZCrossbar* PRZCrossbar :: newFrom( const PRZTag* tag, 
                                     PRZComponent* owner,
                                     INDEX& index)
{
   PRZComponentId idCrossbar(*tag);
   PRZString inputs, outputs, type, outputMux, threshold; 
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
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_TYPE, type)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_002, (char*)PRZ_TAG_TYPE );
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
   
   unsigned thrSF=10000;
   
   if( (tag->getAttributeValueWithName(PRZ_TAG_SF_THR, threshold)) )
   {
      thrSF = threshold.asInteger();
   }
      
   unsigned outMux = 1;
   
   if( tag->getAttributeValueWithName(PRZ_TAG_OUTPUTMUX, outputMux) )
   {
      outMux = outputMux.asInteger();
   }   
   
   unsigned inp = inputs.asInteger();
   unsigned out = outputs.asInteger();
   
   
   PRZCrossbar* crossbar = new PRZCrossbar( idCrossbar, inp, out );
   crossbar->setOwner(owner);
   crossbar->setSimulation(owner->getSimulation());
   crossbar->setDataDelay(dataDelay.asInteger());
   crossbar->setHeaderDelay(headerDelay.asInteger());
   crossbar->setOutputMux(outMux);
   crossbar->setThrSf(thrSF);
   PRZGlobalData& gData = ((PRZSimulation*)(owner->getSimulation()))->globalData((PRZNetwork*)(owner->getOwner()));
      
   PRZString endTag = PRZString("/") + tag->tagName();
   PRZTag* nextTag = PRZComponent::routerBuilder->getTagWithIndex(++index);
   
   if( ! gData.crossbarIsInitializated() )
   {
      gData.crossbarSetInputsType( new PRZRoutingTypeArray(inp) );
      gData.crossbarSetOutputsType( new PRZRoutingTypeArray(out) );
   }  
         
   while( nextTag && nextTag->tagName()!=endTag )
   {
      if( nextTag->tagName() == PRZ_TAG_INPUT )
      {
         // Se trata de una entrada del crossbar
         unsigned number = nextTag->tagId().asInteger();
         PRZString type, channel;
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(PRZ_TAG_CHANNEL, type);
         if(type == "")
         {
            PRZString err;
            err.sprintf(ERR_PRZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel =="") channel = "1";
         
         PRZTypeChannel typeChannel( _string2routingType(type),
                                     channel.asInteger() );
         
         if( ! gData.crossbarIsInitializated() )
         {         
            gData.crossbarInputsType()->setValueAt(number,typeChannel);
         }
      }
      else if( nextTag->tagName() == PRZ_TAG_OUTPUT )
      {
         // Se trata de una salida del crossbar
         unsigned number = nextTag->tagId().asInteger();
         PRZString type, channel;
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         nextTag->getAttributeValueWithName(PRZ_TAG_CHANNEL, channel);
         if(type == "")
         {
            PRZString err;
            err.sprintf(ERR_PRZCROSS_003, (char*)type );
            EXIT_PROGRAM(err);
         }
         if(channel =="") 
         {
            channel = "1";
         } 
         PRZTypeChannel typeChannel( _string2routingType(type),
                                     channel.asInteger() );
         if( ! gData.crossbarIsInitializated() )
         {         
            gData.crossbarOutputsType()->setValueAt(number,typeChannel);      
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


   // Tipos de flujo

   if( type == PRZ_TAG_WORMHOLE )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH);
   }
   else if( type == PRZ_TAG_WORMHOLE_OB )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH_OB);
   }
   else if( type == PRZ_TAG_CUTHROUGH )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT);
   }
   else if( type == PRZ_TAG_CUTHROUGH_MUX )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MUX);
   }
   else if( type == PRZ_TAG_CUTHROUGH_OUTBUFF )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_OB);
   }
   else if( type == PRZ_TAG_MUX_ADAPT )
   {
      crossbar->setCrossbarType(PRZCrossbar::MUX_ADAPT);
   }
   else if( type == PRZ_TAG_MUX_ADAPT_SF )
   {
      crossbar->setCrossbarType(PRZCrossbar::MUX_ADAPT_SF);
   }
   else if( type == PRZ_TAG_MUX_ADAPT_OB )
   {
      crossbar->setCrossbarType(PRZCrossbar::MUX_ADAPT_OB);
   }
   else if( type == PRZ_TAG_WH_OAC )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH_OAC);
   }
   else if( type == PRZ_TAG_WH_MULTIPLE_REQ )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH_MULTIPLE_REQ);
   }
   else if( type == PRZ_TAG_WH_MULTIPLE_REQ_OB )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH_MULTIPLE_REQ_OB);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ_OB )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ_OB);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ_OB_RE )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ_OB_RE);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ_OB_RE_VIRTUAL )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ_OB_RE_VIRTUAL);
   }
   else if( type == PRZ_TAG_WH_MULTIPLE_REQ_SIMPLE )
   {
      crossbar->setCrossbarType(PRZCrossbar::WH_MULTIPLE_REQ_SIMPLE);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ_SIMPLE )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ_SIMPLE);
   }
   else if( type == PRZ_TAG_CT_MULTIPLE_REQ_SIMPLE_3D )
   {
      crossbar->setCrossbarType(PRZCrossbar::CT_MULTIPLE_REQ_SIMPLE_3D);
   }
  
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZCROSS_003, (char*)type );
      EXIT_PROGRAM(err);
   }

   if( ! gData.crossbarIsInitializated() )
   {
      for( int i=1; i<=crossbar->numberOfOutputs(); i++ )
      {
         if( crossbar->isLocalNodeOutput(i) )
         {
            crossbar->setNumberOfLocalPorts( crossbar->numberOfLocalPorts() + 1 );
         }
      }
   }
   
   gData.crossbarSetInitializated();
   owner->addPostRunComponent(crossbar);
   owner->addPreRunComponent(crossbar);
      
   return crossbar;
}

//*************************************************************************


// fin del fichero
