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
// File: ../ATCSimul/src/PRZMessage.cpp
//
// Clase      : PRZMessage
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMessage.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMessage.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZMessage.hpp>

#ifndef __PRZPosition_HPP__
#include <PRZPosition.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZMessage);

unsigned long PRZMessage :: sm_Identifier = 0;

//*************************************************************************
//:
//  f: inline MESSAGETYPE type() const;
//  d:
//
//  f: inline void setType(const MESSAGETYPE& tipo);
//  d:
//
//  f: inline unsigned long getIdentifier() const;
//  d:
//
//  f: inline unsigned short flitNumber() const;
//  d:
//
//  f: inline void setFlitNumber(unsigned order);
//  d:
//
//  f: inline unsigned short packetNumber() const;
//  d:
//
//  f: inline void setPacketNumber(unsigned order);
//  d:
//
//  f: inline unsigned short messageSize() const;
//  d:
//
//  f: inline void setMessageSize(unsigned packets);
//  d:
//
//  f: inline unsigned short packetSize() const;
//  d:
//
//  f: inline void setPacketSize(unsigned flits);
//  d:
//
//  f: inline uTIME generationTime() const;
//  d:
//
//  f: inline void setGenerationTime(const uTIME& aTime);
//  d:
//
//  f: inline uTIME packetInjectionTime() const;
//  d:
//
//  f: inline void setPacketInjectionTime(const uTIME& aTime);
//  d:
//
//  f: inline PRZPosition& source() const;
//  d:
//
//  f: inline void setSource(const PRZPosition& org);
//  d:
//
//  f: inline PRZPosition& destiny() const;
//  d:
//
//  f: inline void setDestiny(const PRZPosition& dst);
//  d:
//
//  f: inline przROUTINGTYPE getRoutingPort() const;
//  d:
//
//  f: inline void setRoutingPort(przROUTINGTYPE port);
//  d:
//
//  f: inline Boolean isHeader() const;
//  d:
//
//  f: inline Boolean isTail() const;
//  d:
//
//  f: inline int short delta() const;
//  d:
//
//  f: inline void setDelta(int short delt);
//  d:
//
//  f: inline unsigned short getChannel() const;
//  d:
//
//  f: inline void setChannel(unsigned channel);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZMessage();
//
//  d:
//:
//*************************************************************************

PRZMessage :: PRZMessage()
            : m_Type(INFO),
              m_Identifier(0),
              m_FlitNumber(1),
              m_PacketNumber(1),
              m_MessageSize(0),
              m_PacketSize(0),
              m_isRequest(false),
              m_GenerationTime(0.0),
              m_InjectionTime(0.0),
              //m_SourcePosition(PRZPosition(0,0,0)),
              //m_DestinyPosition(PRZPosition(0,0,0)),
              m_RoutingPort(_Unknow_),
              m_Channel(1),
              m_PortList(0),
              m_ExternalInfo(0),
              m_distance(0),
              m_lastMissRouted(false)
{
#ifndef NO_TRAZA 
   			  m_Type=INFO;
              m_Identifier=0;
              m_FlitNumber=1;
              m_PacketNumber=1;
              m_MessageSize=0;
              m_PacketSize=0;
              m_GenerationTime=0.0;
              m_InjectionTime=0.0;
              m_SourcePosition=PRZPosition(0,0,0);
              m_DestinyPosition=PRZPosition(0,0,0);
              m_RoutingPort=_Unknow_;
              m_Channel=1;
              m_PortList=0;
              m_ExternalInfo=0;
              //m_distance(0),
   
#endif
   m_Delta[0] = m_Delta[1] = m_Delta[2] = -10;
  // m_MissRouting[0]=m_MissRouting[1]=m_MissRouting[2]=0;
//  cout << "SFSnew" << endl;
}


//*************************************************************************
//:
//  f: PRZMessage(const PRZMessage& aMessage);
//
//  d: Constructor copia.
//:
//*************************************************************************

PRZMessage :: PRZMessage(const PRZMessage& aMessage)
            : m_Type( aMessage.type() ),
              m_Identifier( aMessage.getIdentifier() ),
              m_FlitNumber( aMessage.flitNumber() ),
              m_PacketNumber( aMessage.packetNumber() ),
              m_MessageSize( aMessage.messageSize() ),
              m_PacketSize( aMessage.packetSize() ),
              m_isRequest(aMessage.isRequest()),
              m_GenerationTime( aMessage.generationTime() ),
              m_InjectionTime( aMessage.packetInjectionTime() ),
              m_SourcePosition( aMessage.source() ),
              m_DestinyPosition( aMessage.destiny() ),
              m_RoutingPort( aMessage.getRoutingPort() ),
              m_PortList( aMessage.getPortList() ),
              m_Channel(aMessage.getChannel()),
              m_ExternalInfo(aMessage.getExternalInfo()),
              m_lastMissRouted(aMessage.m_lastMissRouted)
              //m_distance(aMessage.getDistance()),

              
{

   m_Delta[0] = aMessage.delta(0);
   m_Delta[1] = aMessage.delta(1);
   m_Delta[2] = aMessage.delta(2);
  // m_MissRouting[0]=aMessage.m_MissRouting[0];
  // m_MissRouting[1]=aMessage.m_MissRouting[1];
  // m_MissRouting[2]=aMessage.m_MissRouting[2];

//  cout << "SFScopy" << endl;
}


//*************************************************************************
//:
//  f: ~PRZMessage();
//
//  d:
//:
//*************************************************************************

PRZMessage :: ~PRZMessage()
{

//  cout << "SFSdelete" << endl;
}


//*************************************************************************
//:
//  f: PRZMessage& operator=(const PRZMessage& aMessage);
//
//  d: Operador asignacion. Tanto para la lista de peticiones de puertos,
//     como para la informacion externa, se copian los punteros y no su
//     contenido.
//:
//*************************************************************************

PRZMessage& PRZMessage :: operator=(const PRZMessage& aMessage)
{
   m_Type            = aMessage.type();
   m_Identifier      = aMessage.getIdentifier();
   m_FlitNumber      = aMessage.flitNumber();
   m_PacketNumber    = aMessage.packetNumber();
   m_MessageSize     = aMessage.messageSize();
   m_PacketSize      = aMessage.packetSize();
   m_GenerationTime  = aMessage.generationTime();
   m_InjectionTime   = aMessage.packetInjectionTime();
   m_SourcePosition  = aMessage.source();
   m_DestinyPosition = aMessage.destiny();
   m_RoutingPort     = aMessage.getRoutingPort();
   m_isRequest       = aMessage.isRequest();
   m_Delta[0]        = aMessage.delta(0);
   m_Delta[1]        = aMessage.delta(1);
   m_Delta[2]        = aMessage.delta(2);
   m_Channel         = aMessage.getChannel();
   m_PortList        = aMessage.getPortList();
   m_ExternalInfo    = aMessage.getExternalInfo();
   m_MissRouting[0]  = aMessage.m_MissRouting[0];
   m_MissRouting[1]  = aMessage.m_MissRouting[1];
   m_MissRouting[2]  = aMessage.m_MissRouting[2];
   //m_distance        = aMessage.m_distance;
   m_lastMissRouted  = aMessage.m_lastMissRouted;
   
   return *this;
}


//*************************************************************************
//:
//  f: void destroyPortList();
//
//  d: Borra toda la lista de solicitudes de puertos y la pone a cero.
//:
//*************************************************************************

void PRZMessage :: destroyPortList()
{
   if( m_PortList )
   {
      m_PortList->removeAllElements();
      delete m_PortList;
      m_PortList = 0;
   }
}


//*************************************************************************
//:
//  f: Boolean operator==(const PRZMessage& aMessage) const;
//
//  d: Operador de comparacion. No compara los punteros de lista de puertos
//     y la informacion externa.
//:
//*************************************************************************

Boolean PRZMessage :: operator==(const PRZMessage& aMessage) const
{
   if( ( type()                == aMessage.type()                ) &&
       ( getIdentifier()       == aMessage.getIdentifier()       ) &&
       ( flitNumber()          == aMessage.flitNumber()          ) &&
       ( packetNumber()        == aMessage.packetNumber()        ) &&
       ( messageSize()         == aMessage.messageSize()         ) &&
       ( packetSize()          == aMessage.packetSize()          ) &&
       ( generationTime()      == aMessage.generationTime()      ) &&
       ( packetInjectionTime() == aMessage.packetInjectionTime() ) &&
       ( source()              == aMessage.source()              ) &&
       ( destiny()             == aMessage.destiny()             ) &&
       ( getRoutingPort()      == aMessage.getRoutingPort()      ) &&
       ( getChannel()          == aMessage.getChannel()          ) &&
       ( isRequest()           == aMessage.isRequest() )         )
      return true;

   return false;
}


//*************************************************************************
//:
//  f:       static void assignUniqueId(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZMessage :: assignUniqueId(PRZMessage* aMessage)
{
   PRZMessage::sm_Identifier++;
   aMessage->m_Identifier = PRZMessage::sm_Identifier;
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZMessage :: asString() const
{
      PRZString rs = "Message: flit=";
      rs += PRZString(flitNumber())   + "/" +
            PRZString(packetSize())   + " " +
//            PRZString(packetNumber()) + "/" +
//            PRZString(messageSize())  +
            " ID=" + PRZString(getIdentifier()) + 
            " ptr=" + PRZString((int)this) + 
            " src=" + source().asString() + 
            " dst=" + destiny().asString() +
            " Delta=" + PRZString(delta(0)) + "/" + 
                        PRZString(delta(1)) + "/" +
                        PRZString(delta(2)) /*+
            " MissRouting="+PRZString(m_MissRouting[0])+ "/"+
                            PRZString(m_MissRouting[1])+ "/"+
                            PRZString(m_MissRouting[2])+ "/"+
            " Rting=" + PRZString(getRoutingPort())*/;
/*       if(isRequest())
       {
          rs+="  Request";
       }*/
            
      return rs;
}

//************************************************************************


// fin del fichero
