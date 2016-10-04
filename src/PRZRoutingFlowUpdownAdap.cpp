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
// File: ../ATCSimul/src/PRZRoutingFlowUpdownAdap.cpp
//
// Clase      : PRZRoutingFlowUpdownAdap
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingFlowUpdownAdap.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingFlowUpdownAdap.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingFlowUpdownAdap.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingFlowUpdownAdap,PRZRoutingAdaptativeBurbleH1Flow);


//*************************************************************************
//:
//  f: PRZRoutingFlowUpdownAdap(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingFlowUpdownAdap :: PRZRoutingFlowUpdownAdap( PRZComponent& component)
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

void PRZRoutingFlowUpdownAdap :: postInitialize()
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

Boolean PRZRoutingFlowUpdownAdap :: updateMessageInfo(PRZMessage* msg)
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
//  d: UPDOWN ADAPTIVE with Random Selection Function with exclusion.
//     when there's more than one profitable channels we select (both
//     escape and adaptive) randomly, and in each case a channel can be
//     selected once per cicle (through all the possible cases).
//:
//*************************************************************************

Boolean PRZRoutingFlowUpdownAdap :: createMessageRequest(PRZMessage** pMsg)
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
      
   m_myPosition=getOwnerRouter().getFirstCoordinate();
   
   if(m_RequestState==1)
   {
      PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());
      m_listAdap.removeAllElements();  //Clean up previous petition elements
      m_listDeter.removeAllElements();
      gData.getProfitableChannels(m_listAdap,gData.getTableUpdownPointer(),m_myPosition,destino,true); //gData.getMiss());
      gData.getProfitableChannels(m_listDeter,gData.getTableUpdownAdapPointer(),m_myPosition,destino,true); //gData.getMiss());
      m_totalChannelsDeter=m_listDeter.numberOfElements();
      m_totalChannelsAdap=m_listAdap.numberOfElements();
   }
   //We start again with first state
   m_RequestState++;
   if(m_RequestState> m_totalChannelsDeter+m_totalChannelsAdap) 
   {
      m_RequestState=1;
   }

   
   if( m_totalChannelsDeter==0 && m_totalChannelsAdap==0) 
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
   int mm_RequestState;
   if(m_listAdap.numberOfElements()!=0)
   {
       mm_RequestState=1;//(int)(drand48()*1000)%m_listAdap.numberOfElements()+1;
   }
   else if(m_listDeter.numberOfElements()!=0)
   {
       mm_RequestState=1;//(int)(drand48()*1000)%m_listDeter.numberOfElements()+1;
   }
   else
   {
         PRZString err="Both Adaptive and Escape channels are empty";
         EXIT_PROGRAM(err);    
   }



   if(m_RequestState<=m_totalChannelsAdap)
   {
       m_listAdap.elementAt(mm_RequestState-1,output_port);
       m_listAdap.removeAt(mm_RequestState-1);
       msg->setRoutingPort((przROUTINGTYPE)output_port);
       msg->setChannel(1);
   }
   else
   {
       m_listDeter.elementAt(mm_RequestState-1,output_port);
       m_listDeter.removeAt(mm_RequestState-1);
       msg->setRoutingPort((przROUTINGTYPE)output_port);
       msg->setChannel(2);
   }
   if(output_port==0)
   {
        PRZString err="Null port can't be requested";
        EXIT_PROGRAM(err);       
   }  
   

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

Boolean PRZRoutingFlowUpdownAdap :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
  return true;
}

//*************************************************************************


// fin del fichero
