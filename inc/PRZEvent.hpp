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
// File: ../ATCSimul/inc/PRZEvent.hpp
//
// Description:  fichero de definicion de la clase "PRZEvent"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZEvent.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZEvent_HPP__
#define __PRZEvent_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

//*************************************************************************

   typedef enum {
                  _NullEvent_,
                  _FirstElement_,
                  _CrossbarRequest_,
                  _CrossbarPass_,
                  _RoutingHeader_,
                  _Multiplexor_,
                  _BufferData_,
                  Routing_CheckCrossbarResponse,
                  Routing_HeaderTx,
                  Routing_InputStopped,
                  Routing_SendNextRequest,
                  Free_routing,
                  Again_createMessageRequest,
		  _antiS_bubbles_
                } 
                przEVENTTYPE;

//*************************************************************************

   class PRZEvent 
   {
   public:
      PRZEvent();
      PRZEvent( przEVENTTYPE type, 
                unsigned src=0, 
                unsigned dst=0,
                unsigned chn=0,
                PRZMessage* msg=0 );
      PRZEvent(przEVENTTYPE type, PRZMessage* msg);
      PRZEvent(const PRZEvent& event);

      PRZEvent& operator=(const PRZEvent& event);
      Boolean   operator==(const PRZEvent& event) const;
      PRZString asString() const;
            
      unsigned  source() const
      { return m_Source; }
      
      unsigned destiny() const
      { return m_Destiny; }
      
      unsigned channel() const
      { return m_Channel; }
      
      PRZMessage * message() const
      { return m_Message;}
            
      przEVENTTYPE type() const
      { return m_Type; }
      
      DEFINE_RTTI(PRZEvent);

   private:
      przEVENTTYPE m_Type;
      unsigned     m_Source;
      unsigned     m_Destiny;
      unsigned     m_Channel;
      PRZMessage*  m_Message;
   };
   
//*************************************************************************   


#endif


//*************************************************************************

// fin del fichero
