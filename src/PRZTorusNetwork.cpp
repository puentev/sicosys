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
// File: ../ATCSimul/src/PRZTorusNetwork.cpp
//
// Clase      : PRZTorusNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZTorusNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZTorusNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZTorusNetwork.hpp>

#ifndef __PRZMeshNetwork_HPP__
#include <PRZMeshNetwork.hpp>
#endif

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

IMPLEMENT_RTTI_DERIVED(PRZTorusNetwork,PRZNetwork);

//*************************************************************************
//:
//  f: PRZTorusNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:
//:
//*************************************************************************

PRZTorusNetwork :: PRZTorusNetwork( const PRZComponentId& id,
                                    const PRZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : PRZNetwork(id,routerId,x,y,z)
{
   unsigned diametro=getDiameter();   
   initializeHistogram(diametro);
}


//*************************************************************************
//:
//  f: virtual ~PRZTorusNetwork();
//
//  d:
//:
//*************************************************************************

PRZTorusNetwork :: ~PRZTorusNetwork()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZTorusNetwork :: initialize()
{
   if( isInitializated() ) return;

   if( !getSizeX() || !getSizeY() || !getSizeZ() )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_001, getSizeX(), getSizeY(), getSizeZ() );
      EXIT_PROGRAM(err);
   }
   int i, j, k;     
   for( i=0; i<getSizeX(); i++ )
   {
      for( j=0; j<getSizeY(); j++ )
      {
         for( k=0; k<getSizeZ(); k++ )
         {
            PRZPosition pos(i,j,k);
            PRZRouter* newRouter = (PRZRouter*)
               (PRZRouter::routerBuilder->createComponentWithId(getRouterId(),this));
            if( ! newRouter )
            {
               PRZString err;
               err.sprintf( ERR_PRZTONET_002, (char*)getRouterId() );
               EXIT_PROGRAM(err);
            }
            newRouter->setPosition(pos);                       
            addRouter(newRouter);
            
         }
      }
   }
   for( i=0; i<getSizeX(); i++ )
      for( j=0; j<getSizeY(); j++ )
         for( k=0; k<getSizeZ(); k++ )
         {
            initializeConnectionsFor(PRZPosition(i,j,k));
         }

   setInitializated(true);
   setSimulation(getSimulation());
   //Inicializo el histograma

}


//*************************************************************************
//:
//  f: unsigned distance(const PRZPosition& org, const PRZPosition& dst);
//
//  d:
//:
//*************************************************************************

unsigned PRZTorusNetwork :: distance(const PRZPosition& org, const PRZPosition& dst)
{
   int deltaX, deltaY, deltaZ;
   routingRecord(org,dst,deltaX,deltaY,deltaZ);
/*   
   cout << org.asString() << "->" << dst.asString() << ": " << deltaX
        << " " << deltaY << " " << deltaZ << " : "
        << int( abs(deltaX) + abs(deltaY) + abs(deltaZ) - 3 ) << endl;
*/   
   return int( abs(deltaX) + abs(deltaY) + abs(deltaZ) - 3 );
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
//     origen y destino. Comprueba, al ser un toro, que la distancia no
//     sobrepasa el radio / 2  de la red. En caso de tener 2 caminos 
//     equivalentes, se escoge aleatoriamente uno de ellos;
//:
//*************************************************************************

void PRZTorusNetwork :: routingRecord( const PRZPosition& org, 
                                       const PRZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ )
{
   PRZPosition deltaPos = dst - org;
   deltaX = deltaPos.valueForCoordinate(PRZPosition::X);
   deltaY = deltaPos.valueForCoordinate(PRZPosition::Y);
   deltaZ = deltaPos.valueForCoordinate(PRZPosition::Z);
   
   int total=deltaX+deltaY+deltaZ;
   
   int absDeltaX = (deltaX>0) ? deltaX : -deltaX;
   int absDeltaY = (deltaY>0) ? deltaY : -deltaY;
   int absDeltaZ = (deltaZ>0) ? deltaZ : -deltaZ;
   
   int R2x = int(getSizeX()/2);
   int R2y = int(getSizeY()/2);
   int R2z = int(getSizeZ()/2);
   
   /*
   if( (absDeltaX > R2x)||
       ((absDeltaX==R2x) && (getSizeX()%2==0) && (total%2==0))  )
   {
      deltaX = (deltaX>0) ? (deltaX-getSizeX()) : (deltaX+getSizeX());
   }

   if( (absDeltaY > R2y)  ||
       ((absDeltaY==R2y) && (getSizeY()%2==0) && (total%2==0)))
   {
      deltaY = (deltaY>0) ? (deltaY-getSizeY()) : (deltaY+getSizeY());
   }

   if( (absDeltaZ > R2z)  || 
       ((absDeltaZ==R2z) && (getSizeZ()%2==0) && (total%2==0)))
   {
      deltaZ = (deltaZ>0) ? (deltaZ-getSizeZ()) : (deltaZ+getSizeZ());
   }*/
   
   
   if( (absDeltaX > R2x) ||
       ((absDeltaX==R2x) && (getSizeX()%2==0) && (drand48()<0.50)) )
   {
      deltaX = (deltaX>0) ? (deltaX-getSizeX()) : (deltaX+getSizeX());
   }

   if( (absDeltaY > R2y) ||
       ((absDeltaY==R2y) && (getSizeY()%2==0) && (drand48()<0.50)) )
   {
      deltaY = (deltaY>0) ? (deltaY-getSizeY()) : (deltaY+getSizeY());
   }

   if( (absDeltaZ > R2z) || 
       ((absDeltaZ==R2z) && (getSizeZ()%2==0) && (drand48()<0.50)) )
   {
      deltaZ = (deltaZ>0) ? (deltaZ-getSizeZ()) : (deltaZ+getSizeZ());
   }
   /**/
         
      //Toro sin caminos alternativos en redes pares
   /*
    if (absDeltaX > R2x)
      deltaX = (deltaX>0) ? (deltaX-getSizeX()) : (deltaX+getSizeX());
    if (absDeltaY > R2y)
      deltaY = (deltaY>0) ? (deltaY-getSizeY()) : (deltaY+getSizeY());
    if (absDeltaX > R2y)
      deltaZ = (deltaZ>0) ? (deltaZ-getSizeZ()) : (deltaZ+getSizeZ());
	*/
   
   if( abs(deltaX)+abs(deltaY)>getSizeX())
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_003, "Sobrepasa diametro\n");
      EXIT_PROGRAM(err);
   }
   
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = (deltaZ>=0) ? deltaZ+1 : deltaZ-1;
   

}


//*************************************************************************
//:
//  f: static PRZTorusNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZTorusNetwork* PRZTorusNetwork :: newFrom( const PRZTag* tag,
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

   PRZTorusNetwork* net;
   if( tag->getAttributeValueWithName(PRZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   if(tag->tagName()==PRZ_TAG_TORUSNET)
   {
      net = new PRZTorusNetwork( idNetwork, routerId, x, y, z );
   }
   else if(tag->tagName()==PRZ_TAG_MESHNET)
   {
      net = new PRZMeshNetwork( idNetwork, routerId, x, y, z );
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZNBLDR_001, (char*)tag->tagName() );
      EXIT_PROGRAM(err);
   }
      
   net->setConnectionDelay(delay);
   
   net->setOwner(owner);
   net -> setSimulation(owner);
   net -> initialize();
   
   return net;
}

//*************************************************************************
//:
//  f: unsigned long getDiameter() const;
//
//  d:
//:
//*************************************************************************
 unsigned PRZTorusNetwork::getDiameter() const
{
   unsigned maximo;
   unsigned dimension;
   if(getSizeX()>=getSizeY())
      maximo=getSizeX();
   else
      maximo=getSizeY();
   
   if(getSizeZ()>=maximo)
      maximo=getSizeZ();
   
   if(getSizeZ()!=1)
      dimension=3;
   else
      dimension=2;

   return dimension*(int)(maximo/2);
}
//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZTorusNetwork :: asString() const
{
   PRZGlobalData& data = ((PRZSimulation*)getSimulation())->globalData();
   PRZString rs= "Toro(";
   rs +=  PRZString(getSizeX()) + "," +
          PRZString(getSizeY()) + "," +
          PRZString(getSizeZ()) + ")" +
          "\n buffer control = " + data.routerBufferControl() +
          "\n routing control= " + data.routerRoutingControl();
   return rs;
}


//*************************************************************************

// fin del fichero
