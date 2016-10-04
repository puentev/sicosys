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
// File: ../ATCSimul/inc/PRZMidimewNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZMidimewNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZMidimewNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZMidimewNetwork_HPP__
#define __PRZMidimewNetwork_HPP__

//*************************************************************************

   #include <PRZNetwork.hpp>

//*************************************************************************

   class PRZNetworkBuilder;

//*************************************************************************

   class PRZMidimewNetwork : public PRZNetwork
   {
      friend class PRZNetworkBuilder;
      
   public:
      PRZMidimewNetwork( const PRZComponentId& id,
                       const PRZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~PRZMidimewNetwork();

      virtual void initialize();
      unsigned getDiameter() const;
      virtual PRZString asString() const;
      virtual PRZPosition CreatePosition(int num);
      
      virtual unsigned distance(const PRZPosition& src, const PRZPosition& dst);
      virtual void     routingRecord( const PRZPosition& src,
                                      const PRZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ );
      // Run time information
      DEFINE_RTTI(PRZMidimewNetwork);
   protected:
      virtual void initializeConnectionsFor(const PRZPosition& pos);
   private:
      static PRZMidimewNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
      
   };

//*************************************************************************


#endif


// fin del fichero
