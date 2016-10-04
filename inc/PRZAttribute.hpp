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
// File: ../ATCSimul/inc/PRZAttribute.hpp
//
// Description:  fichero de encabezado de la clase "PRZAttribute"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZAttribute.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZAttribute_HPP__
#define __PRZAttribute_HPP__

//************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

//************************************************************************

   class ostream;

//************************************************************************

   class PRZAttribute
   {
   public:
      PRZAttribute(const PRZString& name, const PRZString& value);
      PRZAttribute(const PRZAttribute& attr);
      ~PRZAttribute();

      const PRZString& name() const
      { return m_Name; }

      const PRZString& value() const
      { return m_Value; }

      friend ostream& operator<<(ostream &aStream, const PRZAttribute& attr);

      // Run time information
      DEFINE_RTTI(PRZAttribute);

   private:
      PRZString m_Name;
      PRZString m_Value;
   };


//************************************************************************

#endif


// fin del fichero
