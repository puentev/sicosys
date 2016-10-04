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
// File: ../ATCSimul/src/PRZStringGz.cpp
//
// Clase      : PRZStringGz
//
// Hereda de  : None
//
// Description:  Esta clase proporciona un tratamiento avanzado de cadenas
//              de caracteres, permitiendo de una forma flexible y potente 
//              las operaciones con y entre strings;
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZStringGz.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZStringGz.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZStringGz.hpp>
#include <zlib.h>


//*************************************************************************
//:
//  f: PRZStringGz lineFrom ( istream& aStream, char delim);
//
//  d:
//:
//*************************************************************************

PRZStringGz PRZStringGz :: lineFromGzip ( gzFile& fileGz, char delim)
{
   PRZStringGz result;
   char car=0;
//   while((!gzeof(fileGz))&&(car!='\n'))
//   {
//      gzread(fileGz,&car,1);
//      result+=car;
//   }
   return result;
}

//*************************************************************************

// fin del fichero
