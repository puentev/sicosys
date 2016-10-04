//*************************************************************************
//
// Fichero    :  przseq.inl
//
// Descripcion:  fichero de metodos inline de la clase template:
//               PRZSequence<T>
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZSequence.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZSequence.inl,v $
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

template <class T>
PRZSequence<T> :: PRZSequence()
{

}

//************************************************************************

template <class T>
PRZSequence<T> :: PRZSequence(const PRZSequence<T>& aSeq)
{
   typename PRZSequence<T>::Cursor cursor(aSeq);
   forCursor(cursor)
   {
      addAsLast(cursor.element());
   }
}

//************************************************************************

template <class T>
Boolean PRZSequence<T> :: add(const T& data)
{
   return Inhereited::insertBefore(data,this->getRoot());
}

//************************************************************************

template <class T>
Boolean PRZSequence<T> :: addAsLast(const T& data)
{
   return Inhereited::insertAfter(data,this->getTail());
}

//************************************************************************

template <class T>
Boolean PRZSequence<T> :: firstElement(T& data)
{
   if( this->isValid() )
   {
      data = this->getRoot()->element();
      return true;
   }
   return false;
}

//************************************************************************

template <class T>
Boolean PRZSequence<T> :: lastElement(T& data)
{
   if( this->isValid() )
   {
      PRZNode<T>* lastNode = this->getRoot();
      data = this->getTail->element();
      return true;
   }
   return false;
}

//************************************************************************


// fin del fichero
