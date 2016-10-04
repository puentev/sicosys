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
// File: ../ATCSimul/src/PRZFifoMuxedFlow.cpp
//
// Clase      : PRZFifoMuxedFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFifoMuxedFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZFifoMuxedFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZFifoMuxedFlow.hpp>

#ifndef __PRZFlow_HPP__
#include <PRZFlow.hpp>
#endif

#ifndef __PRZFifoMuxed_HPP__
#include <PRZFifoMuxed.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZNetwork_HPP__
#include <PRZNetwork.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZFifoMuxedFlow,PRZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize() const;
//  d:
//
//  d:
//
//  f: inline unsigned bufferHoles() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZFifoMuxedFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZFifoMuxedFlow :: PRZFifoMuxedFlow(PRZComponent& component)
                   : PRZFlow(component),
                     m_Size(0),
      					m_lastOutput(0),
                     m_SendStop(false)
{
     m_outputs= ((PRZFifoMuxed&)getComponent()).numberOfOutputs();
     m_memory= new QueueFlits[m_outputs]; 
     m_Size = ((PRZFifoMuxed&)getComponent()).getBufferSize(); 

}

PRZFifoMuxedFlow :: ~PRZFifoMuxedFlow()
{
   delete[] m_memory;
}

//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZFifoMuxedFlow :: initialize()
{  
  Inhereited::initialize();
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************
 
Boolean PRZFifoMuxedFlow :: inputReading()
{  
   PRZMessage *msg;
   if( inputInterfaz()->isReadyActive() )
   {
      inputInterfaz()->getData(&msg);   

      //if( ! bufferHoles() )
      //{
      //   PRZString err;
      //   err.sprintf(ERR_PRZFFLOW_001,(char*)getComponent().asString());
      //   err=err+msg->asString();
      //   EXIT_PROGRAM(err);
      //}
      
	  if(msg->isHeader() )
     {
        unsigned check=0;
        unsigned previo=m_lastOutput;
        Boolean antiLoop=false;
        for(unsigned i=0;i<m_outputs;i++)
        {
           if(controlAlgoritm(true,i))
           {
              antiLoop=true;
              break;
           }
        }
        
        if(!antiLoop)
        {
              uTIME delayTime = getOwnerRouter().getCurrentTime() ;
              PRZString err;
              err.sprintf(ERR_PRZFFLOW_001,(char*)getComponent().asString());
              err=err+msg->asString() + "# TIME =" + PRZString(delayTime);
              EXIT_PROGRAM(err);
        }
 		  do
        { 
           m_lastOutput=(m_lastOutput+1)%m_outputs;
        }
        while(!controlAlgoritm(true,m_lastOutput));
     
     }
     
     m_memory[m_lastOutput].enqueue(msg);
                     
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Rx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles()) +"Interfaz =" + PRZString(m_lastOutput+1);
         PRZWRITE2LOG( texto );
      #endif
     
     
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

Boolean PRZFifoMuxedFlow :: stateChange()
{
   if(controlAlgoritm(false,m_outputs)==false)
      m_SendStop=true;
   else
      m_SendStop=false;
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMuxedFlow :: outputWriting()
{
   PRZMessage *flitnow;
   
   for(unsigned i=1;i<=m_outputs;i++)
   {
        outputInterfaz(i)->clearData();

      if( ! outputInterfaz(i)->isStopActive() )   
      {
	     if(m_memory[i-1].numberOfElements()!=0 )
        {
           m_memory[i-1].dequeue(flitnow);
           sendFlit(flitnow,i);
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (" +
                           PRZString(delayTime) + " # " + flitnow->asString() +
                           ") holes= " + PRZString(bufferHoles()) +"Interfaz =" + PRZString(i);
         PRZWRITE2LOG( texto );
      #endif
        }
         
      }
   }      
      
      
     if( m_SendStop )
     {
        inputInterfaz()->sendStopRightNow();
       #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + "STOP UP -->"+PRZString(delayTime);
         PRZWRITE2LOG( texto );
       #endif
     }
     else
     {
     
       #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + "STOP DOWN -->"+PRZString(delayTime);
         PRZWRITE2LOG( texto );
        #endif
        inputInterfaz()->clearStop();
      }
    
    
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMuxedFlow :: dispatchEvent(const PRZEvent& event)
{   
    return true;
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMuxedFlow :: propagateStop()
{
   if(m_SendStop==true)
      return false;
   else
      return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMuxedFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
     #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": STOP UP!! interfaz="+ interfaz;
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
      #endif
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMuxedFlow :: onStopDown(unsigned interfaz, unsigned cv)
{      
   #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": STOP DOWN ";
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
    #endif
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

void PRZFifoMuxedFlow :: sendFlit(PRZMessage* msg, unsigned interfaz)
{
          #ifndef NO_TRAZA
          uTIME delayTime = getOwnerRouter().getCurrentTime() ;
          PRZString texto = getComponent().asString() + " Flit Tx (";
          texto += PRZString(delayTime) + " # " + msg->asString() +
                   ") holes= " + PRZString(bufferHoles()) + "Interfaz =" +PRZString(interfaz);
          PRZWRITE2LOG( texto );
       #endif
   outputInterfaz(interfaz)->sendData(msg);
}

//*************************************************************************
//:
//  f:       virtual Boolean bubbleReady() const;   
//  d:       semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean PRZFifoMuxedFlow ::bubbleReady()const
 {     
    
    return true;
 }   
//*************************************************************************
//:
//  f:       virtual Boolean bufferElements() const;   
//  d:       
//:
//*************************************************************************
unsigned PRZFifoMuxedFlow::bufferElements() const
{ 
   unsigned kaka=0;
   for(int index=0; index<m_outputs;index++)
   {
      kaka=m_memory[index].numberOfElements()+kaka;
   }
   return kaka; 
}

//*************************************************************************
//:
//  f:       virtual Boolean bufferElements() const;   
//  d:       
//:
//*************************************************************************
unsigned PRZFifoMuxedFlow::bufferElements(unsigned interfaz) const
{ 
    return m_memory[interfaz].numberOfElements();
}

//*************************************************************************

// fin del fichero
