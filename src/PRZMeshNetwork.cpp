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
// File: ../ATCSimul/src/PRZMeshNetwork.cpp
//
// Clase      : PRZMeshNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMeshNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMeshNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZMeshNetwork.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZMeshNetwork,PRZTorusNetwork);

//*************************************************************************
//:
//  f: PRZMeshNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:
//:
//*************************************************************************

PRZMeshNetwork :: PRZMeshNetwork( const PRZComponentId& id,
                                    const PRZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : PRZTorusNetwork(id,routerId,x,y,z)
{
   unsigned diametro=getDiameter();   
   initializeHistogram(diametro);
}


//*************************************************************************
//:
//  f: virtual ~PRZMeshNetwork();
//
//  d:
//:
//*************************************************************************

PRZMeshNetwork :: ~PRZMeshNetwork()
{

}
//*************************************************************************
//:
//  f:  void routingRecord( const PRZPosition& org,
//                          const PRZPosition& dst,
//                          int& deltaX,
//                          int& deltaY,
//                          int& deltaZ );
//
//  d: Construye el routing record a partir de lar posiciones de los nodos
//     origen y destino para el caso de la la Malla. En principio est es la unica 
//     diferencia con los toros.
//:
//*************************************************************************

void PRZMeshNetwork :: routingRecord( const PRZPosition& org, 
                                       const PRZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ )
{
   PRZPosition deltaPos = dst - org;
   deltaX = deltaPos.valueForCoordinate(PRZPosition::X);
   deltaY = deltaPos.valueForCoordinate(PRZPosition::Y);
   deltaZ = deltaPos.valueForCoordinate(PRZPosition::Z);
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = (deltaZ>=0) ? deltaZ+1 : deltaZ-1;}
//*************************************************************************
//:
//  f: unsigned long getDiameter() const;
//
//  d:
//:
//*************************************************************************
unsigned PRZMeshNetwork::getDiameter() const
{
   unsigned p=getSizeX();
   unsigned q=getSizeY();
   unsigned r=getSizeZ();
   return p+q+r-3;
}
//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZMeshNetwork :: asString() const
{
   PRZGlobalData& data = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)this);
   PRZString rs= "Mesh(";
   rs +=  PRZString(getSizeX()) + "," +
          PRZString(getSizeY()) + "," +
          PRZString(getSizeZ()) + ")" +
          "\n buffer control = " + data.routerBufferControl() +
          "\n routing control= " + data.routerRoutingControl();
   return rs;
}
//*************************************************************************
// fin del fichero
