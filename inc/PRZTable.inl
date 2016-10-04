//*************************************************************************
//
// Fichero    :  prztable.inl
//
// Descripcion:  fichero de metodos inline de la clase template:
//               PRZQueueTable<T>
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZTable.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZTable.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:17  vpuente
// Nuevo repositorio en marrajo 
//
// Revision 1.3  2000/12/01 09:58:17  esteban
// Ajuste de formato
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
// Revision 1.1  1999/07/07  12:16:56  vpuente
// Initial revision
//
// Revision 1.1  1999/07/07  12:16:56  vpuente
// Initial revision
//
//*************************************************************************

template <class T>
PRZQueueTable<T> :: PRZQueueTable(unsigned size)
                  : m_Size(size),
                    m_Elements(0),
                    m_Array(0)
{
   m_Array = new T*[size];
   for( register int i=0; i<getSize(); i++ )
      m_Array[i] = 0;
}

//*************************************************************************

template <class T>
PRZQueueTable<T> :: PRZQueueTable()
                  : m_Size(1),
                    m_Elements(0),
                    m_Array(0)
{
   m_Array = new T*[1];
   for( register int i=0; i<getSize(); i++ )
      m_Array[i] = 0;
}


//*************************************************************************

template <class T>
PRZQueueTable<T> :: ~PRZQueueTable()
{
   for( register int i=0; i<getSize(); i++ )
      delete m_Array[i];
      
   delete[] m_Array;
}

//*************************************************************************

template <class T>
T& PRZQueueTable<T> :: element(unsigned index)
{
   if( ! indexValid(index) ) 
   {
      PRZString texto = "element() : Index out of range: ";
      texto += PRZString(index);
      EXIT_PROGRAM(texto);   
   }

   return (T&)(*(m_Array[index-1]));

}

//*************************************************************************

template <class T>
void PRZQueueTable<T> :: remove(unsigned index)
{
   if( ! indexValid(index) ) return;

   T* temp = m_Array[index-1];
   delete temp;
   *(m_Array+index-1) = 0;
   
   for( register int i=0, j=0; i<getSize(); i++ )
   {
      if( m_Array[i] )
      {
         m_Array[j] = m_Array[i];
         j++;
      }
   }
   
   m_Elements--;

}

//*************************************************************************

template <class T>
void PRZQueueTable<T> :: swap(unsigned index1, unsigned index2)
{
   if( index1 == index2 ) return;
   if( ! indexValid(index1) ) return;
   if( ! indexValid(index2) ) return;

   T* temp = m_Array[index1-1];
   m_Array[index1-1] = m_Array[index2-1];
   m_Array[index2-1] = temp;
}

//*************************************************************************

template <class T>
void PRZQueueTable<T> :: add(const T& data)
{
   if( m_Elements >= getSize() )
   {
      EXIT_PROGRAM("Queue table full");   
   }
   
   m_Array[m_Elements] = new T(data);
   
   m_Elements++;
}

//*************************************************************************

template <class T>
Boolean PRZQueueTable<T> :: indexValid(unsigned index)
{
   if( (!index) || (index > m_Elements) )
   {
      return false;
   }
   return true;
}

//*************************************************************************


// fin del fichero
