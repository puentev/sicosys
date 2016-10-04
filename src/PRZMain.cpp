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
// File: ../ATCSimul/src/PRZMain.cpp
//
// Description:  main() para el ATC Simulator.
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMain.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZMain.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

//*************************************************************************

#include <ATCSimulator.hpp>


//*************************************************************************
//:
//  f: void main(int argc, char* argv[]);
//
//  d:
//:
//*************************************************************************

int main(int argc, char* argv[])
{  
   int idSimul;

   idSimul = ATCSIMULATOR()->createSimulation(argc,argv);
   ATCSIMULATOR()->getSimulation(idSimul)->run();  
   ATCSIMULATOR()->getSimulation(idSimul)->writeResults();   
   ATCSIMULATOR()->deleteSimulation(idSimul);
   ATCSimulator::destroyInstance();   
   return 1;
}

//*************************************************************************


// fin del fichero
