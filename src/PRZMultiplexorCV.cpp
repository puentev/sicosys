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
// File: ../ATCSimul/src/PRZMultiplexorCV.cpp
//
// Clase      : PRZMultiplexorCV
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiplexorCV.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiplexorCV.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZMultiplexorCV.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZMultiplexorCVFlow_HPP__
#include <PRZMultiplexorCVFlow.hpp>
#endif

#ifndef __PRZMultiplexorCVFlowCT_HPP__
#include <PRZMultiplexorCVFlowCT.hpp>
#endif
/*
#ifndef __PRZMultiplexorBufferCVFlow_HPP__
#include <PRZMultiplexorBufferCVFlow.hpp>
#endif
*/
#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiplexorCV,PRZRunnableComponent);

//*************************************************************************
//:
//  f: PRZMultiplexorCV(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCV :: PRZMultiplexorCV(const PRZComponentId& id)
                  : PRZRunnableComponent(id),
                    m_ctmux(0)
                  
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCV :: buildFlowControl()
{
   if (m_ctmux==1)
   {
      PRZMultiplexorCVFlowCT* flow = new PRZMultiplexorCVFlowCT(*this);
      flow -> initialize();
      setFlowControl(flow);
   }
   else 
   {
       if( getBufferSize() == 1 )
       {
         PRZMultiplexorCVFlow* flow = new PRZMultiplexorCVFlow(*this);
         flow -> initialize();
         setFlowControl(flow);
        }
       else if( getBufferSize() > 1 )
       {
/*          PRZMultiplexorBufferCVFlow* flow = new PRZMultiplexorBufferCVFlow(*this);
          flow -> initialize();
          setFlowControl(flow);*/
      EXIT_PROGRAM("UNIMPLEMENTED");
       }
    }
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz(unsigned number =1 );
//
//  d:
//:
//*************************************************************************

void PRZMultiplexorCV :: buildOutputInterfaz(unsigned number)
{
   // Es necesario sobrecargar el metodo, ya que el interfaz de salida del
   // multiplexor debe llevar N senales de control, siendo N el numero de
   // canales virtuales.

   for( int i=1; i<=number; i++ )   
   {
      PRZInterfaz* interfaz = 
                   PRZInterfaz::createOutputInterfaz(i,numberOfInputs());
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZMultiplexorCV :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZMultiplexorCV* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZMultiplexorCV* PRZMultiplexorCV :: newFrom( const PRZTag* tag, 
                                               PRZComponent* owner )
{
   PRZComponentId idMultiplexor(*tag);
   PRZString muxInputs  = "2";
   PRZString bufferSize = "1";
   unsigned size=2, muxBufferSize=1;
   
   if( tag->getAttributeValueWithName(PRZ_TAG_INPUTS, muxInputs) )
   {
      size = muxInputs.asInteger();
   }
   
   if( tag->getAttributeValueWithName(PRZ_TAG_BUFFER_SIZE, bufferSize) )
   {
      muxBufferSize = bufferSize.asInteger();
      //cerr<<(int)muxBufferSize;
      // Se comprueba que el tamamaño especificado no sea inferior a 1.

   }
   

   
   PRZMultiplexorCV* mux = new PRZMultiplexorCV(idMultiplexor);
   
   if(muxBufferSize==0) mux->m_ctmux=1; else mux->m_ctmux=0;
   muxBufferSize = (muxBufferSize<1) ? 1 : muxBufferSize;
   mux->setNumberOfInputs(size);
   mux->setBufferSize(muxBufferSize);
   
   // Cuando el tamaño del buffer sea 1, entonces se tienen que realizar
   // operaciones especiales para no perder ciclos en el multiplexor.
   // Estas operaciones se realizan en el "postRun()" de la clase.
   if(muxBufferSize==1) owner->addPostRunComponent(mux);
   
   return mux;
}


//*************************************************************************


// fin del fichero
