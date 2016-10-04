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
// File: ../ATCSimul/inc/PRZInterfaz.hpp
//
// Description:   fichero de encabezado de la clase "PRZInterfaz"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZInterfaz.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZInterfaz_HPP__
#define __PRZInterfaz_HPP__

//*************************************************************************

   #include <PRZComponent.hpp>

   #ifndef __PRZPort_HPP__
   #include <PRZPort.hpp>
   #endif
   
//*************************************************************************

   class PRZMessage;
   class PRZGauge;
   
//*************************************************************************

   class PRZInterfaz : public PRZComponent
   {
   
      typedef PRZComponent Inhereited;
      
   public:
      PRZInterfaz( const PRZComponentId& id,
                   unsigned numberCV=1,
                   Boolean wrapper=false );
      PRZInterfaz(const PRZInterfaz& interfaz);
      ~PRZInterfaz();
      
      virtual PRZString   asString() const;
            
      virtual Boolean     isReadyActive(unsigned channel=1) const;      
      virtual void        sendStop(unsigned channel=1);
      virtual void        sendStopRightNow(unsigned channel=1);
      virtual void        clearStop(unsigned channel=1);
      virtual void        clearStopRightNow(unsigned channel=1);
      virtual Boolean     isStopActive(unsigned channel=1) const;
      virtual void        sendData(PRZMessage* data, unsigned channel=1);
      virtual void        sendDataRightNow(PRZMessage* data, unsigned channel=1);
      virtual void        clearData(unsigned channel=1);
      virtual void        clearDataRightNow(unsigned channel=1);
      virtual Boolean     getData(PRZMessage** msg, unsigned channel=1) const;      
      
      static PRZInterfaz* createInputInterfaz( unsigned id=1, 
                                               unsigned numberCV=1 );
      static PRZInterfaz* createOutputInterfaz( unsigned id=1,
                                                unsigned numberCV=1 );

      static PRZInterfaz* createWrapperFor( PRZInterfaz* interfaz[], 
                                            unsigned number );
      
      virtual Boolean     onDataUp(unsigned i, unsigned c);
      virtual Boolean     onDataDown(unsigned i, unsigned c);
      virtual Boolean     onStopUp(unsigned i, unsigned c);
      virtual Boolean     onStopDown(unsigned i, unsigned c);
      
      virtual void        setOwner(PRZComponent* owner);
      virtual void        run(uTIME runTime);
      
      unsigned numberOfCV() const
      { return m_SizeCV; }

      PRZPort* getPort(unsigned channel=1) const
      { return ( (m_Ports) ? *(m_Ports+channel-1) : 0 ); }
      
      unsigned getNumberId() const
      { return m_Number; }
      
      void setNumberId(unsigned number)
      { m_Number = number; }

      void setGauge(PRZGauge* gauge);
      
      // Run time information
      DEFINE_RTTI(PRZInterfaz);

      static PRZString sm_InputPrefix;
      static PRZString sm_OutputPrefix;

   protected:

      void setPort(PRZPort* port, unsigned channel=1) {
         if( m_Ports && isChannelValid(channel) )
            *(m_Ports+channel-1) = port;
      }

      Boolean isChannelValid(unsigned ch) const
      { return ( ((ch>=1)&&(ch<=m_SizeCV)) ? true : false ); }
      
   private:
   	unsigned  m_Number;
      PRZPort** m_Ports;
      unsigned  m_SizeCV;
      Boolean   m_Wrapper;
      PRZGauge* m_Gauge;
   };

//*************************************************************************

   #define forInterfaz(i,size)  for(i=1;i<=size;i++)

//*************************************************************************

#endif


// fin del fichero
