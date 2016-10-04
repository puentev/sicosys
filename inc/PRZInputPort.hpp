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
// File: ../ATCSimul/inc/PRZInputPort.hpp
//
// Description:  fichero de encabezado de la clase "PRZInputPort"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZInputPort.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZInputPort_HPP__
#define __PRZInputPort_HPP__

//************************************************************************

   #include <PRZPort.hpp>

//*************************************************************************

   class PRZInputPort : public PRZPort
   {
   public:
      PRZInputPort( const PRZComponentId& id);                    
      virtual ~PRZInputPort();
  
      virtual Boolean sendDataRightNow(PRZMessage* msg);
      virtual Boolean clearDataRightNow();
      virtual Boolean sendStopRightNow();
      virtual Boolean clearStopRightNow();
      
      virtual Boolean onDataUp(unsigned interfaz, unsigned channel);
      virtual Boolean onDataDown(unsigned interfaz, unsigned channel);
      virtual Boolean onStopUp(unsigned interfaz, unsigned channel);
      virtual Boolean onStopDown(unsigned interfaz, unsigned channel);
          
      virtual PRZString asString() const;

      // Run time information
      DEFINE_RTTI(PRZInputPort);

   };

//************************************************************************

#endif


// fin del fichero
