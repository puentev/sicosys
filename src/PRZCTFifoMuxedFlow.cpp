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
// File: ../ATCSimul/src/PRZCTFifoMuxedFlow.cpp
//
// Clase      : PRZCTFifoMuxedFlow
//
// Hereda de  : PRZMultiportFifoFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZCTFifoMuxedFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZCTFifoMuxedFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZCTFifoMuxedFlow.hpp>

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZFifoMuxed_HPP__
#include <PRZFifoMuxed.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZCTFifoMuxedFlow, PRZFifoMuxedFlow);


//*************************************************************************
//:
//  f: PRZCTFifoMuxedFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZCTFifoMuxedFlow :: PRZCTFifoMuxedFlow(PRZComponent& component)
                     : PRZFifoMuxedFlow(component)
{
  //initialize();
}

//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZCTFifoMuxedFlow :: initialize()
{  
  Inhereited::initialize(); 
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZCTFifoMuxedFlow :: controlAlgoritm(unsigned  interfaz)
{
   unsigned sizeForCT;
   if(((PRZFifoMuxed&)getComponent()).getGranularity()==0)
   {
       sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                               getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());   
   }
   else
   {
       sizeForCT = ((PRZFifoMuxed&)getComponent()).getGranularity();
   } 
   
   return ( bufferHoles(interfaz) >= 1*sizeForCT ) ? true : false;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
//
//  d:
//:
//*************************************************************************

Boolean PRZCTFifoMuxedFlow :: controlAlgoritm(Boolean info, int delta)
{
   unsigned sizeForCT;
   if(((PRZFifoMuxed&)getComponent()).getGranularity()==0)
   {
       sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                               getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());   
   }
   else
   {
       sizeForCT = ((PRZFifoMuxed&)getComponent()).getGranularity();
   } 

   if (info==false)
   {
      //delta==numero de lineas viruales del buffer
      for(unsigned i=0;i<delta;i++)
      {
         //bueno esta linea de codigo tiene tela. Para que el router remoto le
         //de toempo a pillar la señal de stop, el buffer se tiene que parar un 
         //ciclo antes de lo debido. Si no hay un desbordamiento dado que el 
         //el buffer de cada linea virtual hay capacidad para un puto paquete
         //al avanzar hacia adelante puede dar problemas al emplear paquetes 
         //diferente longitud. Ademas si se mete mas de un paquete en cada linea
         //vitual el lio aun puede ser mayor. Notar que para preavanzar
         //la señal de stop un ciclo hay que aumnetar la capacidad de la linea
         //virtual en un phit. Si no la cosa, obviamente no rularia.
        if(bufferHoles(i) >= sizeForCT+1)
        {
            return true;
            }
       }
         return false;
   }
   else
   {
      return ( bufferHoles(delta) >= sizeForCT ) ? true : false;
   }  
}

//*************************************************************************
//:
//  f:       virtual Boolean bubbleReady() const;   
//  d:       semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean PRZCTFifoMuxedFlow ::bubbleReady()const
 {
 #ifndef UNIROUTER
    unsigned sizeForCT = ((PRZSimulation*)getComponent().getSimulation())->
                               getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());    
     return ( bufferHoles() >= 2*sizeForCT ) ? true : false;
 #else
     cerr<<"Que hago aqui?";
   return true;
 #endif
 }
 //*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZCTFifoMuxedFlow :: outputWriting()
{
   return Inhereited::outputWriting();
       
}


//*************************************************************************


// fin del fichero
