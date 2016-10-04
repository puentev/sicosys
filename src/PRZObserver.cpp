//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZObserver.cpp
//
// Clase      : PRZObserver
//
// Hereda de  : None
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZObserver.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZObserver.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZObserver.hpp>

#ifndef __PRZNotifier_HPP__
#include <PRZNotifier.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZObserver);

//*************************************************************************
//:
//  f: virtual void dispatchNotificationEvent(const PRZNotificationEvent& event) = 0;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZObserver();
//
//  d:
//:
//*************************************************************************

PRZObserver :: PRZObserver()
{

}


//*************************************************************************
//:
//  f: ~PRZObserver();
//
//  d:
//:
//*************************************************************************

PRZObserver :: ~PRZObserver()
{
   PRZNotifiersList::Cursor cursor(m_NotifiersList);
   forCursor(cursor)
      cursor.element()->removeObserver(this);
}


//*************************************************************************
//:
//  f: void startHandlingNotificationsFor(PRZNotifier* notifier);
//
//  d:
//:
//*************************************************************************

void PRZObserver :: startHandlingNotificationsFor(PRZNotifier* notifier)
{
   if( ! m_NotifiersList.containsElement(notifier) )
      m_NotifiersList.add(notifier);
   notifier->addObserver(this);
}


//*************************************************************************
//:
//  f: void stopHandlingNotificationsFor(PRZNotifier* notifier);
//
//  d:
//:
//*************************************************************************

void PRZObserver :: stopHandlingNotificationsFor(PRZNotifier* notifier)
{
   PRZNotifiersList::Cursor cursor(m_NotifiersList);
   forCursor(cursor)
      if( cursor.element() == notifier )
      {
         cursor.remove();
         break;
      }
}

//************************************************************************


// fin del fichero
