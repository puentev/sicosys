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
// File: ../ATCSimul/src/PRZMidimewNetwork.cpp
//
// Clase      : PRZMidimewNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMidimewNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMidimewNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZMidimewNetwork.hpp>

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

IMPLEMENT_RTTI_DERIVED(PRZMidimewNetwork,PRZNetwork);

//*************************************************************************
//:
//  f: PRZMidimewNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:Red Midimew. En relalidad funcionara como una red en una sola dimension
//    en lo que respecta a las etiquetas de los nodos.
//:
//*************************************************************************

PRZMidimewNetwork :: PRZMidimewNetwork( const PRZComponentId& id,
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
//  f: virtual ~PRZMidimewNetwork();
//
//  d:
//:
//*************************************************************************

PRZMidimewNetwork :: ~PRZMidimewNetwork()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZMidimewNetwork :: initialize()
{
   if( isInitializated() ) return;
   
   if(getSizeY()!=1 ||getSizeZ()!=1)
   {
     PRZString err;
     err.sprintf( ERR_PRZTONET_002, (char*)getRouterId() );
     err.sprintf("\n Midimew Network Ysize and Zsize must be =1. Al nodes in Xsize!");
     EXIT_PROGRAM(err); 
   }

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
}


//*************************************************************************
//:
//  f: unsigned distance(const PRZPosition& org, const PRZPosition& dst);
//
//  d:
//:
//*************************************************************************

unsigned PRZMidimewNetwork :: distance(const PRZPosition& org, const PRZPosition& dst)
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
//     Emplea la solucion formal del algoritmo : Mon Paper's. Todos los nodos
//     estan en la dimension y
//:
//*************************************************************************

void PRZMidimewNetwork :: routingRecord( const PRZPosition& org, 
                                       const PRZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ )
{
   
   int m,signo;
   //Soluciones optipas del Routing Record
   int x0;
   int y0;
   int x1;
   int y1;
   //
   int b=ceill(sqrt(getSizeX()/2));
   int origen=org.valueForCoordinate(PRZPosition::X);
   int destino=dst.valueForCoordinate(PRZPosition::X);
   //Necesito discernir cuando el mensaje es genrado de acuerdo con el toro
   
    m = abs(destino-origen);
   
    if(destino<=origen)
       signo=-1;
    else
       signo=1;
    
    if (m>getSizeX()/2){
       signo=-signo;
       m=getSizeX()-m;
    }
    //Soluciones Validdas para la ecucion diofantica
    //de los grafos circulantes
    y0=-m%b;
    x0=-y0+m/b;
    y1=b+y0;
    x1=x0-(b-1);   
    //
    if(abs(x0)+abs(y0)<abs(x1)+abs(y1))
    {
       deltaX=signo*x0;
       deltaY=signo*y0;
    }
    else
    {
       deltaX=signo*x1;
       deltaY=signo*y1;
    }
   // a la desesperada con la puta FFT
   if(abs(x1)+abs(y1)<=getDiameter() && abs(x0)+abs(y0)<=getDiameter())
   //if(abs(x1)+abs(y1)==abs(x0)+abs(y0))
    {
       if(drand48()<0.5)
       {
         deltaX=signo*x1;
         deltaY=signo*y1;  
       }
       else
       {
         deltaX=signo*x0;
         deltaY=signo*y0;
       }
    }
    
    if(abs(deltaX)+abs(deltaY)>getDiameter())
    {
      PRZString err;
      err.sprintf("Superado diametro de la red\n");
      EXIT_PROGRAM(err);
    }
    
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
PRZPosition PRZMidimewNetwork::CreatePosition(int num)     
   {  
         unsigned numi=unsigned(num);        
         unsigned x = numi%unsigned(getSizeX());
         return PRZPosition(x,0);

 }

//*************************************************************************
//:
//  f: static PRZMidimewNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZMidimewNetwork* PRZMidimewNetwork :: newFrom( const PRZTag* tag,
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
   
   PRZMidimewNetwork* net = new PRZMidimewNetwork( idNetwork, routerId, x, y, z );
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
unsigned PRZMidimewNetwork::getDiameter() const
{
   unsigned b=ceil(sqrt(getSizeX()/2));
   if (getSizeX()<=2*b*b-2*b+1)
         return b-1;
   else
         return b;

}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZMidimewNetwork :: asString() const
{
   PRZGlobalData& data = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)this);
   PRZString rs= "Midimew(";
   rs +=  PRZString(getSizeX()) + "," +
          PRZString(getSizeY()) + "," +
          PRZString(getSizeZ()) + ")" +
          "\n buffer control = " + data.routerBufferControl() +
          "\n routing control= " + data.routerRoutingControl();
   return rs;
}

//*************************************************************************
//:
//  f: void initializeConnectionsFor(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

void PRZMidimewNetwork :: initializeConnectionsFor(const PRZPosition& pos)
{

   PRZRouter* router = getRouterAt(pos);
   int myNum=pos.valueForCoordinate(PRZPosition::X);
   int b=ceill(sqrt(getSizeX()/2));
   
   PRZRouter* routerXp = getRouterAt(CreatePosition((myNum+b)%getSizeX()));
   PRZRouter* routerXm = getRouterAt(CreatePosition((myNum-b+getSizeX())%getSizeX()));
   PRZRouter* routerYp = getRouterAt(CreatePosition((myNum+(b-1))%getSizeX()));
   PRZRouter* routerYm = getRouterAt(CreatePosition((myNum-(b-1)+getSizeX())%getSizeX()));
   PRZRouter* routerZp = getRouterAt( positionOf(_Zplus_,pos)  );  //No se que hacer con esto.
   PRZRouter* routerZm = getRouterAt( positionOf(_Zminus_,pos) );


   unsigned iXp = router->getInputWithType(_Xplus_);
   unsigned iXm = router->getInputWithType(_Xminus_);
   unsigned iYp = router->getInputWithType(_Yplus_);
   unsigned iYm = router->getInputWithType(_Yminus_);
   unsigned iZp = router->getInputWithType(_Zplus_);
   unsigned iZm = router->getInputWithType(_Zminus_);
   
   unsigned oXp = router->getInputWithType(_Xplus_);
   unsigned oXm = router->getInputWithType(_Xminus_);
   unsigned oYp = router->getInputWithType(_Yplus_);
   unsigned oYm = router->getInputWithType(_Yminus_);
   unsigned oZp = router->getInputWithType(_Zplus_);
   unsigned oZm = router->getInputWithType(_Zminus_);

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

   if( iZp && oZp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZp),
                                        routerZp->getInputInterfaz(iZp),
                                        getConnectionDelay() );

   if( iZm && oZm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZm),
                                        routerZm->getInputInterfaz(iZm),
                                        getConnectionDelay() );

}

//*************************************************************************

// fin del fichero
