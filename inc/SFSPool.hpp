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
// File: ../ATCSimul/inc/SFSPool.hpp
//
// Description:  fichero de encabezado de la clase template: SFSPool<T>
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: SFSPool.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//**************************************************************************

#ifndef __SFSPool_HPP__
#define __SFSPool_HPP__

//************************************************************************

template <class T>
class SFSPool
{
   public:
   SFSPool(int size);
   ~SFSPool();

   T* allocate();
   void release(T* item);
  
   int allocated() { return m_ocupation; }
   int free()      { return  m_size - m_ocupation;}
   private:
   int m_ocupation;
   int m_size;
   int m_current;
   T * m_pool;
   int * m_owned;
};

//************************************************************************

#include "SFSPool.inl"

//************************************************************************

#endif

// fin del fichero
