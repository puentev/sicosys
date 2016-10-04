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
// File: ../ATCSimul/inc/PRZGauge.hpp
//
// Description:  fichero de encabezado de la clase "PRZGauge"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZGauge.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZGauge_HPP__
#define __PRZGauge_HPP__

//************************************************************************

   class PRZRouter;
   class PRZInterfaz;
   class PRZTag;
   class PRZSimulation;
         
//************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif

   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif

   #ifndef __PRZPair_HPP__
   #include <PRZPair.hpp>
   #endif
   
//************************************************************************

   class PRZGauge
   {
      typedef PRZPair<unsigned,uTIME>    TMessageToken;  
      typedef PRZSequence<TMessageToken> TMessageIdList;
   
   public:
      PRZGauge(const PRZString& file);
      ~PRZGauge();

      void setRouter(PRZRouter* router);
      void onDataTx(PRZInterfaz* interfaz);
      void onDataRx(PRZInterfaz* interfaz);
      
      static PRZGauge* newFrom(const PRZTag& tag, PRZSimulation* simulation);
      
      // Run time information
      DEFINE_RTTI(PRZGauge);

   private:
      PRZRouter*     m_Router;
      PRZString      m_FileName;
      TMessageIdList m_MessagesList;
      ofstream*      m_File;
   };

//*************************************************************************

#endif


// fin del fichero
