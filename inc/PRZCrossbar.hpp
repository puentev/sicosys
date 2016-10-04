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
// File: ../ATCSimul/inc/PRZCrossbar.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbar"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbar.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbar_HPP__
#define __PRZCrossbar_HPP__

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

   class PRZCrossbar : public PRZRunnableComponent
   {
      friend class PRZRouterBuilder;
      
      //*********************************************** Begin nested class
      //************************************************* End nested class

      typedef PRZRunnableComponent Inhereited;
      typedef enum { WH,
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
                     CT_MULTIPLE_REQ_SIMPLE_3D} CrossbarType;

   public:
      PRZCrossbar( const PRZComponentId& id, 
                   unsigned inputs,
                   unsigned outputs );
                   
      virtual ~PRZCrossbar();

      virtual PRZString asString() const;      
      
      CrossbarType getCrossbarType()
      { return m_CrossbarType; }
      
      unsigned getInputWith(przROUTINGTYPE type, unsigned channel);
      unsigned getOutputWith(przROUTINGTYPE type, unsigned channel);
      przROUTINGTYPE getTypeForOutput(unsigned output, unsigned channel);
      Boolean  isLocalNodeOutput(unsigned oPort);
      
      unsigned getOutputMux() const;      
      void setOutputMux(unsigned mux);

      unsigned getThrSf() const;
      void     setThrSf(unsigned thrSF);
      
      unsigned numberOfLocalPorts() const;
      void setNumberOfLocalPorts(unsigned number);

                  
      // Run time information
      DEFINE_RTTI(PRZCrossbar);
   
   protected:      
      virtual void buildFlowControl();
      virtual void buildOutputInterfaz(unsigned number);
      void setCrossbarType(CrossbarType type)
      { m_CrossbarType = type; }

   private:
      static PRZCrossbar* newFrom( const PRZTag* tag, 
                                   PRZComponent* owner,
                                   INDEX& index);
      CrossbarType m_CrossbarType;
   };

//*************************************************************************

#endif


// fin del fichero
