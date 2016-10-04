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
// File: ../ATCSimul/src/PRZRoutingFlowHamilton.cpp
//
// Clase      : PRZRoutingFlowHamilton
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingFlowHamilton.cpp,v $
//
// $Date: 2001/10/26 10:25:36 $
//
// $Revision: 1.2 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingFlowHamilton.cpp,v 1.2 2001/10/26 10:25:36 vpuente Exp $";

#include <PRZRoutingFlowHamilton.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingFlowHamilton,PRZRoutingAdaptativeBurbleH1Flow);


//*************************************************************************
//:
//  f: PRZRoutingFlowHamilton(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingFlowHamilton :: PRZRoutingFlowHamilton( PRZComponent& component)
                          : PRZRoutingAdaptativeBurbleH1Flow(component),
                            m_BufferArray(0),
                            m_cycle(0)
{
   
}

//*************************************************************************
//:
//  f:  ~PRZRoutingFlowHamilton();
//
//  d:
//:
//*************************************************************************

 PRZRoutingFlowHamilton :: ~PRZRoutingFlowHamilton()
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

void PRZRoutingFlowHamilton :: postInitialize()
{  
    PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());
    short unsigned maxChannels= 4;// gData.getMaxPorts();
    m_BufferArray=new PRZFifoMemory*[maxChannels];
    
    for(int i=0; i<maxChannels;i++)
    {
         PRZString Name=PRZString("BUF")+ PRZString(i+1)+PRZString("1");
         //rl componentWithIdLike no rula bien. Lo hago con nombre (es lo mismo).
         // PRZComponentId idBuffer( PRZComponentId::FifoMemory, (przROUTINGTYPE)i, "*",1 );
        PRZComponent* compoBuffer = getOwnerRouter().componentWithName(Name); //componentWithIdLike(idBuffer); 
        m_BufferArray[i]=POINTERCAST(PRZFifoMemory,compoBuffer);
    //if((compoBuffer->getRoutingDirection())!=i+1)
    //    cerr<<compoBuffer->getRoutingDirection()<<"Error"<<i+1<<endl;
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

Boolean PRZRoutingFlowHamilton :: updateMessageInfo(PRZMessage* msg)
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
   }
   
    msg->incDistance();
    
   if( m_FirstMessage )
   {
      m_FirstMessage = false;  
   }
   
   if( msg->isTail()) 
   {
      m_FirstMessage = true;
   }
     
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

Boolean PRZRoutingFlowHamilton :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }

   short unsigned output_port;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
      
   Boolean justHamiltonAdavance=false;
   
   PRZMessage* msg=*pMsg;
 
   msg->setType(PRZMessage::REQ);

   
   PRZMessage* header = m_FlitsRx.element(1);
   PRZPosition pos=header->destiny();

   int destino=pos.getFirstCoordinate();
      
   PRZGlobalData& gData = ((PRZSimulation*)(getComponent().getSimulation()))->globalData((PRZNetwork*)getOwnerRouter().getOwner());

   int howManyTimesYouHaveGetOutHamilton = header->getMissRouting(0);
   
   //If gDataMiss==0 only through cycle if not livelock (livelock prone scheme)
   if(howManyTimesYouHaveGetOutHamilton>=gData.getMiss() && gData.getMiss()!=0) 
      justHamiltonAdavance=true;
      
   m_myPosition=getOwnerRouter().getFirstCoordinate();
   
      if( m_RequestState==1)
      {
          m_list.removeAllElements(); 
          m_listEscape.removeAllElements();
          gData.getProfitableChannels(m_list,gData.getTableUpdownAdapPointer(),m_myPosition,destino,true);
          gData.getProfitableChannels(m_listEscape,gData.getTableUpdownPointer(),m_myPosition,destino,true);
          m_totalChannels=m_list.numberOfElements();     
          m_totalChannelsEscape=m_listEscape.numberOfElements();
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
      //Short paths selection
      //Always first with these paths
      if(m_RequestState<=m_totalChannels)
      {
          int mm_RequestState=(int)(drand48()*1000)%m_list.numberOfElements();
          m_list.elementAt(mm_RequestState,output_port);
          m_list.removeAt(mm_RequestState);
      }
      else
      {
         //Safe path selection. Just Hamiltonian paths
          int mm_RequestState=0; //(int)(drand48()*1000)%m_list.numberOfElements();
          m_listEscape.elementAt(mm_RequestState,output_port);
          m_listEscape.removeAt(mm_RequestState);
	  //If we are over a hamiltonian path, the safe path is always
	  //follow the PHCycle
	  if(gData.getOutHam(m_myPosition,(short unsigned)myDirection,output_port))
          {
            output_port=gData.followHam(m_myPosition,(short unsigned)myDirection);
          }         
      }
      
       m_RequestState++;
      if(m_RequestState>m_totalChannels+m_totalChannelsEscape)
      { 
            m_RequestState=1;
      } 
       
          if(output_port==0)
          {
             PRZString err="Null port can't be requested";
             EXIT_PROGRAM(err);       
          }
      
          //We don't have any more chance to use a path diferent to hamiltonian
	  //Livelock free condition.
          if((justHamiltonAdavance )&& gData.getOutHam(m_myPosition,(short unsigned)myDirection,output_port))
          {
             output_port=gData.followHam(m_myPosition,(short unsigned)myDirection);
          }
         
                
          if(!gData.isNeddedBubble(m_myPosition,(short unsigned)myDirection,output_port) )
          {
             msg->setRoutingPort((przROUTINGTYPE)output_port);
             msg->setChannel(1);
             return true;
          }
          else
          {
            if(m_BufferArray[gData.getBubble(m_myPosition,output_port)-1]->bubbleReady())
            {
              #ifndef NO_TRAZA
               uTIME delayTime = getOwnerRouter().getCurrentTime() ;
               PRZString texto = getComponent().asString() + " Bubble Accepted";
               texto += PRZString(delayTime) + " # " + header->asString();
               PRZWRITE2LOG( texto );
              #endif
               msg->setRoutingPort((przROUTINGTYPE)output_port);
               msg->setChannel(1);
               return true;
            }
            else
            {
               #ifndef NO_TRAZA
                uTIME delayTime = getOwnerRouter().getCurrentTime() ;
                PRZString texto = getComponent().asString() + " Bubble Rejected";
                texto += PRZString(delayTime) + " # " + header->asString();
                PRZWRITE2LOG( texto );
               #endif
             	getMessagePool().release(msg);
             	return false;

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

Boolean PRZRoutingFlowHamilton :: controlAlgoritm( Boolean salidaX,
                                                             int delta )
{
  return true;
}

//*************************************************************************


// fin del fichero
