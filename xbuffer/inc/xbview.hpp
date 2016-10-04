//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*****************************************************************
//
// File: ../ATCSimul/xbuffer/inc/xbview.hpp
//
// Description:   fichero de encabezado de la clase "XBufferView"
//
// Notes:   Es una ventana que visualiza el estado de los buffers
//               en una red bidimensional. Permite obtener la ocupacion
//               media de los buffers. 
//
// $Author: vpuente $ 
//
// $RCSfile: xbview.hpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __XBVIEW_HPP__
#define __XBVIEW_HPP__

//*************************************************************************

#include <qdialog.h>
#include <qpushbt.h>
#include <qpixmap.h>

//*************************************************************************

   class XBufferView : public QDialog 
   {
      Q_OBJECT

   public:
      XBufferView( unsigned sizeX,
                   unsigned sizeY,
                   unsigned bufferSize,
                   QWidget *parent=0, 
                   const char *name=0 );
      ~XBufferView();
      
      void displayBufferState(unsigned* ptr);
      
   protected:
      virtual void paintEvent(QPaintEvent* event);
      void clear();
      void fillBuffer(unsigned x, unsigned y, unsigned value, bool foce=FALSE );
      
   private:
      QPushButton*   m_AverageBt;
      QPushButton*   m_QuitBt;
      QPushButton*   m_PrintBt;
      QPixmap*       m_Pixmap;
      unsigned       m_SizeX, m_SizeY;
      unsigned       m_BufferSize;
      unsigned       m_X0, m_Y0;
      unsigned       m_X1, m_Y1;
      unsigned       m_DeltaX, m_DeltaY;            
      unsigned long  m_Samples;
      unsigned long* m_TotalOccupation;
      unsigned*      m_LastValue;
      
   protected slots:
      void onAverageClick();
      void onPrintClick();
   };

//*************************************************************************

#endif

// fin del fichero
