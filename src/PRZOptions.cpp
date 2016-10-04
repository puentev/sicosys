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
// File: ../ATCSimul/src/PRZOptions.cpp
//
// Clase      : PRZOptions
//
// Hereda de  : None
//
// Description:  Permite acceder a las diferentes opciones que se pueden
//              pasar por la linea de comandos a un programa de forma
//              clara y eficiente;
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZOptions.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZOptions.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

//*************************************************************************

#include <PRZOptions.hpp>

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZOptions);


//*************************************************************************
//:
//  f: PRZOptions(int argc, char* argv[]);
//
//  d:
//:
//*************************************************************************

PRZOptions :: PRZOptions(int argc, char* argv[])
{
   for( int i=1; i<argc; i++ )
   {
      m_OptionsLine += PRZString(argv[i]) + PRZString(" ");
   }
}


//*************************************************************************
//:
//  f: PRZOptions(const PRZString& line);
//
//  d:
//:
//*************************************************************************

PRZOptions :: PRZOptions( const PRZString& line )
            : m_OptionsLine(line) 
{

}


//*************************************************************************
//:
//  f: PRZString getValueForOption(char opt) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZOptions :: getValueForOption(char opt) const
{
   PRZString option = PRZString("-") + PRZString(opt);
   
   unsigned index = m_OptionsLine.indexOf(option);
   if( !index ) return "";
   if( (index>1) && (m_OptionsLine[index-1]!=' ') ) return "";
   
   index += 2;
   
   if( index > m_OptionsLine.size() ) return "";
      
   PRZString line = m_OptionsLine.subString(index);
   line = PRZString::stripBlanks(line);
   
   return line.word(1);
}

//*************************************************************************
//:
//  f: int isOptionActive(char opt) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZOptions :: isOptionActive(char opt) const
{
   PRZString option = PRZString("-") + PRZString(opt);   
   unsigned index = m_OptionsLine.indexOf(option);
   return (index==0) ? false : true;
}


//*************************************************************************

// fin del fichero
