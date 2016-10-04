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
// File: ../ATCSimul/inc/PRZTorusNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZTorusNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZTorusNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZTorusNetwork_HPP__
#define __PRZTorusNetwork_HPP__

//*************************************************************************

   #include <PRZNetwork.hpp>

//*************************************************************************

   class PRZNetworkBuilder;

//*************************************************************************

   class PRZTorusNetwork : public PRZNetwork
   {
      friend class PRZNetworkBuilder;
      
   public:
      PRZTorusNetwork( const PRZComponentId& id,
                       const PRZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~PRZTorusNetwork();

      virtual void initialize();
      virtual PRZString asString() const;
      unsigned getDiameter() const;

      virtual unsigned distance(const PRZPosition& src, const PRZPosition& dst);
      virtual void     routingRecord( const PRZPosition& src,
                                      const PRZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ );
      // Run time information
      DEFINE_RTTI(PRZTorusNetwork);
   
   private:
      static PRZTorusNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
      
   };

//*************************************************************************


#endif


// fin del fichero
