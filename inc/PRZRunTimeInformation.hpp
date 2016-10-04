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
// File: ../ATCSimul/inc/PRZRunTimeInformation.hpp
//
// Description:  fichero de encabezado de la clase "PRZRunTimeInformation"
//
// Notes:   La clase PRZRunTimeInformation sirve para realizar en
//               tiempo de ejecucion castings seguros y fiables. En cada
//               clase de la jerarquia de nuestro diseno es necesario
//               utilizar las macros de definicion e implementacion que
//               se acompanan junto con esta clase
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRunTimeInformation.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRunTimeInformation_HPP__
#define __PRZRunTimeInformation_HPP__

//************************************************************************

   class PRZRunTimeInformation
   {
   public:
      PRZRunTimeInformation(const PRZRunTimeInformation* parentClass);
      int isParentChained(const PRZRunTimeInformation* rtti) const;
      const PRZRunTimeInformation* parent() const
      { return m_Parent; }

   private:
      const PRZRunTimeInformation* m_Parent;
      // static m_ClassNumber;
   };

//************************************************************************

   #define POINTERCAST(classTo,object) \
      ( object ) ? \
      ((classTo::static_Rtti.isParentChained(object->runTimeInfo()) ? \
      ((classTo*)(object)):0)) : 0

   #define DEFINE_RTTI(classDefine) \
            virtual const PRZRunTimeInformation* runTimeInfo() const \
            { return &(classDefine::static_Rtti); } \
            static const PRZRunTimeInformation static_Rtti  

   #define IMPLEMENT_RTTI_DERIVED(ClassImplement, ClassParent) \
            const PRZRunTimeInformation \
            ClassImplement::static_Rtti(&ClassParent::static_Rtti)

   #define IMPLEMENT_RTTI_BASE(ClassImplement) \
            const PRZRunTimeInformation \
            ClassImplement::static_Rtti(0L)

//************************************************************************

#endif


// fin del fichero
