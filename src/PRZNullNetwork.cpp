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
// File: ../ATCSimul/src/PRZNullNetwork.cpp
//
// Clase      : PRZNullNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZNullNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZNullNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZNullNetwork.hpp>

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZBuilder_HPP__
#include <PRZBuilder.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZNullNetwork,PRZTorusNetwork);

//*************************************************************************
//:
//  f: PRZNullNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:
//:
//*************************************************************************

PRZNullNetwork :: PRZNullNetwork( const PRZComponentId& id,
                                    const PRZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : PRZTorusNetwork(id,routerId,x,y,z)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZNullNetwork();
//
//  d:
//:
//*************************************************************************

PRZNullNetwork :: ~PRZNullNetwork()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZNullNetwork :: initializeConnectionsFor(const PRZPosition& pos)
{
}


//*************************************************************************
//:
//  f: static PRZNullNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZNullNetwork* PRZNullNetwork :: newFrom( const PRZTag* tag,
                                             PRZComponent* owner )
{
   PRZComponentId idNetwork(*tag);
   PRZString sizeX, sizeY, sizeZ;
   unsigned x, y=1, z=1;
   
   if( !(tag->getAttributeValueWithName(PRZ_TAG_SIZEX, sizeX)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_003, (char*)PRZ_TAG_SIZEX );
      EXIT_PROGRAM(err);
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZEY, sizeY) )
   {
      y = sizeY.asInteger();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZEZ, sizeZ) )
   {
      z = sizeZ.asInteger();
   }
   
   x = sizeX.asInteger();
   
   if( !x || !y || !z )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_004, x, y, z );
      EXIT_PROGRAM(err);
   }
   
   PRZString routerId;
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_REFROUTER, routerId)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_003, (char*)PRZ_TAG_REFROUTER );
      EXIT_PROGRAM(err);
   }

   PRZString connectionDelay;
   unsigned delay=0;

   if( tag->getAttributeValueWithName(PRZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   
   PRZNullNetwork* net = new PRZNullNetwork( idNetwork, routerId, x, y, z );
   net->setConnectionDelay(delay);
   
   
   net -> setSimulation(owner);
   net -> initialize();
   
   return net;
}


//*************************************************************************

// fin del fichero
