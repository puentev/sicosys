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
// File: ../ATCSimul/src/PRZRunTimeInformation.cpp
//
// Clase      : PRZRunTimeInformation
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRunTimeInformation.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRunTimeInformation.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRunTimeInformation.hpp>

//*************************************************************************
//:
//  f: inline const PRZRunTimeInformation* parent() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZRunTimeInformation(const PRZRunTimeInformation* parentClass);
//
//  d:
//:
//*************************************************************************

PRZRunTimeInformation :: PRZRunTimeInformation(const PRZRunTimeInformation* parentClass)
                       : m_Parent(parentClass)
{

}


//*************************************************************************
//:
//  f: int isParentChained(const PRZRunTimeInformation* rtti) const;
//
//  d:
//:
//*************************************************************************

int PRZRunTimeInformation :: isParentChained(const PRZRunTimeInformation* rtti) const
{
   //  buscamos de "rtti" hacia arriba buscando a "this"

   const PRZRunTimeInformation* currentParent = rtti;
   while(currentParent)
   {
      if( currentParent == this ) return 1;
      currentParent = currentParent->parent();
   }

   return 0;
}


//*************************************************************************************


// fin del fichero
