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
// File: ../ATCSimul/inc/PRZRouting.hpp
//
// Description:  fichero de encabezado de la clase "PRZRouting"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZRouting.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZRouting_HPP__
#define __PRZRouting_HPP__

//************************************************************************
   
   #include <PRZRunnableComponent.hpp>

//************************************************************************

   class PRZRouterBuilder;

//************************************************************************

   class PRZRouting : public PRZRunnableComponent
   {
      typedef PRZRunnableComponent Inhereited;
      friend class PRZRouterBuilder;

   public:
      typedef enum { NIL, 
                     DOR_BURBLE,
                     DOR_BURBLE_H1,
                     DOR_BURBLE_H1_REQ_REPLY,
                     DOR_BURBLE_OB, 
                     BURBLE_3D,
                     DOR_CV,
                     DOR_CV2,
                     DOR_CV_3D,
                     BURBLE_ADAPT,
                     BURBLE_ADAPT_H1,
                     BURBLE_ADAPT_H1_DN,
                     BURBLE_ADAPT_MR,
                     BURBLE_ADAPT_MR_RAND,
                     BURBLE_ADAPT_MR_RAND_REQ_REPLY,
                     BURBLE_ADAPT_MR_MAXCREDITS_REQ_REPLY,
                     BURBLE_ADAPT_MR_XY_REQ_REPLY,
                     BURBLE_ADAPT_MR_OB,
                     BURBLE_ADAPT_MR_3D,
                     BURBLE_ADAPT_1R,
                     BURBLE_ADAPT_1R_3D,
                     BURBLE_ADAPT_3CV,
                     BURBLE_ADAPT_3D,
                     BURBLE_ADAPT_3D_H1,
                     BURBLE_ADAPT_H1_REQUEST_REPLY,
                     BURBLE_ADAPT_H1_REQUEST_REPLY_FULL,
                     CV_ADAPT,
                     CV3D_ADAPT,
                     CV_ADAPT_1REQUEST,
                     CV3D_ADAPT_1REQUEST,
		               CV_OAC ,
                     //Irregular Network support (table routing based)
                     UPDOWN,
                     UPDOWNADAP,
                     HAMILTON,
                     HAMILTONADAP
                     } 
                     PRZRoutingControl;
                     
      PRZRouting(const PRZComponentId& id, PRZRoutingControl control);
      virtual ~PRZRouting();
      
      virtual PRZString asString() const;
      
      PRZRoutingControl getRoutingControl() const
      { return m_RoutingControl; }

      // Run time information
      DEFINE_RTTI(PRZRouting);
      
      short getMissRouting()
      {return m_missRouting;}
      
      void setMissRouting(short a)
      { m_missRouting=a;}
      
      //Table routing based functions goes here


   protected:      
      virtual void buildFlowControl();

   private:
      static PRZRouting* newFrom(const PRZTag* tag, PRZComponent* owner);
      PRZRoutingControl m_RoutingControl;
      short      m_missRouting;
      

   };

//*************************************************************************

#endif


// fin del fichero
