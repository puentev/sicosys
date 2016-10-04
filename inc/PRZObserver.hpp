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
// File: ../ATCSimul/inc/PRZObserver.hpp
//
// Description:  fichero de encabezado de la clase "PRZObserver"
//
// Notes:  la clase "PRZObserver" es parte integrante del patron
//              de diseno Observer. Se trata de una clase abstracta,
//              por lo que debemos sobrecargar el metodo virtual puro
//              dispatchNotificationEvent() en las clases hijas. Este 
//              metodo es el que se llama desde el Notifier asociado.
//              El observer puede estar "mirando" a varios notifiers.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZObserver.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZObserver_HPP__
#define __PRZObserver_HPP__

//************************************************************************

   #include <PRZSequence.hpp>
   
   class PRZNotificationEvent;
   
//************************************************************************

   class PRZNotifier;

//************************************************************************

   class PRZObserver
   {
       typedef PRZSequence<PRZNotifier*> PRZNotifiersList;

   public:
      PRZObserver();
      ~PRZObserver();

      virtual void dispatchNotificationEvent(const PRZNotificationEvent& event) = 0;
      void startHandlingNotificationsFor(PRZNotifier* notifier);
      void stopHandlingNotificationsFor(PRZNotifier* notifier);

      // Run time information
      DEFINE_RTTI(PRZObserver);

   private:
      PRZNotifiersList m_NotifiersList;
   };

//************************************************************************


#endif


// fin del fichero
