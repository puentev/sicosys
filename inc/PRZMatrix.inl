//*************************************************************************
//
// Fichero    :  przmatr.inl
//
// Descripcion:  fichero de metodos inline de la clase template:
//               PRZMatrix<T>
//
// Autor:        Jose Miguel Prellezo Gutierrez
//
// $RCSfile: PRZMatrix.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: PRZMatrix.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
// Nuevo repositorio en marrajo 
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
//   PRZMatrix(unsigned x, unsigned y, unsigned z=1);
//
//*************************************************************************

template <class T>
PRZMatrix<T> :: PRZMatrix(unsigned x, unsigned y, unsigned z)
             : m_SizeX(x),
               m_SizeY(y),
               m_SizeZ(z),
               m_Root(0)
{
   m_Root = new T[m_SizeX*m_SizeY*m_SizeZ];
}


//*************************************************************************
//
//   ~PRZMatrix();
//
//*************************************************************************

template <class T>
PRZMatrix<T> :: ~PRZMatrix()
{
   if(m_Root) delete[] m_Root;
}


//*************************************************************************
//
//   Boolean valueAt(unsigned x, unsigned y, unsigned z, T& data) const;
//
//*************************************************************************

template <class T>
Boolean PRZMatrix<T> :: valueAt(unsigned x, unsigned y, unsigned z, T& data) const
{
   if( isLimitsOk(x,y,z) )
   {
      data = *( m_Root + x + y*m_SizeX + z*m_SizeX*m_SizeY );
      return true;
   }
   return false;
}


//*************************************************************************
//
//   Boolean setValueAt(unsigned x, unsigned y, unsigned z, const T& data);
//
//*************************************************************************

template <class T>
Boolean PRZMatrix<T> :: setValueAt( unsigned x, 
                                    unsigned y, 
                                    unsigned z, 
                                    const T& data)
{
   if( isLimitsOk(x,y,z) )
   {
      *( m_Root + x + y*m_SizeX + z*m_SizeX*m_SizeY ) = data;
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
void PRZMatrix<T> :: initialize(const T& data)
{
   unsigned size = m_SizeX*m_SizeY*m_SizeZ;
   for(unsigned i=0; i<size; i++)
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
Boolean PRZMatrix<T> :: isLimitsOk(unsigned x, unsigned y, unsigned z) const
{
   return ( (x<m_SizeX) && (y<m_SizeY) && (z<m_SizeZ) ) ? true : false;
}

            
// fin del fichero
