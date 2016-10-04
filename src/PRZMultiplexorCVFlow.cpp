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
// File: ../ATCSimul/src/PRZMultiplexorCVFlow.cpp
//
// Clase      : PRZMultiplexorCVFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiplexorCVFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiplexorCVFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZMultiplexorCVFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZMultiplexorCV_HPP__
#include <PRZMultiplexorCV.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiplexorCVFlow,PRZFlow);

//*************************************************************************
//:
//  f: PRZMultiplexorCVFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCVFlow :: PRZMultiplexorCVFlow( PRZComponent& component)
                      : PRZFlow(component),
                        m_CurrentOutput(1),
                        m_Messages(0),
                        m_MessageToSend(0)
{
   m_Messages  = new PRZMessageArray(PRZ_MAX_NUMBER_OF_CV);
   m_Messages->initialize((PRZMessage *) 0);
}


//*************************************************************************
//:
//  f: ~PRZMultiplexorCVFlow();
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCVFlow :: ~PRZMultiplexorCVFlow()
{
/*   for( int i=1; i<=PRZ_MAX_NUMBER_OF_CV; i++ )
   {
      PRZMessage* msg;
      m_Messages->valueAt(i,msg);
      delete msg;
   }
   delete m_Messages; */
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: inputReading()
{ 
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: stateChange()
{ 
   cleanOutputInterfaz();      
   if( m_MessageToSend )
   {
      PRZMessage* msg;
      m_Messages->valueAt(m_MessageToSend,msg);
      outputInterfaz()->sendData(msg,m_MessageToSend);
      #ifndef NO_TRAZA
         PRZString texto = getComponent().asString() + " Tx(" +
                           PRZString(m_MessageToSend) + "): " +
                           msg->asString();
         PRZWRITE2LOG(texto);
      #endif
      
      m_Messages->setValueAt(m_MessageToSend,(PRZMessage *)0);
      m_MessageToSend = 0;
      updateCurrentOutput();
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

Boolean PRZMultiplexorCVFlow :: outputWriting()
{ 
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: onStopUp(unsigned interfaz, unsigned cv)
{  
   if( m_MessageToSend == cv )
   {
      // PRZWRITE2LOG(getComponent().asString()+" STOP RX->TX"+PRZString(cv));
      inputInterfaz(cv)->sendStopRightNow();
      m_MessageToSend = 0;
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   PRZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg);
   m_Messages->setValueAt(interfaz,msg);

   #ifndef NO_TRAZA
      PRZString texto = getComponent().asString() + " Rx(" +
                        PRZString(interfaz) + "): " + msg->asString();
      PRZWRITE2LOG(texto);
   #endif
   
   return true;
}


//*************************************************************************
//:
//  f: virtual void postRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCVFlow :: postRun(uTIME time)
{
   unsigned numInputs = ( ( PRZMultiplexorCV& )getComponent() ).numberOfInputs();
   m_MessageToSend = 0;
   
   for( int i=1; i<=numInputs; i++ )
   {
      unsigned channel = getCurrentOutput();
      PRZMessage* msg;
      m_Messages->valueAt(channel,msg);
      
      if( msg && (!outputInterfaz()->isStopActive(channel)) )
      {
         m_MessageToSend = channel;
         break;
      }      
      updateCurrentOutput();   
   }
   
   for( int j=1; j<=numInputs; j++)
   {
      PRZMessage* msg = 0;
      m_Messages->valueAt(j,msg);
      if( msg ) 
      {
         if( m_MessageToSend != j )
         {
            // PRZWRITE2LOG(getComponent().asString()+" TX STOP "+PRZString(j));
            inputInterfaz(j)->sendStopRightNow();
         }
         else
         {
            // PRZWRITE2LOG(getComponent().asString()+" CLEAR STOP "+PRZString(j));
            inputInterfaz(j)->clearStopRightNow();
         }
      }
      else
      {
         // PRZWRITE2LOG(getComponent().asString()+" CLEAR STOP "+PRZString(j));
         inputInterfaz(j)->clearStopRightNow();
      }
   }
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: onReadyDown(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: void cleanOutputInterfaz();
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCVFlow :: cleanOutputInterfaz()
{
   for( int i=1; i<=outputInterfaz()->numberOfCV(); i++ )
   {
      outputInterfaz()->clearData(i);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlow :: controlAlgoritm(Boolean info, int delta)
{
   // Para evitar que sea una clase abstracta.
   return true;
}


//*************************************************************************
//:
//  f: void updateCurrentOutput();
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCVFlow :: updateCurrentOutput()
{
   unsigned numberOfOutputs = outputInterfaz()->numberOfCV();
   
   if( (++m_CurrentOutput) > numberOfOutputs )
      m_CurrentOutput = 1;
}

//*************************************************************************


// fin del fichero
