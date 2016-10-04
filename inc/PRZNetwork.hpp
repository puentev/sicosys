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
// File: ../ATCSimul/inc/PRZNetwork.hpp
//
// Description:  fichero de encabezado de la clase "PRZNetwork"
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZNetwork.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZNetwork_HPP__
#define __PRZNetwork_HPP__

//*************************************************************************

   #ifndef __PRZComponent_HPP__
   #include <PRZComponent.hpp>
   #endif
   
   #ifndef __PRZMatrix_HPP__
   #include <PRZMatrix.hpp>
   #endif

   #ifndef __PRZRouter_HPP__
   #include <PRZRouter.hpp>
   #endif

//*************************************************************************

   class PRZNetwork : public PRZComponent
   {
   public:
      typedef PRZComponent Inhereited;
      typedef PRZMatrix<PRZRouter*> PRZRouterMatrix;
      enum { Flit, Packet, Message };
      
      PRZNetwork( const PRZComponentId& id,
                  const PRZString& routerId,
                  unsigned x, 
                  unsigned y, 
                  unsigned z=1 );
      ~PRZNetwork();

      PRZRouter* getRouterAt(const PRZPosition& position) const;
      PRZRouter* getRouterAt(unsigned x, unsigned y, unsigned z) const;
      
      virtual void run(uTIME runTime=0);
      virtual void terminate();
      virtual void setSimulation(PRZComponent* simulation);
      
      virtual unsigned distance(const PRZPosition& src, const PRZPosition& dst)=0;
      virtual void     routingRecord( const PRZPosition& src,
                                      const PRZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ ) = 0;
      
      void sendMessage(PRZMessage* msg);
      void sendMessage(const PRZMessage& msg);

      //VPV-Rsim redes 3d y rectangulares
      virtual PRZPosition CreatePosition(int num);
      
      //Reporte dinamico de parametros de red  para rsim)
      void setReportMsg(unsigned long numberMsg)
            { m_numberMsg=numberMsg;}
      
      unsigned long getReportMsg()
            {return m_numberMsg;}
      
      PRZString getReport()
            {return m_buffPrint;}
      //**********************************//
      
      
      void writeBufferInformation(ostream& os);
      void writeBufferStatus(ostream& os);
      void writeComponentStatus(ostream& os) const;
      
      void onHeaderPacketReceived(const PRZMessage* msg);
      void onPacketReceived(const PRZMessage* msg);
      void onMessageReceived(const PRZMessage* msg);

      double getTotalDelay(int type=Message) const;
      double getNetworkDelay(int type=Message) const;
      double getBufferDelay(int type=Message) const;
      void*  getExternalInfoAt(unsigned x, unsigned y, unsigned z=0);
      void*  getExternalInfoAt(PRZPosition pos);
      void   clearExternalInfoAt(PRZPosition pos);
      
      void   setBufferSize(unsigned size);

      uTIME getCurrentTime() const
      { return m_CurrentTime; }

      unsigned long getMessagesTx() const
      { return m_MessagesTx; }

      unsigned long getMessagesRx() const
      { return m_MessagesRx; }

      unsigned long getPacketsTx() const
      { return m_PacketsTx; }
      
      unsigned long getPacketsRx() const
      { return m_PacketsRx; }

      unsigned long getFlitsTx() const
      { return m_FlitsTx; }
      
      unsigned long getFlitsRx() const
      { return m_FlitsRx; }


      unsigned long getMessagesToTx() const;
      
      void initializeHistogram(unsigned); 
      
      void destroyHistogram();
      
      PRZString printHistogram();
      
      unsigned getSizeX() const
      { return m_SizeX; }

      unsigned getSizeY() const
      { return m_SizeY; }
      
      unsigned getSizeZ() const
      { return m_SizeZ; }

      int Number_of_nodes();

      uTIME getConnectionDelay() const
      { return m_ConnectionDelay; }

      void setConnectionDelay(uTIME delay)
      { m_ConnectionDelay=delay; }

      unsigned long getTotalDistance() const
      { return m_AverageDistance; }

      unsigned getMessagesInNet() const
      { return m_MessagesInNet; }
      
      //virtual unsigned getDiameter() const
      //{return 1;}
      
      uTIME getMaximLatency(int type) const;
      
      // Run time information
      DEFINE_RTTI(PRZNetwork);
            
   protected:
      void addRouter(PRZRouter* router);
      virtual void initializeConnectionsFor(const PRZPosition& pos);
      PRZPosition positionOf(przROUTINGTYPE dir, const PRZPosition& pos);
            
      void incrementRx(int type, unsigned number=1);
      void incrementTx(int type, unsigned number=1);

      void incrementDelayTotal(int type, uTIME delay);
      void incrementDelayNetwork(int type, uTIME delay);
      void incrementDelayBuffer(int type, uTIME delay);
      void setMaximLatency(int type, uTIME time);
            
      void incrementMessagesDistance(unsigned long dst)
      { m_AverageDistance += dst; }
   
      PRZString getRouterId() const
      { return m_RouterId; }
   
   private:
      PRZRouterMatrix* m_RouterMatrix;
      unsigned         m_SizeX;
      unsigned         m_SizeY;
      unsigned         m_SizeZ;
      uTIME            m_CurrentTime;
      
      unsigned long    m_MessagesTx;
      unsigned long    m_MessagesRx;
      unsigned long    m_PacketsTx;
      unsigned long    m_PacketsRx;
      unsigned long    m_FlitsTx;
      unsigned long    m_FlitsRx;
      
      unsigned long    m_AverageDistance;
      
      double           m_MessageDelayTotal;
      double           m_MessageDelayNetwork;
      double           m_MessageDelayBuffer;
      
      double           m_PacketDelayTotal;
      double           m_PacketDelayNetwork;
      double           m_PacketDelayBuffer;
      
      uTIME            m_MaxPacketLatency;
      uTIME            m_MaxMessageLatency;
      
      PRZString        m_RouterId;
      uTIME            m_ConnectionDelay;

      unsigned         m_MessagesInNet;
      unsigned long    m_numberMsg;
      unsigned long    m_pckRcvLast;
      uTIME            m_LastTime;
      PRZString        m_buffPrint;
      unsigned long    m_FlitsRcvLast;
      double           m_LastTotalLatency;
      unsigned long    m_AverageDistanceold;
      
      unsigned long   *m_Histogram;
      unsigned long    m_diametro;
   };

//*************************************************************************


#endif


// fin del fichero
