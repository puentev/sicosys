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
// File: ../ATCSimul/src/PRZFifoMemoryFlow.cpp
//
// Clase      : PRZFifoMemoryFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFifoMemoryFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZFifoMemoryFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZFifoMemoryFlow.hpp>

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
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

IMPLEMENT_RTTI_DERIVED(PRZFifoMemoryFlow,PRZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize() const;
//  d:
//
//  f: inline unsigned bufferElements() const;
//  d:
//
//  f: inline unsigned bufferHoles() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZFifoMemoryFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZFifoMemoryFlow :: PRZFifoMemoryFlow(PRZComponent& component)
                   : PRZFlow(component),
                     m_Size(0),
                     m_SendStop(false)

{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZFifoMemoryFlow :: initialize()
{
   m_Size = ((PRZFifoMemory&)getComponent()).getBufferSize();   
   Inhereited::initialize();
}


//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d:
//:
//*************************************************************************
 
Boolean PRZFifoMemoryFlow :: inputReading()
{  
   PRZMessage* msg;
   inputInterfaz()->getData(&msg); 
// if(msg) SFSMSGTRACE(msg,"PRZFifoMemoryFlow :: inputReading()    in") else cout << "NOMESG\n";
   if( inputInterfaz()->isReadyActive() )
   {
      if( ! bufferHoles())
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString err;
         err.sprintf(ERR_PRZFFLOW_001,(char*)getComponent().asString());
         err=err+msg->asString()+ "Time crash=" + PRZString(delayTime);
         EXIT_PROGRAM(err);
      }
        
      uTIME delay = getOwnerRouter().getCurrentTime() + 
                    getDataDelay();
      PRZEvent event(_BufferData_,msg);
      getEventQueue().enqueue(event,delay);

      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Rx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles());
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

Boolean PRZFifoMemoryFlow :: stateChange()
{
   // El metodo virtual "controlAlgoritm()" se sobrecarga en las clases
   // hijas. de esta forma, se da un comportamiento WH o CT segun la
   // clase a la cual pertenezca el objeto que hemos instanciado.

   if( controlAlgoritm()==false )
   {
      m_SendStop=true;
      //el ultimo intento para evitar los desbordaminetos de las fifos
      inputInterfaz()->sendStopRightNow();
   }
       
   else m_SendStop=false;

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMemoryFlow :: outputWriting()
{
   outputInterfaz()->clearData();
   
   if( ! outputInterfaz()->isStopActive() )
   {
      uTIME timeStamp;
      if( getEventQueue().firstElementTimeStamp(timeStamp) )
      {
         if( timeStamp <= getOwnerRouter().getCurrentTime() )
         {
            static PRZEvent event;
            getEventQueue().dequeue(event);
            dispatchEvent(event);
         }
      }
   }      
      
      
   if(m_SendStop)
   {
        inputInterfaz()->sendStopRightNow();
      #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": Buffer lleno->Stop ";
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
      #endif
   }
   else
   {
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

Boolean PRZFifoMemoryFlow :: dispatchEvent(const PRZEvent& event)
{
   if( event.type() == _BufferData_ )
   {
      PRZMessage* msg ;
      msg = event.message();
      sendFlit(msg);

      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (";
         texto += PRZString(delayTime) + " # " + msg->asString() +
                  ") holes= " + PRZString(bufferHoles());
         PRZWRITE2LOG( texto );
      #endif
   }
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMemoryFlow :: propagateStop()
{
   if( controlAlgoritm() )
   {
      // En este caso, el buffer dispone de espacio para mas flits,
      // sea la que sea la politica de control de flujo, ya que se ha 
      // sobrecargado el metodo "controlAlgoritm()" convenientemente.
      setStopped(false);
      return false;
   }
   setStopped(true);   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMemoryFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
     #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": STOP UP!! ";
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
      #endif
   return true;
//   return Inhereited::onStopUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFifoMemoryFlow :: onStopDown(unsigned interfaz, unsigned cv)
{      
   #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": STOP DOWN ";
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
    #endif
   if( controlAlgoritm() ) 
   {
     // Hay plazas en el buffer, por lo que se puede propagar la señal de
     // stop, sin ningun peligro de desbordar el buffer.
      
     return Inhereited::onStopDown(interfaz,cv);
   }
   
   // En este caso, no existe espacio en el buffer, por lo que esta parado.
   // Cuando pueda alamcenar algun flit, ya se encargara el mismo de bajar
   // la señal de Stop.
   
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit(const PRZMessage & msg);
//
//  d:
//:
//*************************************************************************

void PRZFifoMemoryFlow :: sendFlit(PRZMessage* msg)
{
   outputInterfaz()->sendData(msg);
}

//*************************************************************************
//:
//  f:       virtual Boolean bubbleReady() const;   

//  d:       semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean PRZFifoMemoryFlow ::bubbleReady()const
 {     
    return true;
 }   
//*************************************************************************

// fin del fichero
