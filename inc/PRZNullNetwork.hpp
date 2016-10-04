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
// File: ../ATCSimul/inc/PRZNullNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZNullNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZNullNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZNullNetwork_HPP__
#define __PRZNullNetwork_HPP__

//*************************************************************************

   #include <PRZNetwork.hpp>
   
   #include <PRZTorusNetwork.hpp>


//*************************************************************************

   class PRZNetworkBuilder;

//*************************************************************************

   class PRZNullNetwork : public PRZTorusNetwork
   {
      friend class PRZNetworkBuilder;
      
   public:
      PRZNullNetwork( const PRZComponentId& id,
                       const PRZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~PRZNullNetwork();

      virtual void initializeConnectionsFor(const PRZPosition& pos);
      // Run time information
      DEFINE_RTTI(PRZNullNetwork);
   
   private:
      static PRZNullNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
      
   };

//*************************************************************************


#endif


// fin del fichero
