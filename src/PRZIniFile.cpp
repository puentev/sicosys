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
// File: ../ATCSimul/src/PRZIniFile.cpp
//
// Clase      : PRZIniFile
//
// Hereda de  : PRZBuilder
//
// Notes:   
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZIniFile.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#include <PRZIniFile.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZIniFile,PRZBuilder);

//*************************************************************************
//:
//  f: PRZIniFile( const PRZString& sgmlFile);
//
//  d:
//:
//*************************************************************************

PRZIniFile :: PRZIniFile(const PRZString& sgmlFile)
            : PRZBuilder(sgmlFile)
{
   loadSgmlInformation();
}


//*************************************************************************
//:
//  f: PRZString getValueFor(const PRZString& token);
//
//  d:
//:
//*************************************************************************

PRZString PRZIniFile :: getValueFor(const PRZString& token)
{      
   unsigned index = getTagIndexWithName(token);
   PRZTag* tag = getTagWithIndex(index);
   if(!tag) return "";
   return tag->tagId();
}


//*************************************************************************
//:
//  f: virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
//                                                     PRZComponent* owner,
//                                                     INDEX& index ) = 0;                                                      
//
//  d:
//:
//*************************************************************************
PRZComponent* PRZIniFile :: parseComponentDefinition( const PRZTag* tag, 
                                                      PRZComponent* owner,
                                                      INDEX& index )
{
   return 0;
}


//*************************************************************************


// fin del fichero
