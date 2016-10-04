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
// File: ../ATCSimul/inc/PRZConsumer.hpp
//
// Description:  fichero de encabezado de la clase "PRZConsumer"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZConsumer.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZConsumer_HPP__
#define __PRZConsumer_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;
   class PRZTag;
   
//************************************************************************

   class PRZConsumer : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;
      
   public:
      PRZConsumer(const PRZComponentId& id);
      virtual ~PRZConsumer();
      
      virtual void      initialize();
      virtual PRZString asString() const;

      // Run time information
      DEFINE_RTTI(PRZConsumer);
   
   protected:      
      virtual void buildFlowControl();
      virtual void buildInputInterfaz(unsigned number=1);
      
   private:
      static PRZConsumer* newFrom( const PRZTag* tag, 
                                   PRZComponent* owner );

   };

//*************************************************************************

#endif


// fin del fichero
