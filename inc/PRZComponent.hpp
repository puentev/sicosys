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
// File: ../ATCSimul/inc/PRZComponent.hpp
//
// Description:  fichero de encabezado de la clase "PRZComponent"
//
// Notes:   La clase PRZComponent es una de las mas importantes.
//               A traves del patron de diseno Composite, es capaz de
//               implementar cualquier componente que integra la
//               arquitectura de un simulador.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZComponent.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZComponent_HPP__
#define __PRZComponent_HPP__

//************************************************************************

   #ifndef __PRZNotifier_HPP__
   #include <PRZNotifier.hpp>
   #endif

   #ifndef __PRZComponentId_HPP__
   #include <PRZComponentId.hpp>
   #endif

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif
      
//*************************************************************************

   class PRZMessage;
   class PRZConnection;
   class PRZInterfaz;
   class PRZBuilder;

//*************************************************************************

   class PRZComponent : public PRZNotifier
   {      
   public:   
      typedef PRZSequence<PRZConnection*> PRZConnectionSet;

      PRZComponent(const PRZComponentId& id);
      PRZComponent(const PRZComponent& component);
      virtual ~PRZComponent();

      virtual void          run(uTIME runTime=0) = 0;
      virtual void          preRun(uTIME runTime);
      virtual void          postRun(uTIME runTime);
      virtual void          initialize();
      virtual void          terminate();
      virtual void          postInitialize();
      
      virtual PRZComponent* componentWithId(const PRZComponentId& id);
      virtual PRZComponent* componentWithIdLike(const PRZComponentId& id);
      virtual PRZComponent* componentWithName(const PRZString& name);
      
      virtual void          addComponent(PRZComponent* aComponent);
      virtual void          addPreRunComponent(PRZComponent* aComponent);
      virtual void          addPostRunComponent(PRZComponent* aComponent);
      virtual void          removeAllComponents();
      virtual PRZInterfaz*  getInputInterfaz(unsigned number = 1);
      virtual PRZInterfaz*  getOutputInterfaz(unsigned number = 1);
      
      void    addConnection(PRZConnection* connection);
      
      virtual Boolean  onDataUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onDataDown(unsigned interfaz, unsigned cv);
      virtual Boolean  onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean  onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onStopDown(unsigned interfaz, unsigned cv);
      virtual PRZString getStatus() const;      
      virtual PRZString asString() const;

      PRZComponentId& componentId() const
      { return (PRZComponentId&)m_Identifier; }

      virtual void setOwner(PRZComponent* owner)
      { m_Owner = owner; }

      PRZComponent* getOwner() const
      { return m_Owner; }

      virtual void setRouter(PRZComponent* router)
      { m_Router = router; }

      PRZComponent* getRouter() const
      { return m_Router; }

      przROUTINGTYPE getRoutingDirection() const
      { return m_Identifier.componentType(); }

      unsigned getChannel() const
      { return m_Identifier.channel(); }
      
      void setRoutingType(przROUTINGTYPE type)
      { m_Identifier.setComponentType(type); }

      PRZString getName() const
      { return m_Identifier.name(); }

      void setName(const PRZString& aName)
      { m_Identifier.setName(aName); }

      void setChannel(unsigned number)
      { m_Identifier.setChannel(number); }
      
      // Run time information
      DEFINE_RTTI(PRZComponent);

   protected:
      Boolean isInitializated() const
      { return m_Initializated; }

      Boolean isTerminated() const
      { return m_Terminated; }

      void setInitializated(Boolean val=true)
      { m_Initializated = val; }

      void setTerminated(Boolean val=true)
      { m_Terminated = val; }

      void setAutoDeleteConnectionSet(Boolean val)
      { m_DeleteConnectionSet = val; }

   public:
      PRZComponent* getSimulation() const
      { return m_Simulation; }

      virtual void setSimulation(PRZComponent* simulation)
      { m_Simulation = simulation; }
      
   protected:
      PRZConnectionSet   m_ConnectionSet;
      PRZComponentId     m_Identifier;
      PRZComponent*      m_Owner;
      PRZComponent*      m_Router;
      Boolean            m_Initializated;
      Boolean            m_Terminated;
      Boolean            m_DeleteConnectionSet;
         
   public:
      static PRZBuilder* routerBuilder;
      static PRZBuilder* networkBuilder;
      static PRZBuilder* simulationBuilder;
   
   private:
      PRZComponent* m_Simulation;

   };


//*************************************************************************

   #define PRZSIMULATION ((PRZSimulation*)(getComponent().getSimulation()))
   #define PRZNETWORK    (PRZSIMULATION->getNetwork())
   #define PRZROUTER     ((PRZRouter*)getOwner())
   
//*************************************************************************

#endif


// fin del fichero
