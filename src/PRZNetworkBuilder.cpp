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
// File: ../ATCSimul/src/PRZNetworkBuilder.cpp
//
// Clase      : PRZNetworkBuilder
//
// Hereda de  : PRZBuilder
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZNetworkBuilder.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZNetworkBuilder.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZNetworkBuilder.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZTorusNetwork_HPP__
#include <PRZTorusNetwork.hpp>
#endif

#ifndef __PRZMeshNetwork_HPP__
#include <PRZMeshNetwork.hpp>
#endif

#ifndef __PRZMidimewNetwork_HPP__
#include <PRZMidimewNetwork.hpp>
#endif

#ifndef __PRZSquareMidimewNetwork_HPP__
#include <PRZSquareMidimewNetwork.hpp>
#endif

#ifndef __PRZIrregularNetwork_HPP__
#include <PRZIrregularNetwork.hpp>
#endif

#ifndef __PRZNullNetwork_HPP__
#include <PRZNullNetwork.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZNetworkBuilder,PRZBuilder);

//*************************************************************************
//:
//  f: PRZNetworkBuilder(const PRZString& sgmlFile);
//
//  d:
//:
//*************************************************************************

PRZNetworkBuilder :: PRZNetworkBuilder(const PRZString& sgmlFile)
                   : PRZBuilder(sgmlFile)
{

}


//*************************************************************************
//:
//  f: PRZComponent* parseComponentDefinition( const PRZTag* tag,
//                                             PRZComponent* owner,
//                                             INDEX& index );
//
//  d:
//:
//*************************************************************************

PRZComponent* PRZNetworkBuilder::parseComponentDefinition( const PRZTag* tag,
                                                           PRZComponent* owner,
                                                           INDEX& index )
{
   PRZString tagName = tag->tagName();
   PRZComponent* rComponent = 0;

   if( tagName == PRZ_TAG_TORUSNET )
   {
      rComponent = PRZTorusNetwork::newFrom(tag,owner);
   }
   else if( tagName == PRZ_TAG_MESHNET )
   {
      rComponent = PRZMeshNetwork::newFrom(tag,owner);
   }
   else if (tagName == PRZ_TAG_NULLNET )
   {
      rComponent = PRZNullNetwork::newFrom(tag,owner);
   }
   else if (tagName == PRZ_TAG_MIDIMEWNET )
   {
      rComponent = PRZMidimewNetwork::newFrom(tag,owner);
   }
   else if (tagName == PRZ_TAG_SQUAREMIDIMEWNET )
   {
      rComponent = PRZSquareMidimewNetwork::newFrom(tag,owner);
   }
   else if (tagName == PRZ_TAG_IRREGULARNET )
   {
      rComponent = PRZIrregularNetwork::newFrom(tag,owner);
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZNBLDR_001, (char*)tagName );
      EXIT_PROGRAM(err);
   }

   return rComponent;
}


//*************************************************************************


// fin del fichero
