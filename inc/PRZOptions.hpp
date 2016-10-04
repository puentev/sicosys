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
// File: ../ATCSimul/inc/PRZOptions.hpp
//
// Description:  fichero de encabezado de la clase "PRZOptions"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZOptions.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZOptions_HPP__
#define __PRZOptions_HPP__

//************************************************************************

   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif
   
   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif
   
//************************************************************************

   class PRZOptions
   {
   public:            
      PRZOptions(int argc, char* argv[]);
      PRZOptions(const PRZString& string);               
  
      PRZString getValueForOption(char option) const;
      Boolean   isOptionActive(char option) const;
      
      // Run time information
      DEFINE_RTTI(PRZOptions);
   
   private:
      PRZString m_OptionsLine;
   
   };

//*************************************************************************

#endif


// fin del fichero
