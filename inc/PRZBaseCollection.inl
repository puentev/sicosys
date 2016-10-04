//*************************************************************************
//
// Fichero    :  przcoll.inl
//
// Descripcion:  fichero de metodos inline de las clases templates:
//               PRZNode<T>, PRZBaseCollection<T> y PRZCursor<T>.
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZBaseCollection.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZBaseCollection.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
// Nuevo repositorio en marrajo 
//
// Revision 1.3  2000/11/22 00:05:21  esteban
// Agrega elementos a la cola sin tener que recorrerla entera
//
// Revision 1.2  2000/11/18 19:07:49  esteban
// Arreglo de los comentarios de la historia.
//
// Revision 1.1.1.1  2000/11/18 17:43:25  esteban
// Al principio todo era un Caos...
//
// Revision 1.2  2000/05/18 17:47:54  vpuente
// *** empty log message ***
//
// Revision 1.1  1999/07/07  12:18:34  vpuente
// Initial revision
//
//*************************************************************************

//*************************************************************************
//
//   PRZBaseCollection()
//
//*************************************************************************

template <class T>
PRZBaseCollection<T> :: PRZBaseCollection()
                      : m_pRoot(0L),
		        m_pTail(0L),
                        m_NumberElements(0)
{

}


//*************************************************************************
//
//   ~PRZBaseCollection()
//
//*************************************************************************

template <class T>
PRZBaseCollection<T> :: ~PRZBaseCollection()
{
//   removeAllElements();
}


//*************************************************************************
//
//   void removeAllElements()
//
//*************************************************************************

template <class T>
void PRZBaseCollection<T> :: removeAllElements()
{
   if( isValid() )
   {
      PRZNode<T>* tempRoot = m_pRoot;
      while(tempRoot)
      {
         m_pRoot = m_pRoot->getNext();
         delete tempRoot;
         tempRoot = m_pRoot;
      }
      m_NumberElements=0;
      m_pTail = 0L;
   }
}


//*************************************************************************
//
//   Boolean elementAt(unsigned index, T& element) const;
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: elementAt(unsigned index, T& element) const
{
   // Los valores posibles de "index" estan entre 0 y numberOfElements()-1
   if( index<numberOfElements() )
   {
      PRZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      element = tempNode->element();
      return true;
   }
   return false;
}
//*************************************************************************
//
//      Boolean  removeAt(unsigned index) ;
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: removeAt(unsigned index)
{
   // Los valores posibles de "index" estan entre 0 y numberOfElements()-1
   if( index<numberOfElements() )
   {
      PRZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      remove(tempNode);
      return true;
   }
   return false;
}
//*************************************************************************
//
//      Boolean  removeAt(unsigned index) ;
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: insertAt(const T& data, unsigned index)
{
   // Los valores posibles de "index" estan entre 0 y numberOfElements()-1
   if( index<numberOfElements() )
   {
      PRZNode<T>* tempNode = m_pRoot;
      unsigned tempIndex = index;
      while(tempIndex)
      {
         tempNode = tempNode->getNext();
         if(!tempNode) return false;
         tempIndex--;
      }
      insertAfter(data,tempNode);
      return true;
   }
   return false;
}

//*************************************************************************
//
//   Boolean allElementsDo(Boolean (*function)(T& data) )
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: allElementsDo(Boolean (*function)(T& data) )
{
   typename PRZBaseCollection<T>::Cursor cursor(*this);
   forCursor(cursor)
   {
      if( (*function)((T&)cursor.element()) == 0)
         return false;
   }
   return true;
}


//*************************************************************************
//
//   Boolean containsElement(const T& dataElement)
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: containsElement(const T& dataElement)
{
   typename PRZBaseCollection<T>::Cursor cursor(*this);
   forCursor(cursor)
   {
      if( cursor.element() == dataElement )
         return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean insertBefore(const T& data, PRZNode<T>* node)
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: insertBefore(const T& data, PRZNode<T>* node)
{
   PRZNode<T>* newNode = new PRZNode<T>(data);
   if(!newNode) return false;
   if(!node)
   {
      // En este caso, se anade al principio de la coleccion
      if(m_pRoot)
      {
         m_pRoot->setBack(newNode);
         newNode->setNext(m_pRoot);
         newNode->setBack(0);
      }
      else
      {
         // Como no habia root, pTail tambien apunta al ultimo elemento.
         m_pTail = newNode;
      }
      m_pRoot = newNode;
   }
   else
   {
      PRZNode<T>* left = node->getBack();
      if(!left) // node == m_pRoot
         m_pRoot = newNode;
      newNode->setNext(node);
      newNode->setBack(left);
      if(left)
         left->setNext(newNode);
      node->setBack(newNode);
   }
   m_NumberElements++;
   return true;
}


//*************************************************************************
//
//   Boolean insertAfter(const T& data, PRZNode<T>* node)
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: insertAfter(const T& data, PRZNode<T>* node)
{
   PRZNode<T>* newNode = new PRZNode<T>(data);
   if(!newNode) return false;
   if(!node)
   {
      // En este caso, se anade al principio de la coleccion
      if(m_pRoot)
      {
         m_pRoot->setBack(newNode);
         newNode->setNext(m_pRoot);
         newNode->setBack(0);
      }
      else
      {
         // Como no habia root, pTail tambien apunta al ultimo elemento.
         m_pTail = newNode;
      }
      m_pRoot = newNode;
   }
   else
   {
      PRZNode<T>* right = node->getNext();
      newNode->setNext(right);
      newNode->setBack(node);
      if(right)
         right->setBack(newNode);
      else
         m_pTail = newNode;
      node->setNext(newNode);
      
   }
   m_NumberElements++;
   return true;
}


//*************************************************************************
//
//   Boolean remove(PRZNode<T>* node)
//
//*************************************************************************

template <class T>
Boolean PRZBaseCollection<T> :: remove(PRZNode<T>* node)
{
   if(node)
   {
   
      if( node == m_pRoot )
      {
         m_pRoot = node->getNext();
         if(m_pRoot)
            m_pRoot->setBack(0);
      }
      else
      {
         PRZNode<T>* left  = node->getBack();
         PRZNode<T>* right = node->getNext();
         if(left)  left->setNext(right);
         if(right) right->setBack(left);
      }
      if( node == m_pTail )
      {
         m_pTail = node->getBack();
      }
      
      
      delete node;
      m_NumberElements--;
      if( m_NumberElements && !getRoot() )
      {
         EXIT_PROGRAM("Error referencial en la Collection");
      }
      return true;
   }
   return false;
}


//*************************************************************************
//*************************************************************************
//
//   PRZCursor(const PRZBaseCollection<T>& coll)
//
//*************************************************************************

template <class T>
PRZCursor<T> :: PRZCursor(const PRZBaseCollection<T>& coll)
              : m_Collection((PRZBaseCollection<T>&)coll),
                m_pCurrent(0L),
                m_Valid(false)
{

}


//*************************************************************************
//
//   void setToFirst()
//
//*************************************************************************

template <class T>
void PRZCursor<T> :: setToFirst()
{
   m_pCurrent = m_Collection.getRoot();
   if(m_pCurrent) m_Valid = true;
   else           m_Valid = false;
}


//*************************************************************************
//
//   void setToNext()
//
//*************************************************************************

template <class T>
void PRZCursor<T> :: setToNext()
{
   if( isValid() ) m_pCurrent = m_pCurrent->getNext();
   if(m_pCurrent) m_Valid = true;
   else           m_Valid = false;
}


//*************************************************************************
//
//   void setToLast()
//
//*************************************************************************

template <class T>
void PRZCursor<T> :: setToLast()
{
   if( !isValid() )
   {
      m_pCurrent = m_Collection.getRoot();
      if(m_pCurrent) m_Valid = true;
   }

   if( isValid() )
   {
      while( m_pCurrent->getNext() )
         m_pCurrent = m_pCurrent->getNext();
   }
}


//*************************************************************************
//
//   Boolean isValid() const
//
//*************************************************************************

template <class T>
Boolean PRZCursor<T> :: isValid() const
{
   return m_Valid ;
}


//*************************************************************************
//
//   T element() const
//
//*************************************************************************

template <class T>
T PRZCursor<T> :: element() const
{
   return m_pCurrent->element();
}


//*************************************************************************
//
//   Boolean remove()
//
//*************************************************************************

template <class T>
Boolean PRZCursor<T> :: remove()
{
   Boolean result = m_Collection.remove(m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   Boolean addAfter(const T& data)
//
//*************************************************************************

template <class T>
Boolean PRZCursor<T> :: addAfter(const T& data)
{
   Boolean result = m_Collection.insertAfter(data,m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   Boolean addBefore(const T& data)
//
//*************************************************************************

template <class T>
Boolean PRZCursor<T> :: addBefore(const T& data)
{
   Boolean result = m_Collection.insertBefore(data,m_pCurrent);
   invalidate();
   return result;
}


//*************************************************************************
//
//   void invalidate()
//
//*************************************************************************

template <class T>
void PRZCursor<T> :: invalidate()
{
   m_pCurrent = 0L;
   m_Valid = false;
}

//*************************************************************************


// fin del fichero
