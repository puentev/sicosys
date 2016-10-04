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
// File: ../ATCSimul/xsimul/inc/przdlg2.hpp
//
// Description:   fichero de encabezado de la clase "PRZDialog2". Se
//               trata de un Dialog que contiene una lista con todas
//               las simulaciones disponibles en el fichero ".sgm"
//               correspondiente. Para localizarle, hace uso del
//               fichero "ATCSimul.ini".
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: przdlg2.hpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZDLG2_HPP__
#define __PRZDLG2_HPP__

//*************************************************************************

#include <qdialog.h>
#include <qlined.h>
#include <qlabel.h>
#include <qlistbox.h>

//*************************************************************************

   class PRZDialog2 : public QDialog 
   {
      Q_OBJECT

   public:
      PRZDialog2( QWidget *parent, const char *name );
      
      static QString getSimulationId()
      { return sm_SimulationId; }

   protected:
      void refreshListBox();
      
   private:
      QListBox*  m_ListBox;
      QLineEdit* m_EditSimul;
      static QString sm_SimulationId;
      static int sm_IndexSelected;
            
   protected slots:
      void onAcceptClick();

   };

//*************************************************************************

#endif

// fin del fichero
