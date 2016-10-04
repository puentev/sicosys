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
// File: ../ATCSimul/src/PRZFlow.cpp
//
// Clase      : PRZFlow
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZFlow.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZEvent_HPP__
#include <PRZEvent.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZFlow);

//*************************************************************************
//:
//  f: virtual Boolean inputReading()    = 0;
//  d:
//
//  f: virtual Boolean stateChange()     = 0;
//  d:
//
//  f: virtual Boolean outputWriting()   = 0;
//  d:
//
//  f: virtual Boolean controlAlgoritm(Boolean data=false) = 0;
//  d:
//
//  f: inline PRZComponent& getComponent() const;
//  d:
//
//  f: inline Boolean isStopped() const;
//  d:
//
//  f: inline PRZEventQueue& getEventQueue() const;
//  d:
//

//:
//*************************************************************************

SFSMessagePool*  PRZFlow::m_MessagePool;

//*************************************************************************
//:
//  f: PRZFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZFlow :: PRZFlow( PRZComponent& component)
         : m_Component(component),
           m_Stopped(false),
           m_State(0),
           m_RegisterTime(0),
           m_InputInterfaz(0),
           m_OutputInterfaz(0)
{
  
}


//*************************************************************************
//:
//  f: virtual ~PRZFlow();
//
//  d:
//:
//*************************************************************************

PRZFlow :: ~PRZFlow()
{
   if(m_MessagePool) delete m_MessagePool;
   if( m_InputInterfaz )  delete[] m_InputInterfaz;
   if( m_OutputInterfaz ) delete[] m_OutputInterfaz;
}

//*************************************************************************
//:
//  f: void PRZFlow :: initializePool(unsigned size)
//
//  d: me pasan desde arriba la capacidad de almacenamiento de la red
//:
//*************************************************************************
void PRZFlow :: initializePool(unsigned size)
{
    //
    //Conozco exactamente la capacidad de almacenamiento de la red 
    //le doy un margen de 1.5 veces para mensajes en espera.
    //
   
   m_poolSize=size*2;
   if(!m_MessagePool) m_MessagePool = new SFSMessagePool(m_poolSize);   
}
//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZFlow :: initialize()
{
   // NOTA: el componente ya es propietario del interfaz, por lo que no
   //       debe ser borrado aqui cuando se destruya. La mision de este
   //       inicializar es la de referenciar los interfazces para poder
   //       acceder a los mismos de forma rapida. Es conveniente que en
   //       las clases hijas que vayan a sobrecargar este metodo, llamen
   //       primero a este de aqui, y luego realicen lo que tengan
   //       previsto.
   
   PRZRunnableComponent& rc = (PRZRunnableComponent&) getComponent();
   unsigned inputs  = rc.numberOfInputs();
   unsigned outputs = rc.numberOfOutputs();
   unsigned i;
   
   m_InputInterfaz = (inputs)  ? new PRZInterfaz*[inputs]  : 0;
   m_OutputInterfaz= (outputs) ? new PRZInterfaz*[outputs] : 0;
    
   for( i=0; i<inputs; i++ )
   {
      *(m_InputInterfaz+i) = getComponent().getInputInterfaz(i+1);
      if( ! (*(m_InputInterfaz+i)) )
      {
         PRZString err;
         err.sprintf( ERR_PRZFLOW_001, (char*)rc.asString() );
         EXIT_PROGRAM(err);
      }
   }

   for( i=0; i<outputs; i++ )
   {
      *(m_OutputInterfaz+i)= getComponent().getOutputInterfaz(i+1);
      if( ! (*(m_OutputInterfaz+i)) )
      {
         PRZString err;
         err.sprintf( ERR_PRZFLOW_002, (char*)rc.asString() );
         EXIT_PROGRAM(err);
      }      
   }


}


//*************************************************************************
//:
//  f: virtual void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZFlow :: postInitialize()
{
   // Por defecto, no hace nada.
}

//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void PRZFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: void setStopped(Boolean stop);
//
//  d:
//:
//*************************************************************************

void PRZFlow :: setStopped(Boolean stop)
{
   m_Stopped = stop;
}


//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZFlow :: run(uTIME runTime)
{
   inputReading();
   stateChange();
   outputWriting();
}


//*************************************************************************
//:
//  f: virtual void preRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZFlow :: preRun(uTIME time)
{
   // Por defecto, no hace nada. Solo se sobrecargara en aquellos componentes
   // que precisen realizar acciones en el segundo semiperiodo de reloj.
}


//*************************************************************************
//:
//  f: virtual void postRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZFlow :: postRun(uTIME time)
{
   // Por defecto, no hace nada. Solo se sobrecargara en aquellos componentes
   // que precisen realizar acciones en el segundo semiperiodo de reloj.
}


//*************************************************************************
//:
//  f: PRZRouter& getOwnerRouter() const;
//
//  d:
//:
//*************************************************************************

PRZRouter& PRZFlow :: getOwnerRouter() const
{
   return *((PRZRouter*)(getComponent().getRouter()));
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFlow :: onStopUp(unsigned interfaz, unsigned cv)
{

   if( propagateStop() && inputInterfaz(interfaz) )     
   {
	   inputInterfaz(interfaz)->sendStopRightNow(cv);
   }
   setStopped(true);
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( inputInterfaz(interfaz) )
	{  
      inputInterfaz(interfaz)->clearStopRightNow(cv);
   }     
   setStopped(false);
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZFlow :: onReadyDown(unsigned interfaz, unsigned cv)
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

Boolean PRZFlow :: propagateStop()
{
   // Este metodo se sobrecarga en aquellas clases que tengan que realizar
   // comprobaciones adicionales para saber si se debe retransmitir de forma
   // inmediata la senal de Stop a la entrada, como es el caso de la Fifo.
   
   return true;
}


//*************************************************************************
//:
//  f: PRZInterfaz* inputInterfaz(unsigned number =1 ) const;
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZFlow :: inputInterfaz(unsigned number) const
{
   return m_InputInterfaz ? *(m_InputInterfaz+number-1) : 0;
}


//*************************************************************************
//:
//  f: PRZInterfaz* outputInterfaz(unsigned number =1 ) const;
//
//  d:
//:
//*************************************************************************
      
PRZInterfaz* PRZFlow :: outputInterfaz(unsigned number) const
{ 
   return m_OutputInterfaz ? *(m_OutputInterfaz+number-1) : 0;
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

Boolean PRZFlow :: dispatchEvent(const PRZEvent& event)
{
   PRZString err;
   err.sprintf( ERR_PRZFLOW_003,
                (char*)getComponent().asString(),
                (char*)event.asString() );
   EXIT_PROGRAM(err);
   return true;
}


//*************************************************************************
//:
//  f: uTIME getHeaderDelay() const;
//
//  d:
//:
//*************************************************************************

uTIME PRZFlow :: getHeaderDelay() const
{
   return ((PRZRunnableComponent&)getComponent()).getHeaderDelay();
}


//*************************************************************************
//:
//  f: uTIME getDataDelay() const;
//
//  d:
//:
//*************************************************************************

uTIME PRZFlow :: getDataDelay() const
{
   return ((PRZRunnableComponent&)getComponent()).getDataDelay();
}


//*************************************************************************
//:
//  f: virtual void setRegisterTime();
//
//  d:
//:
//*************************************************************************

void PRZFlow :: setRegisterTime()
{
   m_RegisterTime = getOwnerRouter().getCurrentTime();
}


//*************************************************************************
//:
//  f: virtual PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZFlow :: getStatus() const
{
  return "";  
}


//*************************************************************************

// fin del fichero
