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
// File: ../ATCSimul/src/PRZFicheroLog.cpp
//
// Clase      : PRZFicheroLog
//
// Hereda de  : PRZBase
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZFicheroLog.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZFicheroLog.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZFicheroLog.hpp>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

static PRZString _logName = "przlog.txt";
static PRZString _bakName = "przlog.bak";
static unsigned  _linesSaved = 250000;

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZFicheroLog,PRZBase);

//*************************************************************************

PRZFicheroLog* PRZFicheroLog :: m_Instance = 0;


//*************************************************************************
//:
//  f: PRZFicheroLog();
//
//  d:
//:
//*************************************************************************

PRZFicheroLog :: PRZFicheroLog()
               : m_LinesWrited(0)
{
   unlink(_logName);
   m_FileLog = new ofstream(_logName);
   if( ! m_FileLog->good() )
   {
      PRZString err;
      err.sprintf( ERR_PRZFILOG_001, (char*)_logName );
      EXIT_PROGRAM(err);
   }
   
   // Cabecera del fichero
   
   time_t theTime = time(0);
   PRZString cabecera = PRZString("//***************************************") +
                        PRZString("\n//   Fichero : ") +   _logName +
                        PRZString("\n//   Date :    ") +
                        PRZString(ctime(&theTime)) +
                        PRZString("//***************************************\n");
                                             
   m_FileLog->write((char*)cabecera, cabecera.size());
}


//*************************************************************************
//:
//  f: ~PRZFicheroLog();
//
//  d:
//:
//*************************************************************************

PRZFicheroLog :: ~PRZFicheroLog()
{
   m_FileLog->close();
   delete m_FileLog;
}


//*************************************************************************
//:
//  f: PRZFicheroLog* instance();
//
//  d:
//:
//*************************************************************************

PRZFicheroLog* PRZFicheroLog :: instance()
{
   if( ! m_Instance )
      m_Instance = new PRZFicheroLog();

   return m_Instance;
}


//*************************************************************************
//:
//  f: void destroyInstance();
//
//  d:
//:
//*************************************************************************

void PRZFicheroLog :: destroyInstance()
{
   delete m_Instance;
   m_Instance = 0L;
}

//*************************************************************************
//:
//  f: void write(const PRZString& text);
//
//  d:
//:
//*************************************************************************

void PRZFicheroLog :: write(const PRZString& text)
{
   (*m_FileLog) << text << endl;

   if( (++m_LinesWrited) >= _linesSaved )
   {
      m_FileLog->close();
      rename(_logName,_bakName);
      m_FileLog->open(_logName,ios::out|ios::trunc);
      m_LinesWrited = 0;
   }   
}


//*************************************************************************

// fin del fichero
