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
// File: ../ATCSimul/inc/PRZFicheroLog.hpp
//
// Description:  fichero de encabezado de la clase "PRZFicheroLog"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZFicheroLog.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZFicheroLog_HPP__
#define __PRZFicheroLog_HPP__

//************************************************************************

   #include <PRZBase.hpp>
   #include <fstream.h>

//************************************************************************

   class PRZFicheroLog : public PRZBase
   {
   public:
      ~PRZFicheroLog();
      static PRZFicheroLog* instance();
      static void destroyInstance();

      void write(const PRZString& line);
      
      // Run time information
      DEFINE_RTTI(PRZFicheroLog);

   private:
      PRZFicheroLog();
      static PRZFicheroLog* m_Instance;
      ofstream*             m_FileLog;
      unsigned              m_LinesWrited;
   };

//*************************************************************************

   #ifdef NO_TRAZA
   #define PRZWRITE2LOG(text) 
   #else
   #define PRZWRITE2LOG(text) PRZFicheroLog::instance()->write(text)
   #endif
      
//*************************************************************************


#endif

// fin del fichero
