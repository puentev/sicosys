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
// File: ../ATCSimul/inc/PRZPriorityQueue.hpp
//
// Description:   fichero de encabezado de la clase "PRZPriorityQueue"
//
// Notes:   La clase PRZPriorityQueue<T> es una cola en la que
//               los datos se ordenan en funcion de la prioridad que
//               se le asigne. Esta prioridad se representa por un
//               valor de tiempo. Los elementos con menor tiempo estan
//               en la cima de la cola.
//               La cola notifica cada vez que se inserta un elemento
//               en el top. Para acceder al elemento notificador se
//               proporciona un metodo "getNotifier()". Asi no es
//               necesario utilizar la herencia multiple.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZPriorityQueue.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZPriorityQueue_HPP__
#define __PRZPriorityQueue_HPP__

//************************************************************************

   #ifndef __PRZBaseCollection_HPP__
   #include <PRZBaseCollection.hpp>
   #endif

   #ifndef __PRZNotifier_HPP__
   #include <PRZNotifier.hpp>
   #endif

//************************************************************************

template<class T>
class PRZTimeStamp
{
public:
   PRZTimeStamp() : m_Time(0.0)
   {  }

   PRZTimeStamp(const T& data, uTIME time) :
               m_Element(data),
               m_Time(time)
   {  }

   PRZTimeStamp(const PRZTimeStamp<T>& aTimeStamp) :
               m_Element(aTimeStamp.m_Element),
               m_Time(aTimeStamp.m_Time)
   {  }

   uTIME timeStamp() const
   { return m_Time; }

   T element() const
   { return m_Element; }

   PRZString asString() const
   {  return PRZString(timeStamp()) + " " + element().asString(); }
   
   Boolean operator==(const PRZTimeStamp<T>& aTimeS) const
   {
      if( (m_Element == aTimeS.element()) &&
          (m_Time == aTimeS.timeStamp()) )
         return true;
      return false;
   }
private:
   T       m_Element;
   uTIME m_Time;
};

//************************************************************************

template <class T>
class PRZPriorityQueue : public PRZBaseCollection<PRZTimeStamp<T> >
{
   typedef PRZBaseCollection<PRZTimeStamp<T> > Inhereited;

public:

   PRZPriorityQueue();
   ~PRZPriorityQueue();

   Boolean enqueue(const T& data, uTIME timeStamp);
   Boolean enqueue(const T& data);

   Boolean dequeue(T& data);
   
   Boolean firstElement(T& data);
   Boolean firstElementTimeStamp(uTIME& time);

   PRZNotifier* getNotifier() const
   { return m_Notifier; }

private:

   void onFirstElement();
   PRZNotifier* m_Notifier;
};

//************************************************************************

#include "PRZPriorityQueue.inl"

#endif


// fin del fichero
