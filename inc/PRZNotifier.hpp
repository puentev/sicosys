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
// File: ../ATCSimul/inc/PRZNotifier.hpp
//
// Description:  fichero de encabezado de la clase "PRZNotifier"
//
// Notes:  la clase "PRZNotifier" es parte integrante del patron
//              de diseno Observer. Cuando se realiza un notifyObservers,
//              se esta accediendo a una lista en la que se guardan las
//              referencias a los observadores, sobre los cuales se 
//              ejecuta el metodo dispatchNotificationEvent().
//
// $Author: vpuente $ 
//
// $RCSfile: PRZNotifier.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZNotifier_HPP__
#define __PRZNotifier_HPP__

//************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif

   #ifndef __PRZSequence_HPP__
   #include <PRZSequence.hpp>
   #endif

//************************************************************************

   class PRZObserver;
  
//************************************************************************

   class PRZNotificationEvent
   {
   public:
      PRZNotificationEvent( int id, 
                            double v1=0.0, 
                            double v2=0.0, 
                            double v3=0.0, 
                            double v4=0.0 )
                   : m_Id(id)
      {
         m_Value[0] = v1;
         m_Value[1] = v2;
         m_Value[2] = v3;
         m_Value[3] = v4;
      }
      
      PRZNotificationEvent(int id, PRZString text)
                   : m_Id(id),
                     m_Text(text)
      {
         m_Value[0] = m_Value[1] = m_Value[2] = m_Value[3] = 0.0;
      }
      
      int id() const
      { return m_Id; }      

      PRZString text() const
      { return m_Text; }
      
      double valueAt(int i) const
      { return ((i>=0) && (i<4)) ? m_Value[i] : 0.0; }      

   private:
      int       m_Id;
      double    m_Value[4];
      PRZString m_Text;
   };

//************************************************************************

   class PRZNotifier : public PRZBase
   {
   public:

      typedef PRZSequence<PRZObserver*> PRZObserversList;

      PRZNotifier();
      ~PRZNotifier();

      virtual void notifyObservers(const PRZNotificationEvent& event);
      virtual void addObserver(PRZObserver* pObserver);
      virtual void removeObserver(const PRZObserver* pObserver);
      virtual void removeAllObservers();

      void enableNotification();
      void disableNotification();

      Boolean isEnableNotification() const
      { return m_EnableNotification; }

      // Run time information
      DEFINE_RTTI(PRZNotifier);

   private:

      PRZObserversList m_ObserverList;
      Boolean m_EnableNotification;
   };

//************************************************************************

   #define EV_LATENCY  1
   #define EV_RESULT   2

//************************************************************************

#endif


// fin del fichero
