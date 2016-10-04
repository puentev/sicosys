//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//
// File: ../ATCSimul/inc/PRZTag.hpp
//
// Description:  fichero de encabezado de la clase "PRZTag"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZTag.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZTag_HPP__
#define __PRZTag_HPP__

//*************************************************************************

   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif

   #ifndef __PRZAttribute_HPP__
   #include <PRZAttribute.hpp>
   #endif

//*************************************************************************

   class PRZAttribute;

//*************************************************************************

class PRZTag  : public PRZBase
{
      typedef PRZSequence<PRZAttribute*> PRZAttributeList;

   public :
      PRZTag();
      PRZTag(const PRZString& name, const PRZString& id);
      PRZTag(const PRZTag& aTag);
      virtual ~PRZTag();

      unsigned numberOfAttributes() const;
      Boolean getAttributeValueWithName(const PRZString& name, PRZString& value) const;
      Boolean addAttribute(PRZAttribute* attr);
      PRZTag& operator=(const PRZTag& aTag);

      friend Boolean  operator>>(istream &aStream, PRZTag& aTag);
      friend ostream& operator<<(ostream &aStream, const PRZTag& aTag);

      PRZString tagName() const
      { return m_TagName; }

      PRZString tagId() const
      { return m_TagId; }

      Boolean isComment() const
      { return m_Comment; }

      static PRZTag* createTag(const PRZString& tagString);

      // Run time information
      DEFINE_RTTI(PRZTag);

   private :
      static PRZString      extractTagName(PRZString& tagString);
      static PRZString      extractTagId(PRZString& tagString);
      static PRZAttribute*  extractAttribute(PRZString& tagString);
      static Boolean        removeDecoration(PRZString& tagString);
      static Boolean        isStringValid(const PRZString& aTag);

      PRZString        m_TagName;
      PRZString        m_TagId;
      PRZAttributeList m_AttributeList;
      Boolean          m_Comment;
   };



#endif

//*************************************************************************

// fin del fichero
