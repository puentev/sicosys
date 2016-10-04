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
// File: ../ATCSimul/xsimul/src/przdlg2.cpp
//
// Description:   fichero de implementacion de la clase "PRZDialog2"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: przdlg2.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: przdlg2.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";


//*************************************************************************

#include "przdlg2.hpp"

#include <qpushbt.h>
#include <qlabel.h>
#include <qmsgbox.h>

#include <fstream.h>
#include "PRZString.hpp"

//*************************************************************************

   #define SIZEX 300
   #define SIZEY 400

//*************************************************************************

QString PRZDialog2 :: sm_SimulationId = "";
int     PRZDialog2 :: sm_IndexSelected = 0;
  
//*************************************************************************
//
//   PRZDialog2( QWidget *parent, const char *name )
//
//*************************************************************************

PRZDialog2 :: PRZDialog2( QWidget *parent, const char *name )
            : QDialog( parent, name, TRUE )
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);
   setCaption("Simulaciones disponibles");

   QPushButton* accept = new QPushButton( "Ok", this );
   QPushButton* cancel = new QPushButton( "Cancel", this );
   QLabel* labelSimul  = new QLabel("Selected", this);
   m_EditSimul = new QLineEdit(this);
   m_ListBox   = new QListBox(this);
   m_ListBox  -> setScrollBar(TRUE);
   
   accept -> setGeometry( 60,SIZEY-50,70,30 );
   cancel -> setGeometry( SIZEX-130,SIZEY-50,70,30 );

   m_ListBox  -> setGeometry( 20,20,250,270 );
   labelSimul -> setGeometry( 20,300,50,30 );
   m_EditSimul-> setGeometry( 100,300,170,30 );
      
   accept     -> setDefault(TRUE);
   accept     -> setFont(QFont("helvetica",12,QFont::Bold));
   cancel     -> setFont(QFont("helvetica",12,QFont::Bold));
   labelSimul -> setFont(QFont("helvetica",12,QFont::Bold));
   m_EditSimul-> setFont(QFont("helvetica",12,QFont::Bold));
   m_ListBox  -> setFont(QFont("helvetica",12,QFont::Bold));
   
   connect( accept, SIGNAL(clicked()), this, SLOT(onAcceptClick()) );
   connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
   connect( m_ListBox, SIGNAL(highlighted(const QString&)), 
            m_EditSimul, SLOT(setText(const QString&)) );
            
   refreshListBox();
   m_ListBox->setSelected(sm_IndexSelected,TRUE);
}

    
//*************************************************************************
//
//   void refreshListBox()
//
//*************************************************************************

void PRZDialog2 :: refreshListBox()
{
   
   ifstream fileINI("./ATCSimul.ini");
   QString simulaSgm;
   if( !fileINI.good() )
   {
      QMessageBox::information( this, 
                                " Error ",
                                "ATCSimul.ini not found");

      return;
   }
   
   while(1)
   {
      char buffer[256];
      fileINI.getline(buffer,256);
      if( fileINI.eof() ) break;
      QString line(buffer);
      if( line.contains("SimulationFile") )
      {
         simulaSgm = line.right(line.length()-line.find('"')-1);
         simulaSgm = simulaSgm.left(simulaSgm.findRev('"',simulaSgm.length()));
         break;
      }
   }
   
   ifstream file((const char*)simulaSgm);
   if( !file.good() )
   {
      QString s;
      s.sprintf("Not found: '%s'",(const char*)simulaSgm);
      QMessageBox::information( this, 
                                " Error : Simula.sgm",
                                s);

      return;
   }
   
   while( 1 )
   {
      char buffer[256];
      file.getline(buffer,256);
      if( file.eof() ) break;
      PRZString string(buffer);
      string = PRZString::stripBlanks(string);
      
      if( string.indexOf("<Simulation ") )
      {        
         string = string.subString(string.indexOf('=')+1);
         string.removeLast();
         string.replace('"', ' ');  
         string = PRZString::stripBlanks(string);
         m_ListBox->insertItem(QString(string));
      }
   } 
}


//*************************************************************************
//
//   void onAcceptClick()
//
//*************************************************************************

void PRZDialog2 :: onAcceptClick()
{
   PRZDialog2::sm_SimulationId = m_EditSimul->text();
   PRZDialog2::sm_IndexSelected= m_ListBox->currentItem();
   close();
}

//*************************************************************************


// fin del fichero
