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
// File: ../ATCSimul/src/PRZEvent.cpp
//
// Clase      : PRZEvent
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZEvent.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZEvent.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZEvent.hpp>

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZEvent);

//*************************************************************************
//:
//  f: inline unsigned source() const;
//  d:
//
//  f: inline unsigned destiny() const;
//  d:
//
//  f: inline unsigned channel() const;
//  d:
//
//  f: inline PRZMessage message() const;
//  d:
//
//  f: inline przEVENTTYPE type() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZEvent();
//
//  d:
//:
//*************************************************************************

PRZEvent :: PRZEvent()
          //: m_Type(_NullEvent_),
          //  m_Source(0),
          //  m_Destiny(0),
          //  m_Channel(0)
            //m_Message(PRZMessage())
{
   #ifndef NO_TRAZA 
            m_Type=_NullEvent_;
            m_Source=0;
            m_Destiny=0;
            m_Channel=0;
            m_Message=0;
   #endif
}


//*************************************************************************
//:
//  f: PRZEvent( przEVENTTYPE type, 
//               unsigned src    =0, 
//               unsigned dst    =0,
//               unsigned channel=0,
//               const PRZMessage& msg = PRZMessage() );
//
//  d:
//:
//*************************************************************************

PRZEvent :: PRZEvent( przEVENTTYPE type, 
                      unsigned src, 
                      unsigned dst,
                      unsigned channel,
                      PRZMessage* msg)
          : m_Type(type),
            m_Source(src),
            m_Destiny(dst),
            m_Channel(channel),
            m_Message(msg)
{
 
}


//*************************************************************************
//:
//  f: PRZEvent( przEVENTTYPE type, 
//               const PRZMessage& msg );
//
//  d:
//:
//*************************************************************************

PRZEvent :: PRZEvent( przEVENTTYPE type,
                      PRZMessage* msg)
          : m_Type(type),
            m_Source(0),
            m_Destiny(0),
            m_Channel(0),
            m_Message(msg)
{
 
}


//*************************************************************************
//:
//  f: PRZEvent(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

PRZEvent :: PRZEvent(const PRZEvent& event)
          : m_Type(event.m_Type),
            m_Source(event.m_Source),
            m_Destiny(event.m_Destiny),
            m_Channel(event.m_Channel),
            m_Message(event.m_Message)
{

}


//*************************************************************************
//:
//  f: PRZEvent& operator=(const PRZEvent& event);
//
//  d:
//:
//*************************************************************************

PRZEvent& PRZEvent :: operator=(const PRZEvent& event)
{
   m_Type    = event.m_Type;
   m_Source  = event.m_Source;
   m_Destiny = event.m_Destiny;
   m_Channel = event.m_Channel;
   m_Message = event.m_Message;
   
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator==(const PRZEvent& event) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZEvent :: operator==(const PRZEvent& event) const
{
   if( (m_Type==event.m_Type) &&
       (m_Source == event.m_Source) &&
       (m_Destiny == event.m_Destiny) &&
       (m_Channel == event.m_Channel) && 
       (*m_Message == *event.m_Message) )
   {
      return true;
   }

   return false;
}


//*************************************************************************
//:
//  f: PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZEvent :: asString() const
{
   PRZString rc = PRZString(m_Type) + ",Src=" + PRZString(m_Source) +
                  ",Dst=" + PRZString(m_Destiny) + ",Chn=" +
                  PRZString(m_Channel);
   return rc;
}

//*************************************************************************

// fin del fichero
