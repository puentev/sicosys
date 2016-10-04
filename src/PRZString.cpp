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
// File: ../ATCSimul/src/PRZString.cpp
//
// Clase      : PRZString
//
// Hereda de  : None
//
// Description:  Esta clase proporciona un tratamiento avanzado de cadenas
//              de caracteres, permitiendo de una forma flexible y potente 
//              las operaciones con y entre strings;
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZString.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZString.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZString.hpp>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <fstream.h>
/*#include <zlib.h>*/

//*************************************************************************
//:
//  f: inline unsigned size() const; 
//  d: Devuelve el numero de caracteres del string;
//: 
//*************************************************************************

//*************************************************************************
//:
//  f: PRZString();
// 
//  d: Constructor por defecto. Inicializa el buffer a 0;
//: 
//*************************************************************************

PRZString :: PRZString() 
           : m_pBuffer(0), 
             m_Tamano(0)
{

}


//*************************************************************************
//:
//  f: PRZString(const PRZString& aPRZString);
//  
//  d: Constructor copia. Construye un buffer y copia todos los caracteres;
//:
//*************************************************************************

PRZString :: PRZString(const PRZString& aString)
           : m_Tamano(aString.m_Tamano),
             m_pBuffer(0)
{
   if(m_Tamano)
   {
      m_pBuffer = new char[m_Tamano+1];
      memcpy(m_pBuffer,aString.m_pBuffer,(m_Tamano+1)*sizeof(char));
   }
}


//*************************************************************************
//:
//  f: PRZString(const char* aString);
//
//  d: Construye un string a partir de una cadena de caracteres;
//:
//*************************************************************************

PRZString :: PRZString(const char* aString)
           : m_Tamano( strlen(aString)  ),
             m_pBuffer(0)
{
   if( m_Tamano )
   {
      m_pBuffer = new char[ m_Tamano+1 ];
      strcpy(m_pBuffer,aString);
   }
}


//*************************************************************************
//:
//  f: PRZString(char aChar);
//
//  d: Construye un string que contiene un unico caracter;
//:
//*************************************************************************

PRZString :: PRZString(char aChar)
           : m_Tamano( 1 )
{
   m_pBuffer = new char[2];
   m_pBuffer[0] = aChar;
   m_pBuffer[1] = '\0';
}


//*************************************************************************
//:
//  f: PRZString(int aInt);
//
//  d: Se construye un string que se compone de los digitos que representan
//     el numero entero que se pasa como parametro;
//:
//*************************************************************************

PRZString :: PRZString(int aInt)
{
   char temp[22];
   ::sprintf(temp,"%d",aInt);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: PRZString(long aLong);
//
//  d: Se construye un string que se compone de los digitos que representan
//     el numero entero largo que se pasa como parametro;
//:
//*************************************************************************

PRZString :: PRZString(long aLong)
{
   char temp[22];
   ::sprintf(temp,"%d",aLong);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: PRZString(unsigned uint);
//
//  d: Se construye un string que se compone de los digitos que representan
//     el numero entero sin signo que se pasa como parametro;
//:
//*************************************************************************

PRZString :: PRZString(unsigned uint)
{
   char temp[22];
   ::sprintf(temp,"%d",uint);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: PRZString(unsigned long ulong);
//
//  d: Se construye un string que se compone de los digitos que representan
//     el numero entero largo sin signo que se pasa como parametro;
//:
//*************************************************************************

PRZString :: PRZString(unsigned long ulong)
{
   char temp[22];
   ::sprintf(temp,"%d",ulong);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: PRZString(double aDouble);
//
//  d: Se construye un string que se compone de los digitos que representan
//     el numero real que se pasa como parametro;
//:
//*************************************************************************

PRZString :: PRZString(double aDouble)
{
   char temp[30];
   ::sprintf(temp,"%g",aDouble);
   m_Tamano = strlen(temp) ;
   m_pBuffer = new char[ m_Tamano+1 ];
   strcpy(m_pBuffer,temp);
}


//*************************************************************************
//:
//  f: ~PRZString();
//
//  d: Destructor. Borra el buffer de caracteres si es que fue creado;
//:
//*************************************************************************

PRZString :: ~PRZString()
{
   delete[] m_pBuffer;
}


//*************************************************************************
//:
//  f: int isLike(const PRZString& aString) const;
//
//  d: Retorna 1 cuando el patron que se pasa como parametro coincide con
//     el string del objeto. En otro caso, se retorna 0;
//:
//*************************************************************************

int PRZString :: isLike(const PRZString& aString) const
{
   if( aString == "*" )
   {
      return 1;
   }
   else if( aString == *this )
   {
      return 1;
   }
   
   return 0;
}

//*************************************************************************
//:
//  f: int asInteger() const;
//
//  d:
//:
//*************************************************************************

int PRZString :: asInteger() const
{
   if( m_pBuffer )
      return atoi(m_pBuffer);
      
   return -1;
}


//*************************************************************************
//:
//  f: double asDouble() const;
//
//  d:
//:
//*************************************************************************

double PRZString :: asDouble() const
{
   if( m_pBuffer )
      return (double)(atof(m_pBuffer));
      
   return -1.0;
}


//*************************************************************************
//:
//  f: PRZString& sprintf(const char *format, ...);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: sprintf(const char *format, ...)
{
    va_list ap;
    va_start( ap, format );
    char buffer[1024];
    vsprintf( buffer, format, ap );
    va_end( ap );
    return operator=(buffer);
}


//*************************************************************************
//:
//  f: unsigned indexOf(const PRZString& aPRZString, unsigned startPos) const;
//
//  d:
//:
//*************************************************************************

unsigned PRZString :: indexOf(const PRZString& aPRZString, unsigned startPos) const
{
   if( startPos>size() ) return 0;

   char* pSource = (char *)aPRZString;
   char* pExiste = strstr( m_pBuffer, pSource );
   if( !pExiste ) return 0;

   unsigned offset = 0;
   for( unsigned i=startPos-1; i<size(); i++)
      if( pExiste == (m_pBuffer+i) )
      {
          offset = i+1;
          break;
      }
   return offset;
}


//*************************************************************************
//:
//  f: unsigned indexOf(char aCharacter, unsigned startPos) const;
//
//  d:
//:
//*************************************************************************

unsigned PRZString :: indexOf(char aCharacter, unsigned startPos) const
{
   if( startPos>size() ) return 0;

   unsigned offset = 0;
   for( unsigned i=startPos-1; i < size(); i++ )
      if( aCharacter==*(m_pBuffer+i) )
      {
         offset = i + 1;
         break;
      }

   return offset;
}


//*************************************************************************
//:
//  f: unsigned lastIndexOf(char aCharacter) const;
//
//  d:
//:
//*************************************************************************

unsigned PRZString :: lastIndexOf(char aCharacter) const
{
   unsigned offset = 0;
   for( int i=size()-1; i>=0; i-- )
      if( aCharacter==*(m_pBuffer+i) )
      {
         offset = i + 1;
         break;
      }
   return offset;
}


//*************************************************************************
//:
//  f: PRZString subString(unsigned first, unsigned increment) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: subString(unsigned first, unsigned increment) const
{
   PRZString rs;
   if( (first+increment-1) > size() )
       return rs;

   unsigned length = (increment) ? increment : m_Tamano-first+1;
   char* pTemp = new char[length+1];
   for(register int i=0; i<length; i++)
       pTemp[i] = m_pBuffer[first+i-1];

   pTemp[length] = '\0';
   
   rs = pTemp;
   delete[] pTemp;
   return rs;
}


//*************************************************************************
//:
//  f: PRZString word(unsigned index) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: word(unsigned index) const
{
   PRZString sinBlancos = PRZString::stripBlanks(*this);

   if( sinBlancos.size()==0 )
      return "";

   sinBlancos += PRZString(" ");
   unsigned firstIndex=1;
   unsigned numeroPalabra=0;

   PRZString palabraBuscada;
   int lastCharacterIsSpace = 0;

   for( unsigned lastIndex=1; lastIndex<=sinBlancos.size(); lastIndex++ )
   {
      char caracter = sinBlancos[lastIndex];
      if( isspace(caracter) )
      {
         if( !lastCharacterIsSpace )
         {
            numeroPalabra++;
            if( numeroPalabra==index )
            {
               unsigned delta = lastIndex-firstIndex;
               palabraBuscada = sinBlancos.subString( firstIndex, delta);
               break;
            }
            else
            {
               firstIndex = lastIndex + 1;
            }
         }
         lastCharacterIsSpace = 1;
      }
      else
         lastCharacterIsSpace = 0;

   }
   return PRZString::stripBlanks(palabraBuscada);
}


//*************************************************************************
//:
//  f: PRZString firstWord() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: firstWord() const
{
   return word(1);
}


//*************************************************************************
//:
//  f: PRZString lastWord() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: lastWord() const
{
   PRZString reflejo = PRZString::invert(*this);
   return PRZString::invert(reflejo.word(1)) ;
}


//*************************************************************************
//:
//  f: void removeFirstWord();
//
//  d:
//:
//*************************************************************************

void PRZString :: removeFirstWord()
{
   PRZString temp = firstWord();
   int index = temp.indexOf(temp);
   (*this) = subString(index+temp.size()+1);
}


//*************************************************************************
//:
//  f: void removeFirst();
//
//  d:
//:
//*************************************************************************

void PRZString :: removeFirst()
{
   (*this) = subString(2);
}


//*************************************************************************
//:
//  f: void removeLast();
//
//  d:
//:
//*************************************************************************

void PRZString :: removeLast()
{
   (*this) = subString(1,size()-1);
}


//*************************************************************************
//:
//  f: PRZString& remove(const PRZString& string);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: remove(const PRZString& string)
{
   int index  = indexOf(string);
   if( index )
   {
      (*this) = ( (index==1) ? PRZString("") : subString(1,index-1) ) + 
                subString(index+string.size());
   }
     
   return *this;
}


//*************************************************************************
//:
//  f: void replace(char org, char dst);
//
//  d:
//:
//*************************************************************************

void PRZString :: replace(char org, char dst)
{
   for(register int i=0; i<m_Tamano; i++)
   {
      if( *(m_pBuffer+i) == org ) 
         *(m_pBuffer+i) = dst;
   }
}

//*************************************************************************
//:
//  f: PRZString& upperCase();
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: upperCase()
{
   char current;
   for(int i=0; i<m_Tamano; i++)
   {
      current = *(m_pBuffer+i);
      if( islower(current) )
         *(m_pBuffer+i) = toupper(current);
   }
   return *this;
}


//*************************************************************************
//:
//  f: PRZString& lowerCase();
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: lowerCase()
{
   char current;
   for(int i=0; i<m_Tamano; i++)
   {
      current = *(m_pBuffer+i);
      if( isupper(current) )
         *(m_pBuffer+i) = tolower(current);
   }
   return *this;
}


//*************************************************************************
//:
//  f: static PRZString lowerCase(const PRZString& aString);
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: lowerCase(const PRZString& aString)
{
   PRZString returnSt = aString;
   return returnSt.lowerCase();
}


//*************************************************************************
//:
//  f: static PRZString lowerCase(const PRZString& aString);
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: upperCase(const PRZString& aString)
{
   PRZString returnSt = aString;
   return returnSt.upperCase();
}


//*************************************************************************
//:
//  f: int operator == (const PRZString& aPRZString) const;
//
//  d:
//:
//*************************************************************************

int PRZString :: operator == (const PRZString& aPRZString) const
{
   if( size() != aPRZString.size() )
       return 0;

   for( unsigned i=0; i < size(); i++)
      if( *(m_pBuffer+i) != aPRZString[i+1] )
         return 0;
   return 1;
}


//*************************************************************************
//:
//  f: int operator == (const char* pString) const;
//
//  d:
//:
//*************************************************************************

int PRZString :: operator == (const char* pString) const
{
   PRZString tempString(pString);
   return  PRZString::operator==(tempString);
}


//*************************************************************************
//:
//  f: int operator != (const PRZString& aPRZString) const;
//
//  d:
//:
//*************************************************************************

int PRZString :: operator != (const PRZString& aPRZString) const
{
   if( size() != aPRZString.size() )
       return 1;

   for( unsigned i=0; i < size(); i++)
      if( *(m_pBuffer+i) != aPRZString[i+1] )
         return 1;
   return 0;
}


//*************************************************************************
//:
//  f: int operator != (const char* pString) const;
//
//  d:
//:
//*************************************************************************

int PRZString :: operator != (const char* pString) const
{
   return operator!=(PRZString(pString));
}


//*************************************************************************
//:
//  f: operator char* () const;
//
//  d:
//:
//*************************************************************************

PRZString :: operator char* () const
{
   return m_pBuffer;
}


//*************************************************************************
//:
//  f: char operator [] (unsigned index) const;
//
//  d:
//:
//*************************************************************************

char PRZString :: operator [] (unsigned index) const
{
   return ((index<=m_Tamano)&&index) ? (m_pBuffer[index-1]) : 0;
}


//*************************************************************************
//:
//  f: PRZString operator + (const PRZString& aPRZString) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: operator + (const PRZString& aString) const
{
   PRZString rs = *this;
   
   if( aString.m_Tamano )
   {
      unsigned sizeTotal = m_Tamano + aString.m_Tamano + 1;
      char* pTemp = new char[sizeTotal];
      memset(pTemp,0,sizeTotal*sizeof(char));
      if(m_pBuffer) strcpy( pTemp, m_pBuffer );
      strcat( pTemp,aString.m_pBuffer );
      rs = pTemp;
      delete[] pTemp;
   }
   return rs;
}


//*************************************************************************
//:
//  f: PRZString operator + (const char* aString) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: operator + (const char* aString) const
{
   return operator+(PRZString(aString));
}


//*************************************************************************
//:
//  f: PRZString operator + (char aCharacter) const;
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: operator + (char aCharacter) const
{
   return operator+(PRZString(aCharacter));
}


//*************************************************************************
//:
//  f: PRZString& operator += (const PRZString& aPRZString);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: operator += (const PRZString& aString)
{
   if( aString.m_Tamano  )
   {
      PRZString temp = operator+(aString);
      operator=(temp);
   }
   return *this;
}


//*************************************************************************
//:
//  f: PRZString& operator += (char car);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: operator += (char car)
{
   return operator+=(PRZString(car));
}


//*************************************************************************
//:
//  f: PRZString& operator = (const PRZString& aPRZString);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: operator = (const PRZString& aString)
{
   delete[] m_pBuffer;
   m_pBuffer = 0;
   
   m_Tamano = aString.m_Tamano;
   
   if( m_Tamano )
   {
      m_pBuffer = new char[m_Tamano+1];
      memcpy(m_pBuffer,aString.m_pBuffer,(m_Tamano+1)*sizeof(char));
   }
   
   return *this;
}


//*************************************************************************
//:
//  f: PRZString& operator = (const char* aString);
//
//  d:
//:
//*************************************************************************

PRZString& PRZString :: operator = (const char* aString)
{
   return operator=(PRZString(aString));
}



//*************************************************************************
//:
//  f: PRZString lineFrom ( istream& aStream, char delim);
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: lineFrom ( istream& aStream, char delim)
{
   PRZString result;
   // unsigned bytes = 0;
   char c;

   for( aStream.get(c); c!=delim && !aStream.fail(); aStream.get(c) )
       result += c;

   return result;
}

//*************************************************************************
//:
//  f: PRZString stripBlanks();
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: stripBlanks(const PRZString& aPRZString)
{
   if( aPRZString.size() == 0 ) return "";

   unsigned first, last;
   for( first=1; isspace(aPRZString[first]); first++ );
   for( last=aPRZString.size(); isspace(aPRZString[last]); last-- );

   if( first > last ) return "";

   PRZString palabraSinBlancos = aPRZString;
   return palabraSinBlancos.subString(first,last-first+1);
}


//*************************************************************************
//:
//  f: static PRZString invert();
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: invert(const PRZString& aPRZString)
{
   PRZString reflejo;
   for( unsigned i=aPRZString.size(); i>=1; i-- )
      reflejo += aPRZString[i];
   return reflejo;
}


//*************************************************************************
//:
//  f: static PRZString  getStringLeftTo(const PRZString& text, char delim);
//
//  d:g
//:
//*************************************************************************

PRZString PRZString :: getStringLeftTo(const PRZString& text, char delim)
{
   unsigned index = text.indexOf(delim);
   if( index )
      return text.subString(1,index-1);
      
   // En caso de no existir el delimitador, se devuelve el string quer se
   // ha pasado como referencia.
   
   return text;
}


//*************************************************************************
//:
//  f: static PRZString  getStringRightTo(const PRZString& text, char delim);
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: getStringRightTo(const PRZString& text, char delim)
{
   unsigned index = text.indexOf(delim);
   if( index )
      return text.subString(index+1);
   
   // En caso de no exitir el delimitador, se devuelve un string vacio.
   return "";
}


//*************************************************************************
//:
//  f: static PRZString withoutBlanks(const PRZString& text);
//
//  d:
//:
//*************************************************************************

PRZString PRZString :: withoutBlanks(const PRZString& text)
{
   PRZString rs;
   for( unsigned i=1; i<=text.size(); i++ )
   {
      if( text[i] != ' ' )
      {
         rs += text[i];
      }
   }
   return rs;
}


//*************************************************************************
//:
//  f: void setChar(int index, char car);
//
//  d:
//:
//*************************************************************************

void PRZString :: setChar( int index, char car )
{
   if( index && index<=m_Tamano ) m_pBuffer[index-1] = car;
}


//*************************************************************************
//:
//  f: friend ostream& operator << ( ostream& os, const PRZString& aPRZString );
//
//  d:
//:
//*************************************************************************

ostream& operator << ( ostream& os, const PRZString& aPRZString )
{
   char* buffer= (char *)aPRZString;
   if(buffer)
      os << buffer;
   return os;
}


//*************************************************************************
//:
//  f: friend ofstream& operator << ( ofstream& ofs, const PRZString& aPRZString );
//
//  d:
//:
//*************************************************************************

ofstream& operator << ( ofstream& ofs, const PRZString& aPRZString )
{
   char* buffer= (char *)aPRZString;
   if(buffer)
      ofs.operator<<(buffer);
   return ofs;
}


//*************************************************************************

// fin del fichero
