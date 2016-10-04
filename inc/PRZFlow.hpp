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
// File: ../ATCSimul/inc/PRZFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZFlow_HPP__
#define __PRZFlow_HPP__

//*************************************************************************

   #include <PRZBase.hpp>

   #ifndef __PRZPriorityQueue_HPP__
   #include <PRZPriorityQueue.hpp>
   #endif

   #ifndef __PRZEvent_HPP__
   #include <PRZEvent.hpp>
   #endif

   #ifndef __SFSPool_HPP__
   #include <SFSPool.hpp>
   #endif
   
//*************************************************************************

   typedef PRZPriorityQueue<PRZEvent> TEventQueue;
   typedef SFSPool<PRZMessage> SFSMessagePool;

//*************************************************************************

   class PRZComponent;
   class PRZInterfaz;
   class PRZRouter;

//*************************************************************************

   class PRZFlow : public PRZBase
   {
   public:
      PRZFlow( PRZComponent& component);
      virtual ~PRZFlow();

      virtual void run(uTIME runTime);      
      virtual void preRun(uTIME time);
      virtual void postRun(uTIME time);
      virtual void initialize();
      virtual void postInitialize();
      virtual void terminate();

      virtual Boolean inputReading()    = 0;
      virtual Boolean stateChange()     = 0;
      virtual Boolean outputWriting()   = 0;

      virtual Boolean controlAlgoritm(Boolean data=false, int delta=0) = 0;
      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual PRZString getStatus() const;

      PRZInterfaz* inputInterfaz(unsigned number =1 )  const;
      PRZInterfaz* outputInterfaz(unsigned number =1 ) const;
      
      PRZRouter& getOwnerRouter() const;
      PRZComponent& getComponent() const 
      { return (PRZComponent&)m_Component; }
      
      // Run time information
      DEFINE_RTTI(PRZFlow);
      
      SFSMessagePool& getMessagePool() const
      { return *m_MessagePool; } 
      
      void initializePool(unsigned size);

   protected:
      virtual Boolean dispatchEvent(const PRZEvent& event);
      virtual Boolean propagateStop();

      virtual void    setState(int state)
      { m_State = state; }

      virtual int     getState() const
      { return m_State; }
                  
      void setStopped(Boolean stop=true);

      Boolean isStopped() const
      { return m_Stopped; }
      
      TEventQueue& getEventQueue() const
      { return (TEventQueue&)m_EventQueue; }


   protected:
      uTIME getHeaderDelay() const;
      uTIME getDataDelay() const;
      
      uTIME getRegisterTime() const
      { return m_RegisterTime; }

      void setRegisterTime(); 

      PRZInterfaz**  m_InputInterfaz;
      PRZInterfaz**  m_OutputInterfaz;
      
   private:
      PRZComponent&   m_Component;
      Boolean         m_Stopped;
      TEventQueue     m_EventQueue;
      int             m_State;
      uTIME           m_RegisterTime;     
      static SFSMessagePool*  m_MessagePool;
      unsigned m_poolSize;

};


//*************************************************************************

#endif


// fin del fichero
