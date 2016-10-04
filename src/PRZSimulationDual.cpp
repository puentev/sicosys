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
// File: ../ATCSimul/src/PRZSimulationDual.cpp
//
// Clase      : PRZSimulationDual
//
// Hereda de  : PRZSimulation
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZSimulationDual.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
// 
//*************************************************************************

static char rcsId[] = "$Id: PRZSimulationDual.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include "PRZSimulationDual.hpp"

#ifndef __PRZCONST_H__
#include "PRZConst.hpp"        // Constantes PRZ
#endif

#ifndef __PRZSBLDR_HPP__
#include "PRZSimulationBuilder.hpp"      // PRZSimulationBuilder
#endif

#include "PRZTrafficPattern.hpp"     
#include "PRZGauge.hpp"     

#include <stdio.h>

//*************************************************************************

   #define SPACES " "
    
//*************************************************************************

// Run time information
IMPLEMENT_RTTI_DERIVED(PRZSimulationDual, PRZSimulation);


//*************************************************************************
//:
//  f: PRZSimulationDual(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZSimulationDual :: PRZSimulationDual(const PRZComponentId& id)
                   : PRZSimulation(id),
                     m_Network(0)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZSimulation();
//
//  d:
//:
//*************************************************************************

PRZSimulationDual :: ~PRZSimulationDual()
{
   delete m_Network;
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZSimulationDual :: initialize()
{
   if(m_Network) m_Network->initialize();
   Inhereited::initialize();
}



//*************************************************************************
//:
//  f: virtual void run(uTIME time=0);
//
//  d:
//:
//*************************************************************************

void PRZSimulationDual :: run(uTIME time)
{
   if( getTrafficPattern()->isEmpty() && 
       (getNetwork(0)->getMessagesInNet() == 0) && 
       (getNetwork(1)->getMessagesInNet() == 0) )
   {
      m_Clock++;
      return;
   }
   
   if( time != 0)
   {
      // En este caso, se ejecuta el simulador durante "time" ciclos,
      // y se regresa. No se controla el tiempo en que debe terminar
      // la ejecucion de la simulacion, pero si se verifica cuando debe
      // dejar de inyectar.
      for( uTIME tempClk=0; tempClk<time; tempClk++ )
      {
         if( m_Clock <= m_StopSimulation )
         {
            getTrafficPattern()->generateMessages(m_q);
            
         }
         getNetwork(0)->run(m_Clock++);
         getNetwork(1)->run(m_Clock++);
         writeCurrentData();
      }
      return;
   }

   if( m_ShowState ) cerr << "Simulando..." << endl;   

   if( getTrafficPattern() -> isFromFile() )
   {
      
      while( getTrafficPattern()->injectMessage() )
      {   
         getNetwork(0)->run(m_Clock);
         getNetwork(1)->run(m_Clock);
         writeCurrentData();

         m_Clock++;// El modelado dela velocidad del procesador esta en el 
         			 //przTraffficPatternTrace, a la hora de inicializar la traza
         m_simulatedClock++;
      }
   }
   else 
   {   
      for( m_Clock=1; m_Clock<=m_SimulationCycles; m_Clock++ )
      {
         if( m_Clock <= m_StopSimulation )
         {
#ifdef PAQ
                     if( m_Clock==30)
                     {                      
                        PRZMessage msg;
                        msg.setGenerationTime(m_Clock);
                        msg.setPacketSize(16);
                        msg.setMessageSize(1);
                        msg.setSource(PRZPosition(0,0,0));
                        msg.setDestiny(PRZPosition(1,1,0));
                        m_Network->sendMessage(msg);
                        //m_Network->sendMessage(msg);
                     }
                     
#else
         getTrafficPattern()->generateMessages(m_q);
         //cerr<<m_q<<" ";
#endif
         } // if
    
         getNetwork(0)->run(m_Clock);
         getNetwork(1)->run(m_Clock);
         writeCurrentData();
         
      } // for
   }
   
   m_Clock--;
}

//*************************************************************************
//:
//  f: void writeCurrentData();
//
//  d:
//:
//*************************************************************************

void PRZSimulationDual :: writeCurrentData()
{
   unsigned long step;
   step= long(m_SimulationCycles/12);

   // Red 0   
   if( (m_Clock%100) == 0 )
   {
      if( showBufferState() ) writeBufferStatus();
      if( isEnableNotification() )
      {
         double clock = m_Clock;
         double latenciaMedia = 0.0;
         double msgTx = getNetwork(0)->getMessagesTx();
         double msgRx = getNetwork(0)->getMessagesRx();         
            
         if( msgRx )
         {
            latenciaMedia = double(getNetwork(0)->getTotalDelay(PRZNetwork::Message)) / msgRx;
         }
         PRZNotificationEvent event(EV_LATENCY,clock,latenciaMedia,msgTx,msgRx);
         notifyObservers(event);
      }
   }

   // Red 1   
   if( (m_Clock%100) == 0 )
   {
      if( showBufferState() ) writeBufferStatus();
      if( isEnableNotification() )
      {
         double clock = m_Clock;
         double latenciaMedia = 0.0;
         double msgTx = getNetwork(1)->getMessagesTx();
         double msgRx = getNetwork(1)->getMessagesRx();         
            
         if( msgRx )
         {
            latenciaMedia = double(getNetwork(1)->getTotalDelay(PRZNetwork::Message)) / msgRx;
         }
         PRZNotificationEvent event(EV_LATENCY,clock,latenciaMedia,msgTx,msgRx);
         notifyObservers(event);
      }
   }
         
   if( m_ShowState && ((m_Clock%step) == 0) )
   {
         cerr << "*";
         cerr.flush();
   }

}


//*************************************************************************
//:
//  f: void writeResults();
//
//  d: 
//:
//*************************************************************************

void PRZSimulationDual :: writeResults()
{
   Inhereited::writeResults();
   
   double cargaAplicada = m_Network->getFlitsTx()*100.0/
                          8.0/m_Radius/m_Clock;
   double cargaReal = m_Network->getFlitsRx()*100.0/
                      8.0/m_Radius/m_Clock;

   double throughput = m_Network->getFlitsRx()/double(m_Clock);
   if( getNetwork()->getSizeZ() > 1 )
   {
      cargaAplicada /= m_Radius;
      cargaReal /= m_Radius;   
   }
                         
   double latMediaMsgTotal = m_Network->getTotalDelay(PRZNetwork::Message) /
                            double(m_Network->getMessagesRx());

   double latMediaMsgNetwork = m_Network->getNetworkDelay(PRZNetwork::Message) /
                               double(m_Network->getMessagesRx());

   double latMediaMsgBuffer = m_Network->getBufferDelay(PRZNetwork::Message) /
                              double(m_Network->getMessagesRx());

   double latMediaPaqTotal = m_Network->getTotalDelay(PRZNetwork::Packet) /
                            double(m_Network->getPacketsRx());

   double latMediaPaqNetwork = m_Network->getNetworkDelay(PRZNetwork::Packet) /
                               double(m_Network->getPacketsRx());

   double latMediaPaqBuffer = m_Network->getBufferDelay(PRZNetwork::Packet) /
                              double(m_Network->getPacketsRx());

   double distanciaMedia = double(m_Network->getTotalDistance()) /
                           double(m_Network->getMessagesTx());
   
   
   time_t localHour2 = time(0);
   PRZString buffer;

   buffer += PRZString("\n***************************************") +
             PRZString("\n Network        = ") + asString() +
             PRZString("\n Started at : " ) + ctime(&m_StartHour) +
             PRZString(" Ended at   : " ) + ctime(&localHour2) +   
             PRZString("***************************************");
  if(m_mensajestraza==0)
  {
   buffer += PRZString("\nTraffic Pattern         = ") + m_TrafficPattern->asString() +
             PRZString("\nSeed                    = ") + PRZString(m_Seed) +                           
             PRZString("\nCycles simulated        = ") + PRZString(m_Clock) +
             PRZString("\nBuffers size            = ") + PRZString(globalData(getNetwork(1)).routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nSupply load             = ") + PRZString(cargaAplicada) + " %" +
             PRZString("\nReal load               = ") + PRZString(cargaReal) + " %" +
             PRZString("\nSupply Thr.             = ") + PRZString((double)m_Network->getMessagesTx()*(double)m_PacketLength/(double)m_Clock) + " flits/cycle" +
             PRZString("\nThroughput              = ") + PRZString(throughput) + " flits/cycle" +
             PRZString("\nAverage distance        = ") + PRZString(distanciaMedia) +
             PRZString("\nMessages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\nMessages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\nMessages to inject      = ") + PRZString(m_Network->getMessagesToTx()) +
             PRZString("\nTotal message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\nNetwork message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\nBuffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\nMaximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));
   }
   else
   {
      double reloj_red=getClock();
      buffer += PRZString("\nTraffic Pattern      = ") + m_TrafficPattern->asString() +
             PRZString("\nExecution Time(Pcy)     = ") + PRZString(m_Clock*reloj_red) +
             PRZString("\nReal Cycles Simulated   = ") + PRZString(m_simulatedClock)  +             PRZString("\nBuffers size            = ") + PRZString(globalData(getNetwork(1)).routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nSupply load             = ") + PRZString(cargaAplicada) + " %" +
             PRZString("\nReal load               = ") + PRZString(cargaReal) + " %" +
             PRZString("\nSupply Thr.             = ") + PRZString((double)m_Network->getMessagesTx()*(double)m_PacketLength/(double)m_Clock) + " flits/cycle" +
             PRZString("\nThroughput              = ") + PRZString(throughput) + " flits/cycle" +
             PRZString("\nAverage distance        = ") + PRZString(distanciaMedia) +
             PRZString("\nMessages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\nMessages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\nMessages to inject      = ") + PRZString(m_Network->getMessagesToTx()) +
             PRZString("\nTotal message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\nNetwork message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\nBuffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\nMaximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));
   }
             
   if( getTrafficPattern()->isBimodal() || (m_MessageLength>1) )
   {
      buffer += PRZString("\nTotal packet latency    = ") + PRZString(latMediaPaqTotal) +
                PRZString("\nNetwork packet latency  = ") + PRZString(latMediaPaqNetwork) +
                PRZString("\nBuffer packet latency   = ") + PRZString(latMediaPaqBuffer) +
                PRZString("\nMaximum packet latency  = ") + 
                PRZString(m_Network->getMaximLatency(PRZNetwork::Packet));
   }
   
             
   buffer += PRZString("\nSimulation time         = ") + 
             convertSeconds(difftime(localHour2,m_StartHour))+
             PRZString("\n");
   
   buffer+=m_Network->getReport();
   if(m_Network->getReport()!="")
      buffer+=m_Network->printHistogram();
   
   
   if( ! isEnableNotification() )
   {
      cout << buffer;
   }
   else
   {
      PRZNotificationEvent event(EV_RESULT,buffer);
      notifyObservers(event);
   }
}


//*************************************************************************
//:
//  f: void writeSimulationStatus(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZSimulationDual :: writeSimulationStatus(ostream& os)
{
   Inhereited::writeSimulationStatus(os);
   
   time_t localHour2 = time(0);
   double latMediaMsgTotal = m_Network->getTotalDelay(PRZNetwork::Message) /
                            double(m_Network->getMessagesRx());

   double latMediaMsgNetwork = m_Network->getNetworkDelay(PRZNetwork::Message) /
                               double(m_Network->getMessagesRx());

   double latMediaMsgBuffer = m_Network->getBufferDelay(PRZNetwork::Message) /
                              double(m_Network->getMessagesRx());
   
   PRZString buffer;
   buffer += PRZString("\n***************************************") +
             PRZString("\n Network        = ") + asString() +
             PRZString("\n Started at : " ) + ctime(&m_StartHour) +
             PRZString(" Ended at   : " ) + ctime(&localHour2) +   
             PRZString("***************************************");
   if(m_mensajestraza==0)
  {
   buffer += PRZString("\nTraffic Pattern         = ") + m_TrafficPattern->asString() +
             PRZString("\nSeed                    = ") + PRZString(m_Seed) +                           
             
             PRZString("\nBuffers size            = ") + PRZString(globalData(getNetwork(1)).routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nCycles simulated        = ") + PRZString(m_Clock) + " / " + PRZString(m_SimulationCycles) +
             PRZString("\nMessages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\nMessages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\nTotal message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\nNetwork message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\nBuffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\nMaximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));
    }
    else
    {
             double reloj_red=getClock();

         buffer += PRZString("\nTraffic Pattern         = ") + m_TrafficPattern->asString() +
             PRZString("\nSeed                    = ") + PRZString(m_Seed) +                           
             
             PRZString("\nBuffers size            = ") + PRZString(globalData(getNetwork(1)).routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nExecution Time(Pcy)     = ") + PRZString(m_Clock*reloj_red)  +
             PRZString("\nReal Cycles Simulated   = ") + PRZString(m_simulatedClock)  +
             PRZString("\nMessages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\nMessages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\nTotal message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\nNetwork message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\nBuffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\nMaximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));   
    }      
   buffer += PRZString("\nSimulation time         = ") + 
             convertSeconds(difftime(localHour2,m_StartHour))+
             PRZString("\n");
   
   //Dinamic evolution of Load and latency
      buffer+=m_Network->getReport();
     
            
   os << buffer;
   os.flush();
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZSimulationDual :: asString() const
{
   return Inhereited::asString() + "; " + m_Network->asString();
}


//*************************************************************************
//:
//  f: static PRZSimulationDual* newFrom(const PRZTag* tag, INDEX& index);
//
//  d:
//:
//*************************************************************************

PRZSimulationDual* PRZSimulationDual::newFrom(const PRZTag* tag, INDEX& index)
{   
   PRZComponentId idSimulation(*tag);
   PRZSimulationDual* simulation = new PRZSimulationDual(idSimulation);   
   
   PRZString endTag = PRZString("/") + tag->tagName();
   PRZTag* nextTag = PRZSimulation::simulationBuilder->
                     getTagWithIndex(++index);
   
   while( nextTag && nextTag->tagName()!=endTag )
   {
      PRZString tagName = nextTag->tagName();

      if( tagName == PRZ_TAG_REPLYNETWORK )
      {
         // Se trata de la red
         PRZString networkId = nextTag->tagId();
         PRZNetwork* net= (PRZNetwork*)(PRZComponent::networkBuilder->
                           createComponentWithId(networkId,simulation));
         simulation->setNetwork(0,net);
      }
      else if( tagName == PRZ_TAG_REQUESTNETWORK )
      {
         // Se trata de la red
         PRZString networkId = nextTag->tagId();
         PRZNetwork* net= (PRZNetwork*)(PRZComponent::networkBuilder->
                           createComponentWithId(networkId,simulation));
         simulation->setNetwork(1,net);
      }

      else if( tagName == PRZ_TAG_SIMULCYCLES)
      {
         PRZString ciclos = nextTag->tagId();
         simulation->setSimulationCycles(ciclos.asInteger());
      }
      else if( tagName == PRZ_TAG_STOPSIMUL )
      {
         PRZString stopSimulation = nextTag->tagId();
         simulation->setStopSimulation(stopSimulation.asInteger());      
      }
      //Necesario antes de imponer la carga por linea de comandos (si no toma por defecto
      //la longitud de Simula.sgm y no siempre coincide
      else if( tagName == PRZ_TAG_MESSAGELENGTH )
      {
         PRZString messageLength = nextTag->tagId();
         simulation->setMessageLength(messageLength.asInteger());      
      }
      else if( tagName == PRZ_TAG_PACKETLENGTH )
      {
         PRZString packetLength = nextTag->tagId();
         simulation->setPacketLength(packetLength.asInteger());      
      }
      else if( tagName == PRZ_TAG_LOAD )
      {
         PRZString load = nextTag->tagId();
         simulation->setDefaultLoadSupply(load.asDouble());      
      }     
      else if( tagName == PRZ_TAG_PROB )
      {
         PRZString prob = nextTag->tagId();
         simulation->setDefaultLoadProb(prob.asDouble());      
      }     
      else if( tagName == PRZ_TAG_PATTERN )
      {
         PRZTrafficPattern* pattern = 
                            PRZTrafficPattern::createTrafficPattern(*nextTag,*simulation);
         simulation->setTrafficPattern(pattern);
      }
      else if( tagName == PRZ_TAG_SEED )
      {
         PRZString seed = nextTag->tagId();
         simulation->setSeed(seed.asInteger());      
      }
      else if( tagName == PRZ_TAG_GAUGE )
      {
         PRZString seed = nextTag->tagId();
         simulation->setSeed(seed.asInteger());
         simulation->m_Gauge = PRZGauge::newFrom(*nextTag,simulation);
      }
      
      nextTag = PRZSimulation::simulationBuilder->getTagWithIndex(++index);      

   } // while

   simulation->setSimulation(simulation);
   simulation->initialize();

   
   return simulation;
}


//*************************************************************************


// fin del fichero
