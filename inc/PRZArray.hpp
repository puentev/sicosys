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
// File: ../ATCSimul/inc/PRZArray.hpp
//
// Description:  fichero de encabezado de la clase "PRZArray<T>"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZArray.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZArray_HPP__
#define __PRZArray_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

//*************************************************************************

   template<class T>
   class PRZArray : public PRZBase
   {
   public:
      PRZArray(unsigned index);
      ~PRZArray();

		void     initialize(const T& data);      
      Boolean  setValueAt(unsigned index, const T& data);   
      Boolean  valueAt(unsigned index, T& data) const;
      Boolean  valueAt(unsigned index, T* data) const;
      unsigned numberOfElements() const
      { return m_Size; }
  
   private:
      Boolean  isLimitsOk(unsigned index) const;
      
      unsigned m_Size;
      T*       m_Root;
   };
   
//*************************************************************************

#include "PRZArray.inl"

#endif

// fin del fichero
