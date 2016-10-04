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
// File: ../ATCSimul/src/PRZComponentId.cpp
//
// Clase      : PRZComponentId
//
// Hereda de  : None
//
// Notes:  La clase PRZComponentId se utiliza para identificar un 
//              PRZComponent.
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZComponentId.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#include <PRZComponentId.hpp>

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

#ifndef __PRZBase_HPP__
#include <PRZBase.hpp>
#endif

IMPLEMENT_RTTI_BASE(PRZComponentId);

//*************************************************************************
//:
//  f: inline ComponentClass componentClass() const;
//  d:
//
//  f: inline void setComponentType(przROUTINGTYPE type);
//  d:
//
//  f: inline przROUTINGTYPE componentType() const;
//  d:
//
//  f: inline void setName(const PRZString& aName);
//  d:
//
//  f: inline PRZString name() const;
//  d:
//
//  f: inline unsigned channel() const;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZComponentId( ComponentClass clase, 
//                     ComponentType tipo, 
//                     const PRZString& name,
//                     unsigned channel );
//
//  d:
//:
//*************************************************************************

PRZComponentId :: PRZComponentId( ComponentClass clase, 
                                  przROUTINGTYPE tipo,
                                  const PRZString& name,
                                  unsigned channel )
                : m_Class(clase),
                  m_Type(tipo),
                  m_Name(name),
                  m_Channel(channel)
{

}


//*************************************************************************
//:
//  f: PRZComponentId(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponentId :: PRZComponentId(const PRZComponentId& id)
                : m_Class(id.componentClass()),
                  m_Type(id.componentType()),
                  m_Name(id.name()),
                  m_Channel(id.channel())
{

}


//*************************************************************************
//:
//  f: PRZComponentId(const PRZTag& tag);
//
//  d:
//:
//*************************************************************************

PRZComponentId :: PRZComponentId(const PRZTag& tag)
                : m_Class(UnknowComponent),
                  m_Type(_Unknow_),
                  m_Channel(1)
{
   // Se construye el identificador a partir del tag Sgml.
   m_Name = tag.tagId();
   
   PRZString channel, type;
   
   if( tag.getAttributeValueWithName(PRZ_TAG_CHANNEL, channel) )
   {
      m_Channel = channel.asInteger();
   }
   
   if( tag.getAttributeValueWithName(PRZ_TAG_TYPE,type) )
   {
      m_Type = _string2routingType(type);
   }

   PRZString component = tag.tagName();
   
   if( component == PRZ_TAG_ROUTER )
      m_Class = PRZComponentId::Router;

   else if( component == PRZ_TAG_BUFFER )
      m_Class = PRZComponentId::FifoMemory;
   
   else if( component == PRZ_TAG_MPBUFFER )
      m_Class = PRZComponentId::MultiportFifo;
   
   else if( component == PRZ_TAG_FIFOMUXED  )
      m_Class = PRZComponentId::FifoMuxed;
   
   else if( component == PRZ_TAG_ROUTING )
      m_Class = PRZComponentId::Routing;
   
   else if( component == PRZ_TAG_ROUTING_MUXED )
      m_Class = PRZComponentId::RoutingMuxed;
   
   else if( component == PRZ_TAG_CROSSBAR )
      m_Class = PRZComponentId::Crossbar;

   else if( component == PRZ_TAG_MUXCV )
      m_Class = PRZComponentId::MultiplexorCV;

   else if( component == PRZ_TAG_CONNECTION )
      m_Class = PRZComponentId::Connection;

   else if( component == PRZ_TAG_INJECTOR )
      m_Class = PRZComponentId::Injector;

   else if( component == PRZ_TAG_CONSUMER )
      m_Class = PRZComponentId::Consumer;

   else if( component == PRZ_TAG_TORUSNET )
      m_Class = PRZComponentId::Network;

   else if( component == PRZ_TAG_SIMULATION )
      m_Class = PRZComponentId::Simulation;

}


//*************************************************************************
//:
//  f: PRZComponentId& operator=(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZComponentId& PRZComponentId :: operator=(const PRZComponentId& id)
{
   m_Class  = id.componentClass();
   m_Type   = id.componentType();
   m_Name   = id.name();
   m_Channel= id.channel();
   
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator==(const PRZComponentId& id) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZComponentId :: operator==(const PRZComponentId& id) const
{
   if( (m_Class==id.componentClass()) &&
       (m_Type==id.componentType())   &&
       (m_Name==id.name())            &&
       (m_Channel==id.channel()) )
   {
      return true;
   }
   return false;
}


//*************************************************************************
//:
//  f: Boolean isLike(const PRZComponentId& id) const;
//
//  d:
//:
//*************************************************************************

Boolean PRZComponentId::isLike(const PRZComponentId& id) const
{
   if( (m_Class==id.componentClass()) &&
       (id.componentType()==_Unknow_ || m_Type==id.componentType() ) &&
       (m_Name.isLike(id.name())) )
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

PRZString PRZComponentId::asString() const
{
   return name();
}

//*************************************************************************
//:
//  f: friend ostream& operator<<(ostream& os, const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

ostream& operator<<(ostream& os, const PRZComponentId& id)
{
   os << "Component id = " << id.name() << endl;
   return os;
}


// fin del fichero
