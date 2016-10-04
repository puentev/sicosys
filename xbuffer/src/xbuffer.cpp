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
// File: ../ATCSimul/xbuffer/src/xbuffer.cpp
//
// Description:   fichero de implementacion de la clase "XBufferMainWindow"
//
// Notes:   Ver xbuffer.hpp 
//
// $Author: vpuente $ 
//
// $RCSfile: xbuffer.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: xbuffer.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";

//*************************************************************************

#include "xbuffer.hpp"

#include <qapp.h>
#include <qfont.h>
#include <qpainter.h>
#include <qfiledlg.h>
#include <qprinter.h>
#include <qrect.h>

#include "xdlg1.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream.h>

//*************************************************************************

   static const unsigned SIZEX = 400;
   static const unsigned SIZEY = 90;

//*************************************************************************


//*************************************************************************
//
//   XBufferMainWindow(QWidget* parent, const char* name);
//
//*************************************************************************
   
XBufferMainWindow :: XBufferMainWindow(QWidget* parent, const char* name)
                   : QDialog(parent,name),
                     m_SizeX(0),
                     m_SizeY(0),
                     m_SizeZ(0),
                     m_BufferSize(0),
                     m_XBufferList(0)
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);

   m_BtRun  = new QPushButton("Run",this,"Bt_run");
   m_BtStop = new QPushButton("Stop",this,"Bt_stop");
   m_BtQuit = new QPushButton("Quit",this,"Bt_quit");

   m_BtQuit -> setGeometry(SIZEX-90,SIZEY-40,70,25);
   m_BtStop -> setGeometry(SIZEX-180,SIZEY-40,70,25);
   m_BtRun  -> setGeometry(SIZEX-270,SIZEY-40,70,25);

   m_Menu = new QMenuBar(this,"menu");
      
   QPopupMenu* file = new QPopupMenu();
   QPopupMenu* view = new QPopupMenu();
   QPopupMenu* opts = new QPopupMenu();
   QPopupMenu* help = new QPopupMenu();
    
   m_Menu -> insertItem("&File",file);
   m_Menu -> insertItem("&View",view);
   m_Menu -> insertItem("&Options",opts);
   m_Menu -> insertItem("&Help",help);  
   
   file -> insertItem("Open",this,SLOT(onOpenMenu()));
   file -> insertItem("Exit",qApp,SLOT(quit()));
   view -> insertItem("Select buffers",this,SLOT(onViewMenu()));
   opts -> insertItem("Delay",this,SLOT(onDelayMenu()));
   help -> insertItem("About",this,SLOT(onAboutMenu()));

   connect(m_BtQuit,SIGNAL(clicked()), qApp, SLOT(quit()) );
   connect(m_BtStop,SIGNAL(clicked()), this, SLOT(stop()) );
   connect(m_BtRun, SIGNAL(clicked()), this, SLOT(run())  );

   initialize();
}


//*************************************************************************
//
//   ~XBufferMainWindow();
//
//*************************************************************************

XBufferMainWindow :: ~XBufferMainWindow() 
{

}


//*************************************************************************
//
//   void initialize();
//
//*************************************************************************

void XBufferMainWindow :: initialize()
{
   m_BtRun->setEnabled(FALSE);
   m_BtStop->setEnabled(FALSE);

}


//*************************************************************************
//
//   void run()
//
//*************************************************************************

void XBufferMainWindow :: run()
{
   ifstream ifs(m_FileName);
   if( ! ifs.good() )
   {
      QString texto = "El fichero '" + m_FileName +"' no puede abrirse";
      onError(texto);
   }
   
   USHORT bufferNumber;
   unsigned i, j, netSize;
   
   ifs.read((char*)&m_SizeX,sizeof(USHORT));
   ifs.read((char*)&m_SizeY,sizeof(USHORT));
   ifs.read((char*)&m_SizeZ,sizeof(USHORT));
   ifs.read((char*)&bufferNumber,sizeof(USHORT));
   ifs.read((char*)&m_BufferSize,sizeof(USHORT));
   
   QString texto;
   texto.sprintf("Dimensiones = (%d,%d,%d)\nBuffers = %d", 
                 m_SizeX,m_SizeY,m_SizeZ,bufferNumber);
   QMessageBox::information(this,"Information",texto);
   
   netSize = m_SizeX * m_SizeY * m_SizeZ;

   initializeBuffers(ifs,bufferNumber);
   unsigned** ptr = new unsigned*[bufferNumber];
   for( i=0; i<bufferNumber; i++ ) ptr[i] = new unsigned[netSize];
   
   while(1)
   {
      ULONG time;
      ifs.read((char*)&time,sizeof(ULONG));
      if( ifs.eof() ) break;
      for( j=0; j<netSize; j++ )
      {
         USHORT x,y,z;
      
         ifs.read((char*)&x,sizeof(USHORT));
         if( ifs.eof() ) break;
         ifs.read((char*)&y,sizeof(USHORT));
         if( ifs.eof() ) break;
         ifs.read((char*)&z,sizeof(USHORT));
         if( ifs.eof() ) break;

         for( i=0; i<bufferNumber; i++)
         {
            USHORT occupation;
            ifs.read((char*)&occupation,sizeof(USHORT));
            if( ifs.eof() ) break;
            ptr[i][j] = occupation;
         }
      } // for
      
      for( i=0; i<bufferNumber; i++ )
      {
         if( *(m_XBufferList+i)  )
            (*(m_XBufferList+i))->displayBufferState(ptr[i]);
         qApp->processEvents(5);
      }
      
   } // while

   for( i=0; i<bufferNumber; i++ )  delete[] ptr[i];
   delete[] ptr;   
   
   ifs.close();
}


//*************************************************************************
//
//   void stop();
//
//*************************************************************************

void XBufferMainWindow :: stop()
{

}


//*************************************************************************
//
//   void onOpenMenu();
//
//*************************************************************************

void XBufferMainWindow :: onOpenMenu()
{
   m_FileName = QFileDialog::getOpenFileName(".","*.dat");
   if( m_FileName != "" ) 
   {
      m_BtRun -> setEnabled(TRUE);
      ifstream ifs(m_FileName);
      USHORT temp, number;
      ifs.read((char*)&temp,sizeof(USHORT));
      ifs.read((char*)&temp,sizeof(USHORT));
      ifs.read((char*)&temp,sizeof(USHORT));
      ifs.read((char*)&number,sizeof(USHORT));
      ifs.read((char*)&temp,sizeof(USHORT));

      for( int j=0; j<number; j++ )
      {
         char name[32];
         unsigned short size;
         ifs.read(name,32);
         ifs.read((char*)&size,sizeof(unsigned short));
         name[31] = '\0';
         if( ifs.eof() ) break;
         m_StrList.append(new QString(name));
      }
      
      ifs.close(); 
   }
}

   
//*************************************************************************
//
//   void onViewMenu();
//
//*************************************************************************

void XBufferMainWindow :: onViewMenu()
{
   XDialog1 dlg(m_StrList,this);
   dlg.exec();
}


//*************************************************************************
//
//   void onDelayMenu();
//
//*************************************************************************

void XBufferMainWindow :: onDelayMenu()
{

}

//*************************************************************************
//
//   void onAboutMenu();
//
//*************************************************************************

void XBufferMainWindow :: onAboutMenu()
{
   QMessageBox::information( this, 
                             "Product information",
                             "Grupo de Arquitectura de Computadores\n"
                             "Universidad de Cantabria\n"
                             "Version 1.0");
}


//*************************************************************************
//
//   void onError(const QString& texto);
//
//*************************************************************************

void XBufferMainWindow :: onError(const QString& texto)
{
   QMessageBox::critical( this, 
                          "Error",
                          texto );
}


//*************************************************************************
//:
//   void initializeBuffers(ifstream& ifs, unsigned number);
//:
//*************************************************************************

void XBufferMainWindow :: initializeBuffers(ifstream& ifs, unsigned number)
{
   unsigned* ptr = new unsigned[m_SizeX*m_SizeY];
   for(unsigned j=0; j<m_SizeX*m_SizeY; j++ ) *(ptr+j) = 0;
   static unsigned lastNumber = number;
   
   if(m_XBufferList)
   {
      delete[] m_XBufferList;
      for( unsigned i=0; i<lastNumber; i++ )
         delete *(m_XBufferList+i);
   }
 
   m_XBufferList = new XBufferView*[number];
   lastNumber = number;
   
   for( unsigned i=0; i<number; i++ )
   {
      char name[32];
      unsigned short size;
      ifs.read(name,32);
      ifs.read((char*)&size,sizeof(unsigned short));
      if( ifs.eof() ) break;
      if( isBufferActive(name) )
      {

         XBufferView* bview = new XBufferView( m_SizeX,
                                               m_SizeY,
                                               size,
                                               0,
                                               name );
         m_XBufferList[i] = bview;

         bview->show();
         bview->displayBufferState(ptr);   
      }
      else
      {
         m_XBufferList[i] = 0;
      }
   }
   
   delete[] ptr;
}


//*************************************************************************
//:
//   bool isBufferActive(const char* name);
//:
//*************************************************************************

bool XBufferMainWindow :: isBufferActive(const char* name)
{
   QString temp = name;
   for( int i=0; i<XDialog1::getBuffersActive().count(); i++ )
   {
      if( name == *(XDialog1::getBuffersActive().at(i)) ) return TRUE;
   }

   return FALSE;
}


//*************************************************************************
//*************************************************************************

void main(int argc, char** argv)
{
   QApplication app(argc,argv);
   // QApplication::setFont(QFont("times",11)); 
   
   XBufferMainWindow mainWindow;
   
   app.setMainWidget(&mainWindow);
   
   mainWindow.show();
   app.exec();

}


//*************************************************************************

// fin del fichero
