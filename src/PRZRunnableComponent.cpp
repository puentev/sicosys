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
// File: ../ATCSimul/src/PRZRunnableComponent.cpp
//
// Clase      : PRZRunnableComponent
//
// Hereda de  : PRZCompositeComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRunnableComponent.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRunnableComponent.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZRunnableComponent.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRunnableComponent,PRZCompositeComponent);


//*************************************************************************
//:
//  f: virtual void buildFlowControl() = 0;
//  d:
//
//  f: inline void setNumberOfInputs(unsigned inputs);
//  d:
//
//  f: inline void setNumberOfOutputs(unsigned outputs);
//  d:
//
//  f: inline unsigned numberOfInputs()  const;
//  d:
//
//  f: inline unsigned numberOfOutputs() const;
//  d:
//
//  f: inline uTIME getHeaderDelay() const;
//  d:
//
//  f: inline uTIME getDataDelay() const;
//  d:
//
//  f: inline void setFlowControl(PRZFlow* flow);
//  d:
//
//  f: void setHeaderDelay(uTIME delay);
//  d:
//
//  f: void setDataDelay(uTIME delay);
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZRunnableComponent(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZRunnableComponent :: PRZRunnableComponent(const PRZComponentId& id)
                      : PRZCompositeComponent(id),
                        m_Inputs(1),
                        m_Outputs(1),
                        m_HeaderDelay(0),
                        m_DataDelay(0),
                        m_Flow(0)
{
   
}


//*************************************************************************
//:
//  f: virtual ~PRZRunnableComponent();
//
//  d:
//:
//*************************************************************************

PRZRunnableComponent :: ~PRZRunnableComponent()
{
   delete m_Flow;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRunnableComponent :: onReadyUp(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onReadyUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRunnableComponent :: onReadyDown(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onReadyDown(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRunnableComponent :: onStopUp(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onStopUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZRunnableComponent :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
	return getFlowControl() -> onStopDown(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: initialize()
{
   if( ! isInitializated() )
   {
      buildInputInterfaz(m_Inputs);
      buildOutputInterfaz(m_Outputs);
      buildFlowControl();
   
      Inhereited::initialize();
   }
}


//*************************************************************************
//:
//  f: virtual void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: postInitialize()
{
   if( getFlowControl() )
   {
      getFlowControl()->postInitialize();
   }
}


//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: run(uTIME runTime)
{

   if( m_Flow )
   {
      m_Flow -> run(runTime);
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void preRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: preRun(uTIME time)
{
   if( m_Flow )
   {
      m_Flow -> preRun(time);
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual void postRun(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: postRun(uTIME time)
{
   if( m_Flow )
   {
      m_Flow -> postRun(time);
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void buildInputInterfaz(unsigned number =1 );
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: buildInputInterfaz(unsigned number)
{
   for( int i=1; i<=number; i++ )
   {
      PRZInterfaz* interfaz = PRZInterfaz::createInputInterfaz(i);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz(unsigned number=1);
//
//  d:
//:
//*************************************************************************

void PRZRunnableComponent :: buildOutputInterfaz(unsigned number)
{
   for( int i=1; i<=number; i++ )   
   {
      PRZInterfaz* interfaz = PRZInterfaz::createOutputInterfaz(i);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}

//*************************************************************************
//:
//  f: PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRunnableComponent :: getStatus() const
{
   if( m_Flow )
   {
      return m_Flow -> getStatus();
   }
   return "";
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRunnableComponent :: asString() const
{
   PRZString string = "PRZRunnableComponent: ";
   string += getName() + "\n ";
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      string += cursor.element()->asString() + "\n ";
   }
   return string;
}

//*************************************************************************



// fin del fichero
