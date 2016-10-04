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
// File: ../ATCSimul/src/PRZRoutingFlowHamiltonAdap.cpp
//
// Clase      : PRZRoutingFlowHamiltonAdap
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingFlowHamiltonAdap.cpp,v $
//
// $Date: 2001/10/26 10:25:47 $
//
// $Revision: 1.2 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingFlowHamiltonAdap.cpp,v 1.2 2001/10/26 10:25:47 vpuente Exp $";

#include <PRZRoutingFlowHamiltonAdap.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingFlowHamiltonAdap,PRZRoutingAdaptativeBurbleH1Flow);


//*************************************************************************
//:
//  f: PRZRoutingFlowHamiltonAdap(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingFlowHamiltonAdap :: PRZRoutingFlowHamiltonAdap( PRZComponent& component)
                          : PRZRoutingAdaptativeBurbleH1Flow(component),
                            m_BufferArray(0)
{
   
}

//*************************************************************************
//:
//  f:  ~PRZRoutingFlowHamiltonAdap();
//
//  d:
//:
//*************************************************************************

 PRZRoutingFlowHamiltonAdap :: ~PRZRoutingFlowHamiltonAdap()
{
   delete [] m_BufferArray;
}

//*************************************************************************
//:
//  f: void postInitialize();
//
//  d: setup pointers pool to all fifos (to apply BR)
//:
//*************************************************************************

void PRZRoutingFlowHamiltonAdap :: postInitialize()
{  
    PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());
    short unsigned maxChannels= 4;// gData.getMaxPorts();
    m_BufferArray=new PRZFifoMemory*[maxChannels];
    
    for(int i=0; i<maxChannels;i++)
    {
         PRZString Name=PRZString("BUF")+ PRZString(i+1)+PRZString("2");
         //rl componentWithIdLike no rula bien. Lo hago con nombre (es lo mismo).
         // PRZComponentId idBuffer( PRZComponentId::FifoMemory, (przROUTINGTYPE)i, "*",1 );
        PRZComponent* compoBuffer = getOwnerRouter().componentWithName(Name); //componentWithIdLike(idBuffer); 
        m_BufferArray[i]=POINTERCAST(PRZFifoMemory,compoBuffer);
        if( !m_BufferArray[i])
        {
           PRZString err;
           err.sprintf( ERR_PRZRAFLO_002, (char*)getComponent().asString(),Name );
           EXIT_PROGRAM(err);  
        }
        
        //cerr<< getOwnerRouter().asString()<<" "<<m_BufferArray[i]->asString()<<endl;


    }

   return;
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingFlowHamiltonAdap :: updateMessageInfo(PRZMessage* msg)
{
   PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

   //If the final path go out the hamiltonian increase the count
   if(gData.getOutHam(
         getOwnerRouter().getFirstCoordinate(),
         (short unsigned)myDirection,
         (short unsigned)msg->getRoutingPort()
         ))
   {
      msg->setMissRouting(msg->getMissRouting(0)+1,0);
     // cerr<<msg->getMissRouting(0)+1;
   }
   
   msg->incDistance();
   
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

Boolean PRZRoutingFlowHamiltonAdap :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   
   Boolean justHamiltonAdavance=false;
   
   Boolean repeat;
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::REQ);
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

   
   PRZMessage* header = m_FlitsRx.element(1);
   PRZPosition pos=header->destiny();

   int destino=pos.getFirstCoordinate();
   
   ProfitableChannels list_temporal;
   
   PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());

   int howManyTimesYouHaveGetOutHamilton = header->getMissRouting(0);
   
   //If misses =0 is not possible to get out from a hamiltonian cycle
//   if(howManyTimesYouHaveGetOutHamilton>gData.getMiss() || gData.getMiss()==0) 
   if(howManyTimesYouHaveGetOutHamilton>gData.getMiss() || gData.getMiss()==0) 
      justHamiltonAdavance=true;
      
   m_myPosition=getOwnerRouter().getFirstCoordinate();
   
   //We start again with the list channels.
   if( m_RequestState==1)
   {
       m_listAdap.removeAllElements();  
       m_listDeter.removeAllElements();  
       m_listEscape.removeAllElements();  
       gData.getProfitableChannels(m_listAdap,gData.getTableUpdownAdapPointer(),m_myPosition,destino,true);
       gData.getProfitableChannels(m_listDeter,gData.getTableUpdownAdapPointer(),m_myPosition,destino,true);
       gData.getProfitableChannels(m_listEscape,gData.getTableUpdownPointer(),m_myPosition,destino,true);
       
       m_totalChannelsAdap=m_listAdap.numberOfElements();
       m_totalChannelsDeter=m_listDeter.numberOfElements();      
       m_totalChannelsEscape=m_listEscape.numberOfElements();      

   }
   
   m_RequestState++;
   if(m_RequestState> m_totalChannelsAdap + m_totalChannelsDeter +m_totalChannelsEscape)
   { 
      m_RequestState=1;
   }
      
   if( m_totalChannelsAdap+m_totalChannelsDeter==0 ) 
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
      
      //Adaptive Case
      if(m_RequestState<=m_totalChannelsAdap)
      {
          //Son todo rutas minimas. Distribuyo el trafico
          int mm_RequestState=(int)(drand48()*1000)%m_listAdap.numberOfElements()+1;
          m_listAdap.elementAt(mm_RequestState-1,output_port);
          m_listAdap.removeAt(mm_RequestState-1);

          if(output_port==0)
          {
             PRZString err="Null port can't be requested";
             EXIT_PROGRAM(err);       
          }
          msg->setRoutingPort((przROUTINGTYPE)output_port);
          msg->setChannel(1);
          return true;

      } 
      else 
      { 
	 przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

         if(m_RequestState<=m_totalChannelsAdap+m_totalChannelsDeter)
         {
            int mm_RequestState=(int)(drand48()*1000)%m_listDeter.numberOfElements()+1;
            m_listDeter.elementAt(mm_RequestState-1,output_port);
            m_listDeter.removeAt(mm_RequestState-1);
         }
         else
         {
            int mm_RequestState=0; //(int)(drand48()*1000)%m_listDeter.numberOfElements()+1;
            m_listEscape.elementAt(mm_RequestState,output_port);
            m_listEscape.removeAt(mm_RequestState);
	    //Modified for SC2o0o. Cheching bad safe paths
	    //A packet inside a hamiltonian ring never can change to other 
	    //hamiltonian part of subring in the same node. This's occurs when
	    //we have complex like a spanning-tree like safe path. This situation can
	    //put the network in deadlock. Them.... the packet must follow their path
	    if(getComponent().getChannel()==2 && gData.getOutHam(m_myPosition,(short unsigned)myDirection,output_port))
	    {
	         output_port=gData.followHam(m_myPosition,(short unsigned)myDirection);
	    }
         }
                 
          if(output_port==0)
          {
             PRZString err="Null port can't be requested: Tables with wrong format?";
             EXIT_PROGRAM(err);       
          }
          
         
                
          if(!gData.isNeddedBubble(m_myPosition,(short unsigned)myDirection,output_port) )
          {
             msg->setRoutingPort((przROUTINGTYPE)output_port);
             msg->setChannel(2);
             return true;
          }
          else
         {
            if(m_BufferArray[gData.getBubble(m_myPosition,output_port)-1]->bubbleReady())
            {
               msg->setRoutingPort((przROUTINGTYPE)output_port);
               msg->setChannel(2);
               return true;
            }
            else
            {
               if(getComponent().getChannel()==2 && gData.getOutHam(m_myPosition,(short unsigned)myDirection,output_port))
               {
                  msg->setRoutingPort((przROUTINGTYPE)gData.followHam(m_myPosition,(short unsigned)myDirection));
                  msg->setChannel(2);
                  return true;
               }
               
               getMessagePool().release(msg);
               return false;
            }
         }
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

Boolean PRZRoutingFlowHamiltonAdap :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
  return true;
}

//*************************************************************************


// fin del fichero
