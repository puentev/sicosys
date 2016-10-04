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
// File: ../ATCSimul/src/PRZBuilder.cpp
//
// Clase      : PRZBuilder
//
// Hereda de  : PRZBase
//
// Notes:   
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZBuilder.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#include <PRZBuilder.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZBuilder,PRZBase);

//*************************************************************************
//:
//  f: PRZString getName() const;
//  d:
//
//  f: virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
//                                                     PRZComponent* owner,
//                                                     INDEX& index ) = 0;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZBuilder( const PRZString& sgmlFile);
//
//  d:
//:
//*************************************************************************

PRZBuilder :: PRZBuilder(const PRZString& sgmlFile)
            : m_FileName(sgmlFile),
              m_TagArray(0)
{
   m_SgmlFile = new ifstream(sgmlFile);
   if( ! m_SgmlFile->good() )
   {
      PRZString err;
      err.sprintf(ERR_PRZBLDR_001, (char*)sgmlFile);
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: ~PRZBuilder();
//
//  d:
//:
//*************************************************************************

PRZBuilder :: ~PRZBuilder()
{
   m_SgmlFile -> close();
   delete m_SgmlFile;
   removeTagArray();
}


//*************************************************************************
//:
//  f: Boolean loadSgmlInformation();
//
//  d:
//:
//*************************************************************************

Boolean PRZBuilder :: loadSgmlInformation()
{
   PRZString linea;
   unsigned numeroLinea = 0;
   PRZSequence<PRZTag*> tagList;

   while(1)
   {
   	linea = PRZString::lineFrom(*m_SgmlFile);
      linea = PRZString::stripBlanks(linea);

      numeroLinea++;
      if( m_SgmlFile->eof() ) break;
      if( linea != "" )
      {
         PRZTag* tag = PRZTag::createTag(linea);
         if(!tag)
         {
            PRZString text = PRZString("Error en la linea ") + 
                             PRZString(numeroLinea);
            text += PRZString(". Fichero ") + getName();
            text += PRZString("Tag incorrectamente definido :\n") + linea;
            THROW_BERROR(text);
         }

         if( ! tag->isComment() )   // Los comentarios no se guardan
         {
            tagList.addAsLast(tag);
         }
      }
   } // while

   removeTagArray();

   m_TagArray = new PRZTagArray(tagList.numberOfElements());
   m_TagArray->initialize(0);
   PRZSequence<PRZTag*>::Cursor cursor(tagList);

   INDEX j = 1;
   forCursor(cursor)
   {
      m_TagArray->setValueAt(j++,cursor.element());
   }

   return true;
}


//*************************************************************************
//:
//  f: PRZTag* getTagWithIndex(INDEX index);
//
//  d:
//:
//*************************************************************************

PRZTag* PRZBuilder :: getTagWithIndex(INDEX index)
{
   if( index && m_TagArray  )
   {
      PRZTag* tempTag;
      if( m_TagArray->valueAt(index,tempTag) )
      {
         return tempTag;
      }
   }
   return 0;
}


//*************************************************************************
//:
//  f: INDEX getTagIndex(const PRZString& tagId);
//
//  d:
//:
//*************************************************************************

INDEX PRZBuilder :: getTagIndex(const PRZString& tagId)
{
   if( m_TagArray )
   {
      PRZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         m_TagArray->valueAt(i,tag);
         if( tag->tagId() == tagId )
            return i;
      }
   }
   // En caso de que el tag con ese nombre no se encuentre, se
   // retorna un 0. Recordar que los indices comienzan en 1.
   return 0;
}


//*************************************************************************
//:
//  f: INDEX getTagIndexWithName(const PRZString& tagName);
//
//  d:
//:
//*************************************************************************

INDEX PRZBuilder :: getTagIndexWithName(const PRZString& tagName)
{
   if( m_TagArray )
   {
      PRZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         m_TagArray->valueAt(i,tag);
         if( tag->tagName() == tagName )
            return i;
      }
   }
   // En caso de que el tag con ese nombre no se encuentre, se
   // retorna un 0. Recordar que los indices comienzan en 1.
   return 0;
}

//*************************************************************************
//:
//  f: PRZComponent* createComponentWithId(const PRZString& id, 
//                                         PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZBuilder :: createComponentWithId( const PRZString& id,
                                                   PRZComponent* owner )
{
   INDEX indice = getTagIndex(id);
   PRZTag* tag = getTagWithIndex(indice);
   if( ! tag )
   {
      PRZString err;
      err.sprintf(ERR_PRZBLDR_002, (char*)id );
      EXIT_PROGRAM(err);
   }
   PRZComponent* rComponent = parseComponentDefinition(tag,owner,indice);
   if( ! rComponent )
   {
      PRZString err;
      err.sprintf(ERR_PRZBLDR_002, (char*)id );
      EXIT_PROGRAM(err);
   }
   return rComponent;
}


//*************************************************************************
//:
//  f: void removeTagArray();
//
//  d:
//:
//*************************************************************************

void PRZBuilder :: removeTagArray()
{
   if( m_TagArray )
   {
      PRZTag* tag;
      for(INDEX i=1; i<=m_TagArray->numberOfElements(); i++ )
      {
         if( m_TagArray->valueAt(i,tag) )
            delete tag;
      }
      delete m_TagArray;
      m_TagArray = 0;
   }
}

//*************************************************************************


// fin del fichero
