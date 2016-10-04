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
// File: ../ATCSimul/inc/SFSSimpleRouter.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbar"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: SFSSimpleRouter.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __SFSSimpleRouter_HPP__
#define __SFSSimpleRouter_HPP__

//************************************************************************

   #include <PRZRunnableComponent.hpp>

   #ifndef __PRZFlow_HPP__
   #include <PRZFlow.hpp>
   #endif

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif

//************************************************************************

   class PRZRouterBuilder;

//************************************************************************

   class SFSSimpleRouter : public PRZRunnableComponent
   {
      friend class PRZRouterBuilder;
      
      //*********************************************** Begin nested class
      //************************************************* End nested class

      typedef PRZRunnableComponent Inhereited;
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
		               CV_OAC } 
                     SFSRoutingControl;
      typedef enum { BC_NIL, BC_WH, BC_CT, BC_VCT, BC_SF } SFSBufferControl;
/*      typedef enum { WH,
                     WH_OB, 
                     CT, 
                     CT_OB,
                     CT_MUX,
                     MUX_ADAPT,
		               MUX_ADAPT_SF,
		               WH_OAC,
                     MUX_ADAPT_OB,
                     WH_MULTIPLE_REQ,
                     WH_MULTIPLE_REQ_OB,
                     CT_MULTIPLE_REQ,
                     CT_MULTIPLE_REQ_OB,
                     CT_MULTIPLE_REQ_OB_RE,
                     CT_MULTIPLE_REQ_OB_RE_VIRTUAL,
                     WH_MULTIPLE_REQ_SIMPLE,
                     CT_MULTIPLE_REQ_SIMPLE,
                     CT_MULTIPLE_REQ_SIMPLE_3D} CrossbarType; */

   public:
      SFSSimpleRouter( const PRZComponentId& id, 
                   unsigned inputs,
                   unsigned outputs,
		             SFSRoutingControl routingControl,
			          SFSBufferControl bufferControl);
                   
      virtual ~SFSSimpleRouter();
      virtual void terminate();

      virtual PRZString asString() const;      
      
/*      CrossbarType getCrossbarType()
      { return m_CrossbarType; } */
      unsigned getBufferSize() const;
      unsigned getInputWith(przROUTINGTYPE type, unsigned channel=1);
      unsigned getOutputWith(przROUTINGTYPE type, unsigned channel=1);
      przROUTINGTYPE getTypeForOutput(unsigned output);
      Boolean  isLocalNodeOutput(unsigned oPort);

      SFSRoutingControl getRoutingControl() const
      { return m_RoutingControl; }
      SFSBufferControl getBufferControl() const
      { return m_BufferControl; }

      
      unsigned numberOfLocalPorts() const;
      void setNumberOfLocalPorts(unsigned number);

      // Run time information
      DEFINE_RTTI(SFSSimpleRouter);
   
   protected:      
      virtual void buildFlowControl();
//      virtual void buildOutputInterfaz(unsigned number);
/*      void setCrossbarType(CrossbarType type)
      { m_CrossbarType = type; }*/

   private:
      static SFSSimpleRouter* newFrom( const PRZTag* tag, 
                                   PRZComponent* owner,
                                   INDEX& index);
/*      CrossbarType m_CrossbarType; */
        SFSRoutingControl m_RoutingControl;
	     SFSBufferControl m_BufferControl;
   };

//*************************************************************************

#endif


// fin del fichero
