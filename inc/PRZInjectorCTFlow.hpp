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
// File: ../ATCSimul/inc/PRZInjectorCTFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZInjectorCTFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZInjectorCTFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZInjectorCTFlow_HPP__
#define __PRZInjectorCTFlow_HPP__

//*************************************************************************

   #include <PRZInjectorFlow.hpp>

//*************************************************************************


   class PRZInjectorCTFlow : public PRZInjectorFlow
   {
      typedef PRZInjectorFlow Inhereited;
      
   public:
      PRZInjectorCTFlow( PRZComponent& component);
      virtual ~PRZInjectorCTFlow();

      virtual Boolean outputWriting();

      // Run time information
      DEFINE_RTTI(PRZInjectorCTFlow);

   private:

      Boolean    isHeaderSent() const
      { return m_HeaderSent; }
      
      void setHeaderSent(Boolean value)
      { m_HeaderSent = value; }
      
      Boolean         m_HeaderSent;
};

//*************************************************************************

#endif


// fin del fichero
