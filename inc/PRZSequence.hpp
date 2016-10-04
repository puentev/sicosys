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
// File: ../ATCSimul/inc/PRZSequence.hpp
//
// Description:   fichero de encabezado de la clase template:
//               "PRZSequence"
//
// Notes:   La clase PRZSequence<T> es una lista en la que se van
//               anadiendo los elementos a traves del metodo "add" de
//               forma LIFO. Si empleamos el metodo "addAsLast", los
//               elementos se agregan al final (FIFO). Podemos recorrer
//               la lista con ayuda de un cursor.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZSequence.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZSequence_HPP__
#define __PRZSequence_HPP__

//*************************************************************************

   #ifndef __PRZBaseCollection_HPP__
   #include <PRZBaseCollection.hpp>
   #endif

//*************************************************************************

   template <class T>
   class PRZSequence : public  PRZBaseCollection<T>
   {
      typedef PRZBaseCollection<T> Inhereited;

   public:
      PRZSequence();
      PRZSequence(const PRZSequence<T>& aSeq);

      Boolean add(const T& data);
      Boolean addAsLast(const T& data);
      Boolean firstElement(T& data);
      Boolean lastElement(T& data);
   };

//*************************************************************************

   #include "PRZSequence.inl"

//*************************************************************************

#endif

// fin del fichero
