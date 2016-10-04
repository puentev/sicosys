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
// File: ../ATCSimul/src/PRZMultiplexorCVFlowCT.cpp
//
// Clase      : PRZMultiplexorCVFlowCT
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiplexorCVFlowCT.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiplexorCVFlowCT.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZMultiplexorCVFlowCT.hpp>

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

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiplexorCVFlowCT,PRZFlow);

//*************************************************************************
//:
//  f: PRZMultiplexorCVFlowCT(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCVFlowCT :: PRZMultiplexorCVFlowCT( PRZComponent& component)
                      : PRZFlow(component),
                        m_memoryRcv(0),
                        m_payload(0),
                        m_current(0),
                        m_inputs(0),
                        m_lpack(0)
                        
{
    m_inputs        = ((PRZMultiplexorCV&)getComponent()).numberOfInputs();
    m_memoryRcv=  new QueueFlits[m_inputs];
   
}


//*************************************************************************
//:
//  f: ~PRZMultiplexorCVFlowCT();
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCVFlowCT :: ~PRZMultiplexorCVFlowCT()
{
   delete [] m_memoryRcv;
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlowCT :: inputReading()
{ 
   PRZMessage *msg;
   m_lpack= ((PRZSimulation*)getComponent().getSimulation())->getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());  

   
   for(int i=1;i<m_inputs+1;i++)
   { 
     if( inputInterfaz(i)->isReadyActive() )
     {
        inputInterfaz(i)->getData(&msg);
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Rx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           " Interfaz" + PRZString(i);
         PRZWRITE2LOG( texto );
      #endif
        //inputInterfaz(i)->getData(msg);          
         m_memoryRcv[i-1].enqueue(msg);
      }
       // if(m_payload==1 && m_current!=i || outputInterfaz()->isStopActive(i))
       //Control de flujo CT. Se entiende que todo lo que hay antes va a pasar de esta señal 
      //hasta que finalize el paquete. 
       if(m_memoryRcv[i-1].numberOfElements()>0)
        {
          inputInterfaz(i)->sendStopRightNow();
        }
        else
       {
         inputInterfaz(i)->clearStopRightNow();
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

Boolean PRZMultiplexorCVFlowCT :: stateChange()
{ 
   PRZMessage* msg;
   cleanOutputInterfaz();
   if(m_payload==1)
   {
      if( m_memoryRcv[m_current-1].numberOfElements()==0)
        return true;
      
       m_memoryRcv[m_current-1].firstElement(msg);
       if(!outputInterfaz()->isStopActive(m_current) || !msg->isHeader())
       {
          m_memoryRcv[m_current-1].dequeue(msg);
          outputInterfaz()->sendData(msg,m_current);
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           " Interfaz" + PRZString(m_current);
         PRZWRITE2LOG( texto );
      #endif
          if(msg->isTail())
          {
             m_payload=0;
          }
       }
    }
    else
    {
        for(int j=m_current;j<m_inputs+m_current;j++)
        {
           unsigned displaced=j%m_inputs;
           if( m_memoryRcv[displaced].numberOfElements()!=0 && !outputInterfaz()->isStopActive(displaced+1))
           {
              m_current=displaced+1;
              m_payload=1;
              m_memoryRcv[m_current-1].dequeue(msg);
              outputInterfaz()->sendData(msg,m_current);
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           " Interfaz" + PRZString(m_current);
         PRZWRITE2LOG( texto );
      #endif
              if(msg->isTail())
              {
                  m_payload=0;
              }
              break;
           }
        }
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

Boolean PRZMultiplexorCVFlowCT :: outputWriting()
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

Boolean PRZMultiplexorCVFlowCT :: onStopDown(unsigned interfaz, unsigned cv)
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

Boolean PRZMultiplexorCVFlowCT :: onStopUp(unsigned interfaz, unsigned cv)
{  

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlowCT :: onReadyUp(unsigned interfaz, unsigned cv)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual void postRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCVFlowCT :: postRun(uTIME time)
{
   
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiplexorCVFlowCT :: onReadyDown(unsigned interfaz, unsigned cv)
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

void PRZMultiplexorCVFlowCT :: cleanOutputInterfaz()
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

Boolean PRZMultiplexorCVFlowCT :: controlAlgoritm(Boolean info, int delta)
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

void PRZMultiplexorCVFlowCT :: updateCurrentOutput()
{

}
//*************************************************************************


// fin del fichero
