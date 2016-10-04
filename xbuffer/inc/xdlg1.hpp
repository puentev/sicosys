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
// File: ../ATCSimul/xbuffer/inc/xdlg1.hpp
//
// Description:   fichero de encabezado de la clase "XDialog1"
//
// Notes:   Este dialogo sirve para seleccionar los buffers que se
//               desean visualizar.
//
// $Author: vpuente $ 
//
// $RCSfile: xdlg1.hpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __XDLG1_HPP__
#define __XDLG1_HPP__

//*************************************************************************

#include <qdialog.h>
#include <qlistbox.h>
#include <qlist.h>
#include <qpushbt.h>

//*************************************************************************

   class XDialog1 : public QDialog 
   {
      Q_OBJECT

   public:
      XDialog1( const QList<QString>& list, 
                QWidget *parent=0, 
                const char *name=0 );
      
      static QList<QString> getBuffersActive()
      { return sm_BuffersActive; }
      
   private:
      QListBox*      m_LeftList;
      QListBox*      m_RightList;
      static QList<QString> sm_BuffersActive;
    
   protected slots:
      void onAcceptClick();
      void onAdd1Click();
      void onAddAllClick();
      void onRemove1Click();
      void onRemoveAllClick();

   };

//*************************************************************************

#endif

// fin del fichero
