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
// File: ../ATCSimul/src/PRZMultiportFifoNoFragFlow.cpp
//
// Clase      : PRZMultiportFifoNoFragFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiportFifoNoFragFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiportFifoNoFragFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZMultiportFifoNoFragFlow.hpp>

#ifndef __PRZMultiportFifo_HPP__
#include <PRZMultiportFifo.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiportFifoNoFragFlow,PRZMultiportFifoFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize() const;
//  d:
//
//
//  f: inline unsigned bufferHoles() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZMultiportFifoNoFragFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZMultiportFifoNoFragFlow :: PRZMultiportFifoNoFragFlow(PRZComponent& component)
                   : PRZMultiportFifoFlow(component),
                     m_registers(0),
                     m_lastIsShort(false),
                     m_isShort(0),
                     m_shortcap(0),
                     m_shortMemory(0)
{
   
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifoNoFragFlow :: initialize()
{
   m_shortcap      = ((PRZMultiportFifo&)getComponent()).getBufferSizeShort(); 
   m_Size          = ((PRZMultiportFifo&)getComponent()).getBufferSize(); 
   m_inputs        = ((PRZMultiportFifo&)getComponent()).numberOfInputs();
   m_memory        = new QueueFlits[m_inputs];  
   m_registers     = new QueueFlits[m_inputs];
   m_shortMemory   = new QueueFlits[m_inputs];
   m_isShort       = new unsigned[m_inputs];
   for(int j=0;j<m_inputs;j++)
   {
      m_isShort[j]=false;
   }
   Inhereited::initialize();
}

PRZMultiportFifoNoFragFlow :: ~PRZMultiportFifoNoFragFlow()
{
   delete[] m_memory;
   delete[] m_registers;
   delete[] m_shortMemory;
   delete[] m_isShort;
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************

Boolean PRZMultiportFifoNoFragFlow :: inputReading()
{ 

   PRZMessage* msg;
   m_lpack= ((PRZSimulation*)getComponent().getSimulation())->getPacketLength((PRZNetwork*)getOwnerRouter().getOwner());  

   
   for(int i=1;i<m_inputs+1;i++)
   { 
      if( inputInterfaz(i)->isReadyActive() )
      {
         inputInterfaz(i)->getData(&msg);
         if(msg->isHeader())
         {
            if(m_lpack > msg->packetSize())
            {
               m_isShort[i-1]=true; 
            }
            else
            {
               m_isShort[i-1]=false;
            }
         }
         m_registers[i-1].enqueue(msg);
      
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Rx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles());
         PRZWRITE2LOG( texto );
         
         PRZWRITE2LOG(getComponent().asString() + " Enqueue:" + PRZString(i)+ " -> " + PRZString(m_memory[i-1].numberOfElements()));
      #endif
      }
   }
   // Control de Stops mas afinado
   for(int index=1;index<m_inputs+1;index++)
   {                                                      //La capacidad de los registros de entrada es independiente del tipo de mensaje
      if( m_registers[index-1].numberOfElements()>0)//|| (m_isShort[index-1] && bufferHolesShort()==0) )
         inputInterfaz(index)->sendStopRightNow();
      else
         inputInterfaz(index)->clearStopRightNow();
   }
   // Cuando un paquete esta saliendo por el puerto de salida no lo puedo dejar adelantar
   // como ocurre en el HW es el que antes tiene que avanzar hacia los bancos de memoria
          
   if(m_payload==0) 
   {    
      for(int j=1;j<m_inputs+1;j++)
      {
         if(m_registers[j-1].numberOfElements()!=0)
         {
            m_registers[j-1].firstElement(msg);
            if(m_lpack > msg->packetSize() && bufferHolesShort()!=0)
            {
               m_registers[j-1].dequeue(msg);
               m_shortMemory[j-1].enqueue(msg); 
            }
            if(m_lpack == msg->packetSize() && bufferHoles() != 0)
            {
               m_registers[j-1].dequeue(msg);
               m_memory[j-1].enqueue(msg); 
            }
         }
      }
   }
   else
   {
      for(int j=m_outLast;j<m_inputs+m_outLast;j++)
      {
         unsigned displaced;
           
         if(outputInterfaz()->isStopActive() && 
             ((m_lastIsShort && bufferHolesShort()<=2)  || (!m_lastIsShort && bufferHoles()<=2)))
         {
            displaced=m_outLast;
            if(m_registers[displaced].numberOfElements()!=0)
            {
               m_registers[displaced].firstElement(msg);
               if(m_lpack > msg->packetSize() && bufferHolesShort() != 0 && m_lastIsShort)
               {
                  m_registers[displaced].dequeue(msg);
                  m_shortMemory[displaced].enqueue(msg); 
                  break;
               }
               if(m_lpack == msg->packetSize() && bufferHoles() != 0 && !m_lastIsShort) 
               {
                  m_registers[displaced].dequeue(msg);
                  m_memory[displaced].enqueue(msg); 
                  break;
               }
            }
         }
         else
         {
            displaced=j%m_inputs;
            
            if(m_registers[displaced].numberOfElements()!=0)
            {
               m_registers[displaced].firstElement(msg);
               if(m_lpack > msg->packetSize() && bufferHolesShort() != 0 )
               {
                  m_registers[displaced].dequeue(msg);
                  m_shortMemory[displaced].enqueue(msg); 
               }
               if(m_lpack == msg->packetSize() && bufferHoles()!=0 ) 
               {
                  m_registers[displaced].dequeue(msg);
                  m_memory[displaced].enqueue(msg); 
               }
            }
         }
      }
   }
   return true;   
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoNoFragFlow :: outputWriting()
{
     PRZMessage*  flitnow;
     outputInterfaz()->clearData();
     

                                           //El demultiplexor ya filtra los stops CT
   if( ! outputInterfaz()->isStopActive() || m_payload==1)
   {            
     if(m_payload==1)
     {
         if(!m_lastIsShort )
         {
            if(m_memory[m_outLast].numberOfElements()!=0 )
            {
               m_memory[m_outLast].dequeue(flitnow);
               if(flitnow->isTail())
              {
                  m_payload=0;
              }
              sendFlit(flitnow);
              return true;
             }
             /*else
             {
                if(m_shortMemory[m_outLast].numberOfElements()==0)
                   cerr<<"posible cuelgue en memoria";
                
             }*/
          }
          else
          {
            if(m_shortMemory[m_outLast].numberOfElements()!=0 )
            {
               m_shortMemory[m_outLast].dequeue(flitnow);
               if(flitnow->isTail())
              {
                  m_payload=0;
              }
              sendFlit(flitnow);
              return true;

             }
             /*else
             {
                if(m_shortMemory[m_outLast].numberOfElements()==0)
                   cerr<<"posible cuelgue en memoria Short";
                
             }*/
          }
     }
     else
     {
        if((m_lastIsShort || bufferElementsShort()==0 ) && bufferElements()!=0)
        {
           for(int j=m_outLast+1;j<m_inputs+m_outLast+1;j++)
           {
              unsigned displaced=j%m_inputs;
              if(m_memory[displaced].numberOfElements()!=0)
              {   
                  m_lastIsShort=false;
                  m_outLast=displaced;
                  m_payload=1;
                  m_memory[displaced].dequeue(flitnow);
                  sendFlit(flitnow);
                  return true;
               }
            }
         }
         else
         {
           for(int j=m_outLast+1;j<m_inputs+m_outLast+1;j++)
           {
              unsigned displaced=j%m_inputs;
              if(m_shortMemory[displaced].numberOfElements()!=0)
              {   
                  m_lastIsShort=true;
                  m_outLast=displaced;
                  m_payload=1;
                  m_shortMemory[displaced].dequeue(flitnow);
                  sendFlit(flitnow);
                  return true;
               }
            }
         }         
      } 
   }     
      

 
   return true;   
}
//*************************************************************************
//:
//  f: virtual void sendFlit(const PRZMessage & msg);
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifoNoFragFlow :: sendFlit(PRZMessage* msg)
{
   outputInterfaz()->sendData(msg);
    #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles());
         PRZWRITE2LOG( texto );
      #endif
}
//*************************************************************************
//:
//  f: 
//
//  d:
//:
//*************************************************************************
unsigned PRZMultiportFifoNoFragFlow::bufferElementsShort() const
{ 
         unsigned kaka=0;
         for(int index=0; index<m_inputs;index++)
         {
            kaka=m_shortMemory[index].numberOfElements()+kaka;
         }
         return kaka; 
      }
// fin del fichero
