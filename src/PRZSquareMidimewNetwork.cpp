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
// File: ../ATCSimul/src/PRZSquareMidimewNetwork.cpp
//
// Clase      : PRZSquareMidimewNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//
// $RCSfile: PRZSquareMidimewNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZSquareMidimewNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZSquareMidimewNetwork.hpp>

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZBuilder_HPP__
#include <PRZBuilder.hpp>
#endif

#include <math.h>

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZSquareMidimewNetwork,PRZNetwork);

//*************************************************************************
//:
//  f: PRZSquareMidimewNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:
//:
//*************************************************************************

PRZSquareMidimewNetwork :: PRZSquareMidimewNetwork( const PRZComponentId& id,
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
//  f: virtual ~PRZSquareMidimewNetwork();
//
//  d:
//:
//*************************************************************************

PRZSquareMidimewNetwork :: ~PRZSquareMidimewNetwork()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZSquareMidimewNetwork :: initialize()
{
   if( isInitializated() ) return;
   
   /* Check consistency of parameters */
   if(getSizeY()!=1 ||getSizeZ()!=1)
   {
     PRZString err;
     err.sprintf( ERR_PRZTONET_002, (char*)getRouterId() );
     err.sprintf("\nSquare Midimew Network Ysize and Zsize must be 1. Al nodes in Xsize!");
     EXIT_PROGRAM(err); 
   }

   if((int)ceil(sqrt(getSizeX()))%2 || getSizeX() < 4 )
   {
     PRZString err;
     err.sprintf( ERR_PRZTONET_002, (char*)getRouterId() );
     err.sprintf("\n Square Midimew Network Xsize must be an even 2's power(4,16,64,256,1024,4096...)");
     EXIT_PROGRAM(err); 
   }

   if( !getSizeX() || !getSizeY() || !getSizeZ() )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_001, getSizeX(), getSizeY(), getSizeZ() );
      EXIT_PROGRAM(err);
   }

   /* Create the routers for each node */
   int i;     
   for( i=0; i<getSizeX(); i++ )
   {
      PRZPosition pos(i,0,0);
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

   /* Connect the routers together */
   for( i=0; i<getSizeX(); i++ )
   {
      initializeConnectionsFor(PRZPosition(i,0,0));
   }
  
   /* Other initialization tasks */
   generateRoutingTable();

   setInitializated(true);
   setSimulation(getSimulation());
}


//*************************************************************************
//:
//  f: unsigned distance(const PRZPosition& org, const PRZPosition& dst);
//
//  d:
//:
//*************************************************************************

unsigned PRZSquareMidimewNetwork :: distance(const PRZPosition& org, const PRZPosition& dst)
{
   /* Calculate topological distance between two nodes */
   int deltaX, deltaY, deltaZ;
   routingRecord(org,dst,deltaX,deltaY,deltaZ);
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
//  d: 
//:
//*************************************************************************

void PRZSquareMidimewNetwork :: routingRecord( const PRZPosition& org, 
                                       const PRZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ )
{
   /* Calculate routing record */
   int origen=org.valueForCoordinate(PRZPosition::X);
   int destino=dst.valueForCoordinate(PRZPosition::X);
   int m = (destino-origen+getSizeX()) % getSizeX();
   
   /* Use lookup table */
   deltaX = m_routingTable[2*(m-1)];
   deltaY = m_routingTable[2*(m-1)+1];
   deltaZ = 0;
   
   /* Routing record values must be incremented by 1 or -1 
    * depending on their sign */
   deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
   deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
   deltaZ = (deltaZ>=0) ? deltaZ+1 : deltaZ-1;
}
//*************************************************************************
//:
//  f: PRZPosition CreatePosition(int num) ;
//
//  d:
//:
//*************************************************************************
PRZPosition PRZSquareMidimewNetwork::CreatePosition(int num)     
{  
   return PRZPosition(num,0,0);
}

//*************************************************************************
//:
//  f: static PRZSquareMidimewNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZSquareMidimewNetwork* PRZSquareMidimewNetwork :: newFrom( const PRZTag* tag,
                                             PRZComponent* owner )
{
   PRZComponentId idNetwork(*tag);
   PRZString sizeX, sizeY, sizeZ;
   unsigned x, y=1, z=1;
   /* Read network size from tag attributes */
   if( !(tag->getAttributeValueWithName(PRZ_TAG_SIZEX, sizeX)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_003, (char*)PRZ_TAG_SIZEX );
      EXIT_PROGRAM(err);
   }
   x = sizeX.asInteger();
   
/*   if( tag->getAttributeValueWithName(PRZ_TAG_SIZEY, sizeY) )
   {
      y = sizeY.asInteger();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_SIZEZ, sizeZ) )
   {
      z = sizeZ.asInteger();
   } */
   
   /* Check size consistency */ 
   if( !x /*|| !y || !z*/ )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_004, x, 0, 0 );
      EXIT_PROGRAM(err);
   }
   /* Read the router identifier */
   PRZString routerId;
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_REFROUTER, routerId)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_003, (char*)PRZ_TAG_REFROUTER );
      EXIT_PROGRAM(err);
   }
   /* Read the connection delay */
   PRZString connectionDelay;
   unsigned delay=0;

   if( tag->getAttributeValueWithName(PRZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   /* Instantiation of the network class */ 
   PRZSquareMidimewNetwork* net = new PRZSquareMidimewNetwork( idNetwork, routerId, x, y, z );
   net->setConnectionDelay(delay);
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
unsigned PRZSquareMidimewNetwork::getDiameter() const
{
   /* Calculate the diameter of the network */
   return 3*sqrt(getSizeX())/4;
}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZSquareMidimewNetwork :: asString() const
{
   PRZGlobalData& data = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)this);
   PRZString rs= "SquareMidimew(";
   rs +=  PRZString(getSizeX()) + "," +
          PRZString(getSizeY()) + "," +
          PRZString(getSizeZ()) + ")" +
          "\n buffer control = " + data.routerBufferControl() +
          "\n routing control= " + data.routerRoutingControl();
   return rs;
}

//*************************************************************************
//:
//  f: void generateRoutingTable()
//
//  d:
//:
//*************************************************************************

void PRZSquareMidimewNetwork :: generateRoutingTable()
{
   cerr << "Generating Routing Table...\n";
   int n = getSizeX();
   m_routingTable = new int[2*(n-1)];
   int sy = sqrt(n);
   int sx = sy/2 - 1;
   for(int m=1; m<n; m++)
   {
      int min=n;
      for(int x=-3*sy/4; x < 3*sy/4; x++ )
      {
         for(int y=-3*sy/4; y < 3*sy/4; y++)
	      {
	         if(m == ((x*sx + y*sy + n) % n) && abs(x)+abs(y) <= min)
		      {
               min = abs(x)+abs(y);
               m_routingTable[2*(m-1)] = x; m_routingTable[2*(m-1)+1] = y;
		      }
	      }
      }
   }
}


//*************************************************************************
//:
//  f: void initializeConnectionsFor(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

void PRZSquareMidimewNetwork :: initializeConnectionsFor(const PRZPosition& pos)
{
   /* Make connections to router at position 'pos' */
   PRZRouter* router = getRouterAt(pos);
   int myNum=pos.valueForCoordinate(PRZPosition::X);
   int sy = sqrt(getSizeX());
   int sx = sy/2 - 1;
   int n = getSizeX();


   /* Get neighbouring routers */
   PRZRouter* routerXp = getRouterAt(CreatePosition( (myNum+sx)%n ));
   PRZRouter* routerXm = getRouterAt(CreatePosition( (myNum-sx+n)%n ));
   PRZRouter* routerYp = getRouterAt(CreatePosition( (myNum+sy)%n ));
   PRZRouter* routerYm = getRouterAt(CreatePosition( (myNum-sy+n)%n ));

   /* Get input port numbers */ 
   unsigned iXp = router->getInputWithType(_Xplus_);
   unsigned iXm = router->getInputWithType(_Xminus_);
   unsigned iYp = router->getInputWithType(_Yplus_);
   unsigned iYm = router->getInputWithType(_Yminus_);
   
   /* Get output port numbers */ 
   unsigned oXp = router->getOutputWithType(_Xplus_);
   unsigned oXm = router->getOutputWithType(_Xminus_);
   unsigned oYp = router->getOutputWithType(_Yplus_);
   unsigned oYm = router->getOutputWithType(_Yminus_);
   
   /* Make the connections */
   if( iXp && oXp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXp),
                                        routerXp->getInputInterfaz(iXp) );

   if( iXm && oXm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXm),
                                        routerXm->getInputInterfaz(iXm),
                                        getConnectionDelay() );

   if( iYp && oYp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYp),
                                        routerYp->getInputInterfaz(iYp),
                                        getConnectionDelay() );

   if( iYm && oYm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYm),
                                        routerYm->getInputInterfaz(iYm),
                                        getConnectionDelay() );
}

//*************************************************************************

// fin del fichero
