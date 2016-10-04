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
// File: ../ATCSimul/src/PRZConsumerFlow.cpp
//
// Clase      : PRZConsumerFlow
//
// Hereda de  : PRZFlow
//
// Notes:  
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZConsumerFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZConsumerFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZConsumerFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZConsumer_HPP__
#include <PRZConsumer.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZConsumerFlow,PRZFlow);


//*************************************************************************
//:
//  f: PRZConsumerFlow();
//
//  d:
//:
//*************************************************************************

PRZConsumerFlow :: PRZConsumerFlow( PRZComponent& component)
                 : PRZFlow(component),
                   m_ReceivingMessage(false),
                   m_DataReceived(false),
                   m_State(Header)
{
   m_InputData=0;
}


//*************************************************************************
//:
//  f: virtual ~PRZConsumerFlow();
//
//  d:
//:
//*************************************************************************

PRZConsumerFlow :: ~PRZConsumerFlow()
{
   if(m_InputData)
      getMessagePool().release(m_InputData);
}


//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void PRZConsumerFlow :: terminate()
{

}


//*************************************************************************
//:
//  f:  virtual void run(uTIME runTime)
//
//  d:
//:
//*************************************************************************

void PRZConsumerFlow :: run(uTIME runTime)
{
   PRZConsumer& consumer = (PRZConsumer&) getComponent();
   if( consumer.getDataDelay() != 0 )
   {
      Inhereited::run(runTime);
   }
}


//*************************************************************************
//:
//  f:  virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZConsumerFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   //cerr << "RX : " << interfaz << "," << cv << endl;  
   PRZConsumer& consumer = (PRZConsumer&) getComponent();
   if( consumer.getDataDelay() == 0 )
   {
      Inhereited::run(getOwnerRouter().getCurrentTime());
   }

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZConsumerFlow :: inputReading()
{

   if( ! inputInterfaz()->isStopActive() )
   {
      if( inputInterfaz()->isReadyActive() )
      {
         if(m_InputData) 
            getMessagePool().release(m_InputData);
         m_DataReceived = inputInterfaz()->getData(&m_InputData);
      }
      else
      {
         m_DataReceived = false;
      }
   }

   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean PRZConsumerFlow :: stateChange()
{
   if( (!inputInterfaz()->isStopActive()) && m_DataReceived )
   {
      #ifndef NO_TRAZA
         PRZString time = getOwnerRouter().getCurrentTime();
         PRZString texto= getComponent().asString() +
                          " Flit Rx. TIME = " +
                          time + ".  " +
                          m_InputData->asString();
      #endif
   	if(m_State  == Information && m_InputData->isHeader())
      {
         PRZString err;
         err.sprintf( "%s : Estoy Recibiendo una cabeza sin haber recibido la cola del mensaje anterior: %s",
                      (char*)getComponent().asString(),
                      (char*)m_InputData->asString() );
         EXIT_PROGRAM(err);
      } 
	

		if( m_State == Header )
   	{
         getOwnerRouter().onHeaderPacketReceived(m_InputData);
   	   m_State = Information;
   	}
	
	
#ifndef UNIROUTER
      if( m_InputData->destiny() != getOwnerRouter().getPosition() )
      {
         PRZString err;
         err.sprintf( ERR_PRZCOFLO_001,
                      (char*)getComponent().asString(),
                      (char*)m_InputData->asString() );
         EXIT_PROGRAM(err);
       }
#endif
   	
   	 
      if( m_InputData->isTail() )
   	{
   	   #ifndef NO_TRAZA
            texto += "  PACKET RX";
           #endif
           getOwnerRouter().onPacketReceived(m_InputData);
     	   m_State = Header;
     	}
     	
     	#ifndef NO_TRAZA
         PRZWRITE2LOG(texto);
      #endif
   }

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZConsumerFlow :: outputWriting()
{
   // Por el momento, se supone que el PRZConsumer siempre va a consumir
   // los flits que le llegan. Si en el futuro se desea simular algun tipo
   // de probabilidad de fallo, este es el lugar donde se debe reflejar.
   
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZConsumerFlow :: controlAlgoritm(Boolean info, int delta)
{
   // Para evitar la abstraccion. No se utiliza en esta clase.
   return true;
}



//*************************************************************************


// fin del fichero
