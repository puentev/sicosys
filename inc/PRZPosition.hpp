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
// File: ../ATCSimul/inc/PRZPosition.hpp
//
// Description:  fichero de encabezado de la clase "PRZPosition"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZPosition.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZPosition_HPP__
#define __PRZPosition_HPP__

//*************************************************************************

   #include <PRZBase.hpp>

//*************************************************************************

   typedef unsigned przCOORDINATE;

//*************************************************************************

   class PRZPosition : public PRZBase
   {
   public:
   
   	typedef enum { X, Y, Z } PRZCoordinate;
      PRZPosition(int x=0, int y=0, int z=0);
      PRZPosition(const PRZPosition& pos);
      
      PRZPosition& operator=(const PRZPosition& pos);
      Boolean      operator==(const PRZPosition& pos) const;
      Boolean      operator!=(const PRZPosition& pos) const;
      PRZPosition  operator+(const PRZPosition& pos) const;
      PRZPosition  operator-(const PRZPosition& pos) const;
      
      int valueForCoordinate( PRZCoordinate coordinate) const;
      void setValueAt(PRZCoordinate coordinate, int value);

      virtual PRZString asString() const;
      
      int getFirstCoordinate() const
      { return m_Values[0];}
      
      // Run time information
      DEFINE_RTTI(PRZPosition);
      
   private:
      int m_Values[3];
         
   };

//*************************************************************************


#endif

// fin del fichero
