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
// File: ../ATCSimul/inc/PRZDelayConnection.hpp
//
// Description:   fichero de encabezado de la clase "PRZDelayConnection"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZDelayConnection.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZDelayConnection_HPP__
#define __PRZDelayConnection_HPP__

//*************************************************************************

   #include <PRZConnection.hpp>

//*************************************************************************
   

   class PRZDelayConnection : public PRZConnection
   {
   public:
      PRZDelayConnection( PRZOutputPort* org,
                          PRZInputPort*  dst,
                          unsigned ciclos);
      PRZDelayConnection();
      ~PRZDelayConnection();

      virtual void    run(uTIME runTime);
      virtual Boolean sendData(PRZMessage* data);
      virtual void    clearData();

      static Boolean connectInterfaces( PRZComponent* owner,
                                        PRZInterfaz* src,
                                        PRZInterfaz* dst,
                                        unsigned ciclos );
      // Run time information
      DEFINE_RTTI(PRZDelayConnection);

   private:
      void addMessage(PRZMessage* aMessage);
      PRZMessage* getNextMessage();
   
      PRZMessage** m_MessageDelay;
      unsigned     m_Delay;
      
   };


//*************************************************************************

#endif


// fin del fichero
