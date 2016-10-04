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
// File: ../ATCSimul/inc/SFSSimpleRouterFlow.hpp
//
// Description:  fichero de encabezado de la clase "SFSSimpleRouterFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: SFSSimpleRouterFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __SFSSimpleRouterFlow_HPP__
#define __SFSSimpleRouterFlow_HPP__

//*************************************************************************

   #include <PRZFlow.hpp>

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif

   #ifndef __PRZQueue_HPP__
   #include <PRZQueue.hpp>
   #endif

   #ifndef __PRZPair_HPP__
   #include <PRZPair.hpp>
   #endif
      
//*************************************************************************

   class PRZMessage;
   
//*************************************************************************
   
   typedef PRZQueue<PRZMessage*>      PRZMessageQueue;

//*************************************************************************

   class SFSSimpleRouterFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;
   
   public:
      SFSSimpleRouterFlow( PRZComponent& component);
      virtual ~SFSSimpleRouterFlow();

      virtual void initialize();
      virtual void terminate();

      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual PRZString getStatus() const;
      
      // Run time information
      DEFINE_RTTI(SFSSimpleRouterFlow);

   protected:
      virtual  void    cleanOutputInterfaces();
      virtual  void    cleanInputInterfaces();
      virtual  Boolean propagateStop();
      virtual  Boolean updateMessageInfo(PRZMessage* msg);
      virtual  Boolean dispatchEvent(const PRZEvent& event);
      
      virtual  unsigned extractOutputPortNumber(przROUTINGTYPE dir);
      virtual  unsigned extractOutputPortNumber(PRZMessage* msg);
      
      unsigned extractOutputChannel(PRZMessage* msg) const;

   protected:
      Boolean bubbleReady(unsigned inPort) const;
      Boolean cleanInterfaces() const
      { return m_CleanInterfaces; }
      
      void setCleanInterfaces(Boolean value)
      { m_CleanInterfaces = value; }
      
      Boolean m_CleanInterfaces;
   private:
      unsigned m_connections[6];
      unsigned m_bufferSize;
      unsigned m_token;
      PRZMessage* m_routing[6];
      int m_routingtime[6];
      PRZMessage* m_sync[6];
      Boolean m_changeDirection[6];
      PRZMessageQueue m_fifos[6];
            
};

//*************************************************************************


#endif


// fin del fichero
