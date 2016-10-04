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
// File: ../ATCSimul/src/PRZGauge.cpp
//
// Clase      : PRZGauge
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZGauge.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZGauge.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZGauge.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

#include <fstream.h>
#include <iomanip.h>
    
//*************************************************************************

// Run time information
IMPLEMENT_RTTI_BASE(PRZGauge);


//*************************************************************************
//:
//  f: PRZRouter(const PRZString& fileName);
//
//  d:
//:
//*************************************************************************

PRZGauge :: PRZGauge(const PRZString& fileName)
          : m_Router(0),
            m_FileName(fileName),
            m_File(0)
{
   m_File = new ofstream(m_FileName);
}


//*************************************************************************
//:
//  f: ~PRZGauge();
//
//  d:
//:
//*************************************************************************

PRZGauge :: ~PRZGauge()
{
   m_MessagesList.removeAllElements();
   m_File->close();
   delete m_File;
}


//*************************************************************************
//:
//  f: void setRouter(PRZRouter* router);
//
//  d:
//:
//*************************************************************************

void PRZGauge :: setRouter(PRZRouter* router)
{
   m_Router = router;
   for( int i=1; i<=4; i++ )
   {
      router->getInputInterfaz(i) -> setGauge(this);
      router->getOutputInterfaz(i)-> setGauge(this);
   }

   PRZSequence<PRZComponent*>::Cursor cursorInj(m_Router->m_InjectorList);
   PRZSequence<PRZComponent*>::Cursor cursorCon(m_Router->m_ConsumerList);
   
   forCursor(cursorInj)
   {
      ((PRZRunnableComponent*)(cursorInj.element()))->getOutputInterfaz()->setGauge(this);
   }
   
   forCursor(cursorCon)
   {
      ((PRZRunnableComponent*)(cursorCon.element()))->getInputInterfaz()->setGauge(this);
   }
}


//*************************************************************************
//:
//  f: void onDataTx(PRZInterfaz* interfaz);
//
//  d:
//:
//*************************************************************************

void PRZGauge :: onDataTx(PRZInterfaz* interfaz)
{
   PRZMessage *msg;
   interfaz->getData(&msg);
   if( msg->isHeader() )
   {
      TMessageIdList::Cursor cursor(m_MessagesList);
      uTIME delay = 0;
      forCursor(cursor)
      {
         if( cursor.element().left() == msg->getIdentifier() )
         {
            delay = m_Router->getCurrentTime() - cursor.element().right();
            cursor.remove();
            break;
         }
      }
      
      if( ! delay )
      {
         // El mensaje proviene del inyector
         m_MessagesList.add( TMessageToken( msg->getIdentifier(),
                                            m_Router->getCurrentTime() ) );
         (*m_File) << PRZString("RX ") 
                   << PRZString(interfaz->getOwner()->asString())
                   << PRZString(".")
                   << PRZString(interfaz->getNumberId())
                   << PRZString(" T=")
                   << PRZString(m_Router->getCurrentTime())
                   << endl;
      }
      else
      {
         (*m_File) << PRZString("TX ") 
                   << PRZString(interfaz->getOwner()->asString())
                   << PRZString(".")
                   << PRZString(interfaz->getNumberId())
                   << PRZString(" ")
                   << PRZString(delay) 
                   << endl;
      }
   }
}


//*************************************************************************
//:
//  f: void onDataRx(PRZInterfaz* interfaz);
//
//  d:
//:
//*************************************************************************

void PRZGauge :: onDataRx(PRZInterfaz* interfaz)
{
   PRZMessage* msg;
   interfaz->getData(&msg);
   if( msg->isHeader() )
   {
      TMessageIdList::Cursor cursor(m_MessagesList);
      Boolean found = false;
      forCursor(cursor)
      {
         if( cursor.element().left() == msg->getIdentifier() )
         {
            found = true;
            break;
         }
      }
   
      if( ! found )
      {
         m_MessagesList.add( TMessageToken( msg->getIdentifier(),
                                            m_Router->getCurrentTime() ) );
         (*m_File) << PRZString("RX ") 
                   << PRZString(interfaz->getOwner()->asString())
                   << PRZString(".") 
                   << PRZString(interfaz->getNumberId())
                   << PRZString(" T=")
                   << PRZString(m_Router->getCurrentTime())
                   << endl;                                            
      }
   }
}


//*************************************************************************
//:
//  f: static PRZGauge* newFrom(const PRZTag& tag, PRZSimulation* simulation);
//
//  d:
//:
//*************************************************************************

PRZGauge* PRZGauge :: newFrom(const PRZTag& tag, PRZSimulation* simulation)
{
   PRZString routerPos, fileName;
   unsigned x=0, y=0, z=0;
   
   tag.getAttributeValueWithName(PRZ_TAG_ROUTER_GAUGE, routerPos);
   tag.getAttributeValueWithName(PRZ_TAG_FILE_GAUGE, fileName);
   
   routerPos.replace( ',' , ' ' );
   x = routerPos.word(1).asInteger();
   y = routerPos.word(2).asInteger();
   z = routerPos.word(3).asInteger();
   
   PRZRouter* router = simulation->getNetwork()->getRouterAt(x,y,z);
   PRZGauge* gauge = new PRZGauge(fileName);
   gauge->setRouter(router);
   return gauge;
}

//*************************************************************************


// fin del fichero
