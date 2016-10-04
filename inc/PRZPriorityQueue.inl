//*************************************************************************
//
// Fichero    :  przprioq.inl
//
// Descripcion:  fichero de metodos inline de la clase template:
//               PRZPriorityQueue<T>
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZPriorityQueue.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZPriorityQueue.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
// Nuevo repositorio en marrajo 
//
// Revision 1.5  2000/12/02 12:33:51  esteban
// Vuelta a la revision 1.2 pero se utiliza la funcion getTail() para insertar
// un objeto al final de la lista en la funcion enqueue(const T&,uTIME).
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
//   PRZPriorityQueue()
//
//*************************************************************************

template <class T>
PRZPriorityQueue<T> :: PRZPriorityQueue()
                     : m_Notifier(0L)
{
   m_Notifier = new PRZNotifier();
}


//*************************************************************************
//
//   ~PRZPriorityQueue()
//
//*************************************************************************

template <class T>
PRZPriorityQueue<T> :: ~PRZPriorityQueue()
{
   delete m_Notifier;
}


//*************************************************************************
//
//   Boolean enqueue(const T& data, przTIME timeStamp)
//
//*************************************************************************

template <class T>
Boolean PRZPriorityQueue<T> :: enqueue(const T& data, uTIME timeStamp)
{
   PRZTimeStamp<T> newElement(data,timeStamp);
   if( ! this->isValid() )
   {
      // no se ha insertado nada aun. Se crea m_pRoot
      Boolean retorno = Inhereited::insertAfter(newElement,0L);
      if(retorno) onFirstElement();
      return retorno;
   }
   PRZNode<PRZTimeStamp<T> >* tempNode = this->getRoot();
   while(tempNode)
   {
      if( tempNode->element().timeStamp() > timeStamp )
      {
         Boolean retorno = Inhereited::insertBefore(newElement,tempNode);
         return retorno;
      }
      tempNode = tempNode->getNext();
   }
   // Si llega aqui se debe insertar al final de la lista.
   return Inhereited::insertAfter(newElement,this->getTail());
}


//*************************************************************************
//
//   Boolean enqueue(const T& data)
//
//*************************************************************************

template <class T>
Boolean PRZPriorityQueue<T> :: enqueue(const T& data)
{
   return PRZPriorityQueue<T>::enqueue(data,0);
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean PRZPriorityQueue<T> :: dequeue(T& data)
{
   Boolean retorno = false;
   if( this->isValid() )
   {
      PRZTimeStamp<T> temp = this->getRoot()->element();
      data = temp.element();
      retorno = Inhereited::remove(this->getRoot());
      // Si despues de borrar el top de la cola, existen mas datos, se
      // notificara a los observers.
      
      // if( isValid() ) onFirstElement();
   
   }
   return retorno;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean PRZPriorityQueue<T> :: firstElement(T& data)
{
   if( this->isValid() )
   {
      PRZTimeStamp<T> temp = this->getRoot()->element();
      data = temp.element();
      return true;
   }
   return false;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
Boolean PRZPriorityQueue<T> :: firstElementTimeStamp(uTIME& time)
{ 
   if( this->isValid() )
   {
      PRZTimeStamp<T> temp = this->getRoot()->element();
      time = temp.timeStamp();      
      return true;
   }
   
   return false;
}


//*************************************************************************
//
//
//
//*************************************************************************

template <class T>
void PRZPriorityQueue<T> :: onFirstElement()
{
//   if(m_Notifier)
//      m_Notifier->notifyObservers(_FirstElement_);
}

//*************************************************************************


// fin del fichero
