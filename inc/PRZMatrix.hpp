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
// File: ../ATCSimul/inc/PRZMatrix.hpp
//
// Description:  fichero de encabezado de la clase "PRZMatrix<T>"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMatrix.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMatrix_HPP__
#define __PRZMatrix_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

//*************************************************************************

   template<class T>
   class PRZMatrix : public PRZBase
   {
   public:
      PRZMatrix(unsigned x, unsigned y, unsigned z);
      ~PRZMatrix();

		void    initialize(const T& data);      
      Boolean setValueAt(unsigned x, unsigned y, unsigned z, const T& data);   
      Boolean valueAt(unsigned x, unsigned y, unsigned z, T& data) const;
   
   private:
      Boolean  isLimitsOk(unsigned x, unsigned y, unsigned z) const;
      
      unsigned m_SizeX;
      unsigned m_SizeY;
      unsigned m_SizeZ;
      T*       m_Root;
   };
   
//*************************************************************************

#include "PRZMatrix.inl"

#endif

// fin del fichero
