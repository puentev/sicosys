//*************************************************************************
//
// Fichero    :  przarray.inl
//
// Descripcion:  fichero de metodos inline de la clase template:
//               PRZArray<T>
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZArray.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZArray.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
// Nuevo repositorio en marrajo 
//
// Revision 1.2.4.1  2000/12/20 10:31:45  esteban
// Manejo de mensajes con punteros
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
//   PRZArray(unsigned size);
//
//*************************************************************************

template <class T>
PRZArray<T> :: PRZArray(unsigned size)
             : m_Size(size),
               m_Root(0)
{
   m_Root = new T[m_Size];
}


//*************************************************************************
//
//   ~PRZArray(unsigned size);
//
//*************************************************************************

template <class T>
PRZArray<T> :: ~PRZArray()
{
   delete[] m_Root;
}


//*************************************************************************
//
//   Boolean valueAt(unsigned index, T& data) const;
//
//*************************************************************************

template <class T>
Boolean PRZArray<T> :: valueAt(unsigned index, T& data) const
{
   if( isLimitsOk(index) )
   {
      data = *(m_Root+index-1);
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean valueAt(unsigned index, T* data) const;
//
//*************************************************************************

template <class T>
Boolean PRZArray<T> :: valueAt(unsigned index, T* data) const
{
   if( isLimitsOk(index) )
   {
      *data = *(m_Root+index-1);
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean setValueAt(unsigned index, const T& data);
//
//*************************************************************************

template <class T>
Boolean PRZArray<T> :: setValueAt(unsigned index, const T& data)
{
   if( isLimitsOk(index) )
   {
      *(m_Root+index-1) = data;
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean initialize(const T& data);
//
//*************************************************************************

template <class T>
void PRZArray<T> :: initialize(const T& data)
{
   for(int i=0; i<m_Size; i++)
   {
      *(m_Root+i) = data;
   }
}

//*************************************************************************
//
//   Boolean  isLimitsOk(unsigned index) const;
//
//*************************************************************************

template <class T>
Boolean PRZArray<T> :: isLimitsOk(unsigned index) const
{
   return (index>0 && index<=m_Size) ? true : false;
}

            
// fin del fichero
