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
// File: ../ATCSimul/xsimul/src/xsimul.cpp
//
// Clase      : XSimulationView
//
// Hereda de  : QDialog
//
// Description:  
//
// $Author: vpuente $ 
//
// $RCSfile: xsimul.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: xsimul.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";


//*************************************************************************

#include "xsimul.hpp"

#include <qapp.h>
#include <qfont.h>
#include <qpainter.h>
#include <qfiledlg.h>
#include <qprinter.h>
#include <qrect.h>

#include <stdlib.h>
#include <fstream.h>

#include "przdlg1.hpp"
#include "przdlg2.hpp"

#include <ATCSimulator.hpp>

//*************************************************************************

#define MINIMO(x,y) (((x)<(y)) ? (x) : (y) )
#define MAXIMO(x,y) (((x)>(y)) ? (x) : (y) )

#define TEMP_FILE1 "xsimul.tmp"

//*************************************************************************

   static const unsigned SIZEX   = 750;
   static const unsigned SIZEY   = 550;
   static const unsigned X_TICKS = 15;
   static const unsigned Y_TICKS = 10;
   
   XSimulationView* XSimulationView :: instance = 0;

//*************************************************************************
//*************************************************************************
//
//  f: XSimulationView(QWidget* parent, const char* name);
//
//  d:
//
//*************************************************************************
   
XSimulationView :: XSimulationView(QWidget* parent, const char* name)
                 : QDialog(parent,name),
                   m_SimulationId(0),
                   m_SimulationStop(FALSE),
                   m_Load(0.0),
                   m_FirstTime(1),
                   m_ResizesNumber(0),
                   m_Clock(0),
                   m_Latencia(0),
                   m_MessagesTx(0),
                   m_MessagesRx(0)
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);
   setCaption("Average Latency");
   m_PointList.setAutoDelete(TRUE);

   m_BtQuit = new QPushButton("Quit",this,"Btquit");
   m_BtStop = new QPushButton("Stop",this,"Btstop");
   m_BtRun  = new QPushButton("Run",this,"Btrun");
   m_BtPrint= new QPushButton("Print",this,"Btprint");
   m_BtShow = new QPushButton("Result",this,"Btshow");

   m_MsgTx  = new QLabel("0",this );
   m_MsgRx  = new QLabel("0",this );
   m_MsgDiff= new QLabel("0",this );

   QLabel* stMsgTx = new QLabel("Messages Tx :",this );
   QLabel* stMsgRx = new QLabel("Messages Rx :",this );      
   QLabel* stDiff  = new QLabel("Difference :",this );
   QLabel* xTitle  = new QLabel("Simulated cycles",this);
   QLabel* yTitle  = new QLabel("Average Latency (cycles)",this);
      
   m_BtQuit -> setFont(QFont("helvetica",12,QFont::Bold));
   m_BtPrint-> setFont(QFont("helvetica",12,QFont::Bold));
   m_BtStop -> setFont(QFont("helvetica",12,QFont::Bold));
   m_BtRun  -> setFont(QFont("helvetica",12,QFont::Bold));
   m_BtShow -> setFont(QFont("helvetica",12,QFont::Bold));
     
   m_MsgTx  -> setFont(QFont("helvetica",12));
   m_MsgRx  -> setFont(QFont("helvetica",12));
   m_MsgDiff-> setFont(QFont("helvetica",12));
   stMsgTx  -> setFont(QFont("helvetica",12,QFont::Bold));
   stMsgRx  -> setFont(QFont("helvetica",12,QFont::Bold));
   stDiff   -> setFont(QFont("helvetica",12,QFont::Bold));
   xTitle   -> setFont(QFont("helvetica",12,QFont::Bold));
   yTitle   -> setFont(QFont("helvetica",12,QFont::Bold));
   
   m_Pixmap      = new QPixmap(size());
   m_LCD         = new QLCDNumber(6,this,"lcd");
   m_ProgressBar = new QProgressBar(this,"progressBar");
   m_Menu        = new QMenuBar(this,"menu");
      
   QPopupMenu* file = new QPopupMenu();
   QPopupMenu* opts = new QPopupMenu();
   QPopupMenu* help = new QPopupMenu();
    
   file -> insertItem("Open",this,SLOT(onOpenMenu()));
   file -> setFont(QFont("helvetica",11,QFont::Bold));
   file -> insertItem("Exit",qApp,SLOT(quit()));
   file -> setFont(QFont("helvetica",11,QFont::Bold));
   opts -> insertItem("Settings",this,SLOT(onSettingsMenu()));
   opts -> setFont(QFont("helvetica",11,QFont::Bold));
   help -> insertItem("About",this,SLOT(onAboutMenu()));
   help -> setFont(QFont("helvetica",11,QFont::Bold));
      
   m_Pixmap -> fill( backgroundColor() );
   m_BtQuit -> setGeometry(SIZEX-100,SIZEY-50,70,25);
   m_BtPrint-> setGeometry(SIZEX-190,SIZEY-50,70,25);
   m_BtShow -> setGeometry(SIZEX-280,SIZEY-50,70,25);
   m_BtStop -> setGeometry(SIZEX-370,SIZEY-50,70,25);
   m_BtRun  -> setGeometry(SIZEX-460,SIZEY-50,70,25);
   m_MsgTx  -> setGeometry(170,SIZEY-83,100,20);
   m_MsgRx  -> setGeometry(170,SIZEY-62,100,20);
   m_MsgDiff-> setGeometry(170,SIZEY-41,100,20);
   stMsgTx  -> setGeometry(40,SIZEY-83,120,20);
   stMsgRx  -> setGeometry(40,SIZEY-62,120,20);
   stDiff   -> setGeometry(40,SIZEY-41,120,20);
   xTitle   -> setGeometry(SIZEX-160,SIZEY-95,150,20);
   yTitle   -> setGeometry(40,80,200,20);
   m_LCD        -> setGeometry(SIZEX-170,50,140,30);
   m_ProgressBar-> setGeometry(SIZEX-340,50,140,30);
   m_ProgressBar-> setProgress(0);

   m_BtRun  -> setDefault(TRUE);
   m_BtRun  -> setEnabled(FALSE);
   m_BtStop -> setEnabled(FALSE);
   m_BtShow -> setEnabled(FALSE);
   
   m_Menu   -> insertItem("&File",file);
   m_Menu   -> insertItem("&Options",opts);
   m_Menu   -> insertItem("&Help",help);
   m_Menu   -> setFont(QFont("helvetica",12,QFont::Bold));
        
   connect( m_BtQuit, SIGNAL(clicked()), 
            this,     SLOT(onButtonQuitClick()) );
            
   connect( m_BtPrint, SIGNAL(clicked()), 
            this,      SLOT(onButtonPrintClick()) );
                        
   connect( m_BtRun, SIGNAL(clicked()), 
            this,    SLOT(onButtonRunClick()) );

   connect( m_BtStop, SIGNAL(clicked()), 
            this,     SLOT(onButtonStopClick()) );
            
   connect( m_BtShow, SIGNAL(clicked()), 
            this,     SLOT(onButtonResultClick()) );  
            
   connect( this, SIGNAL(signalDataReceived(double,double,double,double)), 
            this, SLOT(onDataReceived(double,double,double,double)) );
            
   connect( this, SIGNAL(signalResultReceived(const PRZString&)), 
            this, SLOT(onResultReceived(const PRZString&)) );

   XSimulationView::instance = this;     
}


//*************************************************************************
//
//  f: ~XSimulationView();
//
//  d:
//
//*************************************************************************

XSimulationView :: ~XSimulationView() 
{
   delete m_Pixmap;
}
   

//*************************************************************************
//
//  f: void dispatchNotificationEvent(const PRZNotificationEvent& event);
//
//  d:
//
//*************************************************************************

void XSimulationView :: dispatchNotificationEvent(const PRZNotificationEvent& event)
{
   int id = event.id();   
   if( id == EV_LATENCY )
   {
      double d0 = event.valueAt(0);
      double d1 = event.valueAt(1);
      double d2 = event.valueAt(2);
      double d3 = event.valueAt(3);      
      emit signalDataReceived(d0,d1,d2,d3);
   }
   else if( id == EV_RESULT )
   {
      emit signalResultReceived(event.text());
   }
}     

  
//*************************************************************************
//
//  f: void onButtonRunClick();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onButtonRunClick()
{
   m_BtRun  -> setEnabled(FALSE);
   m_BtStop -> setEnabled(TRUE);
   m_BtShow -> setEnabled(TRUE);

   setMessagesTx(0);
   setMessagesRx(0);
   setMsgDifference(0);

   m_LCD->display(0);
   m_ProgressBar->setTotalSteps(PRZDialog1::getCycles()/100);
   m_ProgressBar->reset();
   m_ProgressBar->setProgress(0);

   initialize(1000,10,0,0);
   m_PointList.clear();
   m_FirstTime = 1;
   m_ResizesNumber = 0;
   m_Clock = m_Latencia = 0;
   

   PRZString param;
   param = PRZString("ATCSimul -q -s ") + 
           PRZString(PRZDialog2::getSimulationId()) +
           PRZString(PRZDialog1::getParameters());
      
   cout << "Simular = " << param << endl;
   m_SimulationId = ATCSIMULATOR()->createSimulation(param);
   ATCSIMULATOR()->getSimulation(m_SimulationId)->enableNotification();
   startHandlingNotificationsFor( ATCSIMULATOR()->getSimulation(m_SimulationId) );
   m_SimulationStop = FALSE;
   
   for( unsigned c=0; c<PRZDialog1::getCycles(); c++ )
   {
      if( m_SimulationStop ) break;  // Por accion del boton "stop"
      ATCSIMULATOR()->getSimulation(m_SimulationId)->run(1);
      qApp->processEvents(10);
   }
   
   ATCSIMULATOR()->getSimulation(m_SimulationId)->writeResults();
   stopHandlingNotificationsFor( ATCSIMULATOR()->getSimulation(m_SimulationId) );
   ATCSIMULATOR()->deleteSimulation(m_SimulationId);
   m_SimulationId   = 0;
   m_SimulationStop = FALSE;
}


//*************************************************************************
//
//  f: void onButtonStopClick();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onButtonStopClick()
{
   m_BtRun  -> setEnabled(TRUE);
   m_BtStop -> setEnabled(FALSE);
   m_BtShow -> setEnabled(TRUE);
   m_SimulationStop = TRUE;
}


//*************************************************************************
//
//  f: void onDataReceived(double v1, double v2, double v3, double v4);
//
//  d:
//
//*************************************************************************

void XSimulationView :: onDataReceived(double v1, double v2, double v3, double v4)
{
   m_Clock      = v1;
   m_Latencia   = v2;
   m_MessagesTx = v3;
   m_MessagesRx = v4;
   
   if( PRZDialog1::isTrazaEnable() )
   {
      PRZString traza;
      traza.sprintf("Rx: %6d  %4.2f", unsigned(m_Clock), m_Latencia);
      cout << traza << endl;
   }
   if( m_Latencia ) 
   {
      setMessagesTx(m_MessagesTx);
      setMessagesRx(m_MessagesRx);
      setMsgDifference(m_MessagesTx-m_MessagesRx);  
      m_ProgressBar->setProgress(m_Clock/100);
      m_LCD -> display(m_Clock);
      drawPoint(m_Clock,m_Latencia);
   }
}


//*************************************************************************
//
//  f: void onResultReceived(const PRZString& text);
//
//  d:
//
//*************************************************************************

void XSimulationView :: onResultReceived(const PRZString& text)
{      
   m_DatosSimulacion = text;

   m_BtRun  -> setEnabled(TRUE);
   m_BtStop -> setEnabled(FALSE);
   m_BtShow -> setEnabled(TRUE);
   
   onButtonResultClick();
}
   
      
//*************************************************************************
//
//  f: void onButtonResultClick();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onButtonResultClick()
{   
   QMessageBox mb( "Simulation results",
                   m_DatosSimulacion,
                   QMessageBox::Information,
                   QMessageBox::Yes,
                   QMessageBox::No,
                   QMessageBox::Cancel|QMessageBox::Default|QMessageBox::Escape,
                   this );
                   
   mb.setButtonText( QMessageBox::Yes,    "Print" );
   mb.setButtonText( QMessageBox::No,     "Save" );
   mb.setButtonText( QMessageBox::Cancel, "Cancel" );

   ofstream tempFile;
   QString text;
    
   switch( mb.exec() ) 
   {
      case QMessageBox::Yes:      // Print and exit
           tempFile.open(TEMP_FILE1);
           tempFile << m_DatosSimulacion;
           tempFile.close();
           text.sprintf("lp %s",TEMP_FILE1);
           system(text);
           break;
                 
      case QMessageBox::No:       // Save and exit
           text = QFileDialog::getOpenFileName();
           tempFile.open(text);
           tempFile << m_DatosSimulacion;
           tempFile.close();
           break;
                 
      case QMessageBox::Cancel:   // Exit without saving        
           break;
   }
 
}
   

//*************************************************************************
//
//  f: void initialize( unsigned xmax, unsigned ymax, 
//                      unsigned xmin, unsigned ymin );
//
//  d:
//
//*************************************************************************

void XSimulationView :: initialize( unsigned xmax, unsigned ymax, 
                                   unsigned xmin, unsigned ymin )
{
   m_Xmax = xmax;
   m_Ymax = ymax;
   m_Xmin = xmin;
   m_Ymin = ymin;
    
   m_X0 = 80;
   m_Y0 = 120;
   m_X1 = 720;
   m_Y1 = 430;
   
   QPainter painter(m_Pixmap);
   drawFrame(&painter);
   painter.end();
    
   bitBlt(this,0,0,m_Pixmap);
}


//*************************************************************************
//
//  f: void drawFrame(QPainter* painter);
//
//  d:
//
//*************************************************************************
   
void XSimulationView :: drawFrame(QPainter* painter)
{
   painter->fillRect(0,0,SIZEX,SIZEY,backgroundColor());

   painter->fillRect(m_X0,m_Y0,m_X1-m_X0,m_Y1-m_Y0,white);

   painter->setPen(QPen(black,2));
   painter->drawLine(m_X0,m_Y0,m_X0,m_Y1);
   painter->drawLine(m_X0,m_Y1,m_X1,m_Y1);

   unsigned delta = (m_Y1-m_Y0) / Y_TICKS;
       
   for(int i=0; i<Y_TICKS; i++)
   {    
      painter->drawLine(m_X0-3,m_Y0+i*delta,m_X0+2,m_Y0+i*delta);
      unsigned value = m_Ymax - i*(m_Ymax-m_Ymin)/Y_TICKS;
      QString string;
      string.sprintf("%d",value);
      painter->drawText(m_X0-40, m_Y0+i*delta+5, string); 
   }
      
   delta = (m_X1-m_X0) / X_TICKS;
      
   for(int j=0; j<X_TICKS; j++)
   {
      painter->drawLine(m_X1-j*delta,m_Y1-2,m_X1-j*delta,m_Y1+3);
      unsigned value = m_Xmax - j*(m_Xmax-m_Xmin)/X_TICKS;
      QString string;
      string.sprintf("%d",value);
      painter->drawText(m_X1-j*delta-10, m_Y1+17, string); 
   }      

}


//*************************************************************************
//
//  f: void focusInEvent(QFocusEvent* event);
//
//  d:
//
//*************************************************************************
   
void XSimulationView :: focusInEvent(QFocusEvent* event)
{
   static int firstTime = 1;
   if( firstTime )
   {
      firstTime = 0;
      initialize(1000,10,0,0);   
   }
}


//*************************************************************************
//
//  f: void mouseDoubleClickEvent(QMouseEvent* event);
//
//  d:
//
//*************************************************************************
   
void XSimulationView :: mouseDoubleClickEvent(QMouseEvent* event)
{
   QString param;
   param.sprintf( "Simulation = %s\nLoad       = %1.4f    \nCycles     = %d",
                  (const char*)PRZDialog2::getSimulationId(),
                  PRZDialog1::getLoad(),
                  PRZDialog1::getCycles() );

   QMessageBox::information( XSimulationView::instance, 
                             " Simulation parameters ",
                             param);
}


//*************************************************************************
//
//  f: void paintEvent(QPaintEvent* event);
//
//  d:
//
//*************************************************************************

void XSimulationView :: paintEvent(QPaintEvent* event)
{
   bitBlt( this, event->rect().topLeft(),m_Pixmap, event->rect() );
}


//*************************************************************************
//
//  f: void drawPoint(double x1, double y1);
//
//  d:
//
//*************************************************************************

void XSimulationView :: drawPoint(double x1, double y1 )
{
   static unsigned toggle = 1; 
      
   resizeXaxis(x1);
   resizeYaxis(y1);
          
   unsigned x2 = translateX(x1);
   unsigned y2 = translateY(y1);
            
   QPainter painter(m_Pixmap);
   painter.setPen(QPen(blue,1));
   PRZPoint* point = m_PointList.getLast();      
   if( point )
   {
         painter.drawLine( translateX(point->x()),
                           translateY(point->y()),
                           x2,
                           y2 );
   }

   m_PointList.append( new PRZPoint(x1,y1) );
      
   if( !(++toggle%(2+m_ResizesNumber/4)) )
   {

      painter.fillRect(x2-2,y2-2,4,4,black);

      QString string;
      string.sprintf("%4.1f",y1);
      painter.setPen(QPen(black));
      painter.translate(x2+3,y2-5);
      painter.rotate(-90.0);
      painter.drawText(0,0,string);
   }
      
   painter.end();
      
   bitBlt(this,0,0,m_Pixmap);
}


//*************************************************************************
//
//  f: unsigned translateX(double x);
//
//  d:
//
//*************************************************************************

unsigned XSimulationView :: translateX(double x)
{ 
   return (m_X0 + (x-m_Xmin)*(m_X1-m_X0)/(m_Xmax-m_Xmin));   
}


//*************************************************************************
//
//  f: unsigned translateY(double y);
//
//  d:
//
//*************************************************************************

unsigned XSimulationView :: translateY(double y)
{
   return (m_Y1 - (y-m_Ymin)*(m_Y1-m_Y0)/(m_Ymax-m_Ymin));
}


//*************************************************************************
//
//  f: void resizeXaxis(unsigned x);
//
//  d:
//
//*************************************************************************

void XSimulationView :: resizeXaxis(unsigned x)
{
   unsigned xmax = m_Xmax;
   unsigned xmin = m_Xmin;
   int change = 0;
            
   if( x > m_Xmax )
   {
      xmax = x + 900;
      change = 1;
   }
      
   if( x < m_Xmin )
   {
      xmin = MAXIMO((x-500),0);
      change = 1;
   }
      
   if(change) 
   {
      m_ResizesNumber++;
      initialize(xmax,m_Ymax,xmin,m_Ymin);
      redrawList(this);
   }
}


//*************************************************************************
//
//  f: void resizeYaxis(unsigned y);
//
//  d:
//
//*************************************************************************

void XSimulationView :: resizeYaxis(unsigned y)
{
   unsigned ymax = m_Ymax;
   unsigned ymin = m_Ymin;
   int change = 0;
            
   if( m_FirstTime || (y >= m_Ymax) )
   {
      ymax = y + 5;
      change = 1;
   }
      
   if( m_FirstTime || (y <= m_Ymin) )
   {
      ymin = MAXIMO((y-5),0);
      change = 1;
   }
      
   if(change) 
   {
      initialize(m_Xmax,ymax,m_Xmin,ymin);
      redrawList(this);
   }
      
   m_FirstTime = 0;
}


//*************************************************************************
//
//  f: void redrawList(QPaintDevice* device);
//
//  d:
//
//*************************************************************************

void XSimulationView :: redrawList(QPaintDevice* device)
{
      
   PRZPoint *point1=0, *point2;
      
   QPaintDevice* buffer = (device==this) ? m_Pixmap : device;
   int toggle = 1;
                           
   for( point2=m_PointList.first(); point2!=0; point2=m_PointList.next() )
   {
      QPainter painter(buffer);
      unsigned x2 = translateX(point2->x());
      unsigned y2 = translateY(point2->y());
      
      if(point1)
      {
         unsigned x1 = translateX(point1->x());
         unsigned y1 = translateY(point1->y());
         painter.setPen(QPen(blue));
         painter.drawLine( x1,y1,x2,y2 );            
      }
      
      painter.end();
      
      if( !(++toggle%(2+m_ResizesNumber/4)) )
      {
         QPainter painter(buffer);
         painter.fillRect( x2-2,y2-2,4,4,black );
         QString string;
         string.sprintf("%4.1f",point2->y());
         unsigned x2 = translateX(point2->x());
         unsigned y2 = translateY(point2->y());
         painter.translate(x2+3,y2-5);
         painter.rotate(-90.0);
         painter.drawText(0,0,string);
         painter.end();
      }      

      point1 = point2;
   }
      
   if(buffer==m_Pixmap) bitBlt(device,0,0,m_Pixmap);
}


//*************************************************************************
//
//  f: void onButtonPrintClick();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onButtonPrintClick()
{
   QPrinter prt;
   prt.setPrintProgram("lpr");
   if ( prt.setup(this) ) 
   {
      QPainter painter(&prt);
      painter.setFont(QFont("times",9));
      drawFrame(&painter);
      
      PRZPoint *point1=0, *point2;
      unsigned pointNumber = 0;
      
      for( point2=m_PointList.first(); point2!=0; point2=m_PointList.next() )
      {
         unsigned x2 = translateX(point2->x());
         unsigned y2 = translateY(point2->y());
      
         if(point1)
         {
            unsigned x1 = translateX(point1->x());
            unsigned y1 = translateY(point1->y());
            painter.setPen(QPen(blue));
            painter.drawLine( x1,y1,x2,y2 );            
         }
         if( ! ((++pointNumber)%(2+m_ResizesNumber/4)) )
         {
            painter.fillRect( x2-2,y2-2,4,4,black );
            QString string;
            string.sprintf("%4.1f",point2->y());
            painter.drawText(x2-7,y2-7,string);
         }
         point1 = point2;
      }
      painter.end();
   
   }
}


//*************************************************************************
//
//  f: void onButtonQuitClick();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onButtonQuitClick()
{
   exit(0);
}

//*************************************************************************
//
//  f: void onSimulationEnd();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onSimulationEnd()
{
   cout << "Simul end" << endl;
   m_BtRun -> setEnabled(TRUE);
}


//*************************************************************************
//
//  f: void onOpenMenu();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onOpenMenu()
{
   PRZDialog2 dlg(this,"dlg2");
   dlg.exec();
   if( PRZDialog2::getSimulationId() != "" )
      m_BtRun->setEnabled(TRUE);
}

   
//*************************************************************************
//
//  f: void onSettingsMenu();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onSettingsMenu()
{
   PRZDialog1 dlg(this,"Opciones");
   dlg.exec();
      
   m_Load = dlg.getLoad();      
}


//*************************************************************************
//
//  f: void onAboutMenu();
//
//  d:
//
//*************************************************************************

void XSimulationView :: onAboutMenu()
{
   QMessageBox::information( XSimulationView::instance, 
                             "Product information",
                             "Grupo de Arquitectura de Computadores\n"
                             "Universidad de Cantabria\n"
                             "Version 1.0");
}


//*************************************************************************
//
//  f: void setMessagesTx(double number);
//
//  d:
//
//*************************************************************************

void XSimulationView :: setMessagesTx(double number)
{
   m_MsgTx->setNum(number);
}


//*************************************************************************
//
//  f: void setMessagesRx(double number);
//
//  d:
//
//*************************************************************************

void XSimulationView :: setMessagesRx(double number)
{
   m_MsgRx->setNum(number);
}
   

//*************************************************************************
//
//  f: void setMsgDifference(double number);
//
//  d:
//
//*************************************************************************

void XSimulationView :: setMsgDifference(double number)
{
   m_MsgDiff->setNum(number);
}   


//*************************************************************************
//*************************************************************************

//*************************************************************************
//
//  f: void main(int argc, char** argv)
//
//  d:
//
//*************************************************************************

void main(int argc, char** argv)
{
   QApplication app(argc,argv);
   // QApplication::setFont(QFont("times",11)); 
   
   XSimulationView mainWindow;
   
   app.setMainWidget(&mainWindow);
   
   mainWindow.show();
   app.exec();
}


//*************************************************************************

// fin del fichero
