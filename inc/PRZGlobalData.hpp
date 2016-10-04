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
// File: ../ATCSimul/inc/PRZGlobalData.hpp
//
// Description:  fichero de encabezado de la clase "PRZGlobalData"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZGlobalData.hpp,v $
//
// $Date: 2001/10/30 11:11:15 $
//
// $Revision: 1.2 $
//
//*************************************************************************

#ifndef __PRZGlobalData_HPP__
#define __PRZGlobalData_HPP__

//************************************************************************

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif
   
   #ifndef __PRZMatrix_HPP__
   #include <PRZMatrix.hpp>
   #endif
   
   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif
   
   #ifndef __PRZQueue_HPP__ 
   #include <PRZQueue.hpp>
   #endif 
   
   #ifndef __PRZPair_HPP__
   #include <PRZPair.hpp>
   #endif
      
   #include <fstream.h>

//*************************************************************************

   class PRZTypeChannel
   {
   public:
      PRZTypeChannel() : m_RoutingType(_Unknow_),
                         m_Channel(0)
      { }
      PRZTypeChannel(przROUTINGTYPE type, unsigned channel) :
                     m_RoutingType(type),
                     m_Channel(channel)
      { }
      PRZTypeChannel(const PRZTypeChannel& type) :
                     m_RoutingType(type.m_RoutingType),
                     m_Channel(type.m_Channel)
      { }
      PRZTypeChannel& operator=(const PRZTypeChannel& type)
      { 
         m_RoutingType = type.m_RoutingType;
         m_Channel = type.m_Channel; 
         return *this;
      }
      Boolean operator==(const PRZTypeChannel& type)
      { 
         return ((m_RoutingType==type.m_RoutingType) &&
                 (m_Channel==type.m_Channel)) ? true : false; 
      }
      przROUTINGTYPE type() const
      { return m_RoutingType; }
   private:
      przROUTINGTYPE m_RoutingType;
      unsigned       m_Channel;
   };
   
//*************************************************************************
   typedef PRZPair<short unsigned,short unsigned> suPAIR;
   typedef PRZArray<przROUTINGTYPE> PRZTypeArray;   
   typedef PRZArray<PRZTypeChannel> PRZRoutingTypeArray;
   typedef suPAIR                   PRZTableTopol; 
   typedef short unsigned           PRZTableRouting;
   typedef PRZQueue<short unsigned> ProfitableChannels;
   typedef PRZQueue<suPAIR>           PRZHamiltonTable;  
//*************************************************************************

   class PRZGlobalData
   {      
   public:   
      PRZGlobalData();
      ~PRZGlobalData();

      //************************************************* Initialization **
      
      Boolean routerIsInitializated() const
      { return m_rInitializated; }
      
      void routerSetInitializated()
      { m_rInitializated=true; }
      
      Boolean simpleRouterIsInitializated() const
      { return m_sInitializated; }

      void simpleRouterSetInitializated()
      { m_sInitializated=true; }

      Boolean crossbarIsInitializated() const
      { return m_cInitializated; }

      void crossbarSetInitializated()
      { m_cInitializated=true; }
      
      //****************************************************** PRZRouter **

      PRZTypeArray* routerOutputsType() const
      { return m_rOutputsType; }
      
      void routerSetOutputsType(PRZTypeArray* type)
      { m_rOutputsType=type; } 
      
      PRZTypeArray* routerInputsType() const
      { return m_rInputsType; }

      void routerSetInputsType(PRZTypeArray* type)
      { m_rInputsType=type; } 

      unsigned routerBufferSize() const
      { return m_rBufferSize; }
      
      void routerSetBufferSize(unsigned size)
      { m_rBufferSize=size; }

      PRZString routerBufferControl() const
      { return m_rBufferControl; }
      
      void routerSetBufferControl(const PRZString& control)
      { m_rBufferControl=control; }

      PRZString routerRoutingControl() const
      { return m_rRoutingControl; }
      
      void routerSetRoutingControl(const PRZString& control)
      { m_rRoutingControl=control; }
      
      //*****************************************************Irregular Networks
      
      void initializeTableUpdown(PRZString filename, PRZTableRouting** table);

      void initializeHamiltonCycle(PRZString filename);
      
      void initializeTopol(PRZString filename);
      
      Boolean addHamiltonChannels(ProfitableChannels& list,unsigned currentNode,unsigned currentChannel) const;

      void getProfitableChannels(ProfitableChannels& list,PRZTableRouting* table, int source,int dest, Boolean onlyMinimal=true);
      
      Boolean isNeddedBubble(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const;
      
      short unsigned getBubble(unsigned currentNode, short unsigned nextChannel) const;
      
      Boolean  getOutHam(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const;

      short unsigned followHam(unsigned currentNode, short unsigned currentChannel) const;
      
      void initializeHamiltonRoutingTable(PRZTableRouting** table);
	    
      PRZString getTableFile() const
      { return m_tableFile;}
      
      void setTableFile(const PRZString& file)
      {  m_tableFile=file;}
      
      PRZTableRouting* getTableUpdownPointer() const
      { return m_tableUpdown;}

      PRZTableRouting* getTableUpdownAdapPointer() const
      { return m_tableUpdownAdap;}
                  
      void setTableUpdownPointer(PRZTableRouting* temp)
      { m_tableUpdown=temp;}

      void setTableUpdownAdapPointer(PRZTableRouting* temp)
      { m_tableUpdownAdap=temp;}

                        
      short unsigned getMaxPorts() const
      { return m_maxPorts;}
      
      void setMaxPorts(short unsigned ports)
      { m_maxPorts=ports;}
      
      void setTopFileName(PRZString name)
      { m_nameFileTopol=name;}
      
      PRZString getTopFileName() const
      { return m_nameFileTopol;}
      
      PRZString getStringId() const
      { return m_identForResults;}
      
      void setStringId(PRZString name)
      { m_identForResults=name;}
         

      //**************************************************** SFSSimpleRouter **
      
      PRZRoutingTypeArray* simpleRouterOutputsType() const
      { return m_sOutputsType; }
      
      void simpleRouterSetOutputsType(PRZRoutingTypeArray* type)
      { m_sOutputsType=type; } 
      
      PRZRoutingTypeArray* simpleRouterInputsType() const
      { return m_sInputsType; }

      void simpleRouterSetInputsType(PRZRoutingTypeArray* type)
      { m_sInputsType=type; } 

      unsigned simpleRouterNumberOfLocalPorts() const
      { return m_sLocalPorts; }

      void simpleRouterSetNumberOfLocalPorts(unsigned number)
      { m_sLocalPorts = number; }

      //**************************************************** PRZCrossbar **
      
      PRZRoutingTypeArray* crossbarOutputsType() const
      { return m_cOutputsType; }
      
      void crossbarSetOutputsType(PRZRoutingTypeArray* type)
      { m_cOutputsType=type; } 
      
      PRZRoutingTypeArray* crossbarInputsType() const
      { return m_cInputsType; }

      void crossbarSetInputsType(PRZRoutingTypeArray* type)
      { m_cInputsType=type; } 

      unsigned crossbarNumberOfLocalPorts() const
      { return m_cLocalPorts; }

      void crossbarSetNumberOfLocalPorts(unsigned number)
      { m_cLocalPorts = number; }

      unsigned crossbarOutputMux() const
      { return m_cOutputMux; }

      void crossbarSetOutputMux(unsigned number)
      { m_cOutputMux = number; }
      
      unsigned getThrStarvationFree() const
      { return m_thrSf; }

      void setThrStarvationFree(unsigned number)
      { m_thrSf = number; }
           
      unsigned getNumberOfHeaders() const
      { return m_NumberOfHeaders; }

      void setNumberOfHeaders(unsigned number)
      { m_NumberOfHeaders = number; }
      
      void  notMiss()
      { m_areMissesAllowed=0;}     

      void  Miss(unsigned maxOuts)
      { m_areMissesAllowed=maxOuts;}
      
      unsigned getMiss() const
      { return m_areMissesAllowed;}
      
      // Run time information
      DEFINE_RTTI(PRZGlobalData);

   private:      
      PRZRoutingTypeArray* m_sInputsType;        // SFSSimpleRouter
      PRZRoutingTypeArray* m_sOutputsType;
      unsigned             m_sLocalPorts;
      unsigned             m_sOutputMux;
      Boolean              m_sInitializated;

      PRZRoutingTypeArray* m_cInputsType;        // PRZCrossbar
      PRZRoutingTypeArray* m_cOutputsType;
      unsigned             m_cLocalPorts;
      unsigned             m_cOutputMux;
      unsigned             m_thrSf;
      Boolean              m_cInitializated;
      
      PRZTypeArray*        m_rInputsType;        // PRZRouter
      PRZTypeArray*        m_rOutputsType;      
      unsigned             m_rBufferSize;
      PRZString            m_rBufferControl;
      PRZString            m_rRoutingControl;
      PRZString            m_tableFile;
      Boolean              m_areMissesAllowed;
      Boolean              m_rInitializated;

      unsigned             m_NumberOfHeaders;    // Others
      
                                                 //Irregular Network (table)
       short unsigned     m_maxPorts;
       PRZTableRouting*   m_tableUpdown;
       PRZTableRouting*   m_tableUpdownAdap;
       unsigned           m_nodes;
       PRZHamiltonTable*  m_hamiltonTable;
       PRZTableTopol*     m_tableTopol; 
       PRZString          m_identForResults;
       PRZString          m_nameFileTopol;

   };


//*************************************************************************

#endif


// fin del fichero
