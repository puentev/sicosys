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
// File: ../ATCSimul/inc/PRZComponentId.hpp
//
// Description:  fichero de encabezado de la clase "PRZComponentId"
//
// Notes:   La clase PRZComponentId se utiliza para identificar un 
//               PRZComponent.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZComponentId.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZComponentId_HPP__
#define __PRZComponentId_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

   #ifndef __PRZTag_HPP__
   #include <PRZTag.hpp>
   #endif

//*************************************************************************

   class PRZComponentId
   {
   public:
      typedef enum { UnknowComponent,
                     Port,
                     Crossbar,
                     FifoMemory,
                     MultiportFifo,
                     FifoMuxed,
                     Connection,
                     RunComponent,
                     InputInterfaz,
                     OutputInterfaz,
                     Injector,
                     Consumer,
                     Router,
                     Routing,
                     RoutingMuxed,
                     Test,
                     Network,
                     MultiplexorCV,
                     ViewInterfaz,
                     Simulation
                   } ComponentClass;

      PRZComponentId(const PRZComponentId& id);
      PRZComponentId( ComponentClass clase, 
                      przROUTINGTYPE tipo=_Unknow_,
                      const PRZString& name = "",
                      unsigned channel =1);
      PRZComponentId(const PRZTag& tag);
      
      PRZComponentId& operator=(const PRZComponentId& id);
      Boolean operator==(const PRZComponentId& id) const;
      Boolean isLike(const PRZComponentId& id) const;
      PRZString asString() const;

      ComponentClass componentClass() const
      { return m_Class; }

      void setComponentType(przROUTINGTYPE type)
      { m_Type = type; }

      przROUTINGTYPE componentType() const
      { return m_Type; }

      void setName(const PRZString& aName)
      { m_Name = aName; }

      PRZString name() const
      { return m_Name; }
      
      void setChannel(unsigned channel)
      { m_Channel = channel; }

      unsigned channel() const
      { return m_Channel; }

      // Run time information
      DEFINE_RTTI(PRZComponentId);

      friend ostream& operator<<(ostream& os, const PRZComponentId& id);

   private:
      ComponentClass m_Class;
      przROUTINGTYPE m_Type;
      PRZString      m_Name;
      unsigned       m_Channel;
   };

//*************************************************************************

   #define INPUT_PORT   PRZComponentId( PRZComponentId::Port,   \
                                        _Unknow_,               \
                                        "IP", 4 )

   #define OUTPUT_PORT  PRZComponentId( PRZComponentId::Port,  \
                                        _Unknow_,              \
                                        "OP", 4 )

   #define INJECTOR_ID   PRZComponentId( PRZComponentId::Injector, \
                                         _Unknow_,                 \
                                         "INJ" )

   #define CONSUMER_ID   PRZComponentId( PRZComponentId::Consumer,  \
                                         _Unknow_,                  \
                                         "CSM" )

   #define ROUTER_ID   PRZComponentId( PRZComponentId::Router,  \
                                       _Unknow_,                \
                                       "ROU" )

   #define BUFFER_ID   PRZComponentId( PRZComponentId::FifoMemory, \
                                       _Unknow_,                   \
                                       "BUF" )
   #define MPBUFFER_ID   PRZComponentId( PRZComponentId::MultiportFifo, \
                                       _Unknow_,                   \
                                       "MPBUF" )

   #define CROSSBAR_ID PRZComponentId( PRZComponentId::Crossbar,  \
                                       _Unknow_,                  \
                                       "CRB" )

   #define ROUTING_ID PRZComponentId( PRZComponentId::Routing,  \
                                      _Unknow_,                 \
                                      "RTNG" )

   #define TEST_ID PRZComponentId( PRZComponentId::Test,  \
                                   _Unknow_,              \
                                   "TEST" )

   #define NETWORK_ID PRZComponentId( PRZComponentId::Network,  \
                                      _Unknow_,              \
                                      "NETWORK" )
                                         
//*************************************************************************

#endif


// fin del fichero
