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
// File: ../ATCSimul/inc/PRZMeshNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZTorusNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMeshNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMeshNetwork_HPP__
#define __PRZMeshNetwork_HPP__

//*************************************************************************

   #include <PRZTorusNetwork.hpp>

//*************************************************************************

   class PRZNetworkBuilder;

//*************************************************************************

   class PRZMeshNetwork : public PRZTorusNetwork
   {
      friend class PRZNetworkBuilder;
      
   public:
      PRZMeshNetwork( const PRZComponentId& id,
                       const PRZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~PRZMeshNetwork();

     virtual PRZString asString() const;
     unsigned getDiameter() const;

      virtual void     routingRecord( const PRZPosition& src,
                                      const PRZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ );
      // Run time information
      DEFINE_RTTI(PRZMeshNetwork);
   
   };

//*************************************************************************


#endif


// fin del fichero
