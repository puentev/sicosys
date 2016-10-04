//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZNetwork.cpp
//
// Clase      : PRZNetwork
//
// Hereda de  : PRZComponent, PRZObserver
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZNetwork.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZNetwork.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZNetwork.hpp>

#ifndef __PRZDelayConnection_HPP__
#include <PRZDelayConnection.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZGlobalData_HPP__
#include <PRZGlobalData.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZNetwork,PRZComponent);

//*************************************************************************
//:
//  f: inline uTIME void setReportMsg(unsigned long numberMsg)
//  d:  rsim dinamic load evaluation
//
//  f: inline uTIME getCurrentTime() const;
//  d:
//
//  f: inline unsigned long getMessagesTx() const;
//  d:
//
//  f: inline unsigned long getMessagesRx() const;
//  d:
//
//  f: inline unsigned long getMessagesToTx() const;
//  d:
//
//  f: inline unsigned getSizeX() const;
//  d:
//
//  f: inline unsigned getSizeY() const;
//  d:
//
//  f: inline unsigned getSizeZ() const;
//  d:
//
//  f: inline uTIME getConnectionDelay() const;
//  d:
//
//  f: inline void setConnectionDelay(uTIME delay);
//  d:
//
//  f: inline double getTotalMessagesDelay() const;
//  d:
//
//  f: inline unsigned long getTotalDistance() const;
//  d:
//
//  f: inline uTIME getMaximLatency() const;
//  d:
//
//  f: inline void incrementMessagesTx();
//  d:
//
//  f: inline void incrementMessagesRx();
//  d:
//
//  f: inline void incrementMessagesDelay(uTIME delay);
//  d:
//
//  f: inline void incrementMessagesDistance(unsigned long dst);
//  d:
//
//  f: inline void setMaximLatency(uTIME time);
//  d:
//
//  f: inline PRZString getRouterId() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZNetwork( const PRZComponentId& id, 
//                 const PRZString& routerId,
//                 unsigned x, 
//                 unsigned y, 
//                 unsigned z );
//
//  d:
//:
//*************************************************************************

PRZNetwork :: PRZNetwork( const PRZComponentId& id,
                          const PRZString& routerId,
                          unsigned x, 
                          unsigned y, 
                          unsigned z )
            : PRZComponent(id),
              m_RouterId(routerId),
              m_SizeX(x),
              m_SizeY(y),
              m_SizeZ(z),
              m_CurrentTime(0.0),
              m_MessagesTx(0),
              m_MessagesRx(0),
              m_PacketsTx(0),
              m_PacketsRx(0),
              m_FlitsTx(0),
              m_FlitsRx(0),
              m_MessageDelayTotal(0.0),
              m_MessageDelayNetwork(0.0),
              m_MessageDelayBuffer(0.0),
              m_PacketDelayTotal(0.0),
              m_PacketDelayNetwork(0.0),
              m_PacketDelayBuffer(0.0),
              m_AverageDistance(0),
              m_MaxPacketLatency(0),
              m_MaxMessageLatency(0),
              m_ConnectionDelay(0),
              m_RouterMatrix(0),
              m_MessagesInNet(0),
              m_numberMsg(~0),
              m_pckRcvLast(0),
              m_FlitsRcvLast(0),
              m_LastTime(0),
              m_AverageDistanceold(0),
              m_buffPrint(""),
              m_LastTotalLatency(0)
{
   m_RouterMatrix = new PRZRouterMatrix(m_SizeX, m_SizeY, m_SizeZ);
   m_RouterMatrix->initialize(0);
}


//*************************************************************************
//:
//  f: virtual ~PRZNetwork();
//
//  d:
//:
//*************************************************************************

PRZNetwork :: ~PRZNetwork()
{
   delete [] m_Histogram;

}


//*************************************************************************
//:
//  f: virtual void terminate();
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: terminate()
{
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {

            PRZRouter* router = getRouterAt(i,j,k);
            router->terminate();
            delete router;
         }
         
   delete m_RouterMatrix;
   m_RouterMatrix = 0;
   
   Inhereited::terminate();
}

//*************************************************************************
//:
//  f: void addRouter(const PRZPosition& pos, PRZRouter* router);
//
//  d:
//: 
//*************************************************************************

void PRZNetwork :: addRouter(PRZRouter* router)
{
   unsigned x = router->getPosition().valueForCoordinate(PRZPosition::X);
   unsigned y = router->getPosition().valueForCoordinate(PRZPosition::Y);
   unsigned z = router->getPosition().valueForCoordinate(PRZPosition::Z);
   PRZRouter* tempRouter = 0;
   m_RouterMatrix -> valueAt(x,y,z,tempRouter);
   if( tempRouter )
   {
      PRZString text;
      text.sprintf( ERR_PRZNET_001,
                    (char*)PRZPosition(x,y,z).asString() );
      EXIT_PROGRAM(text);
   }
   m_RouterMatrix -> setValueAt(x,y,z,router);
   router->setOwner(this);
}


//*************************************************************************
//:
//  f: PRZRouter* getRouterAt(const PRZPosition& position) const;
//
//  d:
//:
//*************************************************************************

PRZRouter* PRZNetwork :: getRouterAt(const PRZPosition& position) const
{
   unsigned x = position.valueForCoordinate(PRZPosition::X);
   unsigned y = position.valueForCoordinate(PRZPosition::Y);
   unsigned z = position.valueForCoordinate(PRZPosition::Z);
   return getRouterAt(x,y,z);
}


//*************************************************************************
//:
//  f: PRZRouter* getRouterAt(unsigned x, unsigned y, unsigned z) const;
//
//  d:
//:
//*************************************************************************

PRZRouter* PRZNetwork :: getRouterAt(unsigned x, unsigned y, unsigned z) const
{
   PRZRouter* router;
   if( m_RouterMatrix->valueAt(x,y,z,router) )
      return router;
   return 0;
}


//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: run(uTIME runTime)
{
   m_CurrentTime = runTime;  
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZRouter* router = getRouterAt(i,j,k);
            router->run(runTime);
         }

   PRZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
   forCursor(connectionCursor)
   {
      connectionCursor.element()->run(runTime);
   }
}


//*************************************************************************
//:
//  f: virtual void setSimulation(PRZComponent* simulation);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: setSimulation(PRZComponent* simulation)
{
   if( !isInitializated() )
   {
      Inhereited::setSimulation(simulation);
      return;
   }
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZRouter* router = getRouterAt(i,j,k);
            router->setSimulation(simulation);
         }
   PRZRouter* router = getRouterAt(0,0,0);
}


//*************************************************************************
//:
//  f: virtual void setBufferSize(unsigned size);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: setBufferSize(unsigned size)
{
   if(size!=0)
   {
      PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData(this);
      gData.routerSetBufferSize(size);
   
      for( register int i=0; i<m_SizeX; i++ )
         for( register int j=0; j<m_SizeY; j++ )
            for( register int k=0; k<m_SizeZ; k++ )
            {
               PRZRouter* router = getRouterAt(i,j,k);
               router->setBufferSize(size);
	    }
            
   }
   //En cualquier caso ya conozco el valor definitivo de los buffers
   //luego puedo inyectar sin problemas.	
   PRZRouter* router = getRouterAt(0,0,0);    
   router->setPoolSize(m_SizeX*m_SizeY*m_SizeZ);

}

//*************************************************************************
//:
//  f: void sendMessage(const PRZMessage& msg);
//
//  d: front end para simos. Permite inyectar msg externamente desde
//     la jerarquia de memoria de simos.
//:
//*************************************************************************

void PRZNetwork :: sendMessage(const PRZMessage& msg)
{
	sendMessage(new PRZMessage(msg));
}
//*************************************************************************
//:
//  f: void sendMessage(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************
void PRZNetwork :: sendMessage(PRZMessage* msg)
{
   PRZPosition source = msg->source();  

   PRZRouter* router = getRouterAt(source);
   if(!router)
   {
      PRZString text;
      text.sprintf(ERR_PRZNET_002,
                   (char*)source.asString());
      EXIT_PROGRAM(text);
   }
  // msg->setGenerationTime(getCurrentTime());
   PRZMessage::assignUniqueId(msg);
   router->sendMessage(msg);
  
   incrementTx(Message);
   incrementTx(Packet,msg->messageSize());
   incrementTx(Flit,msg->messageSize()*msg->packetSize());
   unsigned radio = ((PRZSimulation*)getSimulation())->getRadius();
   unsigned distancia=distance(source,msg->destiny());
   m_Histogram[distancia-1]=m_Histogram[distancia-1]+1;
   incrementMessagesDistance(distancia);
   m_MessagesInNet ++;
}

//*************************************************************************
//:
//  f: PRZPosition CreatePosition(int num) ;
//
//  d:
//:
//*************************************************************************
PRZPosition PRZNetwork::CreatePosition(int num)     
{  
   unsigned numi=unsigned(num);
         
   if(m_SizeZ==1)
   {
      unsigned x = numi%unsigned(m_SizeX);
      unsigned y = numi/unsigned(m_SizeX);
      return PRZPosition(x,y);
   }
   else
   {
      unsigned z=numi/(m_SizeX*m_SizeY);
      unsigned zproyectada=numi-z*(m_SizeX*m_SizeY);
      unsigned x=zproyectada%m_SizeX;
      unsigned y=zproyectada/m_SizeX;
            
      return PRZPosition(x,y,z);
   }
}
//*************************************************************************
//:
//  f: int Number_of_nodes() const
//
//  d:
//:
//*************************************************************************
int PRZNetwork::Number_of_nodes()
{
   return m_SizeX*m_SizeY*m_SizeZ;
}
         
         
//*************************************************************************
//:
//  f: void* getExternalInfoAt(unsigned x, unsigned y, unsigned z=0);
//
//  d:
//:
//*************************************************************************

void* PRZNetwork :: getExternalInfoAt(unsigned x, unsigned y, unsigned z)
{
   PRZRouter* router = getRouterAt(x,y,z);
   if( ! router )  
   {
      PRZString text;
      text.sprintf( ERR_PRZNET_003, x, y );
      EXIT_PROGRAM(text);
   }
   return router->getExternalInfo();
   
}
//*************************************************************************
//:
//  f: void* getExternalInfoAt(PRZPosition pos);
//
//  d:
//:
//*************************************************************************

void* PRZNetwork :: getExternalInfoAt(PRZPosition pos)
{
   
   PRZRouter* router = getRouterAt(pos);
   if( ! router )  
   {
      PRZString text;
      EXIT_PROGRAM(text);
   }
   return router->getExternalInfo();
   
}
//*************************************************************************
//:
//  f:       void   clearExternalInfoAt(PRZPosition pos);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: clearExternalInfoAt(PRZPosition pos)
{
   
   PRZRouter* router = getRouterAt(pos);
   if( ! router )  
   {
      PRZString text;
      EXIT_PROGRAM(text);
   }
   router->setExternalInfo(0);
   
}
//*************************************************************************
//:
//  f: void initializeHistogram(int a) ;
//
//  d:Genera un histograma de distancias recorridas
//:
//*************************************************************************
void PRZNetwork ::initializeHistogram(unsigned diametro)
{
   m_Histogram=new unsigned long[diametro];
   m_diametro=diametro;
   
   for(int i=0;i<diametro;i++)
       m_Histogram[i]=0;
}
//*************************************************************************
//:
//  f: void destroyHistogram() ;
//
//  d:Genera un histograma de distancias recorridas
//:
//*************************************************************************
void PRZNetwork ::destroyHistogram()
{
   delete m_Histogram;
}
//*************************************************************************
//:
//  f: PRZString printHistogram() ;
//
//  d:Genera un histograma de distancias recorridas
//:
//*****************************************************************
PRZString PRZNetwork ::printHistogram()
{
   PRZString temp;
   double suma=0;
   int i;
   PRZString percent;
   for(i=0;i<m_diametro;i++)
   {
      suma=m_Histogram[i]+suma;
   }
   
   PRZString buffer="##****************************************************\n";
   buffer=buffer+"##\tHops\t Message Count \t Percent\n";
   buffer=buffer+"##*****************************************************\n";
   for(i=0;i<m_diametro;i++)
   {
      if(m_Histogram[i]!=0)
      {
         temp.sprintf("##\t%d \t %d        \t %1.3f\n",i+1,m_Histogram[i],double(m_Histogram[i]/suma*100.0));
         buffer+=temp;
      }
   }
   return buffer;
}
//*************************************************************************
//:
//  f: unsigned long getMessagesToTx() const;
//
//  d:
//:
//*************************************************************************

unsigned long PRZNetwork :: getMessagesToTx() const
{
   unsigned long totalMssg = 0;
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZRouter* router = getRouterAt(i,j,k);
            totalMssg += router->getMessagesToSend();
         }
   return totalMssg;
}


//*************************************************************************
//:
//  f: void writeComponentStatus(ostream& os) const;
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: writeComponentStatus(ostream& os) const
{
   os << "TIME=" << getCurrentTime() << endl << endl;
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZRouter* router = getRouterAt(i,j,k);
            router->writeComponentStatus(os);
         }
}


//*************************************************************************
//:
//  f: void writeBufferInformation(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: writeBufferInformation(ostream& os)
{
   unsigned short xSize = getSizeX();
   unsigned short ySize = getSizeY();
   unsigned short zSize = getSizeZ();
   
   os.write((char*)&xSize,sizeof(unsigned short));
   os.write((char*)&ySize,sizeof(unsigned short));
   os.write((char*)&zSize,sizeof(unsigned short));
   
   PRZRouter* router = getRouterAt(0,0,0);
   if( router ) router-> writeBufferInformation(os);
}


//*************************************************************************
//:
//  f: void writeBufferStatus(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: writeBufferStatus(ostream& os)
{
   os.write((char*)&m_CurrentTime,sizeof(uTIME));

   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZRouter* router = getRouterAt(i,j,k);
            router->writeBufferStatus(os);
         }
}


//*************************************************************************
//:
//  f: void onHeaderPacketReceived(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: onHeaderPacketReceived(const PRZMessage* msg)
{
     
   //incrementMessagesDistance(msg.getDistance());
}


//*************************************************************************
//:
//  f: void onPacketReceived(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************
void PRZNetwork :: onPacketReceived(const PRZMessage* msg)
{
   uTIME t1 = msg->generationTime();
   uTIME t2 = msg->packetInjectionTime();
   uTIME t3 = getCurrentTime();
   uTIME totalTime = t3 - t1;
   /*
   cerr << "Current=" << t3 << "\tGen=" << t1 << "\tInj=" << t2  <<
            "\tTotal=" << totalTime << "\tNet=" << (t3-t2) <<
            "\tBuff=" << (t2-t1) << endl;
   */
   incrementRx(Packet);
   incrementRx(Flit,msg->packetSize());
   incrementDelayTotal(Packet,totalTime);
   incrementDelayNetwork(Packet,t3-t2);
   incrementDelayBuffer(Packet,t2-t1);
   
   if( totalTime > getMaximLatency(Packet) )
      setMaximLatency(Packet,totalTime);
   
   //Traza dinamica en Rsim
   if(m_PacketsRx-m_pckRcvLast>=m_numberMsg )
   {
      if(m_LastTime==0){
         m_buffPrint="##*************************************************************************\n";
         m_buffPrint+="##Proc   \tRXLoad    \tRXLoad      \tMeanLatency\tMean Distance\n";
         m_buffPrint+="##Cy     \t(p/NetCyc)\t(fli/NetCyc)\t(NetCyc)   \t(hops)\n";
         m_buffPrint+="##*************************************************************************\n";
         
         //cout <<m_buffPrint;
         //m_buffPrint="";
      }
      
      uTIME Time_interval=m_CurrentTime-m_LastTime;          
      
      PRZString temp;
      
      m_buffPrint+=PRZString("##")+PRZString(m_CurrentTime)+PRZString("   \t");
      
      temp.sprintf("%2.3f    \t",double(m_PacketsRx-m_pckRcvLast)/double(Time_interval));
      m_buffPrint+=temp;
      temp.sprintf("%5.3f    \t",double(m_FlitsRx-m_FlitsRcvLast)/double(Time_interval));
      m_buffPrint+=temp;
      temp.sprintf("%6.3f    \t",(m_PacketDelayTotal)/m_numberMsg);
      m_buffPrint+=temp;
      
      //Informacion de distancia media recorrida
      //Util para valorar el remapeo de la MIDIMEW entre otras cosas

      temp.sprintf("%6.3f\n",double(m_AverageDistance-m_AverageDistanceold)/double(m_PacketsRx-m_pckRcvLast));
      m_buffPrint+=temp;
      
      //Reset Stats
      m_FlitsRcvLast=m_FlitsRx;
      m_pckRcvLast=m_PacketsRx;      
      m_LastTime=m_CurrentTime;
      m_LastTotalLatency=m_PacketDelayTotal;
      m_PacketDelayTotal=0; //pa burro yo
      m_AverageDistanceold=m_AverageDistance;

          

   }
   //if(m_numberMsg!=0)
   //    m_buffPrint+=printHistogram();
}

//*************************************************************************
//:
//  f: void onMessageReceived(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: onMessageReceived(const PRZMessage* msg)
{
   uTIME t1 = msg->generationTime();
   uTIME t2 = msg->packetInjectionTime();
   uTIME t3 = getCurrentTime();
   uTIME totalTime = t3 - t1;

   incrementRx(Message);
   incrementDelayTotal(Message,totalTime);
   incrementDelayNetwork(Message,t3-t2);
   incrementDelayBuffer(Message,t2-t1);
   
   if( totalTime > getMaximLatency(Message) ) 
      setMaximLatency(Message,totalTime);

   m_MessagesInNet--;
}


//*************************************************************************
//:
//  f: void setMaximLatency(int type, uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: setMaximLatency(int type, uTIME time)
{
   if( type == PRZNetwork::Packet )
   {
      m_MaxPacketLatency = time;
   }
   else if( type == PRZNetwork::Message )
   {
      m_MaxMessageLatency = time;
   }
}


//*************************************************************************
//:
//  f: uTIME getMaximLatency(int type) const;
//
//  d:
//:
//*************************************************************************

uTIME PRZNetwork :: getMaximLatency(int type) const
{
   uTIME rc = 0;
   
   if( type == PRZNetwork::Packet )
   {
      rc = m_MaxPacketLatency;
   }
   else if( type == PRZNetwork::Message )
   {
      rc = m_MaxMessageLatency;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: void incrementDelayTotal(int type, uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: incrementDelayTotal(int type, uTIME time)
{
   if( type == PRZNetwork::Packet )
   {
      m_PacketDelayTotal += double(time);
   }
   else if( type == PRZNetwork::Message )
   {
      m_MessageDelayTotal += double(time);
   }
}


//*************************************************************************
//:
//  f: void incrementDelayNetwork(int type, uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: incrementDelayNetwork(int type, uTIME time)
{
   if( type == PRZNetwork::Packet )
   {
      m_PacketDelayNetwork += time;
   }
   else if( type == PRZNetwork::Message )
   {
      m_MessageDelayNetwork += time;
   }
}


//*************************************************************************
//:
//  f: void incrementDelayBuffer(int type, uTIME time);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: incrementDelayBuffer(int type, uTIME time)
{
   if( type == PRZNetwork::Packet )
   {
      m_PacketDelayBuffer += time;
   }
   else if( type == PRZNetwork::Message )
   {
      m_MessageDelayBuffer += time;
   }
}

//*************************************************************************
//:
//  f: void incrementRx(int type, unsigned number=1);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: incrementRx(int type, unsigned number)
{
   if( type == PRZNetwork::Flit )
   {
      m_FlitsRx += number;
   }
   else if( type == PRZNetwork::Packet )
   {
      m_PacketsRx += number;
   }
   else if( type == PRZNetwork::Message )
   {
      m_MessagesRx += number;
   }
}


//*************************************************************************
//:
//  f: void incrementTx(int type, unsigned number=1);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: incrementTx(int type, unsigned number)
{
   if( type == PRZNetwork::Flit )
   {
      m_FlitsTx += number;
   }
   else if( type == PRZNetwork::Packet )
   {
      m_PacketsTx += number;
   }
   else if( type == PRZNetwork::Message )
   {
      m_MessagesTx += number;
   }
}


//*************************************************************************
//:
//  f: double getTotalDelay(int type=Message) const;
//
//  d:
//:
//*************************************************************************

double PRZNetwork :: getTotalDelay(int type) const
{
   double rc = 0.0;
   
   if( type == PRZNetwork::Packet )
   {
      rc = m_PacketDelayTotal;
   }
   else if( type == PRZNetwork::Message )
   {
      rc = m_MessageDelayTotal;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: double getNetworkDelay(int type=Message) const;
//
//  d:
//:
//*************************************************************************

double PRZNetwork :: getNetworkDelay(int type) const
{
   double rc = 0.0;
   
   if( type == PRZNetwork::Packet )
   {
      rc = m_PacketDelayNetwork;
   }
   else if( type == PRZNetwork::Message )
   {
      rc = m_MessageDelayNetwork;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: double getBufferDelay(int type=Message) const;
//
//  d:
//:
//*************************************************************************

double PRZNetwork :: getBufferDelay(int type) const
{
   double rc = 0.0;
   
   if( type == PRZNetwork::Packet )
   {
      rc = m_PacketDelayBuffer;
   }
   else if( type == PRZNetwork::Message )
   {
      rc = m_MessageDelayBuffer;
   }
   
   return rc;
}


//*************************************************************************
//:
//  f: void initializeConnectionsFor(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

void PRZNetwork :: initializeConnectionsFor(const PRZPosition& pos)
{

   PRZRouter* router = getRouterAt(pos);
   PRZRouter* routerXp = getRouterAt( positionOf(_Xplus_,pos)  );
   PRZRouter* routerXm = getRouterAt( positionOf(_Xminus_,pos) );
   PRZRouter* routerYp = getRouterAt( positionOf(_Yplus_,pos)  );
   PRZRouter* routerYm = getRouterAt( positionOf(_Yminus_,pos) );
   PRZRouter* routerZp = getRouterAt( positionOf(_Zplus_,pos)  );
   PRZRouter* routerZm = getRouterAt( positionOf(_Zminus_,pos) );


   unsigned iXp = router->getInputWithType(_Xplus_);
   unsigned iXm = router->getInputWithType(_Xminus_);
   unsigned iYp = router->getInputWithType(_Yplus_);
   unsigned iYm = router->getInputWithType(_Yminus_);
   unsigned iZp = router->getInputWithType(_Zplus_);
   unsigned iZm = router->getInputWithType(_Zminus_);
   
   unsigned oXp = router->getInputWithType(_Xplus_);
   unsigned oXm = router->getInputWithType(_Xminus_);
   unsigned oYp = router->getInputWithType(_Yplus_);
   unsigned oYm = router->getInputWithType(_Yminus_);
   unsigned oZp = router->getInputWithType(_Zplus_);
   unsigned oZm = router->getInputWithType(_Zminus_);

   if( iXp && oXp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXp),
                                        routerXp->getInputInterfaz(iXp) );

   if( iXm && oXm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oXm),
                                        routerXm->getInputInterfaz(iXm),
                                        getConnectionDelay() );

   if( iYp && oYp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYp),
                                        routerYp->getInputInterfaz(iYp),
                                        getConnectionDelay() );

   if( iYm && oYm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oYm),
                                        routerYm->getInputInterfaz(iYm),
                                        getConnectionDelay() );

   if( iZp && oZp )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZp),
                                        routerZp->getInputInterfaz(iZp),
                                        getConnectionDelay() );

   if( iZm && oZm )
      PRZConnection::connectInterfaces( this,
                                        router->getOutputInterfaz(oZm),
                                        routerZm->getInputInterfaz(iZm),
                                        getConnectionDelay() );

}


//*************************************************************************
//:
//  f: PRZPosition positionOf(przROUTINGTYPE dir, const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

PRZPosition PRZNetwork :: positionOf(przROUTINGTYPE dir, const PRZPosition& pos)
{

   PRZPosition rp = pos;
   switch(dir)
   {
      case _Xplus_:
               rp = rp + PRZPosition(1,0,0);
               break;
               
      case _Xminus_:
               rp = rp - PRZPosition(1,0,0);      
               break;
              
      case _Yplus_:
               rp = rp + PRZPosition(0,1,0);     
               break;
              
      case _Yminus_:
              rp = rp - PRZPosition(0,1,0);           
              break;

      case _Zplus_:
               rp = rp + PRZPosition(0,0,1);     
               break;
              
      case _Zminus_:
              rp = rp - PRZPosition(0,0,1);           
              break;
   }
   
   if( rp.valueForCoordinate(PRZPosition::X) < 0 )
      rp.setValueAt(PRZPosition::X, m_SizeX-1);
      
   if( rp.valueForCoordinate(PRZPosition::X) >= m_SizeX )
      rp.setValueAt(PRZPosition::X, 0);
      
   if( rp.valueForCoordinate(PRZPosition::Y) < 0 )
      rp.setValueAt(PRZPosition::Y, m_SizeY-1);
      
   if( rp.valueForCoordinate(PRZPosition::Y) >= m_SizeY )
      rp.setValueAt(PRZPosition::Y, 0);
      
   if( rp.valueForCoordinate(PRZPosition::Z) < 0 )
      rp.setValueAt(PRZPosition::Z, m_SizeZ-1);
      
   if( rp.valueForCoordinate(PRZPosition::Z) >= m_SizeZ )
      rp.setValueAt(PRZPosition::Z, 0);
      
   return rp;
   
}


//*************************************************************************


// fin del fichero
