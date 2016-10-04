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
// File: ../ATCSimul/inc/PRZRouter.hpp
//
// Description:  fichero de encabezado de la clase "PRZRouter"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRouter.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRouter_HPP__
#define __PRZRouter_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>
   
   #ifndef __PRZPosition_HPP__
   #include <PRZPosition.hpp>
   #endif

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif

//************************************************************************

   class PRZFifoMemory;
   class PRZPRZMultiportFifo;
   class PRZInjector;
   class PRZGauge;
   
//************************************************************************

   class PRZRouter : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      typedef PRZSequence<PRZFifoMemory*> PRZBufferList;
      typedef PRZSequence<PRZComponent*>  PRZInjectorList;
      typedef PRZSequence<PRZComponent*>  PRZConsumerList;
      typedef PRZSequence<PRZMessage*>    PRZMessageList;
      
      friend class PRZRouterBuilder;
      friend class PRZGauge;
      
   public:

      enum { INJECT_RANDOM, INJECT_ROUND_ROBIN, INJECT_FREE };
            
      PRZRouter(const PRZComponentId& id);
      virtual ~PRZRouter();

      virtual PRZString asString() const;
      virtual void run(uTIME runTime);
      virtual void initialize();
      virtual void terminate();
      virtual void postInitialize();
      virtual void sendMessage(PRZMessage* msg);

      virtual unsigned getBufferOccupation( przROUTINGTYPE type,
                                            unsigned channel =1 );
      
      unsigned getMessagesToSend() const;      
      void writeBufferInformation(ostream& os);
      void writeBufferStatus(ostream& os);
      
      void addBuffer(PRZFifoMemory* fifo);      
      void addInjector(PRZComponent* injector);
      void addConsumer(PRZComponent* consumer);

      void setBufferSize(unsigned size);
      
      void onPacketReceived(PRZMessage* msg);
      void onHeaderPacketReceived(const PRZMessage* msg);
      void writeComponentStatus(ostream& os);
      
      unsigned getNumberOfBuffers() const
      { return m_BufferList.numberOfElements(); }
      
      uTIME getCurrentTime() const
      { return m_CurrentTime; }

      PRZPosition& getPosition() const
      { return (PRZPosition&)m_Position; }
      
      int getFirstCoordinate() const
      {return m_Position.getFirstCoordinate();}
      
      void setPosition(const PRZPosition& position)
      { m_Position=position; }

      void setInjectorType(int type)
      { m_InjectionType = type; }

      void* getExternalInfo() { 
         void* rc=m_ExternalInfo; 
         m_ExternalInfo=0; 
         return rc;
      }

      void setExternalInfo(void* ptr)
      { m_ExternalInfo = ptr; }
      
      void setPoolSize(unsigned size);
      void addToPoolSize(unsigned size) { m_ExtraPoolSize += size; };
            
      // Run time information
       DEFINE_RTTI(PRZRouter);

      unsigned   getInputWithType(przROUTINGTYPE type);
      unsigned   getOutputWithType(przROUTINGTYPE type);

      unsigned   valueForBufferSize();
      PRZString  valueForBufferControl();
      PRZString  valueForRoutingControl();
      
   protected:
      virtual void buildFlowControl();
      virtual void buildInputInterfaz(unsigned number=1);
      virtual void buildOutputInterfaz(unsigned number=1);

      virtual PRZInjector* chooseInjector();
      
   private:
      unsigned getTotalBuffeSize()const;
      
      static PRZRouter* newFrom( const PRZTag* tag, 
                                 PRZComponent* owner, 
                                 INDEX& i);

      uTIME           m_CurrentTime;
      PRZPosition     m_Position;
      PRZBufferList   m_BufferList;
      PRZInjectorList m_InjectorList;
      PRZConsumerList m_ConsumerList;
      unsigned        m_InjectorRoundRobin;
      int             m_InjectionType;
      PRZMessageList  m_MessageList;
      void*           m_ExternalInfo;
      unsigned        m_ExtraPoolSize;
      
   };

//*************************************************************************

#endif


// fin del fichero
