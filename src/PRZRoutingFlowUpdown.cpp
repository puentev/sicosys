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
// File: ../ATCSimul/src/PRZRoutingFlowUpdown.cpp
//
// Clase      : PRZRoutingFlowUpdown
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingFlowUpdown.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingFlowUpdown.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingFlowUpdown.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingFlowUpdown,PRZRoutingAdaptativeBurbleH1Flow);


//*************************************************************************
//:
//  f: PRZRoutingFlowUpdown(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingFlowUpdown :: PRZRoutingFlowUpdown( PRZComponent& component)
                          : PRZRoutingAdaptativeBurbleH1Flow(component)
{
   
}


//*************************************************************************
//:
//  f: void postInitialize();
//
//  d:
//:
//*************************************************************************

void PRZRoutingFlowUpdown :: postInitialize()
{  
   return;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlowUpdown :: updateMessageInfo(PRZMessage* msg)
{
   if( m_FirstMessage )
   {
      m_FirstMessage = false;  
   }
   
   if( msg->isTail()) m_FirstMessage = true;
     
   return true;
}

//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage& msg);
//
//  d: UPdown message request. Random Selection Function (when we have 
//     more than one profitable channel. The selecion function get once
//     each channel before begin again requesting a path to the crossbar
//:
//*************************************************************************

Boolean PRZRoutingFlowUpdown :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);
   
   PRZMessage* header = m_FlitsRx.element(1);
   PRZPosition pos=header->destiny();

   int destino=pos.getFirstCoordinate();
   
   ProfitableChannels list;
   
   m_myPosition=getOwnerRouter().getFirstCoordinate();
   
   //We start again with the list channels.
   if( m_RequestState==1)
   {
        PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());
        m_list.removeAllElements();  //Clean up previous petition elements
        gData.getProfitableChannels(m_list,gData.getTableUpdownPointer(),m_myPosition,destino,gData.getMiss());
        m_totalChannels=m_list.numberOfElements();
    }
   
   if( m_totalChannels==0 ) 
   { 
      if(destino!=m_myPosition)
      {
         PRZString err="destiny!=actual position";
         EXIT_PROGRAM(err);       
      }
         //IF the table dont contain any valid output port then we are on
         //destination
         msg->setRoutingPort(_LocalNode_);
         msg->setChannel( getCurrentLocalPort() );
         return true;
   }

   short unsigned output_port;
   int mm_RequestState=(int)(drand48()*1000)%m_list.numberOfElements()+1;
   
   if( m_totalChannels<mm_RequestState ) 
   {
         PRZString err="Wrong RequestState";
         EXIT_PROGRAM(err);     
   }
   //We remove the actual element up to empty the m_list
   m_list.elementAt(mm_RequestState-1,output_port);
   m_list.removeAt(mm_RequestState-1);

   if(output_port==0)
   {
        PRZString err="Null port can't be requested";
        EXIT_PROGRAM(err);       
   }  

   msg->setRoutingPort((przROUTINGTYPE)output_port);
   msg->setChannel(1);
   m_RequestState++;
   m_RequestState=(m_RequestState%m_totalChannels);
   if(m_RequestState==0) m_RequestState=1;
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean salidaX, int delta);
//
//  d: 
//
//:
//*************************************************************************

Boolean PRZRoutingFlowUpdown :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
  return true;
}

//*************************************************************************


// fin del fichero
