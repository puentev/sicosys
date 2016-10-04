//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZPosition.cpp
//
// Clase      : PRZPosition
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZPosition.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZPosition.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZPosition.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZPosition,PRZBase);

//*************************************************************************
//:
//  f: PRZPosition(int x=0, int y=0, int z=0);
//
//  d:
//:
//*************************************************************************

PRZPosition :: PRZPosition(int x, int y, int z)
{
	m_Values[0] = x;
	m_Values[1] = y;
	m_Values[2] = z;		
}


//*************************************************************************
//:
//  f: PRZPosition(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

PRZPosition :: PRZPosition(const PRZPosition& pos)
{
	m_Values[0] = pos.m_Values[0];
	m_Values[1] = pos.m_Values[1];
	m_Values[2] = pos.m_Values[2];
}


//*************************************************************************
//:
//  f: PRZPosition& operator=(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************
      
PRZPosition& PRZPosition :: operator=(const PRZPosition& pos)
{
	m_Values[0] = pos.m_Values[0];
	m_Values[1] = pos.m_Values[1];
	m_Values[2] = pos.m_Values[2];
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator==(const PRZPosition& pos) const;
//
//  d:
//:
//*************************************************************************
      
Boolean PRZPosition :: operator==(const PRZPosition& pos) const
{
	if( ( m_Values[0] == pos.m_Values[0] ) &&
	    ( m_Values[1] == pos.m_Values[1] ) &&
	    ( m_Values[2] == pos.m_Values[2] ) )
	{
		return true;
	}
   return false;
}


//*************************************************************************
//:
//  f: Boolean operator!=(const PRZPosition& pos) const;
//
//  d:
//:
//*************************************************************************
      
Boolean PRZPosition :: operator!=(const PRZPosition& pos) const
{
	if( ( m_Values[0] != pos.m_Values[0] ) ||
	    ( m_Values[1] != pos.m_Values[1] ) ||
	    ( m_Values[2] != pos.m_Values[2] ) )
	{
		return true;
	}
   return false;
}

//*************************************************************************
//:
//  f: PRZPosition operator+(const PRZPosition& pos) const;
//
//  d:
//:
//*************************************************************************
      
PRZPosition PRZPosition :: operator+(const PRZPosition& pos) const
{
   return PRZPosition( m_Values[0] + pos.m_Values[0],
	                    m_Values[1] + pos.m_Values[1],
	                    m_Values[2] + pos.m_Values[2] );
}


//*************************************************************************
//:
//  f: PRZPosition operator-(const PRZPosition& pos) const;
//
//  d:
//:
//*************************************************************************
      
PRZPosition PRZPosition :: operator-(const PRZPosition& pos) const
{
   return PRZPosition( m_Values[0] - pos.m_Values[0],
	                    m_Values[1] - pos.m_Values[1],
	                    m_Values[2] - pos.m_Values[2] );
}

//*************************************************************************
//:
//  f: virtual int valueForCoordinate( PRZCoordinate coordinate) const;
//
//  d:
//:
//*************************************************************************
      
int PRZPosition :: valueForCoordinate( PRZCoordinate coordinate) const
{
   switch(coordinate)
   {
   	case PRZPosition::X : return m_Values[0];
   	case PRZPosition::Y : return m_Values[1];
   	case PRZPosition::Z : return m_Values[2];
   }
   
   return 0;
}


//*************************************************************************
//:
//  f: virtual void setValueAt( PRZCoordinate coordinate, int value);
//
//  d:
//:
//*************************************************************************
      
void PRZPosition :: setValueAt( PRZCoordinate coordinate, int value)
{
   switch(coordinate)
   {
   	case PRZPosition::X : m_Values[0] = value;
   	                      break;
   	case PRZPosition::Y : m_Values[1] = value;
   	                      break;
   	case PRZPosition::Z : m_Values[2] = value;
   	                      break;
   }
   
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************
      
PRZString PRZPosition :: asString() const
{
      PRZString rc = "(";
      rc += PRZString(m_Values[0]) + "," +
            PRZString(m_Values[1]) + "," +
            PRZString(m_Values[2]) + ")";
      return rc;
}

//*************************************************************************

// fin del fichero
