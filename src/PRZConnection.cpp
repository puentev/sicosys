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
// File: ../ATCSimul/src/PRZConnection.cpp
//
// Clase      : PRZConnection
//
// Hereda de  : PRZBase
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZConnection.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZConnection.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZConnection.hpp>

#ifndef __PRZInputPort_HPP__
#include <PRZInputPort.hpp>
#endif

#ifndef __PRZOutputPort_HPP__
#include <PRZOutputPort.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZMessage_HPP__
#include <PRZMessage.hpp>
#endif

#ifndef __PRZDelayConnection_HPP__
#include <PRZDelayConnection.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZConnection,PRZBase);

//*************************************************************************
//:
//  f: inline PRZInputPort* getInputPort() const;
//  d:
//
//  f: inline PRZOutputPort* getOutputPort() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZConnection(PRZOutputPort* org, PRZInputPort* dst);
//
//  d:
//:
//*************************************************************************

PRZConnection :: PRZConnection( PRZOutputPort* org,
                                PRZInputPort* dst)
               : m_OutputPort(org),
                 m_InputPort(dst)
{
   setSource(org);
   setDestiny(dst);
}


//*************************************************************************
//:
//  f: PRZConnection(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZConnection :: PRZConnection()
               : m_InputPort(0),
                 m_OutputPort(0)
{

}


//*************************************************************************
//:
//  f: ~PRZConnection();
//
//  d:
//:
//*************************************************************************

PRZConnection :: ~PRZConnection()
{

}


//*************************************************************************
//:
//  f: void setSource(PRZOutputPort* org);
//
//  d:
//:
//*************************************************************************

void PRZConnection :: setSource(PRZOutputPort* org)
{
   if(!org) return;
   m_OutputPort = org;
   m_OutputPort->establishConnection(this);
}


//*************************************************************************
//:
//  f: void setDestiny(PRZInputPort* dst);
//
//  d:
//:
//*************************************************************************

void PRZConnection :: setDestiny(PRZInputPort* dst)
{
   if(!dst) return;
   m_InputPort = dst;
   m_InputPort->establishConnection(this);
}


//*************************************************************************
//:
//  f: Boolean sendData(const PRZMessage& data);
//
//  d:
//:
//*************************************************************************

Boolean PRZConnection :: sendData(PRZMessage* data)
{
   if( m_InputPort )
   {
      m_InputPort->sendData(data);
      return m_InputPort->onDataUp(0,0);
   }
   return false;
}


//*************************************************************************
//:
//  f: Boolean clearData();
//
//  d:
//:
//*************************************************************************

void PRZConnection :: clearData()
{
   if( m_InputPort )
   {
      m_InputPort->clearData();
      m_InputPort->onDataDown(0,0);
   }
}


//*************************************************************************
//:
//  f: Boolean sendStop();
//
//  d:
//:
//*************************************************************************

Boolean PRZConnection :: sendStop()
{
   if( m_OutputPort )
   {
      m_OutputPort->sendStop();
      return m_OutputPort->onStopUp(0,0);
   }
   return false;
}


//*************************************************************************
//:
//  f: void clearStop();
//
//  d:
//:
//*************************************************************************

void PRZConnection :: clearStop()
{
   if( m_OutputPort )
   {
      m_OutputPort -> clearStop();
      m_OutputPort -> onStopDown(0,0);
   }
}


//*************************************************************************
//:
//  f: virtual void run(uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZConnection :: run(uTIME time)
{
   if( m_InputPort && m_OutputPort )
   {
      if( m_OutputPort->isDataChanged() )
      {
         if( m_OutputPort->isReadyActive() )
         {
            m_InputPort->sendData(m_OutputPort->getData());
            m_InputPort->onDataUp(0,0);
         }
         else
         {
            m_InputPort->clearData();
            m_InputPort->onDataDown(0,0);
         }
         m_OutputPort->setDataChanged(false);
      }
      
      if( m_InputPort->isStopChanged() )
      {
         if( m_InputPort->isStopActive() )   // Señal de Stop
         {
            m_OutputPort->sendStop();
            m_OutputPort->onStopUp(0,0);
         }
         else
         {
            m_OutputPort->clearStop();
            m_OutputPort->onStopDown(0,0);      
         }
         m_InputPort->setStopChanged(false);
      }
   }
}


//*************************************************************************
//:
//  f: static Boolean connectInterfaces( PRZComponent* owner,
//                                       PRZInterfaz* src, 
//                                       PRZInterfaz* dst,
//                                       unsigned delay =0 );
//
//  d:
//:
//*************************************************************************

Boolean PRZConnection :: connectInterfaces( PRZComponent* owner,
                                            PRZInterfaz* src, 
                                            PRZInterfaz* dst,
                                            unsigned delay )
{
   if( delay != 0 )
     return PRZDelayConnection::connectInterfaces(owner,src,dst,delay);
     
   if( ! (owner && src && dst) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCONEC_001,owner,src,dst);
      EXIT_PROGRAM(err);
   }
   if( src->numberOfCV() != dst->numberOfCV() )
   {
      PRZString err;
      err.sprintf( ERR_PRZCONEC_002, 
                   src->numberOfCV(), 
                   dst->numberOfCV() );
      EXIT_PROGRAM(err);
   }

   for( int i=1; i<=src->numberOfCV(); i++ )
   {
      PRZOutputPort* srcPort = POINTERCAST(PRZOutputPort,src->getPort(i));
      PRZInputPort*  dstPort = POINTERCAST(PRZInputPort, dst->getPort(i) );
   
      if( ! dstPort )
      {
         PRZString err;
         err.sprintf(ERR_PRZCONEC_003, (char*)(dst->getName()) );
         EXIT_PROGRAM(err);
      }
      if( ! srcPort )
      {
         PRZString err;
         err.sprintf(ERR_PRZCONEC_004, (char*)(src->getName()) );
         EXIT_PROGRAM(err);
      }
      
      PRZConnection* conn = new PRZConnection(srcPort,dstPort);

      // Se pasan la conexiones al owner para que las destruya cuando proceda.
   
      owner->addConnection(conn);
   }
   
   return true;
}


//*************************************************************************
//:
//  f: static PRZConnection* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZConnection* PRZConnection :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   // PRZComponentId idConnection(*tag);
   PRZString source, destiny;

   if( ! (tag->getAttributeValueWithName(PRZ_TAG_SOURCE, source)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCONEC_005, (char*)(tag->tagId()) );
      EXIT_PROGRAM(err);
   }

   if( ! (tag->getAttributeValueWithName(PRZ_TAG_DESTINY, destiny)) )
   {
      PRZString err;
      err.sprintf(ERR_PRZCONEC_006, (char*)(tag->tagId()) );
      EXIT_PROGRAM(err);
   }
   
   // Los identificadores pueden venir indexados (con un punto)
   
   PRZString sourceName = PRZString::getStringLeftTo(source,'.');
   PRZString sourcePort = PRZString::getStringRightTo(source,'.');
   unsigned sPort = (sourcePort!="") ? sourcePort.asInteger() : 1; 

   PRZString destinyName = PRZString::getStringLeftTo(destiny,'.');
   PRZString destinyPort = PRZString::getStringRightTo(destiny,'.');
   unsigned dPort = (destinyPort!="") ? destinyPort.asInteger() : 1; 
   
   PRZComponent* sComponent = owner->componentWithName(sourceName);
   PRZComponent* dComponent = owner->componentWithName(destinyName);
   
   if( !sComponent )
   {
      PRZString err;
      err.sprintf(ERR_PRZCONEC_007, (char*)source, (char*)(tag->tagId()) ); 
      EXIT_PROGRAM(err);
   }
   
   if( !dComponent )
   {
      PRZString err;
      err.sprintf(ERR_PRZCONEC_008, (char*)destiny, (char*)(tag->tagId()) ); 
      EXIT_PROGRAM(err);
   }
   
   PRZConnection::connectInterfaces( owner,
                                     sComponent->getOutputInterfaz(sPort),
                                     dComponent->getInputInterfaz(dPort) );
   return 0;
}

//*************************************************************************


// fin del fichero
