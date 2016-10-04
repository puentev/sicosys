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
// File: ../ATCSimul/inc/PRZInjector.hpp
//
// Description:  fichero de encabezado de la clase "PRZInjector"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZInjector.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZInjector_HPP__
#define __PRZInjector_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;
   class PRZTag;
   
//************************************************************************

   class PRZInjector : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      typedef enum { WH, CT } PRZInjectorType;
      friend class PRZRouterBuilder;
      
   public:
      PRZInjector(const PRZComponentId& id, PRZInjectorType m_Type);
      virtual ~PRZInjector();

      virtual void      initialize();
      virtual PRZString asString() const;

      void sendMessage(PRZMessage* aMessage);
      unsigned getAvaliableMessages() const;
      unsigned getMessagesToSend() const;
      
      static void setNumberOfHeaders(unsigned number)
      { sm_NumberOfHeaders = number; }

      static unsigned numberOfHeaders()
      { return sm_NumberOfHeaders; }
      
      // Run time information
      DEFINE_RTTI(PRZInjector);
      
      void setMissRouting(int short a)
      {m_MissRouting=a;}
      
      int short setMissRouting() const
      {return m_MissRouting;}
      
      int short getMissRouting()
      {
         return m_MissRouting;
      }
            
      void setIsolate(Boolean a)
      {
         m_isolate= a;
      }
      
      Boolean getIsolate() const
      {
         return m_isolate;
      }
      
   protected:      
      virtual void buildFlowControl();
      
   //esete metodo solo lo puede ver el flujo.

      

   private:
      static PRZInjector* newFrom( const PRZTag* tag, 
                                   PRZComponent* owner );
      PRZInjectorType m_InjectorType;
      static unsigned sm_NumberOfHeaders;
      int short           m_MissRouting;
      Boolean             m_isolate;

   };

//*************************************************************************

#endif


// fin del fichero
