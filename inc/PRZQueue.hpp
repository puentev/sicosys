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
// File: ../ATCSimul/inc/PRZQueue.hpp
//
// Description:   fichero de encabezado de la clase template:
//               "PRZQueue"
//
// Notes:   La clase PRZQueue<T> es una cola en la que se van
//               anadiendo los elementos a traves del metodo "enqueue"
//               de forma FIFO. Para sacar elementos de la cola, se 
//               utiliza el metodo "dequeue".
//
// $Author: vpuente $ 
//
// $RCSfile: PRZQueue.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZQueue_HPP__
#define __PRZQueue_HPP__

//************************************************************************

   #ifndef __PRZBaseCollection_HPP__
   #include <PRZBaseCollection.hpp>
   #endif

//************************************************************************

   template <class T>
   class PRZQueue : public  PRZBaseCollection<T>
   {
      typedef PRZBaseCollection<T> Inhereited;

   public:
      PRZQueue();

      Boolean enqueue(const T& data);
      Boolean dequeue(T& data);
      Boolean firstElement(T& data);
   };

//*************************************************************************

   #include "PRZQueue.inl"
   
//*************************************************************************   

#endif

// fin del fichero
