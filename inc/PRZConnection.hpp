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
// File: ../ATCSimul/inc/PRZConnection.hpp
//
// Description:   fichero de encabezado de la clase "PRZConnection"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZConnection.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************


#ifndef __PRZConnection_HPP__
#define __PRZConnection_HPP__

//*************************************************************************

   #ifndef __PRZBase_HPP__
   #include <PRZBase.hpp>
   #endif
   
//*************************************************************************
   
   class PRZInputPort;
   class PRZOutputPort;
   class PRZMessage;
   class PRZInterfaz;
   class PRZComponent;
   class PRZTag;
   class PRZRouterBuilder;
   
//*************************************************************************

   class PRZConnection : public PRZBase
   {
      friend class PRZRouterBuilder;
      
   public:
      PRZConnection( PRZOutputPort* org,
                     PRZInputPort*  dst );
      PRZConnection();
      ~PRZConnection();

      virtual void run(uTIME runTime);

      virtual Boolean sendData(PRZMessage* data);
      virtual void    clearData();
      virtual Boolean sendStop();
      virtual void    clearStop();

      static Boolean connectInterfaces( PRZComponent* owner,
                                        PRZInterfaz* src, 
                                        PRZInterfaz* dst,
                                        unsigned delay =0 );

      PRZInputPort* getInputPort() const
      { return m_InputPort; }

      PRZOutputPort* getOutputPort() const
      { return m_OutputPort; }

      // Run time information
      DEFINE_RTTI(PRZConnection);

   protected:
      void setSource(PRZOutputPort* org);
      void setDestiny(PRZInputPort* dst);

   private:
      static PRZConnection* newFrom(const PRZTag* tag, PRZComponent* owner);

      PRZInputPort*  m_InputPort;
      PRZOutputPort* m_OutputPort;
   };


//*************************************************************************

#endif


// fin del fichero
