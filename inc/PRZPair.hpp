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
// File: ../ATCSimul/inc/PRZPair.hpp
//
// Description:  fichero de encabezado de la clase "PRZPair"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZPair.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZPair_HPP__
#define __PRZPair_HPP__

//*************************************************************************

   template <class Q, class T>
   class PRZPair
   {
   public:
      PRZPair( )
      { }

      PRZPair( const Q& name, const T& value ) :
             m_Left(name), m_Right(value) 
      { }

      PRZPair( const PRZPair<Q,T>& pair ) :
             m_Left(pair.left()), m_Right(pair.right())
      {  }

      PRZPair& operator=(const PRZPair<Q,T>& pair) { 
         m_Left = pair.left(); 
         m_Right= pair.right();
         return *this; 
      }

      Boolean operator==(const PRZPair<Q,T>& pair) const {
         return (m_Left==pair.left()) && (m_Right==pair.right());
      }
            
      Q left() const
      { return m_Left; }
      
      T right() const
      { return m_Right; }
      
      void setLeft(const Q& data)
      { m_Left = data; }

      void setRight(const T& data)
      { m_Right = data; }
   
   private:
      Q m_Left;
      T m_Right;
};

//*************************************************************************

#endif


// fin del fichero
