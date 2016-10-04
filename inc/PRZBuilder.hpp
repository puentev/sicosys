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
// File: ../ATCSimul/inc/PRZBuilder.hpp
//
// Description:  fichero de encabezado de la clase "PRZBuilder"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZBuilder.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZBuilder_HPP__
#define __PRZBuilder_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif
   
   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif
   
   #include <fstream.h>

//*************************************************************************

   class PRZTag;
   class PRZComponent;

//*************************************************************************

   class PRZBuilder : public PRZBase
   {
      typedef PRZArray<PRZTag*> PRZTagArray;
      
   public:
      PRZBuilder(const PRZString& sgmlFile);
      virtual ~PRZBuilder();

      Boolean loadSgmlInformation();
      PRZTag* getTagWithIndex(INDEX index);
      INDEX   getTagIndex(const PRZString& tagId);
      INDEX   getTagIndexWithName(const PRZString& tagName);
      
      PRZComponent* createComponentWithId( const PRZString& name,
                                           PRZComponent* owner=0);
                                           
      virtual PRZComponent* parseComponentDefinition( const PRZTag* tag, 
                                                      PRZComponent* owner,
                                                      INDEX& index ) = 0;
      // Define RTTI
      DEFINE_RTTI(PRZBuilder);
      
      PRZString getName() const
      { return m_FileName; }

   private:
      void removeTagArray();
      
      ifstream*    m_SgmlFile;
      PRZTagArray* m_TagArray;
      PRZString    m_FileName;
   };

//****************************************************


#endif

// fin del fichero
