//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//
// File: ../ATCSimul/inc/PRZMessage.hpp
//
// Description:  fichero de encabezado de la clase "PRZMessage"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMessage.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMessage_HPP__
#define __PRZMessage_HPP__

//*************************************************************************

   #ifndef __PRZPosition_HPP__
   #include <PRZPosition.hpp>
   #endif

   #ifndef __PRZPair_HPP__
   #include <PRZPair.hpp>
   #endif
	
   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif

   typedef PRZPair<unsigned,unsigned> TUPair;
   typedef PRZSequence<TUPair>        TPortList;

//*************************************************************************
//   #define SFSMSGTRACE(msg,text) cout << (msg)->getIdentifier() << text << endl;
   #define SFSMSGTRACE(msg,text) 
   
   
   class PRZMessage
   {
   
   public:
      enum MESSAGETYPE { INVALID = 0, 
                         HEADER, 
                         INFO, 
                         TAIL,
                         eREADY, 
                         STOP, 
                         REQ,
                         MREQ };

      PRZMessage();
      PRZMessage(const PRZMessage& aMessage);
      ~PRZMessage();

      PRZMessage& operator=(const PRZMessage& aMessage);
      Boolean operator==(const PRZMessage& aMessage) const;
      virtual PRZString asString() const;

      static void assignUniqueId(PRZMessage* msg);

      MESSAGETYPE type() const
      { return m_Type; }

      unsigned long getIdentifier() const
      { return m_Identifier; }
      
      void setType(const MESSAGETYPE& tipo)
      { m_Type = tipo; }

      unsigned short flitNumber() const
      { return m_FlitNumber; }

      void setFlitNumber(unsigned order)
      { m_FlitNumber = order; }

      unsigned short packetNumber() const
      { return m_PacketNumber; }

      void setPacketNumber(unsigned order)
      { m_PacketNumber = order; }

      unsigned short messageSize() const
      { return m_MessageSize; }

      void setMessageSize(unsigned packets)
      { m_MessageSize = packets; }

      unsigned short packetSize() const
      { return m_PacketSize; }

      void setPacketSize(unsigned flits)
      { m_PacketSize = flits; }

      uTIME generationTime() const
      { return m_GenerationTime; }

      void setGenerationTime(const uTIME& aTime)
      { m_GenerationTime = aTime; }

      uTIME packetInjectionTime() const
      { return m_InjectionTime; }

      void setPacketInjectionTime(const uTIME& aTime)
      { m_InjectionTime = aTime; }
      
      PRZPosition& source() const
      { return (PRZPosition&)m_SourcePosition; }

      void setSource(const PRZPosition& org)
      { m_SourcePosition = org; }

      PRZPosition& destiny() const
      { return (PRZPosition&)m_DestinyPosition; }

      void setDestiny(const PRZPosition& dst)
      { m_DestinyPosition = dst; }
      

      przROUTINGTYPE getRoutingPort() const
      { return m_RoutingPort; }

      void setRoutingPort(przROUTINGTYPE port)
      { m_RoutingPort = port; }


      Boolean isHeader() const
      { return m_Type==HEADER; }
      
      Boolean isTail() const
      { return m_Type==TAIL; }

      int short delta(int short header=0) const
      { return m_Delta[header]; }

      int short deltaAbs(int short header=0) const
      { return (m_Delta[header]>=0) ? m_Delta[header] : (-m_Delta[header]); }
      
      void setDelta(int short delt, int short header=0)
      { m_Delta[header] = delt; }

      void setMissRouting(int short delt, int short header=0)
      { m_MissRouting[header] = delt; }
      
      int short getMissRouting(int short header=0)
      { return m_MissRouting[header]; }
      
      Boolean getLastMissRouted()
      {return  m_lastMissRouted;}
      
      void setLastMissRouted(Boolean flag)
      { m_lastMissRouted=flag;}
         
      void incDistance()
      {m_distance++;}
      
      unsigned long getDistance() const
      {return m_distance;}
                  
      unsigned short getChannel() const
      { return m_Channel; }
      
      void setChannel(unsigned channel)
      { m_Channel = channel; }
      
      TPortList* getPortList() const
      { return m_PortList; }

      void setPortList(TPortList* portList)
      { m_PortList = portList; }

      void* getExternalInfo() const
      { return m_ExternalInfo; }

      void setExternalInfo(void* ptr)
      { m_ExternalInfo = ptr; }
      
      void setRequest()
      {
         m_isRequest = true;
      }
      
      Boolean isRequest() const
      {
         return m_isRequest;
      }

      void destroyPortList();
      
      unsigned getMessageLength() const
      {
         return m_MessageSize;
      }
      
      
      unsigned getPacketLength() const
      {
         return m_PacketSize;
      }     
      // Run time information
      DEFINE_RTTI(PRZMessage);

   private:
      MESSAGETYPE          m_Type;
      unsigned short       m_FlitNumber;
      unsigned short       m_PacketNumber;
      unsigned short       m_MessageSize;
      unsigned short       m_PacketSize;
      uTIME                m_GenerationTime;
      uTIME                m_InjectionTime;
      PRZPosition          m_SourcePosition;
      PRZPosition          m_DestinyPosition;
      przROUTINGTYPE       m_RoutingPort;
      int short            m_Delta[3];
      unsigned short       m_Channel;
      unsigned long        m_Identifier;
      TPortList*           m_PortList;
      void*                m_ExternalInfo;
      //ccNuma Support
      Boolean              m_isRequest;
      
      //Miss Routing Suport.
      int short            m_MissRouting[3];
      Boolean              m_lastMissRouted;
      static unsigned long sm_Identifier;
      unsigned long        m_distance;
   };

//*************************************************************************

   #define PRZMESSAGE_READY  PRZMessage(PRZMessage::READY)
   #define PRZMESSAGE_STOP   PRZMessage(PRZMessage::STOP)

//*************************************************************************

#endif


// fin del fichero
