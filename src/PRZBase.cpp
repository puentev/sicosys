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
// File: ../ATCSimul/src/PRZBase.cpp
//
// Clase      : PRZBase
//
// Hereda de  : None
//
// Notes:   La clase PRZBase es el root de la jerarquia de herencia
//               de la mayor parte de las clases en la PRZApplication.
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZBase.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZBase.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZBase.hpp>
#include <stdlib.h>

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZBase);

//*************************************************************************

PRZString PRZBase :: sm_ErrorLocation = "";
PRZString PRZBase :: sm_ErrorText = "" ;

//*************************************************************************
//:
//  f: inline static void setErrorLocation(const PRZString& loc);
//  d:
//
//  f: inline static void setErrorText(const PRZString& text);
//  d:
//
//  f: inline static PRZString errorText();
//  d:
//
//  f: inline static PRZString errorLocation();
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZBase();
//
//  d:
//:
//*************************************************************************

PRZBase :: PRZBase()
{

}


//*************************************************************************
//:
//  f: PRZBase(const PRZBase& aBase);
//
//  d:
//:
//*************************************************************************

PRZBase :: PRZBase(const PRZBase& aBase)
{

}


//*************************************************************************
//:
//  f: ~PRZBase();
//
//  d:
//:
//*************************************************************************

PRZBase :: ~PRZBase()
{

}


//*************************************************************************
//:
//  f: void printError(const PRZString& errorText);
//
//  d:
//:
//*************************************************************************

void PRZBase :: printError(const PRZString& errorText)
{
   cout << errorText;
}


//*************************************************************************
//:
//  f: void exitProgram(int exitCode);
//
//  d:
//:
//*************************************************************************

void PRZBase :: exitProgram(int exitCode)
{
   exit(exitCode);
}

//*************************************************************************
//*************************************************************************

przROUTINGTYPE _string2routingType(const PRZString& string)
{
   if( string == PRZ_TAG_XPLUS ) return _Xplus_;
   else if( string == PRZ_TAG_XMINUS ) return _Xminus_;
   else if( string == PRZ_TAG_YPLUS )  return _Yplus_;
   else if( string == PRZ_TAG_YMINUS ) return _Yminus_;
   else if( string == PRZ_TAG_ZPLUS )  return _Zplus_;
   else if( string == PRZ_TAG_ZMINUS ) return _Zminus_;
   else if( string == PRZ_TAG_LOCAL )  return _LocalNode_;
   
   return _Unknow_;
}

//*************************************************************************

PRZString _routingType2string(przROUTINGTYPE type)
{
   switch(type)
   {
      case _Xplus_ :    return PRZ_TAG_XPLUS;
      case _Xminus_:    return PRZ_TAG_XMINUS;
      case _Yplus_ :    return PRZ_TAG_YPLUS;
      case _Yminus_:    return PRZ_TAG_YMINUS;
      case _Zplus_ :    return PRZ_TAG_ZPLUS;
      case _Zminus_:    return PRZ_TAG_ZMINUS;
      case _LocalNode_: return PRZ_TAG_LOCAL;
   }
   
   return "";
}
  
//*************************************************************************

   
// fin del fichero
