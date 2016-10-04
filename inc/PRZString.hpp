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
// File: ../ATCSimul/inc/PRZString.hpp
//
// Description:   fichero de encabezado de la clase "PRZString"
//
// Notes:   La clase PRZString proporciona un tratamiento
//               avanzado de las cadenas de caracteres.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZString.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZString_HPP__
#define __PRZString_HPP__

//*****************************************************************
   
   #include <iostream.h>
   #include <fstream.h>
   #include <stdlib.h>


//*****************************************************************

   class PRZString
   {
   public:

      PRZString();
      PRZString(const PRZString& aPRZString);
      PRZString(const char* aString);
      PRZString(char aChar);
      PRZString(int aInt);
      PRZString(long aLong);
      PRZString(unsigned uint);
      PRZString(unsigned long ulong);
      PRZString(double aDouble);

      ~PRZString();

      unsigned size() const { return m_Tamano; }
      
      int    isLike(const PRZString& aString) const;
      int    asInteger() const;
      double asDouble() const;

      unsigned indexOf(const PRZString& aPRZString, unsigned startPos=1) const;
      unsigned indexOf(char aCharacter, unsigned startPos=1) const;
      unsigned lastIndexOf(char aCharacter) const;

      PRZString  subString(unsigned first, unsigned increment=0) const;
      PRZString  word(unsigned index) const;
      PRZString  firstWord() const;
      PRZString  lastWord() const;
      void       removeFirstWord();

      void removeFirst();
      void removeLast();
      PRZString& remove(const PRZString& string);
      void replace(char org, char dst);
                 
      PRZString& upperCase();
      PRZString& lowerCase();

      PRZString& sprintf(const char *format, ...);
      
      void setChar(int index, char car);

      static PRZString  stripBlanks(const PRZString& aPRZString);
      static PRZString  invert(const PRZString& aPRZString);
      static PRZString  lineFrom(istream& aStream, char delim='\n');

      static PRZString  lowerCase(const PRZString& aString);
      static PRZString  upperCase(const PRZString& aString);
      static PRZString  getStringLeftTo(const PRZString& text, char delim);
      static PRZString  getStringRightTo(const PRZString& text, char delim);
		static PRZString  withoutBlanks(const PRZString& string);
           
      int        operator == (const PRZString& aPRZString) const;
      int        operator == (const char* pString) const;
      int        operator != (const PRZString& aPRZString) const;
      int        operator != (const char* pString) const;
      char       operator [] (unsigned index) const;
      PRZString  operator +  (const PRZString& aPRZString) const;
      PRZString  operator +  (const char* aString) const;
      PRZString  operator +  (char aCharacter) const;
      PRZString& operator += (const PRZString& aPRZString);
      PRZString& operator += (char car);
      PRZString& operator =  (const PRZString& aPRZString);
      PRZString& operator =  (const char* aString);

      operator char*() const;

      friend ostream&  operator << (ostream& os, const PRZString& aPRZString);
      friend ofstream& operator << (ofstream& os, const PRZString& aPRZString);

   private:
      char*    m_pBuffer;
      unsigned m_Tamano;

   };


#endif


// fin del fichero
