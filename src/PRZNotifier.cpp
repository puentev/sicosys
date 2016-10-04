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
// File: ../ATCSimul/src/PRZNotifier.cpp
//
// Clase      : PRZNotifier
//
// Hereda de  : PRZBase
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZNotifier.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZNotifier.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZNotifier.hpp>

#ifndef __PRZObserver_HPP__
#include <PRZObserver.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZNotifier,PRZBase);

//*************************************************************************
//:
//  f: inline Boolean isEnableNotification() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZNotifier();
//
//  d:
//:
//*************************************************************************

PRZNotifier :: PRZNotifier()
             : m_EnableNotification(true)
{

}


//*************************************************************************
//:
//  f: ~PRZNotifier();
//
//  d:
//:
//*************************************************************************

PRZNotifier :: ~PRZNotifier()
{
//   removeAllObservers();
}


//*************************************************************************
//:
//  f: void enableNotification();
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: enableNotification()
{
   m_EnableNotification = true;
}


//*************************************************************************
//:
//  f: void disableNotification();
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: disableNotification()
{
   m_EnableNotification = false;
}


//*************************************************************************
//:
//  f: void notifyObservers(const PRZNotificationEvent& event);
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: notifyObservers(const PRZNotificationEvent& event)
{
   if( isEnableNotification() )
   {
      PRZObserversList::Cursor cursor(m_ObserverList);
      forCursor(cursor)
      {
         cursor.element()->dispatchNotificationEvent(event);
      }
   }
}


//*************************************************************************
//:
//  f: void addObserver(PRZObserver* pObserver);
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: addObserver(PRZObserver* pObserver)
{
   if( ! m_ObserverList.containsElement(pObserver) )
      m_ObserverList.add(pObserver);
}


//*************************************************************************
//:
//  f: void removeObserver(const PRZObserver* pObserver);
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: removeObserver(const PRZObserver* pObserver)
{
   PRZObserversList::Cursor cursor(m_ObserverList);
   forCursor(cursor)
      if( cursor.element() == pObserver )
      {
         // El observer solo puede estar una sola vez en la lista
         cursor.remove();
         break;
      }
}


//*************************************************************************
//:
//  f: void removeAllObserver();
//
//  d:
//:
//*************************************************************************

void PRZNotifier :: removeAllObservers()
{
   PRZObserversList::Cursor cursor(m_ObserverList);
   forCursor(cursor)
   {
      cursor.element()->stopHandlingNotificationsFor(this);
      cout << "REMOVE" << endl;
   }
   m_ObserverList.removeAllElements();
}

//*************************************************************************


// fin del fichero
