//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*****************************************************************
//
// File: ../ATCSimul/inc/PRZStringGz.hpp
//
// Description:   fichero de encabezado de la clase "PRZStringGz"
//
// Notes:   La clase PRZStringGz proporciona un tratamiento
//               avanzado de las cadenas de caracteres.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZStringGz.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************
#include <zlib.h>

#ifndef __PRZStringGz_HPP__
#define __PRZStringGz_HPP__

#ifndef __PRZString_HPP__
#include <PRZString.hpp>
#endif
   


//*****************************************************************

   class PRZStringGz :public PRZString
   {
   public:
      static PRZStringGz  lineFromGzip(gzFile& fileGz, char delim='\n');
   };


#endif


// fin del fichero
