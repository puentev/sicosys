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
// File: ../ATCSimul/inc/PRZIrregularNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZIrregularNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZIrregularNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//
//*************************************************************************

#ifndef __PRZIrregularNetwork_HPP__
#define __PRZIrregularNetwork_HPP__

//*************************************************************************

   #include <PRZNetwork.hpp>

//*************************************************************************

   class PRZNetworkBuilder;

//*************************************************************************

   class PRZIrregularNetwork : public PRZNetwork
   {
      friend class PRZNetworkBuilder;
      
   public:
      PRZIrregularNetwork( const PRZComponentId& id,
                       const PRZString& routerId,
                       unsigned x, 
                       unsigned y, 
                       unsigned z=1 );
      ~PRZIrregularNetwork();

      virtual void initialize(const PRZTag* tag);
      virtual PRZString asString() const;
      unsigned getDiameter() const;

      virtual unsigned distance(const PRZPosition& src, const PRZPosition& dst);
      virtual void     routingRecord( const PRZPosition& src,
                                      const PRZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ );
      // Run time information
      DEFINE_RTTI(PRZIrregularNetwork);
      
   protected:
    
   private:
      static PRZIrregularNetwork* newFrom(const PRZTag* tag, PRZComponent* owner);
      PRZString   m_fileName;
   };

//*************************************************************************


#endif


// fin del fichero
