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
// File: ../ATCSimul/src/PRZTag.cpp
//
// Clase      : PRZTag
//
// Hereda de  : PRZBase
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZTag.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZTag.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZTag.hpp>

#ifndef __PRZAttribute_HPP__
#include <PRZAttribute.hpp>
#endif

#include <iostream.h>

//************************************************************************

   static const char  LEFT_DELIMITER        = '<';
   static const char  RIGHT_DELIMITER       = '>';
   static const char* TAG_COMMENT_DELIMITER = "!--";


//************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZTag,PRZBase);

//*************************************************************************
//:
//  f: inline PRZString tagName() const;
//  d: 
//
//  f: inline PRZString tagId() const;
//  d:
//
//  f: inline Boolean isComment() const;
//  d:
//: 
//*************************************************************************


//*************************************************************************
//:
//  f: PRZTag();
// 
//  d: 
//: 
//*************************************************************************

PRZTag :: PRZTag()
        : m_TagName(""),
          m_TagId(""),
          m_Comment(false)
{

}

//*************************************************************************
//:
//  f: PRZTag(const PRZString& name, const PRZString& id);
// 
//  d:
//: 
//*************************************************************************

PRZTag :: PRZTag(const PRZString& name, const PRZString& id)
        : m_TagName(name),
          m_TagId(id),
          m_Comment(false)
{

}

//*************************************************************************
//:
//  f: PRZTag(const PRZTag& aTag);
// 
//  d: 
//: 
//*************************************************************************

PRZTag :: PRZTag(const PRZTag& aTag)
        : m_TagName(aTag.tagName()),
          m_TagId(aTag.tagId()),
          m_Comment(aTag.isComment())
{

   PRZAttributeList* list = (PRZAttributeList*)&(aTag.m_AttributeList);
   PRZAttribute* attr;
   PRZAttributeList::Cursor cursor(*list);
   forCursor(cursor)
   {
      attr = new PRZAttribute(*(cursor.element()) );
      if(attr) m_AttributeList.addAsLast(attr);
   }

}


//*************************************************************************
//:
//  f: ~PRZTag();
// 
//  d: 
//: 
//*************************************************************************

PRZTag :: ~PRZTag()
{
   PRZAttributeList::Cursor cursor(m_AttributeList);
   forCursor(cursor)
   {
      delete cursor.element();
   }

   m_AttributeList.removeAllElements();
}

//*************************************************************************
//:
//  f: unsigned numberOfAttributes() const;
// 
//  d: Constructor por defecto. Inicializa el buffer a 0;
//: 
//*************************************************************************

unsigned PRZTag :: numberOfAttributes() const
{
   return m_AttributeList.numberOfElements();
}


//*************************************************************************
//:
//  f: Boolean getAttributeValueWithName( const PRZString& name, 
//                                        PRZString& value ) const;
// 
//  d: 
//: 
//*************************************************************************

Boolean PRZTag :: getAttributeValueWithName(const PRZString& name, PRZString& value) const
{
   PRZAttributeList::Cursor cursor(m_AttributeList);
   PRZAttribute* attr;
   forCursor(cursor)
   {
       attr = cursor.element();
       if( attr->name() == name )
       {
          value = attr->value();
          return true;
       }
   }
   return false;
}

//*************************************************************************
//:
//  f: Boolean addAttribute(PRZAttribute* attr);
// 
//  d: 
//: 
//*************************************************************************

Boolean PRZTag :: addAttribute(PRZAttribute* attr)
{
   if( (!attr) || m_AttributeList.containsElement(attr) )
      return false;
   m_AttributeList.addAsLast(attr);
   return true;
}


//*************************************************************************
//:
//  f: PRZTag* createTag(const PRZString& tagStr);
// 
//  d: 
//: 
//*************************************************************************

PRZTag* PRZTag :: createTag(const PRZString& tagStr)
{
   PRZTag* tag = 0L;
   PRZString tagString = tagStr;
   if( isStringValid(tagString) )
   {
      Boolean comment = removeDecoration(tagString);
      if( comment )
      {
         tag = new PRZTag(TAG_COMMENT_DELIMITER,tagString);
         tag->m_Comment = true;
         return tag;
      }

      PRZString name = PRZTag::extractTagName(tagString);
      if( name == "" )
         return 0L;

      PRZString tagId = PRZTag::extractTagId(tagString);
      tag = new PRZTag(name,tagId);

      Boolean anadirAttr = true;
      do
      {
         anadirAttr = tag->addAttribute( extractAttribute(tagString) );
      }
      while(anadirAttr);
   }
   return tag;
}

//*************************************************************************
//:
//  f: PRZString extractTagName(PRZString& tagString);
// 
//  d: 
//: 
//*************************************************************************

PRZString PRZTag :: extractTagName(PRZString& tagString)
{
   PRZString name = tagString.firstWord();
   tagString.removeFirstWord();
   return name;
}


//*************************************************************************
//:
//  f: PRZString extractTagId(PRZString& tagString);
// 
//  d: 
//: 
//*************************************************************************

PRZString PRZTag :: extractTagId(PRZString& tagString)
{
   PRZString id = tagString.firstWord();
   PRZString value = "";
   if( id == "id" )
   {
      tagString.removeFirstWord();
      tagString.removeFirstWord();     // "="
      if( tagString[(unsigned)1] != '"' )
      {
         value = tagString.firstWord();
         tagString.removeFirstWord();
      }
      else
      {
         unsigned index = tagString.indexOf('"',2);
         value = tagString.subString(2,index-2);
         tagString = PRZString::stripBlanks(tagString.subString(index+1));
      }
   }
   return value;
}


//*************************************************************************
//:
//  f: PRZAttribute* extractAttribute(PRZString& tagString);
// 
//  d: 
//: 
//*************************************************************************

PRZAttribute* PRZTag :: extractAttribute(PRZString& tagString)
{
   PRZAttribute* attr = 0L;
   PRZString id, value;

   id = tagString.firstWord();
   tagString.removeFirstWord();
   tagString.removeFirstWord();     // "="
   if( tagString[(unsigned)1] != '"' )
   {
      value = tagString.firstWord();
      tagString.removeFirstWord();
   }
   else
   {
      unsigned index = tagString.indexOf('"',2);
      value = tagString.subString(2,index-2);
      tagString = PRZString::stripBlanks(tagString.subString(index+1));
   }

   if( (id=="") || (value=="") )
      return 0L;

   return new PRZAttribute(id,value);
}


//*************************************************************************
//:
//  f: Boolean isStringValid(const PRZString& aTag);
// 
//  d: 
//: 
//*************************************************************************

Boolean PRZTag :: isStringValid(const PRZString& aTag)
{
   PRZString tempString = PRZString::stripBlanks(aTag);
   if( (tempString[(unsigned)1] == LEFT_DELIMITER) &&
       (tempString[tempString.size()] == RIGHT_DELIMITER) )
      return true;
   return false;
}


//*************************************************************************
//:
//  f: Boolean removeDecoration(PRZString& tagString);
// 
//  d: 
//: 
//*************************************************************************

Boolean PRZTag :: removeDecoration(PRZString& tagString)
{
   tagString = PRZString::stripBlanks(tagString);
   if( tagString[(unsigned)1] == LEFT_DELIMITER )
      tagString.removeFirst();
   if( tagString[tagString.size()] == RIGHT_DELIMITER )
      tagString.removeLast();

   if( tagString.subString(1,3) == TAG_COMMENT_DELIMITER )
   {
      tagString = tagString.subString(4);
      return true;   // Es un comentario
   }

   PRZString line;
   char currentCharacter, lastCharacter = 0x0;

   Boolean comillasAbiertas = false;

   for( int i=1; i<=tagString.size(); i++ )
   {
      currentCharacter = tagString[(unsigned)i];
      if( (currentCharacter == '=') && !comillasAbiertas )
      {
         if( lastCharacter != ' ')
            line += " = ";
         else
            line += "= ";
         lastCharacter = ' ';
      }

      else if( (lastCharacter != ' ') || (currentCharacter != ' ') || comillasAbiertas )
      {
         if( currentCharacter == '"' )
            comillasAbiertas = (comillasAbiertas) ? false : true;
         
         line += currentCharacter;
         lastCharacter = currentCharacter;
      }
   } // for

   // en caso de que no exista un numero par de comillas, se anaden unas al final.
   if( comillasAbiertas )
      line += '"';

   tagString = line;
   return false;   // El string no es un comentario.
}


//*************************************************************************
//:
//  f: PRZTag& operator=(const PRZTag& aTag);
// 
//  d: 
//: 
//*************************************************************************

PRZTag& PRZTag :: operator=(const PRZTag& aTag)
{
   m_AttributeList.removeAllElements();

   PRZAttributeList* list = (PRZAttributeList*)&(aTag.m_AttributeList);
   PRZAttribute* attr;
   PRZAttributeList::Cursor cursor(*list);
   forCursor(cursor)
   {
      attr = new PRZAttribute(*(cursor.element()) );
      if(attr) m_AttributeList.addAsLast(attr);
   }
   m_TagName = aTag.tagName();
   m_TagId   = aTag.tagId();
   m_Comment = aTag.isComment();

   return *this;
}


//*************************************************************************
//:
//  f: friend Boolean operator >> (istream &aStream, PRZTag& aTag);
// 
//  d: 
//: 
//*************************************************************************

Boolean operator >> (istream &aStream, PRZTag& aTag)
{
   PRZString line = PRZString::lineFrom(aStream);
   PRZTag* tempTag = PRZTag::createTag(line);

   if(!tempTag) return false;

   aTag = *tempTag;
   delete tempTag;

   return true;
}


//*************************************************************************
//:
//  f: friend ostream& operator << (ostream &os, const PRZTag& aTag);
// 
//  d: 
//: 
//*************************************************************************

ostream& operator << (ostream &os, const PRZTag& aTag)
{
   os << "[tagName= " << aTag.tagName();

   if( aTag.tagId() != "" )
      os << ", tagId= " << aTag.tagId() << "]" << endl;
   else
      os << " ]" << endl;


   PRZTag::PRZAttributeList::Cursor cursor((PRZTag::PRZAttributeList&)aTag.m_AttributeList);
   forCursor(cursor)
   {
      os << "   " << *( cursor.element() ) << endl;
   }


   return os;
}

//************************************************************************


// fin del fichero
