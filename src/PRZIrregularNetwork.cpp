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
// File: ../ATCSimul/src/PRZIrregularNetwork.cpp
//
// Clase      : PRZIrregularNetwork
//
// Hereda de  : PRZNetwork
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZIrregularNetwork.cpp,v $
//
// $Date: 2001/10/30 11:11:06 $
//
// $Revision: 1.2 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZIrregularNetwork.cpp,v 1.2 2001/10/30 11:11:06 vpuente Exp $";


#include <PRZIrregularNetwork.hpp>

#ifndef __PRZMeshNetwork_HPP__
#include <PRZMeshNetwork.hpp>
#endif

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZBuilder_HPP__
#include <PRZBuilder.hpp>
#endif

#include <math.h>
#include <stdio.h>

#ifndef __PRZGlobalData_HPP__
#include <PRZGlobalData.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZIrregularNetwork,PRZNetwork);

//*************************************************************************
//:
//  f: PRZIrregularNetwork( const PRZComponentId& id, 
//                      const PRZString& routerId,
//                      unsigned x, 
//                      unsigned y, 
//                      unsigned z );
//
//  d:
//:
//*************************************************************************

PRZIrregularNetwork :: PRZIrregularNetwork( const PRZComponentId& id,
                                    const PRZString& routerId,
                                    unsigned x, 
                                    unsigned y, 
                                    unsigned z )
                 : PRZNetwork(id,routerId,x,y,z)
{
   unsigned diametro=getDiameter();   
   //Todos los nodos
   initializeHistogram(diametro);
}


//*************************************************************************
//:
//  f: virtual ~PRZIrregularNetwork();
//
//  d:
//:
//*************************************************************************

PRZIrregularNetwork :: ~PRZIrregularNetwork()
{

}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZIrregularNetwork :: initialize(const PRZTag* tag)
{
   if( isInitializated() ) return;


   if(getSizeY()!=1 ||getSizeZ()!=1)
   {
     PRZString err;
     err.sprintf( ERR_PRZTONET_002, (char*)getRouterId() );
     err.sprintf("\n Irregular Networks Ysize and Zsize must be =1. Al nodes in Xsize!");
     EXIT_PROGRAM(err); 
   }


   if( !getSizeX() || !getSizeY() || !getSizeZ() )
   {
      PRZString err;
      err.sprintf(ERR_PRZTONET_001, getSizeX(), getSizeY(), getSizeZ() );
      EXIT_PROGRAM(err);
   }
   int i, j, k;     
   j = 0;
   k = 0;
   for( i=0; i<getSizeX(); i++ )
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
// *********************************
// abrir aqui el fichero de topologia
// cambiar el for por un mientras queden lineas por leer
//       leer una linea 
//       obtener el nodo a inicializar las conexiones
//       obtener los vecinos y sus puertos
//       inicializar el router (habra que cambiar initializeConnetionsFor )

	PRZString FileTop;
	ifstream*  m_File;
	PRZString line,line_dst;
	Boolean salida=false;
        PRZRouter* router_org;
        PRZRouter* router_dst;
	unsigned org,dst,puerto_dst;
	unsigned puerto_org=1;
	
	      
      if( tag->getAttributeValueWithName(PRZ_TAG_FILETOP, FileTop) )
      {
          m_fileName=FileTop;
          m_File = new ifstream(FileTop);
          if( !m_File->good() )
          {
             PRZString err;
             err.sprintf(ERR_PRZTRAFI_005, (char*)FileTop ); 
             EXIT_PROGRAM(err);
           }
// el fichero se ha abierto bien y hay que empezar a leer las lineas
	while (!salida)
	{
         line = PRZString::lineFrom(*m_File);
         salida = m_File->eof();
         line = PRZString::stripBlanks(line);
	 puerto_org = 1;
// Cada linea tiene el numero del encamiandor que se esta inicilizando
	org = line.firstWord().asInteger();
	line.removeFirstWord();
	router_org = getRouterAt(org,0,0);
	while (line.size() != 0) {
// seguido de los destinos con su puerto separados por un slash
           line_dst = line.firstWord();
   	   line.removeFirstWord();
	   dst = PRZString::getStringLeftTo(line_dst,'/').asInteger();
           if(dst>getSizeX())
           {
              PRZString err="SizeX en sgml no es coherente con el fichero de topologia!";
              EXIT_PROGRAM(err);
           }
	   router_dst = getRouterAt(dst,0,0);
	   puerto_dst = PRZString::getStringRightTo(line_dst,'/').asInteger();

           PRZConnection::connectInterfaces( this,
                                             router_org->getOutputInterfaz(puerto_org),
                                             router_dst->getInputInterfaz(puerto_dst) );
	   puerto_org++;
		                
           }
	}
        
          // PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
          // gData.initializeTopol(FileTop);

      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_P10_FLASH ); //esto debe ser el mensaje de error
         EXIT_PROGRAM(err);
      }
      // Cierro el fichero 

      m_File->close();
      delete m_File;
 
      PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
      gData.setTopFileName(FileTop); 
      
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

// No se donde se usa esta funcion y si es necesaria 
// en caso de que no sea necesaria habra que borrarla

unsigned PRZIrregularNetwork :: distance(const PRZPosition& org, const PRZPosition& dst)
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
//     origen y destino. Hecho para un TORO NxN con todos los nods 
//     en la dimension X. 
//     Se supone que los nodos del toro estan bien numerados y ordenados.
//:
//*************************************************************************

// El routing record para las redes irregulares no es necesario



void PRZIrregularNetwork :: routingRecord( const PRZPosition& org, 
                                       const PRZPosition& dst,
                                       int&  deltaX,
                                       int&  deltaY,
                                       int&  deltaZ )
{
   int m,signo;
   //
   int x0;
   int y0;
   int x1;
   int y1;
   //
   int R=int(sqrt(getSizeX()));
   int origen=org.valueForCoordinate(PRZPosition::X);
   int destino=dst.valueForCoordinate(PRZPosition::X);

   
   deltaX = (destino%R)-(origen%R);
   deltaY = int(destino/R)-int(origen/R);
   deltaZ = 0;
   
 
   int absDeltaX = (deltaX>0) ? deltaX : -deltaX;
   int absDeltaY = (deltaY>0) ? deltaY : -deltaY;
   int absDeltaZ = 0;
   
   int R2 = int(R/2);
   
   if( (absDeltaX > R2) ||
       ((absDeltaX==R2) && (R%2==0) && (drand48()<0.50)) )
   {
      deltaX = (deltaX>0) ? (deltaX-R) : (deltaX+R);
   }

   if( (absDeltaY > R2) ||
       ((absDeltaY==R2) && (R%2==0) && (drand48()<0.50)) )
   {
      deltaY = (deltaY>0) ? (deltaY-R) : (deltaY+R);
   }
    /**/

         
   
   if( abs(deltaX)+abs(deltaY)>R)
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
//  f: static PRZIrregularNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZIrregularNetwork* PRZIrregularNetwork :: newFrom( const PRZTag* tag,
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

   PRZIrregularNetwork* net;
   if( tag->getAttributeValueWithName(PRZ_TAG_DELAY, connectionDelay) )
   {
      delay = connectionDelay.asInteger();
   }
   if(tag->tagName()==PRZ_TAG_IRREGULARNET)
   {
      net = new PRZIrregularNetwork( idNetwork, routerId, x, y, z );
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
   net -> initialize(tag);
   
   return net;
}

//*************************************************************************
//:
//  f: unsigned long getDiameter() const;
//
//  d: 
//:
//*************************************************************************
unsigned PRZIrregularNetwork::getDiameter() const
{
   return getSizeX();
}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

// no se para que sirve (una mas, hoy es mi dia de desesperacion...)

PRZString PRZIrregularNetwork :: asString() const

{

   PRZGlobalData& data = ((PRZSimulation*)getSimulation())->globalData();
   PRZString rs= "Irregular(";
   rs +=  PRZString(getSizeX()) + "," +
          PRZString(getSizeY()) + "," +
          PRZString(getSizeZ()) + ")" +
         "\n Topol File="+ m_fileName +
          "\n buffer control = " + data.routerBufferControl() +
          "\n routing control= " + data.routerRoutingControl();
   if(data.getMiss())
   {
      rs +=" Non minimal routes allowed :Mhops=";
      rs +=PRZString(data.getMiss());
   }
   
   rs+=PRZString("\n tables  = ")+data.getStringId();
   return rs;
}


//*************************************************************************

// fin del fichero
