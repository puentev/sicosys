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
// File: ../ATCSimul/src/PRZDelayConnection.cpp
//
// Clase      : PRZDelayConnection
//
// Hereda de  : PRZConnection 
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZDelayConnection.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZDelayConnection.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZDelayConnection.hpp>

#ifndef __PRZInputPort_HPP__
#include <PRZInputPort.hpp>
#endif

#ifndef __PRZOutputPort_HPP__
#include <PRZOutputPort.hpp>
#endif

#ifndef __PRZMessage_HPP__
#include <PRZMessage.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZDelayConnection,PRZConnection);


//*************************************************************************
//:
//  f: PRZDelayConnection( PRZOutputPort* org,
//                         PRZInputPort*  dst,
//                         unsigned ciclo );
//
//  d:
//:
//*************************************************************************

PRZDelayConnection :: PRZDelayConnection( PRZOutputPort* org,
                                          PRZInputPort* dst,
                                          unsigned ciclo )
                    : PRZConnection(org,dst),
                      m_Delay(ciclo),
                      m_MessageDelay(0)
{
   m_MessageDelay = new PRZMessage*[m_Delay];
   for( int i=0; i<m_Delay; i++ )
      *(m_MessageDelay+i) = 0;
}


//*************************************************************************
//:
//  f: ~PRZDelayConnection();
//
//  d:
//:
//*************************************************************************

PRZDelayConnection :: ~PRZDelayConnection()
{
   for( int i=0; i<m_Delay; i++ )
      delete *(m_MessageDelay+i);
      
   delete[] m_MessageDelay;
}


//*************************************************************************
//:
//  f: virtual void run(TIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZDelayConnection :: run(uTIME runTime)
{
   if( getInputPort() && getOutputPort() )
   {
      PRZMessage* newMsg = (getOutputPort()->isReadyActive()) ? 
                           getOutputPort()->getData() :
                           0 ;
      if(newMsg) SFSMSGTRACE(newMsg,"PRZDelayConnection :: run(uTIME time)    in");
      addMessage(newMsg);
      
      newMsg = getNextMessage();
      if(newMsg) SFSMSGTRACE(newMsg,"PRZDelayConnection :: run(uTIME time)     out");
      
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
   }
}


//*************************************************************************
//:
//  f: Boolean sendData(const PRZMessage& data);
//
//  d:
//:
//*************************************************************************

Boolean PRZDelayConnection :: sendData(PRZMessage* data)
{
   if( getInputPort() )
   {
      addMessage(data);
      PRZMessage* newMsg = getNextMessage();
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
         delete newMsg;
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
      
      return true;
   }

   return false;
}


//*************************************************************************
//:
//  f: void clearData();
//
//  d:
//:
//*************************************************************************

void PRZDelayConnection :: clearData()
{
   if( getInputPort() )
   {
      PRZMessage* newMsg = 0;
      addMessage(newMsg);
      newMsg = getNextMessage();
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
   }
}

//*************************************************************************
//:
//  f: void addMessage(PRZMessage* msg);
//
//  d:
//:
//*************************************************************************

void PRZDelayConnection :: addMessage(PRZMessage* msg)
{
   // Los mensajes entran por 0 y salen por m_Delay-1;
   *m_MessageDelay = msg;
}


//*************************************************************************
//:
//  f: PRZMessage* getNextMessage();
//
//  d:
//:
//*************************************************************************

PRZMessage* PRZDelayConnection :: getNextMessage()
{
   // Los mensajes entran por 0 y salen por m_Delay-1;
   
   PRZMessage* rm = *(m_MessageDelay+m_Delay-1);
   for(int i=m_Delay-1; i>0; i-- )
      *(m_MessageDelay+i) = *(m_MessageDelay+i-1);
      
   *m_MessageDelay = 0;   
   
   return rm;
}


//*************************************************************************
//:
//  f: static Boolean connectInterfaces( PRZComponent* owner,
//                                       PRZInterfaz* src, 
//                                       PRZInterfaz* dst,
//                                       unsigned ciclos );
//
//  d:
//:
//*************************************************************************

Boolean PRZDelayConnection :: connectInterfaces( PRZComponent* owner,
                                                 PRZInterfaz* src, 
                                                 PRZInterfaz* dst,
                                                 unsigned ciclos )
{
   if( ! (owner && src && dst) )
   {
      PRZString err;
      err.sprintf(ERR_PRZDECON_001,owner,src,dst);
      EXIT_PROGRAM(err);
   }
   if( src->numberOfCV() != dst->numberOfCV() )
   {
      PRZString err;
      err.sprintf( ERR_PRZDECON_002, 
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
         err.sprintf(ERR_PRZDECON_003, (char*)(dst->getName()) );
         EXIT_PROGRAM(err);
      }
      if( ! srcPort )
      {
         PRZString err;
         err.sprintf(ERR_PRZDECON_004, (char*)(src->getName()) );
         EXIT_PROGRAM(err);
      }
   
      PRZDelayConnection* conn = 
                          new PRZDelayConnection(srcPort,dstPort, ciclos);
   
      owner->addConnection(conn);

   }
   
   return true;
}

//*************************************************************************


// fin del fichero
