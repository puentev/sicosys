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
// File: ../ATCSimul/inc/PRZIniFile.hpp
//
// Description:  fichero de encabezado de la clase "PRZIniFile"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZIniFile.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZIniFile_HPP__
#define __PRZIniFile_HPP__

//*************************************************************************

   #ifndef __PRZBuilder_HPP__
   #include <PRZBuilder.hpp>
   #endif
   

//*************************************************************************

   class PRZTag;
   class PRZComponent;

//*************************************************************************

   class PRZIniFile : protected PRZBuilder
   {
   public:
      PRZIniFile(const PRZString& sgmlFile);

      PRZString getValueFor(const PRZString& token);
                                           
      virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
                                                      PRZComponent* owner,
                                                      INDEX& index );
      // Define RTTI
      DEFINE_RTTI(PRZIniFile);
      
   };

//****************************************************


#endif

// fin del fichero
