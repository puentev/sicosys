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
// File: ../ATCSimul/src/PRZComponent.cpp
//
// Clase      : PRZComponent
//
// Hereda de  : PRZNotifier
//
// Notes:  La clase PRZComponent es una de las mas importantes.
//              A traves del patron de diseno Composite, es capaz de
//              implementar cualquier componente que integra la
//              arquitectura de un simulador.
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZComponent.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZComponent.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZComponent.hpp>

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZBuilder_HPP__
#include <PRZBuilder.hpp>
#endif

//*************************************************************************

PRZBuilder*   PRZComponent::routerBuilder     = 0;
PRZBuilder*   PRZComponent::networkBuilder    = 0;
PRZBuilder*   PRZComponent::simulationBuilder = 0;

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZComponent,PRZNotifier);

//*************************************************************************
//:
//  f: virtual void run(uTIME runTime=0) = 0;
//  d:
//
//  f: inline PRZComponentId componentId() const;
//  d:
//
//  f: inline virtual void setOwner(PRZComponent* owner);
//  d:
//
//  f: inline PRZComponent* getOwner() const;
//  d:
//
//  f: inline virtual void setRouter(PRZComponent* router);
//  d:
//
//  f: inline PRZComponent* getRouter() const;
//  d:
//
//  f: inline przROUTINGTYPE getRoutingDirection() const;
//  d:
//
//  f: inline void setRoutingType(przROUTINGTYPE type);
//  d:
//
//  f: inline PRZString getName() const;
//  d:
//
//  f: inline void setName(const PRZString& aName);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZComponent(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponent :: PRZComponent(const PRZComponentId& id)
              : m_Identifier(id),
                m_Owner(0),
                m_Router(0),
                m_Initializated(false),
                m_Terminated(false),
                m_DeleteConnectionSet(true),
                m_Simulation(0)
{

}


//*************************************************************************
//:
//  f: PRZComponent(const PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZComponent :: PRZComponent(const PRZComponent& component)
              : m_Identifier(component.m_Identifier),
                m_Owner(component.m_Owner),
                m_Router(component.m_Router),
                m_DeleteConnectionSet(component.m_DeleteConnectionSet),
                m_Simulation(component.m_Simulation)
{

}

//*************************************************************************
//:
//  f: virtual ~PRZComponent();
//
//  d:
//:
//*************************************************************************

PRZComponent :: ~PRZComponent()
{
   if( m_DeleteConnectionSet )
   {
      PRZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      { 
         delete cursor.element();
      }
   }
}


//*************************************************************************
//:
//  f: void initialize();
//
//  d:
//:
//*************************************************************************

void PRZComponent :: initialize()
{
   // No es necesaria ninguna inicializacion en este punto.
   setInitializated(true);
}


//*************************************************************************
//:
//  f: void terminate();
//
//  d:
//:
//*************************************************************************

void PRZComponent :: terminate()
{
   // No es necesaria ninguna inicializacion en este punto.
   setTerminated(true);
}

//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZComponent :: postInitialize()
{
   // No es necesaria ninguna post-inicializacion en este punto.
}


//*************************************************************************
//:
//  f: void postRun(uTime time);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: preRun(uTIME time)
{
   // Existen ciertos componentes que requieren realizar acciones en el
   // instante anterior a realizar el run. Esos componentes se deben 
   // registrar previamente a traves de "addPreRunComponent()"
}

//*************************************************************************
//:
//  f: void postRun(uTime time);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: postRun(uTIME time)
{
   // Existen ciertos componentes que requieren realizar acciones en el
   // segundo semiperiodo de reloj. Esos componentes se deben registrar
   // previamente a traves de "addPostRunComponent()"
}


//*************************************************************************
//:
//  f: PRZComponent* componentWithId(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZComponent :: componentWithId(const PRZComponentId& id)
{
   if( componentId() == id )
      return this;
   return 0;
}


//*************************************************************************
//:
//  f: PRZComponent* componentWithIdLike(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZComponent :: componentWithIdLike(const PRZComponentId& id)
{
   if( componentId().isLike(id) )
      return this;
   return 0;
}


//*************************************************************************
//:
//  f: PRZComponent* componentWithName(const PRZString& name);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZComponent :: componentWithName(const PRZString& name)
{
   if( componentId().name() == name )
      return this;
   return 0;
}

//*************************************************************************
//:
//  f: void addComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: addComponent(PRZComponent* aComponent)
{
   // Por defecto, no hace nada
}


//*************************************************************************
//:
//  f: void addPreRunComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: addPreRunComponent(PRZComponent* aComponent)
{
   // Por defecto, no hace nada
}


//*************************************************************************
//:
//  f: void addPostRunComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: addPostRunComponent(PRZComponent* aComponent)
{
   // Por defecto, no hace nada
}


//*************************************************************************
//:
//  f: void removeAllComponents();
//
//  d:
//:
//*************************************************************************

void PRZComponent :: removeAllComponents()
{
   // Por defecto, no hace nada
}

//*************************************************************************
//:
//  f: virtual PRZInterfaz*  getInputInterfaz(unsigned number = 1);
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZComponent :: getInputInterfaz(unsigned number)
{
   return 0;
}


//*************************************************************************
//:
//  f: virtual PRZInterfaz*  getOutputInterfaz(unsigned number = 1);
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZComponent :: getOutputInterfaz(unsigned number)
{
   return 0;
}


//*************************************************************************
//:
//  f: void addConnection(PRZConnection* connection);
//
//  d:
//:
//*************************************************************************

void PRZComponent :: addConnection(PRZConnection* connection)
{
   if( ! m_ConnectionSet.containsElement(connection) )
      m_ConnectionSet.add(connection);
}
      
      
//*************************************************************************
//:
//  f: virtual Boolean onDataUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onDataUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onDataDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onReadyUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onReadyDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onStopUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZComponent :: onStopDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: PRZString getStatus() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZComponent :: getStatus() const
{
   return "";
}


//*************************************************************************
//:
//  f: PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZComponent :: asString() const
{
   return "PRZComponent";
}


//*************************************************************************


// fin del fichero
