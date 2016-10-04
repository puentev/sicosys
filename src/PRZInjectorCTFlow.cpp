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
// File: ../ATCSimul/src/PRZInjectorCTFlow.cpp
//
// Clase      : PRZInjectorCTFlow
//
// Hereda de  : PRZInjectorFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZInjectorCTFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZInjectorCTFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZInjectorCTFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZInjector_HPP__
#include <PRZInjector.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZInjectorCTFlow,PRZInjectorFlow);

//*************************************************************************
//:
//  f: inline Boolean isHeaderSent() const;
//  d:
//
//  f: inline void setHeaderSent(Boolean value);
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZInjectorCTFlow(PRZcomponent& component);
//
//  d:
//:
//*************************************************************************

PRZInjectorCTFlow :: PRZInjectorCTFlow(PRZComponent& component)
                   : PRZInjectorFlow(component),
                     m_HeaderSent(false)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZInjectorCTFlow();
//
//  d:
//:
//*************************************************************************

PRZInjectorCTFlow :: ~PRZInjectorCTFlow()
{

}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//: 
//*************************************************************************

Boolean PRZInjectorCTFlow :: outputWriting()
{
   outputInterfaz() -> clearData();


	if( isAnyMessageToSend())
   {
      //He intentado hacer asi la gestion de el inyector de reques
      //y reply.. pero no hay manera. La señales de stop del canal
      //de request no llegan.
      //
      //Lo intento con los demultiplexores
      //
      //PRZMessage* topMessage;
      //getMessageQueue().firstElement(topMessage);
      //Boolean isReq= ((PRZInjector&) getComponent()).getIsolate() && topMessage->isRequest();      
      //if(getAlgoritmToSend()->isRequestNext() && !isHeaderSent())
      //{
      //   if(outputInterfaz(2)->isStopActive() ) return true;
     // }
      
  	   if( (!outputInterfaz()->isStopActive())  || isHeaderSent() )
      {
         PRZMessage* msg = getAlgoritmToSend()->getNextFlitToSend();
         if(!msg) return true;
	 
	 
         if( msg->isHeader() ) setHeaderSent(true);
         else if(msg->isTail() ) setHeaderSent(false);
     
         //if( !(  ((PRZInjector&) getComponent()).getIsolate() ) )
         //{
            outputInterfaz()->sendData(msg);
         //}
         //else
         //{
            
         //   if(msg.isRequest())
         //   {
         //     outputInterfaz(2) -> sendData(msg);             
         //   }
         //   else
         //   { 
         //      outputInterfaz(1) -> sendData(msg);
         //   }
         //}
            
         #ifndef NO_TRAZA
            uTIME delayTime = getOwnerRouter().getCurrentTime() ;
            PRZString texto = getComponent().asString() + " Flit Tx. TIME = ";
            texto += PRZString(delayTime) + " # " + msg->asString();
            if( msg->isHeader() )
            {
               texto += PRZString("  CABECERA: ") + 
                        msg->source().asString() + " => " + 
                        msg->destiny().asString();
            }
            PRZWRITE2LOG( texto );
         #endif
     }
   }
   return true;   
}


//*************************************************************************


// fin del fichero
