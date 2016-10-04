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
// File: ../ATCSimul/xbuffer/src/xbview.cpp
//
// Description:   fichero de implementacion de la clase "XBufferView"
//
// Notes:   Ver xbview.cpp
//
// $Author: vpuente $ 
//
// $RCSfile: xbview.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: xbview.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";


//*************************************************************************

#include "xbview.hpp"

#include <qpainter.h>
#include <qfont.h>
#include <qapp.h>
#include <qprinter.h>

#include <stdlib.h>
#include <iostream.h>

//*************************************************************************

static const unsigned SIZEX = 400;
static const unsigned SIZEY = 450;

//*************************************************************************
  

//*************************************************************************
//
//   XBufferView( unsigned sizeX,
//                   unsigned sizeY,
//                   unsigned bufferSize,
//                   QWidget *parent, 
//                   const char *name )
//
//*************************************************************************

XBufferView :: XBufferView( unsigned sizeX,
                            unsigned sizeY,
                            unsigned bufferSize,
                            QWidget *parent, 
                            const char *name )
                : QDialog( parent, name, FALSE ),
                  m_SizeX(sizeX),
                  m_SizeY(sizeY),
                  m_BufferSize(bufferSize),
                  m_X0(30),
                  m_Y0(30),
                  m_X1(SIZEX-30),
                  m_Y1(SIZEY-80),
                  m_Samples(0),
                  m_TotalOccupation(0),
                  m_LastValue(0)
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);
   setCaption(name);

   m_DeltaX = (m_X1 - m_X0) / m_SizeX;
   m_DeltaY = (m_Y1 - m_Y0) / m_SizeY;

   m_AverageBt = new QPushButton("Average",this); 
   m_QuitBt    = new QPushButton("Ok",this );
   m_PrintBt   = new QPushButton("Print",this);
   m_Pixmap    = new QPixmap(size());
   
   m_AverageBt -> setGeometry(50,SIZEY-50,80,30);
   m_PrintBt   -> setGeometry(150,SIZEY-50,80,30);
   m_QuitBt    -> setGeometry(270,SIZEY-50,70,30);
   m_QuitBt    -> setDefault(TRUE);
   m_Pixmap    -> fill(backgroundColor());

   m_TotalOccupation = new unsigned long[m_SizeX*m_SizeY];
   m_LastValue       = new unsigned[m_SizeX*m_SizeY];

   for(register int i=0; i<m_SizeX*m_SizeY; i++ )
   {
      *(m_TotalOccupation+i) = 0;
      *(m_LastValue+i) = 0;
   }   
   connect( m_AverageBt, SIGNAL(clicked()), this, SLOT(onAverageClick()) );
   connect( m_PrintBt, SIGNAL(clicked()), this, SLOT(onPrintClick()) );
   connect( m_QuitBt, SIGNAL(clicked()), this, SLOT(reject()) );

   clear();
}
    


//*************************************************************************
//
//   ~XBufferView()
//
//*************************************************************************

XBufferView :: ~XBufferView()
{
   delete   m_Pixmap;
   delete[] m_TotalOccupation;
   delete[] m_LastValue;
}


//*************************************************************************
//
//   void paintEvent(QPaintEvent* event)
//
//*************************************************************************

void XBufferView :: paintEvent(QPaintEvent* event)
{
   bitBlt( this, event->rect().topLeft(),m_Pixmap, event->rect() );
}


//*************************************************************************
//
//   void drawGrid()
//
//*************************************************************************

void XBufferView :: clear()
{
   QPainter painter(m_Pixmap);
   painter.fillRect(0,0,x(),y(),backgroundColor());
   painter.setPen(QPen(blue));
   painter.drawLine(m_X0-1,m_Y1-m_DeltaY*m_SizeY-1,m_X0-1,m_Y1+1);
   painter.drawLine(m_X0-1,m_Y1+1,m_X0+m_DeltaX*m_SizeX+1,m_Y1+1);
   painter.drawLine(m_X0+m_DeltaX*m_SizeX+1,m_Y1+1,m_X0+m_DeltaX*m_SizeX+1,m_Y1-m_DeltaY*m_SizeY-1);
   painter.drawLine(m_X0+m_DeltaX*m_SizeX+1,m_Y1-m_DeltaY*m_SizeY-1,m_X0-1,m_Y1-m_DeltaY*m_SizeY-1);
   painter.end();
   bitBlt(this,QPoint(0,0),m_Pixmap,geometry(),CopyROP);
}


//*************************************************************************
//
//   void fillBuffer(unsigned x, unsigned y, unsigned value, bool force=FALSE)
//
//*************************************************************************

void XBufferView :: fillBuffer(unsigned x1, unsigned y1, unsigned value, bool force)
{
   if( (m_SizeX < x1) || (m_SizeY < y1) ) return;
   
   static unsigned deltaX = (m_X1 - m_X0) / m_SizeX;
   static unsigned deltaY = (m_Y1 - m_Y0) / m_SizeY;
   unsigned x2 = m_X0 + deltaX*x1;
   unsigned y2 = m_Y1 - deltaY*( 1 + y1 );
   
   if( !force && (*(m_LastValue+x1+y1*m_SizeX) == value) )
   {
      QPainter painter(m_Pixmap);
      QString string;
      string.sprintf("%d",value);
      painter.setPen(QPen(red));
      painter.setFont(QFont("Times",10));
      painter.drawText(x2+deltaX/2-5, y2+deltaY/2+5, string);
      painter.end();
      bitBlt(this,0,0,m_Pixmap);
      return;
   }
   *(m_LastValue+x1+y1*m_SizeX) = value;
   
   QPainter painter(m_Pixmap);
   int intensity = int(255*(double(value)/double(m_BufferSize)));
   int inverse   = 255 - intensity;
   QColor color(inverse,inverse,inverse);
   painter.fillRect( QRect( QPoint(x2,y2), 
                            QPoint(x2+deltaX,y2+deltaY) ), 
                     color );
   QString string;
   string.sprintf("%d",value);
   painter.setPen(QPen(red));
   painter.setFont(QFont("Times",10));
   painter.drawText(x2+deltaX/2-5, y2+deltaY/2+5, string);
   painter.end();
   bitBlt(this,0,0,m_Pixmap);
}


//*************************************************************************
//
//   void displayBufferState(unsigned* ptr);
//
//*************************************************************************

void XBufferView :: displayBufferState(unsigned* ptr)
{
   m_Samples++;
   for(register int i=0, count=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++, count++ )
      {
         unsigned value = *(ptr+count);
         fillBuffer(i,j,value);
         *(m_TotalOccupation+count) = *(m_TotalOccupation+count) + value;
      }
}


//*************************************************************************
//
//   void onAverageClick()
//
//*************************************************************************

void XBufferView :: onAverageClick()
{
   for(register int i=0, count=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++, count++ )
      {
         fillBuffer( i, 
                     j,
                     unsigned( (*(m_TotalOccupation+count))/m_Samples), 
                     TRUE );
      }
}


//*************************************************************************
//
//   void onPrintClick()
//
//*************************************************************************

void XBufferView :: onPrintClick()
{
   static unsigned deltaX = (m_X1 - m_X0) / m_SizeX;
   static unsigned deltaY = (m_Y1 - m_Y0) / m_SizeY;
   QPrinter prt;
   prt.setPrintProgram("lpr");
   if( prt.setup(this) )
   {
      QPainter painter(&prt);
      painter.setFont(QFont("Times",10));
      unsigned x1, y1;
      for( x1=0; x1<m_SizeX; x1++ )
      {
         for( y1=0; y1<m_SizeY; y1++ )
         {
            unsigned x2 = m_X0 + deltaX*x1;
            unsigned y2 = m_Y1 - deltaY*( 1 + y1 );
            double value = double(*(m_TotalOccupation+y1+x1*m_SizeY)) /
                           double(m_Samples);   

            QString string;
            string.sprintf("%2.2f",value);  
            painter.drawText(x2+deltaX/2-5, y2+deltaY/2+5, string);
            
         }
      }
      for( x1=0; x1<m_SizeX; x1++ )
      {
         painter.drawLine(m_X0+x1*m_DeltaX,m_Y0,m_X0+x1*m_DeltaX,m_Y1);
      }
      for( y1=0; y1<m_SizeY; y1++ )
      {
         painter.drawLine(m_X0,m_Y0+m_DeltaY*y1,m_X1,m_Y0+m_DeltaY*y1);
      }
      painter.drawLine(m_X0,m_Y1,m_X1,m_Y1);
      painter.drawLine(m_X1,m_Y0,m_X1,m_Y1);
      
      QString buffer;
      buffer.sprintf("BUFFER = %s",caption());
      painter.drawText(m_X0+20,m_Y1+50,buffer);      
      painter.end();
   }
   
   


}

//*************************************************************************

//*************************************************************************
//*************************************************************************


/*
void main(int argc, char** argv)
{
   QApplication app(argc,argv);
   QApplication::setFont(QFont("times",11)); 
   
   XBufferView view(8,8,8);
   view.fillBuffer(0,0,5);
   app.setMainWidget(&view);
   
   view.show();
   app.exec();

}
*/



// fin del fichero
