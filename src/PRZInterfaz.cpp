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
// File: ../ATCSimul/src/PRZInterfaz.cpp
//
// Clase      : PRZInterfaz
//
// Hereda de  : PRZComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZInterfaz.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZInterfaz.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZInterfaz.hpp>

#ifndef __PRZMessage_HPP__
#include <PRZMessage.hpp>
#endif

#ifndef __PRZInputPort_HPP__
#include <PRZInputPort.hpp>
#endif

#ifndef __PRZOutputPort_HPP__
#include <PRZOutputPort.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZGauge_HPP__
#include <PRZGauge.hpp>
#endif

//*************************************************************************

   PRZString PRZInterfaz::sm_InputPrefix = "I";
   PRZString PRZInterfaz::sm_OutputPrefix = "O";
   
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZInterfaz,PRZComponent);

//*************************************************************************
//:
//  f: inline unsigned numberOfCV() const;
//  d:
//
//  f: inline PRZPort* getReadyPort(unsigned channel=1) const;
//  d:
//
//  f: inline PRZPort* getStopPort(unsigned channel=1) const;
//  d:
//
//  f: inline PRZPort* getDataPort(unsigned channel=1) const;
//  d:
//
//  f: inline unsigned getNumberId() const;
//  d:
//
//  f: inline void setNumberId(unsigned number);
//  d:
//
//  f: inline void setReadyPort(PRZPort* ready, unsigned channel=1);
//  d:
//
//  f: inline void setStopPort(PRZPort* stop, unsigned channel=1);
//  d:
//
//  f: inline void setDataPort(PRZPort* data, unsigned channel=1);
//  d:
//
//  f: inline Boolean isChannelValid(unsigned ch) const;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZInterfaz( const PRZComponentId& id, 
//                  unsigned numberCV, 
//                  Boolean wrapper=false );
//
//  d:
//:
//*************************************************************************

PRZInterfaz :: PRZInterfaz( const PRZComponentId& id, 
                            unsigned numberCV,
                            Boolean wrapper )
             : PRZComponent(id),
               m_Ports(0),
               m_SizeCV(numberCV),
               m_Wrapper(wrapper),
               m_Gauge(0)
{
   m_Ports = new PRZPort*[m_SizeCV];
   for( int i=0; i<m_SizeCV; i++ )
   {
      *(m_Ports+i) = 0;
   }
}


//*************************************************************************
//:
//  f: PRZInterfaz(const PRZInterfaz& interfaz);
//
//  d:
//:
//*************************************************************************

PRZInterfaz :: PRZInterfaz(const PRZInterfaz& interfaz)
             : PRZComponent(interfaz),
               m_Ports(0),
               m_Number(interfaz.m_Number),
               m_SizeCV(interfaz.m_SizeCV),
               m_Wrapper(true),
               m_Gauge(interfaz.m_Gauge)
{
   m_Ports = new PRZPort*[m_SizeCV];
   for( int i=0; i<m_SizeCV; i++ )
   {
      *(m_Ports+i) = *(interfaz.m_Ports+i);
   }
}


//*************************************************************************
//:
//  f: virtual ~PRZInterfaz();
//
//  d:
//:
//*************************************************************************

PRZInterfaz :: ~PRZInterfaz()
{
   if( ! m_Wrapper )
   {
      for( int i=0; i<m_SizeCV; i++ )
      {
         delete *(m_Ports+i);
      }
   }

   delete[] m_Ports;
}


//*************************************************************************
//:
//  f: virtual void sendData(const PRZMessage& data, unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: sendData(PRZMessage* data, unsigned channel)                                    
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendData(data);
      if(m_Gauge) m_Gauge->onDataTx(this);
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void sendDataRightNow(const PRZMessage& data, unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: sendDataRightNow(PRZMessage* data, unsigned channel)                                    
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendDataRightNow(data);
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void clearData(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: clearData(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearData();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void clearDataRightNow(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: clearDataRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearDataRightNow();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual Boolean isReadyActive(unsigned channel=1) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: isReadyActive(unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      return (*(m_Ports+channel-1))->isReadyActive();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }

   return false;
}


//*************************************************************************
//:
//  f: virtual void sendStop(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: sendStop(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendStop();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void sendStopRightNow(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: sendStopRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendStopRightNow();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
   
}

//*************************************************************************
//:
//  f: virtual void clearStop(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: clearStop(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearStop();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void clearStopRightNow(unsigned channel=1);
//
//  d:
//:
//*************************************************************************
      
void PRZInterfaz :: clearStopRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearStopRightNow();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean isStopActive(unsigned channel=1) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: isStopActive(unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      return (*(m_Ports+channel-1))->isStopActive();
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      err+=getName();
      EXIT_PROGRAM(err);
   }

   return false;
}




//*************************************************************************
//:
//  f: virtual Boolean getData(PRZMessage& msg, unsigned channel=1) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: getData(PRZMessage ** msg, unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      if( (*(m_Ports+channel-1))->isReadyActive() )
      {
          *msg = (*(m_Ports+channel-1))->getData();
          return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZINTER_001, channel );
      EXIT_PROGRAM(err);
   }

   return false;
}


//*************************************************************************
//:
//  f: static PRZInterfaz* createInputInterfaz( unsigned number=1,
//                                              unsigned numberCV=1 );
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZInterfaz :: createInputInterfaz( unsigned number,
                                                 unsigned numberCV)
{
	PRZString name = PRZInterfaz::sm_InputPrefix + PRZString(number);
	
   PRZComponentId idInterfaz( PRZComponentId::InputInterfaz,
                              _LocalNode_,
                              name );

   PRZInterfaz* interfaz = new PRZInterfaz(idInterfaz,numberCV);
   interfaz->setNumberId(number);
   for( int i=1; i<=numberCV; i++ )
   {
      PRZPort* port = new PRZInputPort(INPUT_PORT);
      port->setChannel(i);
      interfaz->setPort(port,i);
   }
      
   return interfaz;
}


//*************************************************************************
//:
//  f: static PRZInterfaz* createOutputInterfaz( unsigned number=1,
//                                               unsigned numberCV=1 );
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZInterfaz :: createOutputInterfaz(unsigned number,
                                                 unsigned numberCV )
{
	PRZString name = PRZInterfaz::sm_OutputPrefix + PRZString(number);
	
   PRZComponentId idInterfaz( PRZComponentId::OutputInterfaz,
                              _LocalNode_,
                              name );

   PRZInterfaz* interfaz = new PRZInterfaz(idInterfaz, numberCV);
   interfaz->setNumberId(number);
   for( int i=1; i<=numberCV; i++ )
   {
      PRZPort* port = new PRZOutputPort(OUTPUT_PORT);
      port->setChannel(i);
      interfaz->setPort(port,i);
   }
   
   return interfaz;
}

//*************************************************************************
//:
//  f: static PRZInterfaz* createWrapperFor( PRZInterfaz* interfaz[],
//                                           unsigned number );
//
//  d:
//:
//*************************************************************************

PRZInterfaz* PRZInterfaz::createWrapperFor( PRZInterfaz* interfaz[],
                                            unsigned number )
{
   // Un wrapper se puede agrupar a mas de un interfaz, o bien uno solo, el
   // cual puede tener varios canales virtuales.
   
   for( int i=0; i<number; i++ )
   {
      if( ! interfaz[i] ) return 0;
   }
   
   unsigned numberCV = (number==1) ? interfaz[0]->numberOfCV() : number;
   
   PRZInterfaz* wrapper = new PRZInterfaz( interfaz[0]->componentId(),
                                           numberCV,
                                           true );
   
   if( number == 1 ) 
   {
      // En este caso, se hace un wrapper de un interfaz. Hay que considerar
      // que puede tener varios canales virtuales.
      
      for( int j=1; j<=numberCV; j++ )
         wrapper->setPort(interfaz[0]->getPort(j),j);
   }
   else
   {
      // Aqui existen varios interfaces (con un CV, se supone) que se van a 
      // unir en el wrapper.
      
      for( int j=0; j<number; j++ )
         wrapper->setPort(interfaz[j]->getPort(),j+1);   
   }

   wrapper->setNumberId(interfaz[0]->getNumberId());

   return wrapper;
}


//*************************************************************************
//:
//  f: virtual void setOwner(PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

void PRZInterfaz :: setOwner(PRZComponent* owner)
{
   if( ! m_Wrapper )
   {
      // Solo cuando el interfaz es propietario (no es un wrapped) se
      // permite el cambio de owner.
      
      Inhereited :: setOwner(owner);
      for( int i=0; i<m_SizeCV; i++ )
      {
         (*(m_Ports+i))->setOwner(this);
      }
   }
}


//*************************************************************************
//:
//  f: virtual Boolean onDataUp(unsigned interfaz, unsigned canal);
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: onDataUp(unsigned interfaz, unsigned canal)
{
   Boolean rc = getOwner()->onReadyUp( interfaz, canal );
   return rc;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataDown(unsigned interfaz, unsigned canal);
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: onDataDown(unsigned interfaz, unsigned canal)
{
   return getOwner()->onReadyDown( interfaz, canal );
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned canal);
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: onStopUp(unsigned interfaz, unsigned canal)
{
   return getOwner()->onStopUp( interfaz, canal );
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned canal);
//
//  d:
//:
//*************************************************************************

Boolean PRZInterfaz :: onStopDown(unsigned interfaz, unsigned canal)
{
   return getOwner()->onStopDown(interfaz,canal);
}


//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZInterfaz :: run(uTIME runTime)
{
   // No hace nada. Es necesario para que no sea una clase abstracta.
}


//*************************************************************************
//:
//  f: void setGauge(PRZGauge* gauge)
//
//  d:
//:
//*************************************************************************

void PRZInterfaz :: setGauge(PRZGauge* gauge)
{
   for( int i=0; i<m_SizeCV; i++ )
   {
      m_Ports[i]->setGauge(gauge);
   }
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZInterfaz :: asString() const
{
   PRZString string = "Interfaz:";
   string += getName();
   //if(getOwner()) string+=getOwner()->asString();
   return string;
}

//*************************************************************************


// fin del fichero
