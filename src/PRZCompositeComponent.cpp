//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//**************************************************************************
//:
// File: ../ATCSimul/src/PRZCompositeComponent.cpp
//
// Clase      : PRZCompositeComponent
//
// Hereda de  : PRZComponent
//
// Notes:   La clase PRZCompositeComponent se utiliza para implementar
//               el patron de diseno Composite sobre los componentes de
//               nuestro simulador. 
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCompositeComponent.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//**************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZCompositeComponent.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCompositeComponent.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZCompositeComponent,PRZComponent);

//*************************************************************************
//:
//  f: inline void setAutoDestroyChildren(Boolean destroy=true);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZCompositeComponent(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZCompositeComponent :: PRZCompositeComponent(const PRZComponentId& id)
                       : PRZComponent(id),
                         m_DestroyChildren(true)
{

}


//*************************************************************************
//:
//  f: ~PRZCompositeComponent();
//
//  d:
//:
//*************************************************************************

PRZCompositeComponent :: ~PRZCompositeComponent()
{

}


//*************************************************************************
//:
//  f:  virtual void terminate();
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: terminate()
{
   if(m_DestroyChildren) removeAllComponents();
      
   Inhereited::terminate();
}


//*************************************************************************
//:
//  f: virtual PRZComponent* componentWithId(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZCompositeComponent :: componentWithId(const PRZComponentId& id)
{
   PRZComponent* comp = Inhereited::componentWithId(id);
   if(comp) return comp;

   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId() == id )
         return cursor.element();
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual PRZComponent* componentWithIdLike(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZCompositeComponent :: componentWithIdLike(const PRZComponentId& id)
{
   PRZComponent* comp = Inhereited::componentWithIdLike(id);
   if(comp) return comp;

   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId().isLike(id) )
         return cursor.element();
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual PRZComponent* componentWithName(const PRZString& name);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZCompositeComponent :: componentWithName(const PRZString& name)
{
   PRZComponent* comp = Inhereited::componentWithName(name);
   if(comp) return comp;

   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId().name() == name )
         return cursor.element();
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual void addComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: addComponent(PRZComponent* aComponent)
{
   // Solo se anade si no esta en la coleccion.
   if( aComponent && !m_ComponentSet.containsElement(aComponent) )
   {
      m_ComponentSet.add(aComponent);
      aComponent->setOwner(this);
   }
}


//*************************************************************************
//:
//  f: virtual void addPreRunComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: addPreRunComponent(PRZComponent* aComponent)
{
   // Solo se anade si no esta en la coleccion. Esta coleccion no es
   // propietaria de sus elementos, sino que unicamente se utiliza para
   // referirse a ellos, y por consiguiente, no es responsabilidad suya
   // la destruccion de los componentes que la integran.
   
   if( aComponent && !m_PreRunComponentSet.containsElement(aComponent) )
   {
      m_PreRunComponentSet.add(aComponent);
   }
}

//*************************************************************************
//:
//  f: virtual void addPostRunComponent(PRZComponent* aComponent);
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: addPostRunComponent(PRZComponent* aComponent)
{
   // Solo se anade si no esta en la coleccion. Esta coleccion no es
   // propietaria de sus elementos, sino que unicamente se utiliza para
   // referirse a ellos, y por consiguiente, no es responsabilidad suya
   // la destruccion de los componentes que la integran.
   
   if( aComponent && !m_PostRunComponentSet.containsElement(aComponent) )
   {
      m_PostRunComponentSet.add(aComponent);
   }
}


//*************************************************************************
//:
//  f: virtual void removeAllComponents();
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: removeAllComponents()
{
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      // delete cursor.element();
   }

   m_ComponentSet.removeAllElements();
   m_PreRunComponentSet.removeAllElements();
   m_PostRunComponentSet.removeAllElements();
   
}


//*************************************************************************
//:
//  f: virtual PRZInterfaz*  getInputInterfaz(unsigned number = 1);
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZCompositeComponent :: getInputInterfaz(unsigned number)
{
   PRZComponentId idInterfaz( PRZComponentId::InputInterfaz, 
                              _Unknow_,
                              PRZString("I") + PRZString(number) );
                              
   PRZComponent* componentFound = componentWithIdLike(idInterfaz);                           
   PRZInterfaz* interfaz = POINTERCAST(PRZInterfaz,componentFound);
   return interfaz;
}


//*************************************************************************
//:
//  f: virtual PRZInterfaz*  getOutputInterfaz(unsigned number = 1);
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZCompositeComponent :: getOutputInterfaz(unsigned number)
{
   PRZComponentId idInterfaz( PRZComponentId::OutputInterfaz, 
                              _Unknow_,
                              PRZString("O") + PRZString(number) );
                              
   PRZComponent* componentFound = componentWithIdLike(idInterfaz);                           
   PRZInterfaz* interfaz = POINTERCAST(PRZInterfaz,componentFound);
   return interfaz;
}


//*************************************************************************
//:
//  f: unsigned numberOfComponents() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZCompositeComponent :: numberOfComponents() const
{
   return m_ComponentSet.numberOfElements();
}


//*************************************************************************
//:
//  f: virtual void setSimulation(PRZComponent* simulation);
//
//  d:
//:
//*************************************************************************

void PRZCompositeComponent :: setSimulation(PRZComponent* simulation)
{
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      cursor.element()->setSimulation(simulation);
   }
   
   Inhereited::setSimulation(simulation);
}
      
//*************************************************************************
//:
//  f: PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZCompositeComponent :: asString() const
{
   PRZString string = "PRZCompositeComponent: ";
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
