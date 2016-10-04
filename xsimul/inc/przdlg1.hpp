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
// File: ../ATCSimul/xsimul/inc/przdlg1.hpp
//
// Description:   fichero de encabezado de la clase "PRZDialog1". Se
//               trata de un Dialog que permite la entrada de los
//               datos mas relevantes con los que se puede configurar
//               la simulacion, como son el numero de ciclos, la 
//               carga aplicada, si se genera o no una traza. Para
//               ello, se hace uso de los controles entry-field, static
//               labels y check-boxes.
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: przdlg1.hpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZDLG1_HPP__
#define __PRZDLG1_HPP__

//*************************************************************************

   #include <qdialog.h>
   #include <qlined.h>
   #include <qlabel.h>
   #include <qchkbox.h>
   #include <qcombo.h>
   #include "PRZString.hpp"

//*************************************************************************

   class PRZDialog1 : public QDialog 
   {
      Q_OBJECT

   public:
      PRZDialog1( QWidget *parent, const char *name );
      
      static PRZString getParameters()
      { return m_Parameters; }

      static unsigned getCycles()
      { return m_Cycles; }

      static double getLoad()
      { return m_Load; }
   
      static unsigned isTrazaEnable()
      { return m_Traza; }
         
   private:
      bool validar(bool isValid);
      static double     m_Load;
      static unsigned   m_Cycles;
      static unsigned   m_PacketSize;
      static unsigned   m_BufferSize;
      static unsigned   m_Traza;
      static PRZString  m_Traffic;
      static unsigned   m_Bimodal;
      static unsigned   m_Buffers;
      static unsigned   m_CurrentTraffic;
      static double     m_ProbBimodal;
      static double     m_MsgBimodal;
      static unsigned   m_NodosHot;
      static unsigned   m_ProbHot;
      static double     m_P10Flash;
      static double     m_AlphaFlash;
      static double     m_SigmaLocal;
      static PRZString  m_BuffersFile;
      static PRZString  m_Parameters;
      
      QLineEdit* m_EditLoad;
      QLineEdit* m_EditCycles;
      QLineEdit* m_EditBuffer;
      QLineEdit* m_EditPacket;
      QLineEdit* m_EditP1;
      QLineEdit* m_EditP2;
      QLineEdit* m_EditP3;
      QLineEdit* m_EditP4;
      QLineEdit* m_EditP5;

      QLabel*    m_LabelP1;
      QLabel*    m_LabelP2;
      QLabel*    m_LabelP3;
      QLabel*    m_LabelP4;
      QLabel*    m_LabelP5;
      
      QCheckBox* m_CheckTraza;
      QCheckBox* m_CheckBimodal;
      QCheckBox* m_CheckBuffer;

      QComboBox* m_ComboTraffic;
      
   protected slots:
      void onAcceptClick();
      void onTrafficSelect(const QString&);
      void onBimodalCheck(bool);
      void onBufferCheck(bool);
   };

//*************************************************************************

#endif

// fin del fichero
