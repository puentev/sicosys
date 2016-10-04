//*************************************************************************
//
// Fichero    : SFSPool.inl
//
// Descripcion: fichero de metodos inline de la clase template: SFSPool<T>
//
// Comentarios:  
//
// Autor:        Esteban Fernandez Stafford
//
// $RCSfile: SFSPool.inl,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
// $Log: SFSPool.inl,v $
// Revision 1.1.1.1  2001/10/08 05:43:07  vpuente
// Revision previa a SC2001
//
// Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
// Nuevo repositorio en marrajo 
//
// Revision 1.1.2.8  2001/07/17 10:43:53  vpuente
// Hamilton Adaptativo y determinista metidos
//
// Revision 1.1.2.7  2001/06/11 11:57:24  esteban
// Posibilidad de anular el pool.
//
// Revision 1.1.2.6  2001/03/01 12:17:49  esteban
// Nuevo Router de Burbuja Adaptativo con canales virtuales para cc-NUMA
// y bufferes multiplexados para prevencion de HLB.
// Modificacion de PRZOutputPort::SendDataRightNow para que no se envie
// el mismo mensaje dos veces.
// Modificacion del PRZConsumerFlow para recuprar su estado anterior, ya no
// es necesario evitar que vengan mensajes repetidos.
// El router con Arbitraje SIC ya funciona.
//
// Revision 1.1.2.5  2001/02/28 12:23:33  esteban
// Nuevo Router Canales Adaptativos 2D Worm-Hole header de un solo flit y
// Arbitraje SIC (Este router no funciona correctamente, posiblemente el
// crossbar permite que se lean los mensajes mas de una vez. Para evitar
// problemas con la localizacion de memoria se ha hecho un apaño en el
// inputReading() del PRZConsumerFlow).
//
// Revision 1.1.2.4  2001/02/20 09:49:02  esteban
// Añadido soporte para routers con control de flujo wormhole con canales virtuales
//
// Revision 1.1.2.3  2001/02/19 14:53:30  esteban
// Optimizacion de SFSPool::Release
//
// Revision 1.1.2.2  2001/02/01 12:57:13  vpuente
// "Se incorpora control mas optimizado del Pool"
//
// Revision 1.1.2.1  2001/01/17 15:18:13  esteban
// Nueva gestion de mensajes con un Pool
//
//
//**************************************************************************

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

template <class T>
SFSPool<T>::SFSPool(int size)
{
#ifndef NO_POOL
   m_size=size;
   if((m_owned = new int[size]) == 0)
   {
      cerr << "Allocation error 1\n";
      exit(1);
   }
   if((m_pool = new T[size]) == 0)
   {
      cerr << "Allocation error 2\n";
      exit(1);
   }
   for(int i=0; i<m_size; i++)
      *(m_owned+i)=0;
   m_current=0;
   m_ocupation=0;
#endif
}

template <class T>
SFSPool<T>::~SFSPool()
{
   #ifndef NO_TRAZA
      PRZWRITE2LOG( PRZString("SFSPool: Removing ") + PRZString(m_ocupation) + " unreleased items\n" );
  	#endif
   cerr << "SFSPool: Removing " << m_ocupation << " unreleased items\n";
   delete [] m_pool;
   delete [] m_owned;
}

template <class T>
inline T* SFSPool<T>::allocate()
{
#ifndef NO_POOL
   if(m_size-1 == m_ocupation) return 0;
   do
   { 
      ++m_current%=m_size;
   }
   while(*(m_owned+m_current));
   (*(m_owned+m_current))=1;
   m_ocupation++;
   #ifndef NO_TRAZA
      PRZWRITE2LOG( PRZString("SFSPool: Allocating ") + PRZString((int)(m_pool+m_current)));
 	#endif
   return m_pool+m_current;
#else
   return new T;
#endif
}

template <class T>
inline void SFSPool<T>::release(T* item)
{
#ifndef NO_POOL
   if(!item)
   { 
      #ifndef NO_TRAZA
         PRZWRITE2LOG( PRZString("SFSPool: Releasing null item"));
   	#endif
      cerr << "SFSPool: Releasing null item\n";
   }
   if((*(m_owned+(item - m_pool)))==0) 
   {
      #ifndef NO_TRAZA
         PRZWRITE2LOG( PRZString("SFSPool: Releasing unallocated item ") + PRZString((int)item));
   	#endif
      cerr << "SFSPool: Releasing unallocated item\n";
   }
   //Clear used element
   T clearTipe;
   *item=clearTipe;
   
   m_ocupation--;
   (*(m_owned+(item - m_pool)))=0;
   #ifndef NO_TRAZA
      PRZWRITE2LOG( PRZString("SFSPool: Releasing ") + PRZString((int)item));
 	#endif
   #else
      delete  item;
   #endif
}
//************************************************************************


// fin del fichero
