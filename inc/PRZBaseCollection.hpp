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
// File: ../ATCSimul/inc/PRZBaseCollection.hpp
//
// Description:   fichero de encabezado de las clases templates:
//               PRZNode<T>, PRZBaseCollection<T> y PRZCursor<T>.
//
// Notes:   Estas clases son la base sobre la que se asienta el
//               entramado de las distintas colecciones que se pueden
//               usar, como la PRZSequence, PRZQueue, ...
//
// $Author: vpuente $ 
//
// $RCSfile: PRZBaseCollection.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZBaseCollection_HPP__
#define __PRZBaseCollection_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

//*************************************************************************

   template <class T> class PRZCursor;

//*************************************************************************

   template<class T>
   class PRZNode
   {
   public:

      PRZNode(const T& data, PRZNode<T>* next = 0L, PRZNode<T>* back = 0L) :
              m_Element(data),
              m_pNext(next),
              m_pBack(back)
      {  }

      PRZNode<T>* getNext() const
      { return m_pNext; }

      PRZNode<T>* getBack() const
      { return m_pBack; }

      void setNext(PRZNode<T>* next)
      { m_pNext=next; }

      void setBack(PRZNode<T>* back)
      { m_pBack=back; }

      T element() const
      { return m_Element; }

   private:
      T    m_Element;
      PRZNode<T>* m_pNext;
      PRZNode<T>* m_pBack;
   };

//*************************************************************************


   template <class T>
   class PRZBaseCollection : public PRZBase
   {
      friend class PRZCursor<T>;

   public:

      typedef PRZCursor<T> Cursor;

      virtual  ~PRZBaseCollection();
      void     removeAllElements();
      Boolean  allElementsDo(Boolean (*function)(T& data) );
      Boolean  containsElement(const T& element);
      Boolean  elementAt(unsigned index, T& element) const;
      Boolean  removeAt(unsigned index) ;
      Boolean  insertAt(const T& data, unsigned index);

      unsigned numberOfElements() const
      { return m_NumberElements; }

      Boolean isValid() const
      { return (m_NumberElements ? true : false); }

      unsigned getUserInformation() const { return m_UserInformation; }
      void setUserInformation(unsigned info) { m_UserInformation=info; }
      
   protected:
      PRZBaseCollection();
      Boolean insertBefore(const T& data, PRZNode<T>* node);
      Boolean insertAfter(const T& data, PRZNode<T>* node);
      Boolean remove(PRZNode<T>* node);

      PRZNode<T>* getRoot() const
      { return m_pRoot; }

      PRZNode<T>* getTail() const
      { return m_pTail; }

      void setRoot(PRZNode<T>* newRoot)
      { m_pRoot = newRoot; }

   private:
      PRZNode<T>*       m_pRoot;
      PRZNode<T>*       m_pTail;
      unsigned          m_NumberElements;
      unsigned          m_UserInformation;
   };

//*************************************************************************

   template <class T>
   class PRZCursor
   {
   public:
      PRZCursor(const PRZBaseCollection<T>& coll);
      void setToFirst();
      void setToNext();
      void setToLast();
      Boolean isValid() const;
      T element() const;
      Boolean remove();
      Boolean addAfter(const T& data);
      Boolean addBefore(const T& data);

   private:
      void invalidate();

      PRZBaseCollection<T>& m_Collection;
      PRZNode<T>*           m_pCurrent;
      Boolean               m_Valid;
   };

//*************************************************************************

   #define forCursor(x) for(x.setToFirst();x.isValid();x.setToNext())

//*************************************************************************

   #include <PRZBaseCollection.inl>

//*************************************************************************

#endif


// fin del fichero
