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
// File: ../ATCSimul/inc/PRZTable.hpp
//
// Description:  fichero de encabezado de la clase "PRZQueueTable"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZTable.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZTable_HPP__
#define __PRZTable_HPP__

//*************************************************************************

   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif

//*************************************************************************

   template <class T>
   class PRZQueueTable
   {
   public:
      PRZQueueTable(unsigned size);
      PRZQueueTable();
      ~PRZQueueTable();

      T& element(unsigned index);
      void remove(unsigned index);
      void add(const T& element);
      void swap(unsigned index1, unsigned index2);
      unsigned numberOfElements() const
      { return m_Elements; }

      unsigned getSize() const
      { return m_Size; }
      
   private:
      Boolean indexValid(unsigned index);

      unsigned m_Size;
      unsigned m_Elements;
      T**      m_Array;
   };

//*************************************************************************

   #include "PRZTable.inl"
   

#endif


// fin del fichero
