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
// File: ../ATCSimul/src/PRZSimulation.cpp
//
// Clase      : PRZSimulation
//
// Hereda de  : PRZComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZSimulation.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
// 
//*************************************************************************

static char rcsId[] = "$Id: PRZSimulation.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include "PRZSimulation.hpp"

#ifndef __PRZCONST_H__
#include "PRZConst.hpp"        // Constantes PRZ
#endif

#ifndef __PRZSBLDR_HPP__
#include "PRZSimulationBuilder.hpp"      // PRZSimulationBuilder
#endif

#ifndef __PRZTRAFI_HPP__
#include "PRZTrafficPattern.hpp"      // PRZTrafficPattern
#endif

//#ifndef __PRZTRAFI_HPP__
//#include "PRZTrafficPatternTrace.hpp"      // PRZTrafficPattern
//#endif

#ifndef __PRZGAUGE_HPP__
#include "PRZGauge.hpp"      // PRZGauge
#endif

#include <stdio.h>

//*************************************************************************

   #define SPACES " "
    
//*************************************************************************

// Run time information
IMPLEMENT_RTTI_DERIVED(PRZSimulation,PRZComponent);

//*************************************************************************
//:
//  f: inline void setNetwork(PRZNetwork* net);
//  d:
//
//  f: inline PRZNetwork* getNetwork() const;
//  d:
//
//  f: inline void setSimulationCycles(unsigned long cycles);
//  d:
//
//  f: inline void setStopSimulation(unsigned long cycles);
//  d:
//
//  f: inline void setSeed(unsigned seed);
//  d:
//
//  f: inline void setMessageLength(unsigned length);
//  d:
//
//  f: inline unsigned getMessageLength() const;
//  d:
//
//  f: inline void setPacketLength(unsigned length);
//  d:
//
//  f: inline unsigned getPacketLength() const;
//  d:
//
//  f: inline void setRadius(unsigned radius);
//  d:
//
//  f: inline unsigned getRadius() const;
//  d:
//
//  f: inline void setDefaultLoadSupply(double value);
//  d:
//
//  f: inline void setShowState(Boolean value);
//  d:
//
//  f: inline void setTrafficPattern(PRZTrafficPattern* pattern);
//  d:
//
//  f: inline void setShowBufferState(Boolean value);
//  d:
//
//  f: inline void setBuffersFileName(const PRZString& name);
//  d:
//
//  f: inline PRZString buffersFileName() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZSimulation(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZSimulation :: PRZSimulation(const PRZComponentId& id)
               : PRZComponent(id),
                 m_Network(0),
                 m_TrafficPattern(0),
                 m_SimulationCycles(0),
                 m_StopSimulation(~0),
                 m_Seed(113),
                 m_MessageLength(0),
                 m_PacketLength(0),
                 m_q(0.0),
                 m_defaultLoad(-1.0),
                 m_defaultProb(-1.0),
                 m_Radius(0),
                 m_ShowState(true),
                 m_StartHour(0),
                 m_Clock(0),
                 m_BufferOutput(0),
                 m_ShowBufferState(false),
                 m_mensajestraza(0),
                 m_Gauge(0),
                 m_simulatedClock(0),
                 m_clockTR(0)
{
   setSimulation(this);
   disableNotification();
}


//*************************************************************************
//:
//  f: virtual ~PRZSimulation();
//
//  d:
//:
//*************************************************************************

PRZSimulation :: ~PRZSimulation()
{
   getNetwork()->terminate();
   
   if(m_BufferOutput)
   {
      m_BufferOutput -> close();
      delete m_BufferOutput;
   }
   
   delete m_Gauge;   
   delete m_TrafficPattern;
   delete m_Network;
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: initialize()
{
   if( isInitializated() ) return;
   m_StartHour = time(0);
   m_Radius = m_Network->getSizeX();
  // m_q = 8.0*m_A/m_Radius/m_MessageLength/m_PacketLength;  
   
   // Se inicializan las series de numeros pseudo-aleatorios.
   srand(m_Seed);
   srand48(m_Seed);
   
   if(m_Network) m_Network->initialize();
   Inhereited::initialize();
}


//*************************************************************************
//:
//  f: void setLoadSupply(double A);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: setLoadSupply(double value)
{
   // Correccion en los valores de carga cuando el trafico es
   // bimodal corto
   double realPacketLegth=m_PacketLength;
   
   if(getTrafficPattern()->isBimodal() && getTrafficPattern()->getMessagesBimodal()<1)
   {
      realPacketLegth=m_PacketLength*(1+getTrafficPattern()->getProbBimodal()*(getTrafficPattern()->getMessagesBimodal()-1));
   }
   m_q = 8.0*value/m_Radius/m_MessageLength/realPacketLegth;

   
}

//*************************************************************************
//:
//  f: void setLoadProb(double A);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: setLoadProb(double value)
{
   unsigned realPacketLegth=m_PacketLength;
   
   if(getTrafficPattern()->isBimodal() && getTrafficPattern()->getMessagesBimodal()<1)
   {
      realPacketLegth=m_PacketLength*(1+getTrafficPattern()->getProbBimodal()*(1-getTrafficPattern()->getMessagesBimodal()));
   }
   m_q = value/realPacketLegth;
}

//*************************************************************************
//:
//  f: virtual void run(uTIME time=0);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: run(uTIME time)
{
   if( getTrafficPattern()->isEmpty() && 
       (getNetwork()->getMessagesInNet() == 0) )
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
         getNetwork()->run(m_Clock++);
         writeCurrentData();
      }
      return;
   }

   if( m_ShowState ) cerr << "Simulando..." << endl;   

   if( getTrafficPattern() -> isFromFile() )
   {
       
      while( getTrafficPattern()->injectMessage() )
      {   
         getNetwork()->run(m_Clock);
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
                     if( m_Clock==0)
                     {                      
                        PRZMessage msg;
                        msg.setGenerationTime(m_Clock);
                        msg.setSource(PRZPosition(7,4,0));
                        msg.setDestiny(PRZPosition(0,2,0));
                        m_Network->sendMessage(msg);


                        //m_Network->sendMessage(msg);
                     }
                     
                     
#else
         getTrafficPattern()->generateMessages(m_q);
         //cerr<<m_q<<" ";
#endif
         } // if
    
         getNetwork()->run(m_Clock);         
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

void PRZSimulation :: writeCurrentData()
{
   unsigned long step;
   step= long(m_SimulationCycles/12);

   
   if( (m_Clock%100) == 0 )
   {
      if( showBufferState() ) writeBufferStatus();
      if( isEnableNotification() )
      {
         double clock = m_Clock;
         double latenciaMedia = 0.0;
         double msgTx = m_Network->getMessagesTx();
         double msgRx = m_Network->getMessagesRx();         
            
         if( msgRx )
         {
            latenciaMedia = double(m_Network->getTotalDelay(PRZNetwork::Message)) / msgRx;   
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

void PRZSimulation :: writeResults()
{
   
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
   
   unsigned nodes=getNetwork()->getSizeX();
   if(getNetwork()->getSizeY()!=0)
      nodes=nodes*getNetwork()->getSizeY();
   if(getNetwork()->getSizeZ()!=0)
      nodes=nodes*getNetwork()->getSizeZ();
                            
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
             PRZString("\nBuffers size            = ") + PRZString(globalData().routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nSupply Thr. Norm        = ") + PRZString((double)m_Network->getFlitsTx()/(double)m_Clock/(double)nodes) + " flits/cycle/router" +
             PRZString("\nAccept Thr. Norm        = ") + PRZString((double)throughput/(double)nodes) + " flits/cycle/router" +
             PRZString("\nSupply Thr.             = ") + PRZString((double)m_Network->getFlitsTx()/(double)m_Clock) + " flits/cycle" +
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
             PRZString("\nReal Cycles Simulated   = ") + PRZString(m_simulatedClock)  +             PRZString("\nBuffers size            = ") + PRZString(globalData().routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nSupply load             = ") + PRZString(cargaAplicada) + " %" +
             PRZString("\nReal load               = ") + PRZString(cargaReal) + " %" +
             PRZString("\nSupply Thr.             = ") + PRZString((double)m_Network->getFlitsTx()/(double)m_Clock) + " flits/cycle" +
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

void PRZSimulation :: writeSimulationStatus(ostream& os)
{     
   time_t localHour2 = time(0);
    double throughput = m_Network->getFlitsRx()/double(m_Clock);
   unsigned nodes=getNetwork()->getSizeX();
   if(getNetwork()->getSizeY()!=0)
      nodes=nodes*getNetwork()->getSizeY();
   if(getNetwork()->getSizeZ()!=0)
      nodes=nodes*getNetwork()->getSizeZ();
   double cargaAplicada = m_Network->getFlitsTx()*100.0/
                          8.0/m_Radius/m_Clock;
   double cargaReal = m_Network->getFlitsRx()*100.0/
                      8.0/m_Radius/m_Clock;
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
             
             PRZString("\nBuffers size            = ") + PRZString(globalData().routerBufferSize()) +
             PRZString("\nMessages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\nPackets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\nCycles simulated        = ") + PRZString(m_Clock) + " / " + PRZString(m_SimulationCycles) +
             PRZString("\nReal Cycles Simulated   = ") + PRZString(m_simulatedClock)  +
                        PRZString("\nSupply Thr. Norm        = ") + PRZString((double)m_Network->getFlitsTx()/(double)m_Clock/(double)nodes) + " flits/cycle/router" +
             PRZString("\nAccept Thr. Norm        = ") + PRZString((double)throughput/(double)nodes) + " flits/cycle/router" +
             PRZString("\nSupply Thr.             = ") + PRZString((double)m_Network->getFlitsTx()/(double)m_Clock) + " flits/cycle" +
             PRZString("\nThroughput              = ") + PRZString(throughput) + " flits/cycle" +
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
             
             PRZString("\nBuffers size            = ") + PRZString(globalData().routerBufferSize()) +
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
//  f: char* writeSimulationStatus();
//
//  d:
//:
//*************************************************************************

char* PRZSimulation :: writeSimulationStatus()
{     
   time_t localHour2 = time(0);
   double latMediaMsgTotal = m_Network->getTotalDelay(PRZNetwork::Message) /
                            double(m_Network->getMessagesRx());

   double latMediaMsgNetwork = m_Network->getNetworkDelay(PRZNetwork::Message) /
                               double(m_Network->getMessagesRx());

   double latMediaMsgBuffer = m_Network->getBufferDelay(PRZNetwork::Message) /
                              double(m_Network->getMessagesRx());
   
   PRZString buffer;
   buffer += PRZString("\n##***************************************") +
             PRZString("\n## Network        = ") + asString() +
             PRZString("\n## Started at : " ) + ctime(&m_StartHour) +
             PRZString("## Ended at   : " ) + ctime(&localHour2) +   
             PRZString("##***************************************");
   if(m_mensajestraza==0)
  {
   buffer += PRZString("\n##Traffic Pattern         = ") + m_TrafficPattern->asString() +
             PRZString("\n##Seed                    = ") + PRZString(m_Seed) +                           
             
             PRZString("\n##Buffers size            = ") + PRZString(globalData().routerBufferSize()) +
             PRZString("\n##Messages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\n##Packets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\n##Cycles simulated        = ") + PRZString(m_Clock) + " / " + PRZString(m_SimulationCycles) +
             PRZString("\n##Messages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\n##Messages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\n##Total message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\n##Network message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\n##Buffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\n##Maximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));
    }
    else
    {
             double reloj_red=getClock();

             buffer += PRZString("\n##Traffic Pattern         = ") + m_TrafficPattern->asString() +
             PRZString("\n##Seed                    = ") + PRZString(m_Seed) +                           
             
             PRZString("\n##Buffers size            = ") + PRZString(globalData().routerBufferSize()) +
             PRZString("\n##Messages length         = ") + PRZString(m_MessageLength) + " packet(s)" +
             PRZString("\n##Packets length          = ") + PRZString(m_PacketLength) + " flits" +
             PRZString("\n##Execution Time(Pcy)     = ") + PRZString(m_Clock*reloj_red)  +
             PRZString("\n##Real Cycles Simulated   = ") + PRZString(m_simulatedClock)  +
             PRZString("\n##Messages generated      = ") + PRZString(m_Network->getMessagesTx()) +
             PRZString("\n##Messages received       = ") + PRZString(m_Network->getMessagesRx()) +
             PRZString("\n##Total message latency   = ") + PRZString(latMediaMsgTotal) +
             PRZString("\n##Network message latency = ") + PRZString(latMediaMsgNetwork) +
             PRZString("\n##Buffer message latency  = ") + PRZString(latMediaMsgBuffer) +
             PRZString("\n##Maximum message latency = ") + 
             PRZString(m_Network->getMaximLatency(PRZNetwork::Message));   
    }      
   buffer += PRZString("\n##Simulation time         = ") + 
             convertSeconds(difftime(localHour2,m_StartHour))+
             PRZString("\n");
   
   //Dinamic evolution of Load and latency
      buffer+=m_Network->getReport();
     
            
  return (char *)buffer;
}

//*************************************************************************
//:
//  f: PRZString convertSeconds(double secs);
//
//  d:
//:
//*************************************************************************

PRZString PRZSimulation :: convertSeconds(double secs)
{
   char buffer[10];
   unsigned long hours=0, minutes=0;
   unsigned long seconds = (unsigned long)secs;
   if( seconds > 3600 )
   {
      hours = (unsigned long)(seconds/3600);
      seconds = seconds%3600;
   }
   if( seconds > 60 )
   {
      minutes = (unsigned long)(seconds/60);
      seconds = seconds%60;
   }
   
   sprintf(buffer,"%02d:%02d:%02d\0",hours,minutes,seconds);
   return buffer;
}


//*************************************************************************
//:
//  f: void writeBufferStatus();
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: writeBufferStatus()
{
   if( ! m_BufferOutput )
   {
      m_BufferOutput = new ofstream(buffersFileName());
      m_Network->writeBufferInformation(*m_BufferOutput);
   }
   
   m_Network -> writeBufferStatus(*m_BufferOutput);
}


//*************************************************************************
//:
//  f: void setTrafficPattern(const PRZString& name);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: setTrafficPattern(const PRZString& name)
{
   Boolean isBimodal = false;
   double probBimodal;
   unsigned msgBimodal;
   
   if( getTrafficPattern() )
   {
      if( getTrafficPattern()->isBimodal() ) isBimodal=true;
      probBimodal = getTrafficPattern()->getProbBimodal();
      msgBimodal  = getTrafficPattern()->getMessagesBimodal();
      delete getTrafficPattern();
   }
   
   PRZTag tag(PRZ_TAG_PATTERN,((isBimodal)?PRZ_TAG_BIMODAL:PRZ_TAG_MODAL));
   
   if( isBimodal )
   {
      tag.addAttribute(new PRZAttribute(PRZ_TAG_PROBBIMODAL,probBimodal));
      tag.addAttribute(new PRZAttribute(PRZ_TAG_MSGBIMODAL,msgBimodal));   
   }
   
   // Los traficos pueden venir con 2 parametros
   PRZString tempName = name;
   tempName.replace(',',' ');
   
   if( tempName.word(1) != "" )
      tag.addAttribute(new PRZAttribute(PRZ_TAG_TYPE,tempName.word(1)));

   if( tempName.word(2) != "" )
      tag.addAttribute(new PRZAttribute(PRZ_TAG_PARAM1,tempName.word(2)));
   
   if( tempName.word(3) != "" )
      tag.addAttribute(new PRZAttribute(PRZ_TAG_PARAM2,tempName.word(3)));

   PRZTrafficPattern* pattern =
                      PRZTrafficPattern::createTrafficPattern(tag,*this);
   setTrafficPattern(pattern);
}


//*************************************************************************
//:
//  f: void setBimodalTraffic(const PRZString& params);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: setBimodalTraffic(const PRZString& params)
{
   if( ! getTrafficPattern() )
   {
      setTrafficPattern( new PRZTrafficPatternRandom(*this) );
   }

   PRZString parameters = params;
   parameters.replace(',',' ');
   if( (parameters.word(1)=="") || (parameters.word(2)=="") )
   {
      EXIT_PROGRAM(ERR_PRZSIMUL_001);
   }
   
   getTrafficPattern()->setBimodal();
   getTrafficPattern()->setMessagesBimodal(parameters.word(1).asDouble());
   getTrafficPattern()->setProbBimodal(parameters.word(2).asDouble());
}


//*************************************************************************
//:
//  f: void setTrafficPatternFile(const PRZString& name);
//
//  d:
//:
//*************************************************************************

void PRZSimulation :: setTrafficPatternFile(const PRZString& name)
{
   delete getTrafficPattern();
   PRZTrafficPattern* pattern = 
                      PRZTrafficPattern::createTrafficPattern(name,*this);
   setTrafficPattern(pattern);
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZSimulation :: asString() const
{
   return m_Network->asString();
}


//*************************************************************************
//:
//  f: static PRZSimulation* newFrom(const PRZTag* tag, INDEX& index);
//
//  d:
//:
//*************************************************************************

PRZSimulation* PRZSimulation::newFrom(const PRZTag* tag, INDEX& index)
{   
   PRZComponentId idSimulation(*tag);
   PRZSimulation* simulation = new PRZSimulation(idSimulation);   
   
   PRZString endTag = PRZString("/") + tag->tagName();
   PRZTag* nextTag = PRZSimulation::simulationBuilder->
                     getTagWithIndex(++index);
   
   while( nextTag && nextTag->tagName()!=endTag )
   {
      PRZString tagName = nextTag->tagName();

      if( tagName == PRZ_TAG_NETWORK )
      {
         // Se trata de la red
         PRZString networkId = nextTag->tagId();
         PRZNetwork* net= (PRZNetwork*)(PRZComponent::networkBuilder->
                           createComponentWithId(networkId,simulation));
         simulation->setNetwork(net);
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
