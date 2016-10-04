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
// File: ../ATCSimul/xbuffer/inc/xbuffer.hpp
//
// Description:   fichero de implementacion de la clase "XBufferMainWindow"
//
// Notes:   Ventana principal de la aplicacion. Unicamente consta de
//               un menu y de una serie de push-buttons.
//
// $Author: vpuente $ 
//
// $RCSfile: xbuffer.hpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef _XBUFFER_HPP_
#define _XBUFFER_HPP_ 

#include <qdialog.h>
#include <qpushbt.h>
#include <qlist.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qmsgbox.h>

#include "xbview.hpp"

//*************************************************************************

   typedef unsigned short USHORT;
   typedef unsigned long  ULONG;
   
   class ifstream;
   
//*************************************************************************

   class XBufferMainWindow : public QDialog
   {
      Q_OBJECT;
      typedef QDialog Inhereited;
      
   public:
      XBufferMainWindow(QWidget* parent=0, const char* name=0);
      ~XBufferMainWindow();
      
   protected:
      void initialize();
      void initializeBuffers(ifstream& ifs, unsigned bufferNumber);
      bool isBufferActive(const char* name);
   private:      
      QPushButton*        m_BtQuit;
      QPushButton*        m_BtStop;
      QPushButton*        m_BtRun;
      QMenuBar*           m_Menu;        
      XBufferView**       m_XBufferList;
      QList<QString>      m_StrList;
      QString             m_FileName;
      USHORT              m_SizeX;
      USHORT              m_SizeY;
      USHORT              m_SizeZ;
      USHORT              m_BufferSize;
      
   public slots:
      void stop();
      void run();
      void onOpenMenu();
      void onViewMenu();
      void onDelayMenu();
      void onAboutMenu();
      void onError(const QString& texto);
   };

//*************************************************************************
   
#endif


// fin del fichero
