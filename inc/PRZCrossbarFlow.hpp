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
// File: ../ATCSimul/inc/PRZCrossbarFlow.hpp
//
// Description:  fichero de encabezado de la clase "PRZCrossbarFlow"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZCrossbarFlow.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZCrossbarFlow_HPP__
#define __PRZCrossbarFlow_HPP__

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
   
   typedef PRZPair<unsigned,unsigned> uPAIR;
   typedef PRZArray<uPAIR>            PRZCrossbarTable;
   typedef PRZArray<PRZMessage*>      PRZMessageTable;

//*************************************************************************

   class PRZCrossbarFlow : public PRZFlow
   {
      typedef PRZFlow Inhereited;
   
   protected:
      typedef PRZQueue<uPAIR> PRZRequestQueue;   
            
   public:
      typedef enum { Free, Connect, PassMessage, Waiting } PRZCrossbarState;
      
      PRZCrossbarFlow( PRZComponent& component);
      virtual ~PRZCrossbarFlow();

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
      DEFINE_RTTI(PRZCrossbarFlow);

   protected:
      virtual  void    cleanOutputInterfaces();
      virtual  void    cleanInputInterfaces();
      virtual  Boolean propagateStop();
      virtual  void    cleanConnectionRequestFor( unsigned port, 
                                                  const uPAIR& pair=uPAIR(0,0));
      virtual  Boolean dispatchEvent(const PRZEvent& event);
      Boolean  addConnectionRequest(unsigned source, uPAIR destiny);
      unsigned getConnectionRequestFor(unsigned port);
      Boolean  isAnyRequestForOutput(unsigned output);
      
      uPAIR    getOutputForInput(unsigned inputPort) const;
      virtual  Boolean  isOutputPortFree(unsigned outputPort, unsigned channel=0) const;   
      void     establishConnection(unsigned input, unsigned output, unsigned channel=0);
      virtual  void     clearConnection(unsigned input);
      
      virtual  unsigned extractOutputPortNumber(przROUTINGTYPE dir, unsigned channel);
      virtual  unsigned extractOutputPortNumber(PRZMessage* msg);
      virtual  Boolean  updateMessageInfo(PRZMessage* msg, unsigned oPort, unsigned channel);
      
      unsigned extractOutputChannel(PRZMessage* msg) const;

      unsigned getLastInputForOutput(unsigned output) const;
      void     setLastInputForOutput(unsigned input,unsigned output);
      void     setStateForInput(unsigned i, PRZCrossbarState state);
      uPAIR    checkMultipleRequestFor(unsigned oPort);
      void     removeAllRequestEventsForInput(unsigned iPort);
      void     removeAllRequestEventsForOutput(unsigned oPort);
      void     enableEvent(int event);
      void     disableEvent(int event);
      Boolean  isEventEnable(int event);
      PRZCrossbarState getStateForInput(unsigned i) const;

      Boolean cleanInterfaces() const
      { return m_CleanInterfaces; }
      
      void setCleanInterfaces(Boolean value)
      { m_CleanInterfaces = value; }
            
   protected:
      
      typedef PRZArray<PRZCrossbarState> PRZStateTable;

      PRZCrossbarTable*   m_PortAsignamentTable;
      PRZArray<unsigned>* m_OutputStateTable;
      PRZArray<unsigned>* m_OutputLastUseTable;
      PRZMessageTable*    m_MessageReceivedTable;
      PRZStateTable*      m_InputStateTable;
      PRZRequestQueue*    m_RequestQueue;
      Boolean             m_CleanInterfaces;
      Boolean             m_EventsActive[2];
};

//*************************************************************************


#endif


// fin del fichero
