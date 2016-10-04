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
// File: ../ATCSimul/xbuffer/src/xdlg1.cpp
//
// Description:   fichero de implementacion de la clase "XDialog1"
//
// Notes:   Ver xdlg1.hpp
//
// $Author: vpuente $ 
//
// $RCSfile: xdlg1.cpp,v $
//
// $Date: 2001/10/08 05:43:09 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: xdlg1.cpp,v 1.1.1.1 2001/10/08 05:43:09 vpuente Exp $";


//*************************************************************************

#include "xdlg1.hpp"
#include <iostream.h>

//*************************************************************************

   QList<QString> XDialog1::sm_BuffersActive;

//*************************************************************************

   #define SIZEX 450
   #define SIZEY 350

//*************************************************************************
//
//   XDialog1( const QList<QString>& list, QWidget *parent, const char *name)
//
//*************************************************************************

XDialog1 :: XDialog1( const QList<QString>& list, QWidget *parent, const char *name )
          : QDialog( parent, name, TRUE )
{
   setMinimumSize(SIZEX,SIZEY);
   setMaximumSize(SIZEX,SIZEY);
   setCaption("Buffers Selection");

   QPushButton* btAccept    = new QPushButton( "Ok", this );
   QPushButton* btCancel    = new QPushButton( "Cancel", this );
   QPushButton* btAdd1      = new QPushButton( ">", this );
   QPushButton* btAddAll    = new QPushButton( ">>", this );
   QPushButton* btRemove1   = new QPushButton( "<", this );
   QPushButton* btRemoveAll = new QPushButton( "<<", this );
  
   m_LeftList = new QListBox(this,"leftList");
   m_RightList= new QListBox(this,"rightList");
   
   btAccept   -> setGeometry( SIZEX-200,SIZEY-50,70,30 );
   btCancel   -> setGeometry( SIZEX-100,SIZEY-50,70,30 );
   btAdd1     -> setGeometry( SIZEX/2-12,50,25,25 );
   btAddAll   -> setGeometry( SIZEX/2-12,90,25,25 );
   btRemove1  -> setGeometry( SIZEX/2-12,130,25,25 );
   btRemoveAll-> setGeometry( SIZEX/2-12,170,25,25 );
   m_LeftList -> setGeometry( 20,20,SIZEX/2-50,SIZEY-100);
   m_RightList-> setGeometry( SIZEX/2+30,20,SIZEX/2-50,SIZEY-100);
   
   btAccept -> setDefault(TRUE);

   connect( btAccept, SIGNAL(clicked()), SLOT(onAcceptClick()) );
   connect( btCancel, SIGNAL(clicked()), SLOT(reject()) );
   connect( btAdd1,      SIGNAL(clicked()), SLOT(onAdd1Click()) );
   connect( btAddAll,    SIGNAL(clicked()), SLOT(onAddAllClick()) );
   connect( btRemove1,   SIGNAL(clicked()), SLOT(onRemove1Click()) );
   connect( btRemoveAll, SIGNAL(clicked()), SLOT(onRemoveAllClick()) );
   connect( m_LeftList,  SIGNAL(selected(int)),SLOT(onAdd1Click()) );
      
   QString* str;
   QList<QString> lista(list);

   for(str=lista.first();str!=0;str=lista.next())
   {
      m_LeftList->insertItem(*str);
      
   }
   
}
    

//*************************************************************************
//
//   void onAcceptClick();
//
//*************************************************************************

void XDialog1 :: onAcceptClick()
{
   XDialog1::sm_BuffersActive.clear();
   for(int i=0; i<m_RightList->count(); i++ )
   {
      QString right = m_RightList->text(i);
      XDialog1::sm_BuffersActive.append(new QString(right));
   }
   reject();
}


//*************************************************************************
//
//   void onAdd1Click();
//
//*************************************************************************

void XDialog1 :: onAdd1Click()
{
   int index =  m_LeftList->currentItem();
   if( index >=0 )
   {
      QString left = m_LeftList->text(index);
      bool found = FALSE;

      for(int i=0; i<m_RightList->count(); i++ )
      {
         QString right = m_RightList->text(i);
         if(left==right) found = TRUE;
      }
      if(!found) m_RightList->insertItem(left);
   }
}


//*************************************************************************
//
//   void onAddAllClick();
//
//*************************************************************************

void XDialog1 :: onAddAllClick()
{
   onRemoveAllClick();
   for(int i=0; i<m_LeftList->count(); i++ )
   {
      QString left = m_LeftList->text(i);
      m_RightList->insertItem(left);
   }

}


//*************************************************************************
//
//   void onRemove1Click();
//
//*************************************************************************

void XDialog1 :: onRemove1Click()
{

}


//*************************************************************************
//
//   void onRemoveAllClick();
//
//*************************************************************************

void XDialog1 :: onRemoveAllClick()
{
   m_RightList->clear();
}


//*************************************************************************

// fin del fichero
