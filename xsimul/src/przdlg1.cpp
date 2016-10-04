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
// File: ../ATCSimul/xsimul/src/przdlg1.cpp
//
// Description:   fichero de implementacion de la clase "PRZDialog1"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: przdlg1.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: przdlg1.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";


//*************************************************************************

#include "przdlg1.hpp"

#include <iostream.h>

#include <qpushbt.h>
#include <qmsgbox.h>
#include <qgrpbox.h>

//*************************************************************************

   #define SIZEX 400
   #define SIZEY 470

   #define _CYCLES_   PRZString(" -c ")
   #define _TRAFFIC_  PRZString(" -t ")
   #define _LOAD_     PRZString(" -p ")
   #define _BIMODAL_  PRZString(" -B ")
   #define _BUFFERS_  PRZString(" -b ")
   #define _PACKET_   PRZString(" -L ")
   #define _BUFSIZE_  PRZString(" -u ")
   
   
//*************************************************************************
  
double    PRZDialog1 :: m_Load   = 0.1;
unsigned  PRZDialog1 :: m_Cycles = 5000;
unsigned  PRZDialog1 :: m_PacketSize = 16;
unsigned  PRZDialog1 :: m_BufferSize = 64;
unsigned  PRZDialog1 :: m_Traza  = 0;
PRZString PRZDialog1 :: m_Traffic= "RANDOM";
unsigned  PRZDialog1 :: m_CurrentTraffic = 0;
unsigned  PRZDialog1 :: m_Bimodal = 0;
unsigned  PRZDialog1 :: m_Buffers = 0;
double    PRZDialog1 :: m_ProbBimodal = 0.0;
double    PRZDialog1 :: m_MsgBimodal = 0;
unsigned  PRZDialog1 :: m_NodosHot = 0;
unsigned  PRZDialog1 :: m_ProbHot = 0;
double    PRZDialog1 :: m_P10Flash = 0.01;
double    PRZDialog1 :: m_AlphaFlash = 10.0;
double    PRZDialog1 :: m_SigmaLocal = 1.0;
PRZString PRZDialog1 :: m_BuffersFile= "";
PRZString PRZDialog1 :: m_Parameters= "";


//*************************************************************************
//
//   PRZDialog1( QWidget *parent, const char *name )
//
//*************************************************************************

PRZDialog1 :: PRZDialog1( QWidget *parent, const char *name )
            : QDialog( parent, name, TRUE )
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);
   setCaption("Simulation parameters");

   QGroupBox* line1 = new QGroupBox("Inputs",this);
   QGroupBox* line2 = new QGroupBox("Outputs",this);
   QGroupBox* line3 = new QGroupBox("Bimodal traffic",this);

   QPushButton* accept = new QPushButton( "Ok", this );
   QPushButton* cancel = new QPushButton( "Cancel", this );
   QLabel* labelLoad   = new QLabel("Load", this);
   QLabel* labelCycles = new QLabel("Cycles",this);
   QLabel* labelTraffic= new QLabel("Traffic",this);
   QLabel* labelBuffer = new QLabel("Buffer size",this);
   QLabel* labelPacket = new QLabel("Packet length",this);
      
   m_LabelP1      = new QLabel("",this);
   m_LabelP2      = new QLabel("",this);
   m_LabelP3      = new QLabel("Size",this);
   m_LabelP4      = new QLabel("Prob.",this);
   m_LabelP5      = new QLabel("File name",this);
   m_EditLoad     = new QLineEdit(this);
   m_EditCycles   = new QLineEdit(this);
   m_EditBuffer   = new QLineEdit(this);
   m_EditPacket   = new QLineEdit(this);
   m_EditP1       = new QLineEdit(this);
   m_EditP2       = new QLineEdit(this);
   m_EditP3       = new QLineEdit(this);
   m_EditP4       = new QLineEdit(this);
   m_EditP5       = new QLineEdit(this);

   m_ComboTraffic = new QComboBox(this);
   m_CheckBimodal = new QCheckBox("Enable",this);
   m_CheckTraza   = new QCheckBox("Trace",this);
   m_CheckBuffer  = new QCheckBox("Buffers state",this);
      
   line1         -> setGeometry( 15,15,370,250 );
   line2         -> setGeometry( 15,290,370,100 );
   line3         -> setGeometry( 225,125,145,120 );

   accept        -> setGeometry( 90,SIZEY-40,70,25 );
   cancel        -> setGeometry( SIZEX-160,SIZEY-40,70,25 );

   labelTraffic  -> setGeometry( 30,50,70,25 );
   labelLoad     -> setGeometry( 30,90,70,25 );
   labelCycles   -> setGeometry( 30,130,70,25 );
   labelBuffer   -> setGeometry( 30,170,70,25 );
   labelPacket   -> setGeometry( 30,210,70,25 );
   m_ComboTraffic-> setGeometry( 90,50,115,25 );
   m_EditLoad    -> setGeometry( 110,90,95,25 );
   m_EditCycles  -> setGeometry( 110,130,95,25 );
   m_EditBuffer  -> setGeometry( 110,170,95,25 );
   m_EditPacket  -> setGeometry( 110,210,95,25 );
   m_EditP1      -> setGeometry( 280,50,75,25 );
   m_EditP2      -> setGeometry( 280,90,75,25 );   
   m_EditP3      -> setGeometry( 280,170,75,25 );
   m_EditP4      -> setGeometry( 280,210,75,25 );   
   m_EditP5      -> setGeometry( 250,350,100,25 );   
   m_LabelP1     -> setGeometry( 240,50,60,25 );
   m_LabelP2     -> setGeometry( 240,90,60,25 );      
   m_LabelP3     -> setGeometry( 240,170,60,25 );
   m_LabelP4     -> setGeometry( 240,210,60,25 );      
   m_LabelP5     -> setGeometry( 180,350,60,25 );         
   m_CheckBimodal-> setGeometry( 240,140,120,25 );
   m_CheckTraza  -> setGeometry( 30,320,120,25 );
   m_CheckBuffer -> setGeometry( 30,350,120,25 );
   
   accept -> setDefault(TRUE);

   m_EditLoad   -> setEnabled(TRUE);
   m_EditCycles -> setEnabled(TRUE);
   m_EditBuffer -> setEnabled(TRUE);
   m_EditPacket -> setEnabled(TRUE);
   m_EditP1 -> setEnabled(FALSE);
   m_EditP2 -> setEnabled(FALSE);
   m_EditP3 -> setEnabled(FALSE);
   m_EditP4 -> setEnabled(FALSE);
   m_EditP5 -> setEnabled(FALSE);

   m_CheckTraza  -> setChecked(PRZDialog1::m_Traza);
   if( m_Bimodal )
   {
      m_CheckBimodal ->setChecked(TRUE);
      onBimodalCheck(TRUE);
   }
   
   m_EditP3 -> setText((char*)PRZString(m_MsgBimodal));
   m_EditP4 -> setText((char*)PRZString(m_ProbBimodal));
      
   connect( accept, SIGNAL(clicked()), SLOT(onAcceptClick()) );
   connect( m_ComboTraffic, SIGNAL(activated(const QString&)), 
            this, SLOT(onTrafficSelect(const QString&)) );
   connect( m_CheckBimodal, SIGNAL(toggled(bool)), 
            this, SLOT(onBimodalCheck(bool)) );
   connect( m_CheckBuffer, SIGNAL(toggled(bool)), 
            this, SLOT(onBufferCheck(bool)) );

   connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
   
   m_EditLoad -> setFocus();
   QString text;
   text.sprintf("%1.3f",PRZDialog1::m_Load);
   m_EditLoad -> setText(text);
   
   m_EditCycles -> setText((char*)PRZString(m_Cycles));
   m_EditPacket -> setText((char*)PRZString(m_PacketSize));
   m_EditBuffer -> setText((char*)PRZString(m_BufferSize));
   
   m_ComboTraffic->insertItem("RANDOM");
   m_ComboTraffic->insertItem("PERMUTATION");
   m_ComboTraffic->insertItem("PERFECT-SUFFLE");
   m_ComboTraffic->insertItem("BIT-REVERSAL");
   m_ComboTraffic->insertItem("LOCAL");
   m_ComboTraffic->insertItem("HOT-SPOT");
   m_ComboTraffic->insertItem("FLASH");
   m_ComboTraffic->setCurrentItem(m_CurrentTraffic);   
   onTrafficSelect(m_ComboTraffic->currentText());
}
    

//*************************************************************************
//
//   void onAcceptClick();
//
//*************************************************************************

void PRZDialog1 :: onAcceptClick()
{
   m_Traza = (m_CheckTraza->isChecked()) ? 1 : 0;
   m_CurrentTraffic = m_ComboTraffic->currentItem();
   m_Traffic = m_ComboTraffic->currentText();

   if( m_Traffic == "HOT-SPOT" )
   {
      QString nodos = m_EditP1->text();
      QString prob  = m_EditP2->text();
      bool isValid;
      m_NodosHot = nodos.toULong(&isValid);
      if( ! validar(isValid) ) return;
      m_ProbHot = prob.toULong(&isValid);
      if( ! validar(isValid) ) return;
      m_Traffic = PRZString("HOT-SPOT") + "," + 
                  PRZString(m_NodosHot) + "," +
                  PRZString(m_ProbHot);
   }
   else if( m_Traffic == "FLASH" )
   {
      QString p10   = m_EditP1->text();
      QString alpha = m_EditP2->text();
      bool isValid;
      m_P10Flash = p10.toDouble(&isValid);
      if( ! validar(isValid) ) return;
      m_AlphaFlash = alpha.toDouble(&isValid);
      if( ! validar(isValid) ) return;
      m_Traffic = PRZString("FLASH") + "," + 
                  PRZString(m_P10Flash) + "," +
                  PRZString(m_AlphaFlash);
   }

   else if( m_Traffic == "LOCAL" )
   {
      QString sigma = m_EditP1->text();
      bool isValid;
      m_SigmaLocal = sigma.toDouble(&isValid);
      if( ! validar(isValid) ) return;
      m_Traffic = PRZString("LOCAL") + "," + 
                  PRZString(m_SigmaLocal);
   }

   if( m_CheckBimodal->isChecked() )
   {
      QString size = m_EditP3->text();
      QString prob = m_EditP4->text();
      bool isValid;
      m_MsgBimodal = size.toDouble(&isValid);
      if( ! validar(isValid) ) return;
      m_ProbBimodal = prob.toDouble(&isValid);
      if( ! validar(isValid) ) return;
   }

   if( m_CheckBuffer->isChecked() )
   {
      m_BuffersFile = m_EditP5 -> text();
   }
   
   QString value = m_EditLoad->text();
   bool isValid;
   m_Load = value.toDouble(&isValid);   
   if( ! validar(isValid) ) return;
   
   value = m_EditCycles->text();
   m_Cycles= value.toULong(&isValid);
   if( ! validar(isValid) ) return;

   value = m_EditPacket->text();
   m_PacketSize = value.toULong(&isValid);
   if( ! validar(isValid) ) return;

   value = m_EditBuffer->text();
   m_BufferSize = value.toULong(&isValid);
   if( ! validar(isValid) ) return;

   m_Parameters = "";
   
   m_Parameters += _CYCLES_  + PRZString(m_Cycles);
   m_Parameters += _TRAFFIC_ + PRZString(m_Traffic);   
   m_Parameters += _LOAD_    + PRZString(m_Load);

   if( m_CheckBimodal->isChecked() )
   {
      m_Parameters += _BIMODAL_ + PRZString(m_MsgBimodal) + 
                      PRZString(",") + PRZString(m_ProbBimodal);
   }   

   if( m_CheckBuffer->isChecked() && (m_BuffersFile!="") )
   {
      m_Parameters += _BUFFERS_ + PRZString(m_BuffersFile);
   }   

   if( m_PacketSize )
   {
      m_Parameters += _PACKET_ + PRZString(m_PacketSize);
   }   

   if( m_BufferSize )
   {
      if(m_BufferSize!=0)
      {
         m_Parameters += _BUFSIZE_ + PRZString(m_BufferSize);
      }
   }   
   
   close();
}


//*************************************************************************
//
//   void onTrafficSelect(const char* text);
//
//*************************************************************************

void PRZDialog1 :: onTrafficSelect(const QString& text)
{
   m_Traffic = (const char*)text;
   
   if( m_Traffic == "HOT-SPOT" )
   {
      m_EditP1 -> setEnabled(TRUE);
      m_EditP2 -> setEnabled(TRUE);
      m_LabelP1-> setText("nodes");
      m_LabelP2-> setText("prob.");
      m_EditP1 -> setText((char*)PRZString(m_NodosHot));
      m_EditP2 -> setText((char*)PRZString(m_ProbHot));
      m_EditP1 -> setFocus();
   }
   else if( m_Traffic == "FLASH" )
   {
      m_EditP1 -> setEnabled(TRUE);
      m_EditP2 -> setEnabled(TRUE);
      m_LabelP1-> setText("p10");
      m_LabelP2-> setText("alpha");
      m_EditP1 -> setText((char*)PRZString(m_P10Flash));
      m_EditP2 -> setText((char*)PRZString(m_AlphaFlash));
      m_EditP1 -> setFocus();
   }
   else if( m_Traffic == "LOCAL" )
   {
      m_EditP1 -> setEnabled(TRUE);
      m_EditP2 -> setEnabled(FALSE);   
      m_LabelP1-> setText("sigma");
      m_LabelP2-> setText("");
      m_EditP1 -> setText((char*)PRZString(m_SigmaLocal));
      m_EditP1 -> setFocus();
   }
   else
   {
      m_EditP1 -> setEnabled(FALSE);
      m_EditP2 -> setEnabled(FALSE);
      m_LabelP1-> setText("");
      m_LabelP2-> setText("");      
   }
}


//*************************************************************************
//
//   void onBimodalCheck(bool check);
//
//*************************************************************************

void PRZDialog1 :: onBimodalCheck(bool check)
{
   if( check )
   {
      m_EditP3 -> setEnabled(TRUE);
      m_EditP4 -> setEnabled(TRUE);
      m_EditP3 -> setFocus();
      m_Bimodal = 1;
   }
   else
   {
      m_EditP3 -> setEnabled(FALSE);
      m_EditP4 -> setEnabled(FALSE);
      m_Bimodal = 0;
   }
}


//*************************************************************************
//
//   void onBufferCheck(bool check);
//
//*************************************************************************

void PRZDialog1 :: onBufferCheck(bool check)
{
   if( check )
   {
      m_EditP5 -> setEnabled(TRUE);
      m_EditP5 -> setFocus();
   }
   else
   {
      m_EditP5 -> setEnabled(FALSE);
   }
}

//*************************************************************************
//
//   bool validar(bool isValid)
//
//*************************************************************************

bool PRZDialog1 :: validar(bool isValid)
{
   if( isValid == FALSE )
   {
      QMessageBox mb( "Validacion",
                      "El valor introducido no es valido",
                      QMessageBox::Information,
                      QMessageBox::Yes | QMessageBox::Default, 0 , 0 );
      mb.setFont(QFont("helvetica",12));
      mb.setButtonText( QMessageBox::Yes, "Accept" );
      mb.exec();
   }
   
   return isValid;
}


//*************************************************************************

// fin del fichero
