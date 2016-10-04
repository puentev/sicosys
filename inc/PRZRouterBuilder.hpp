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
// File: ../ATCSimul/inc/PRZRouterBuilder.hpp
//
// Description:  fichero de encabezado de la clase "PRZRouterBuilder"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRouterBuilder.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZRouterBuilder_HPP__
#define __PRZRouterBuilder_HPP__

//*************************************************************************

   #ifndef __PRZBuilder_HPP__
   #include <PRZBuilder.hpp>
   #endif
   
//*************************************************************************

   class PRZRouterBuilder : public PRZBuilder
   {
     
   public:
      PRZRouterBuilder(const PRZString& sgmlFile);

      virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
                                                      PRZComponent* owner,
                                                      INDEX& index );
      // Define RTTI
      DEFINE_RTTI(PRZRouterBuilder);      
};

//****************************************************


#endif

// fin del fichero
