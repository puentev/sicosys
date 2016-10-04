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
// File: ../ATCSimul/inc/PRZRoutingMuxed.hpp
//
// Description:  fichero de encabezado de la clase "PRZRoutingMuxed"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRoutingMuxed.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRoutingMuxed_HPP__
#define __PRZRoutingMuxed_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;

//************************************************************************

   class PRZRoutingMuxed : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;

   public:
      typedef enum { NIL,
      			MUXED_BUBBLE_ADAP,
               MUXED_BUBBLE_ADAP_FULL,
               MUXED_BUBBLE_ADAP_FULL_REQ_REP
                } PRZRoutingMuxedControl;
                     
      PRZRoutingMuxed(const PRZComponentId& id,
            			 PRZRoutingMuxedControl control, 
                      unsigned inputs);
      virtual ~PRZRoutingMuxed();
      
      
      PRZRoutingMuxedControl getRoutingMuxedControl() const
      { return m_RoutingControl; }

      // Run time information
      DEFINE_RTTI(PRZRoutingMuxed);
      
      short getMissRouting()
      {return m_missRouting;}
      
      void setMissRouting(short a)
      { m_missRouting=a;}
      
      PRZString asString() const;
      
   protected:      
      virtual void buildFlowControl();

   private:
      static PRZRoutingMuxed* newFrom(const PRZTag* tag, PRZComponent* owner);
      PRZRoutingMuxedControl m_RoutingControl;
      short      m_missRouting;

   };

//*************************************************************************

#endif


// fin del fichero
