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
// File: ../ATCSimul/src/ATCSimulator.cpp
//
// Clase      : ATCSimulator
//
// Hereda de  : PRZNotifier
//
// $Author: vpuente $ 
//:
// $RCSfile: ATCSimulator.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: ATCSimulator.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include "ATCSimulator.hpp"

#include "PRZRouterBuilder.hpp"       // PRZRouterBuilder
#include "PRZNetworkBuilder.hpp"       // PRZNetworkBuilder
#include "PRZSimulationBuilder.hpp"       // PRZSimulationBuilder
#include "PRZSimulation.hpp"       // PRZSimulation
#include "PRZIniFile.hpp"         // PRZIniFile
#include "PRZOptions.hpp"         // PRZOptions
#include "PRZConst.hpp"         // Constantes PRZ

#include <signal.h>


//*************************************************************************

#define  OPT_SIMUL_NAME         's'
#define  OPT_SIMUL_LOAD         'l'
#define  OPT_SIMUL_PROB         'p' //VPV
#define  OPT_RSIM_EVOL          'd' //Rsim dinamic evolution of net
#define  OPT_SIMUL_STATE        'q'
#define  OPT_SIMUL_CYCLES       'c'
#define  OPT_SIMUL_BUFFERFILE   'b'
#define  OPT_SIMUL_PATTERNFILE  'f'
#define  OPT_SIMUL_TRAFFIC      't'
#define  OPT_SIMUL_BIMODALTRAFF 'B'
#define  OPT_SIMUL_PACKETLENGTH 'L'
#define  OPT_SIMUL_BUFFERSIZE   'u'
#define  OPT_CLOCK_TRACE        'C'


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(ATCSimulator,PRZNotifier);

//*************************************************************************

ATCSimulator* ATCSimulator :: m_Instance = 0;


//*************************************************************************
//:
//  f: static void handlerSignalUSR1(int val);
//
//  d:
//:
//*************************************************************************

static void handlerSignalUSR1(int val)
{
   for(int i=1; i<=PRZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      PRZSimulation* simulation = ATCSIMULATOR()->getSimulation(i);
      if(simulation)
      {
         ofstream ofs;
         ofs.open(PRZ_LOG_FILENAME,ios::app);
         simulation->getNetwork()->writeComponentStatus(ofs);
         ofs.close();
      }
   }

   signal(SIGUSR1,handlerSignalUSR1);
}


//*************************************************************************
//:
//  f: static void handlerSignalUSR2(int val);
//
//  d:
//:
//*************************************************************************

static void handlerSignalUSR2(int val)
{
   for(int i=1; i<=PRZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      PRZSimulation* simulation = ATCSIMULATOR()->getSimulation(i);
      if(simulation)
      {
         ofstream ofs;
         ofs.open(PRZ_LOG_FILENAME,ios::app);
         simulation->writeSimulationStatus(ofs);
         ofs.close();
      }
   }

   signal(SIGUSR2,handlerSignalUSR2);
}

//*************************************************************************
//:
//  f: ATCSimulator();
//
//  d:
//:
//*************************************************************************

ATCSimulator :: ATCSimulator()
              : m_SimulationArray(0),
                m_OldHandler(0)
{
   disableNotification();
   m_SimulationArray = new TSimulationArray(PRZ_MAX_NUMBER_SIMULATIONS);
   m_SimulationArray->initialize(0);   
   m_OldHandler = signal(SIGUSR2,handlerSignalUSR2);
   signal(SIGUSR1,handlerSignalUSR1);
   
}


//*************************************************************************
//:
//  f: ~ATCSimulator();
//
//  d:
//:
//*************************************************************************

ATCSimulator :: ~ATCSimulator()
{
   deleteAllSimulations();
   delete m_SimulationArray;
   signal(SIGUSR2,m_OldHandler);
}


//*************************************************************************
//:
//  f: ATCSimulator* instance();
//
//  d:
//:
//*************************************************************************

ATCSimulator* ATCSimulator :: instance()
{
   if( ! m_Instance )
      m_Instance = new ATCSimulator();

   return m_Instance;
}


//*************************************************************************
//:
//  f: void destroyInstance();
//
//  d:
//:
//*************************************************************************

void ATCSimulator :: destroyInstance()
{
   delete m_Instance;
   m_Instance = 0L;
}



//*************************************************************************
//:
//  f: unsigned createSimulation(int argc, char* argv[]);
//
//  d:
//:
//*************************************************************************

unsigned ATCSimulator :: createSimulation(int argc, char* argv[])
{
   PRZString param;
   for( int i=0; i<argc; i++ )
   {
      param += PRZString(argv[i]) + " ";
   }
   return createSimulation(param);
}


//*************************************************************************
//:
//  f: unsigned createSimulation(const PRZString& param);
//
//  d:
//:
//*************************************************************************

unsigned ATCSimulator :: createSimulation(const PRZString& param)
{
   unsigned simulations=0;
   
   for(int i=1; i<=PRZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      PRZSimulation* temp = 0;
      m_SimulationArray->valueAt(i,temp);
      if(temp) simulations++;
   }   
   
   if( simulations == PRZ_MAX_NUMBER_SIMULATIONS )
   {
      PRZString err;
      err.sprintf(ERR_ATCSIMUL_001, PRZ_MAX_NUMBER_SIMULATIONS);
      EXIT_PROGRAM(err);
   }

   PRZSimulation* simulation = initialize(param);
   if( simulation )
   {
      for(int i=1; i<=PRZ_MAX_NUMBER_SIMULATIONS; i++)
      {
         PRZSimulation* temp = 0;
         m_SimulationArray->valueAt(i,temp);
         if( ! temp )
         {
            m_SimulationArray->setValueAt(i,simulation);
            return i;
         }  
      }
   }
   return 0;
}


//*************************************************************************
//:
//  f: PRZSimulation* getSimulation(unsigned index) const;
//
//  d:
//:
//*************************************************************************

PRZSimulation* ATCSimulator :: getSimulation(unsigned index) const
{
   PRZSimulation* simulation = 0;
   m_SimulationArray->valueAt(index,simulation);
   return simulation;
}


//*************************************************************************
//:
//  f: void deleteSimulation(unsigned index);
//
//  d:
//:
//*************************************************************************

void ATCSimulator :: deleteSimulation(unsigned index)
{
   PRZSimulation* simulation = getSimulation(index);
   if( simulation )
   {
      delete simulation;
      m_SimulationArray->setValueAt(index,(PRZSimulation*)0);
   }
}


//*************************************************************************
//:
//  f: void deleteAllSimulations();
//
//  d:
//:
//*************************************************************************

void ATCSimulator :: deleteAllSimulations()
{
   for(int i=1; i<=PRZ_MAX_NUMBER_SIMULATIONS; i++)
   {
      deleteSimulation(i);
   }
}


//*************************************************************************
//:
//  f: PRZSimulation* initialize(const PRZString& param);
//
//  d:
//:
//*************************************************************************

PRZSimulation* ATCSimulator :: initialize(const PRZString& param)
{
   PRZOptions opts(param);
   installBuilders(param.word(1));
   //installBuilders("/disco4/prellezo/sim_alfa/mak/ATCSimul");
   PRZSimulationBuilder* sBuilder = (PRZSimulationBuilder*)
                                    PRZComponent::simulationBuilder;
   
   PRZString simulationName = opts.getValueForOption(OPT_SIMUL_NAME);
   PRZString bufferFile     = opts.getValueForOption(OPT_SIMUL_BUFFERFILE); 
   PRZString loadString     = opts.getValueForOption(OPT_SIMUL_LOAD);
   PRZString probString     = opts.getValueForOption(OPT_SIMUL_PROB);
   PRZString rsimtrace      = opts.getValueForOption(OPT_RSIM_EVOL);
   PRZString cyclesString   = opts.getValueForOption(OPT_SIMUL_CYCLES);
   PRZString patternFile    = opts.getValueForOption(OPT_SIMUL_PATTERNFILE);
   PRZString traffic        = opts.getValueForOption(OPT_SIMUL_TRAFFIC);
   PRZString bimodal        = opts.getValueForOption(OPT_SIMUL_BIMODALTRAFF);
   PRZString packetLength   = opts.getValueForOption(OPT_SIMUL_PACKETLENGTH);
   PRZString bufferSize     = opts.getValueForOption(OPT_SIMUL_BUFFERSIZE);
   PRZString clockTrace     = opts.getValueForOption(OPT_CLOCK_TRACE);

   Boolean   noDisplay      = opts.isOptionActive(OPT_SIMUL_STATE);

   PRZSimulation* simulation = (PRZSimulation*)
                  (sBuilder->createComponentWithId(simulationName));
   
   // Una vez creada la simulacion a ejecutar, los builders ya no se van
   // a volver a utilizar. Para evitar que mantenga retenida parte de la
   // memoria durante la simulacion, se desinstalan.
   
   uninstallBuilders();


   if( ! simulation )
   {
      PRZString texto = " Simulacion no encontrada: '";
      texto += simulationName + "'";
      EXIT(texto);
   }
   if( probString != "" && loadString != "" )
   {
      PRZString texto = "Can't use -l and -p together";
      EXIT(texto);
   }
   if( clockTrace != "" )
   {
      simulation -> setClock(packetLength.asDouble());
   }
   //Esto ha de ir antes que las cargas (estaba mal)
   if( packetLength != "" )
   {
      simulation -> setPacketLength(packetLength.asInteger());
   }


   if( cyclesString != "" )
   {
      simulation->setSimulationCycles(cyclesString.asInteger());
   }

   if( patternFile != "" )
   {
      simulation->setTrafficPatternFile(patternFile);
   }

   if( traffic != "" )
   {
      simulation->setTrafficPattern(traffic);
   }

   if( bimodal != "" )
   {
      simulation->setBimodalTraffic(bimodal);
   }
   
   if( probString == "" && loadString == "")
   {
      if(simulation->defaultLoadProb() != -1 && simulation->defaultLoadSupply() != -1)
      {
         PRZString texto = "Can't specify load and probability";
         EXIT(texto);
      }
      else if(simulation->defaultLoadProb() != -1)
      {
         simulation->setLoadProb(simulation->defaultLoadProb());  
	   }
      else if(simulation->defaultLoadSupply() != -1)
      {
         simulation->setLoadSupply(simulation->defaultLoadSupply());  
	   }
	   else
	   {
         PRZString texto = "Must specify load or probability";
         EXIT(texto);
	   }
   }
   else if( probString != "" )
   {
      simulation->setLoadProb(probString.asDouble());
   }
   else if( loadString != "" )
   {
      simulation->setLoadSupply(loadString.asDouble());
   }
   
   if( noDisplay ) simulation->setShowState(false);
   
   if( bufferFile != "" )
   {
      simulation -> setShowBufferState(true);
      simulation -> setBuffersFileName(bufferFile);
   }

   if( bufferSize != "" )
   {
      simulation -> setBufferSize(bufferSize.asInteger());
   }
   else
   {
      //indirectamnet tengo que arrancar el pool de mensajes
      simulation ->  setBufferSize(0);
   }
   if( rsimtrace != "" )
   {
      simulation -> getNetwork()->setReportMsg(rsimtrace.asInteger());
   }
   return simulation;
}


//*************************************************************************
//:
//  f: void installBuilers(const PRZString& name);
//
//  d:
//:
//*************************************************************************

void ATCSimulator :: installBuilders(const PRZString& name)
{
//   PRZIniFile iniFile(name+".ini");

   PRZIniFile iniFile("ATCSimul.ini");

   PRZString rFile = iniFile.getValueFor("RouterFile");
   PRZString nFile = iniFile.getValueFor("NetworkFile");
   PRZString sFile = iniFile.getValueFor("SimulationFile");
   PRZRouterBuilder*     rBuilder = new PRZRouterBuilder(rFile);
   PRZNetworkBuilder*    nBuilder = new PRZNetworkBuilder(nFile);
   PRZSimulationBuilder* sBuilder = new PRZSimulationBuilder(sFile);
   
   rBuilder->loadSgmlInformation();
   nBuilder->loadSgmlInformation();
   sBuilder->loadSgmlInformation();
   
   PRZComponent::routerBuilder    = rBuilder;
   PRZComponent::networkBuilder   = nBuilder;
   PRZComponent::simulationBuilder= sBuilder;
}


//*************************************************************************
//:
//  f: void uninstallBuilers();
//
//  d:
//:
//*************************************************************************

void ATCSimulator :: uninstallBuilders()
{
   delete PRZComponent::routerBuilder;
   delete PRZComponent::networkBuilder;
   delete PRZComponent::simulationBuilder;

   PRZComponent::routerBuilder    = 0;
   PRZComponent::networkBuilder   = 0;
   PRZComponent::simulationBuilder= 0;
}

//*************************************************************************

// fin del fichero
