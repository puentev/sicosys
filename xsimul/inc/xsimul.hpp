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
// File: ../ATCSimul/xsimul/inc/xsimul.hpp
//
// Description:   fichero de encabezado de la clase "XSimulationView"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: xsimul.hpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef _XSIMUL_HPP_
#define _XSIMUL_HPP_ 

#include <qdialog.h>
#include <qpushbt.h>
#include <qlist.h>
#include <qpixmap.h>
#include <qlcdnum.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qprogbar.h>
#include <qmsgbox.h>

#include <PRZObserver.hpp>

//*************************************************************************

   class PRZPoint
   {
   public:
      PRZPoint(double x, double y) : m_X(x), m_Y(y) { }
      PRZPoint(const PRZPoint& p)  : m_X(p.m_X), m_Y(p.m_Y) { }
      PRZPoint& operator=(const PRZPoint& p) { 
         m_X = p.m_X;
         m_Y = p.m_Y;
         return *this;
      }
      double x() const { return m_X; }
      double y() const { return m_Y; }
   private:
      double m_X,m_Y;
   };


//*************************************************************************

   class XSimulationView : public QDialog, public PRZObserver
   {
      Q_OBJECT
      typedef QDialog Inhereited;
      
   public:
      XSimulationView(QWidget* parent=0, const char* name=0);
      ~XSimulationView();
      void initialize( unsigned xmax, 
                       unsigned ymax, 
                       unsigned xmin,
                       unsigned ymin );

      static XSimulationView* instance;
      
      void onSimulationEnd(); 
      virtual void dispatchNotificationEvent(const PRZNotificationEvent& event);
            
   protected:
      virtual void paintEvent(QPaintEvent* event);
      virtual void focusInEvent(QFocusEvent* event);
      virtual void mouseDoubleClickEvent(QMouseEvent* event);

      void setMessagesTx(double number);
      void setMessagesRx(double number);
      void setMsgDifference(double number);
      
   private:   
      void resizeXaxis(unsigned x);
      void resizeYaxis(unsigned y);
      void redrawList(QPaintDevice* device );
      void drawPoint(double x1, double y1);
      void drawFrame(QPainter* painter);
      unsigned translateX(double x); 
      unsigned translateY(double y);
          
      int      m_SimulationId;
      bool     m_SimulationStop;
      unsigned m_X0, m_Y0, m_X1, m_Y1;
      unsigned m_Xmax, m_Ymax, m_Xmin, m_Ymin;
      unsigned m_Ancho, m_Alto;
      double   m_Load;
      int      m_FirstTime;
      unsigned m_NetSizeX, m_NetSizeY;
      unsigned m_ResizesNumber;
      double   m_Clock, m_Latencia;
      double   m_MessagesTx, m_MessagesRx;
      
      QString         m_DatosSimulacion;
      QPushButton*    m_BtQuit;
      QPushButton*    m_BtStop;
      QPushButton*    m_BtRun;
      QPushButton*    m_BtPrint;      
      QPushButton*    m_BtShow;
      QList<PRZPoint> m_PointList;  
      QPixmap*        m_Pixmap;
      QLCDNumber*     m_LCD;
      QMenuBar*       m_Menu;        
      QLabel*         m_MsgTx;
      QLabel*         m_MsgRx;
      QLabel*         m_MsgDiff;
      QProgressBar*   m_ProgressBar;
      
   public slots:
      void onButtonStopClick();
      void onButtonRunClick();
      void onButtonPrintClick();
      void onButtonResultClick();
      void onButtonQuitClick();
      void onOpenMenu();
      void onSettingsMenu();
      void onAboutMenu();
      void onDataReceived(double,double,double,double);
      void onResultReceived(const PRZString& text);
         
   signals:
      void signalDataReceived(double,double,double,double);
      void signalResultReceived(const PRZString& text);

   };

//*************************************************************************
   
#endif


// fin del fichero
