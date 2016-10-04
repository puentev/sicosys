//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//
// File: ../ATCSimul/inc/PRZCompositeComponent.hpp
//
// Description:  fichero de encabezado de la clase "PRZCompositeComponent"
//
// Notes:   La clase PRZCompositeComponent se utiliza para implementar
//               el patron de diseno Composite sobre los componentes de
//               nuestro simulador. 
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCompositeComponent.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCompositeComponent_HPP__
#define __PRZCompositeComponent_HPP__

//*************************************************************************

   #ifndef __PRZComponent_HPP__
   #include <PRZComponent.hpp>
   #endif

   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif

//*************************************************************************

   class PRZCompositeComponent : public PRZComponent
   {
      typedef PRZComponent Inhereited;

   public:
      typedef PRZSequence<PRZComponent*> PRZComponentSet;

      PRZCompositeComponent(const PRZComponentId& id);
      virtual ~PRZCompositeComponent();

      virtual PRZComponent* componentWithId(const PRZComponentId& id);
      virtual PRZComponent* componentWithIdLike(const PRZComponentId& id);
      virtual PRZComponent* componentWithName(const PRZString& name);
      
      virtual void          addComponent(PRZComponent* aComponent);
      virtual void          addPreRunComponent(PRZComponent* aComponent);
      virtual void          addPostRunComponent(PRZComponent* aComponent);
      virtual void          removeAllComponents();
      virtual void          terminate();
      virtual PRZInterfaz*  getInputInterfaz(unsigned number = 1);
      virtual PRZInterfaz*  getOutputInterfaz(unsigned number = 1);

      unsigned              numberOfComponents() const;
      virtual PRZString     asString() const;

      void setAutoDestroyChildren(Boolean destroy=true)
      { m_DestroyChildren = destroy; }

      virtual void setSimulation(PRZComponent* simulation);

      // Run time information
      DEFINE_RTTI(PRZCompositeComponent);

   protected:
      PRZComponentSet  m_ComponentSet;
      PRZComponentSet  m_PreRunComponentSet;      
      PRZComponentSet  m_PostRunComponentSet;
            
   private:
      Boolean          m_DestroyChildren;
   };

//*************************************************************************

#endif

// fin del fichero
