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
// File: ../ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMRVirtual.cpp
//
// Clase      : PRZRoutingAdaptativeBurbleFlowMRVirtual
//
// Hereda de  : PRZRoutingFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRoutingAdaptativeBurbleFlowMRVirtual.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRoutingAdaptativeBurbleFlowMRVirtual.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZRoutingAdaptativeBurbleFlowMRVirtual.hpp>

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
#endif


#ifndef __PRZRouting_HPP__
#include <PRZRouting.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRoutingAdaptativeBurbleFlowMRVirtual,PRZRoutingAdaptativeBurbleFlowMR);


//*************************************************************************
//:
//  f: PRZRoutingAdaptativeBurbleFlowMRVirtual(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZRoutingAdaptativeBurbleFlowMRVirtual :: PRZRoutingAdaptativeBurbleFlowMRVirtual( PRZComponent& component)
                            : PRZRoutingAdaptativeBurbleFlowMR(component),
                              m_BufferXpReq(0),
                              m_BufferXmReq(0),
                              m_BufferYpReq(0),
                              m_BufferYmReq(0),
                              m_BufferXpRep(0),
                              m_BufferXmRep(0),
                              m_BufferYpRep(0),
                              m_BufferYmRep(0)
{
  // setState(Free);
}

//*************************************************************************
//:
//  f: void postInitialize();
//
//  d: facilta el accceso a los bufferes locales del router para aplicar bubble
//     aniado sobre elñ punto de partida (Routing VC)
//:
//*************************************************************************

void PRZRoutingAdaptativeBurbleFlowMRVirtual :: postInitialize()
{  
   PRZComponentId idXp( PRZComponentId::FifoMemory, _Xplus_,  "BUF13", 3 );
   PRZComponentId idXm( PRZComponentId::FifoMemory, _Xminus_, "BUF23", 3 );
   PRZComponentId idYp( PRZComponentId::FifoMemory, _Yplus_,  "BUF33", 3 );
   PRZComponentId idYm( PRZComponentId::FifoMemory, _Yminus_, "BUF43", 3 );

   PRZComponent* compoXp = getOwnerRouter().componentWithIdLike(idXp); 
   PRZComponent* compoXm = getOwnerRouter().componentWithIdLike(idXm); 
   PRZComponent* compoYp = getOwnerRouter().componentWithIdLike(idYp);
   PRZComponent* compoYm = getOwnerRouter().componentWithIdLike(idYm);
   
   m_BufferXpReq = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXmReq = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYpReq = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYmReq = POINTERCAST(PRZFifoMemory,compoYm);
   
   if( !(m_BufferXpReq && m_BufferXmReq && m_BufferYpReq && m_BufferYmReq) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
	
   PRZComponentId idXpRep( PRZComponentId::FifoMemory, _Xplus_,  "BUF12", 2 );
   PRZComponentId idXmRep( PRZComponentId::FifoMemory, _Xminus_, "BUF22", 2 );
   PRZComponentId idYpRep( PRZComponentId::FifoMemory, _Yplus_,  "BUF32", 2 );
   PRZComponentId idYmRep( PRZComponentId::FifoMemory, _Yminus_, "BUF42", 2 );
   
   compoXp = getOwnerRouter().componentWithIdLike(idXpRep); 
   compoXm = getOwnerRouter().componentWithIdLike(idXmRep); 
   compoYp = getOwnerRouter().componentWithIdLike(idYpRep);
   compoYm = getOwnerRouter().componentWithIdLike(idYmRep);
    
   m_BufferXpRep = POINTERCAST(PRZFifoMemory,compoXp);
   m_BufferXmRep = POINTERCAST(PRZFifoMemory,compoXm);
   m_BufferYpRep = POINTERCAST(PRZFifoMemory,compoYp);
   m_BufferYmRep = POINTERCAST(PRZFifoMemory,compoYm);
   
   if( !(m_BufferXpRep && m_BufferXmRep && m_BufferYpRep && m_BufferYmRep) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRAFLO_001, (char*)getComponent().asString() );
      EXIT_PROGRAM(err);  
   }
}

//*************************************************************************
//:
//  f: Boolean createMessageRequest(PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMRVirtual :: createMessageRequest(PRZMessage** pMsg)
{
   if((*pMsg = getMessagePool().allocate())==0)
   {
         PRZString err;
         err.sprintf(ERR_SFSPOOL_001);
         EXIT_PROGRAM(err);    
   }
   PRZMessage* msg=*pMsg;
   msg->setType(PRZMessage::MREQ);
   
   if( m_FlitsRx.numberOfElements() == 0 ) return false;
 
   PRZMessage *headerMsg = m_FlitsRx.element(1);
   unsigned channelDor=2;
   if(m_FlitsRx.element(1)->isRequest()) channelDor=3;
   
   //El criterio de consumo cambia en funcion de si hay miss
   //routing o no (1/1 es consumo sin missrouting y 0/0 con misrouting)
   PRZRouting& routing=(PRZRouting&)(getComponent());
   short missrouting=routing.getMissRouting();
   short testR;
   if(missrouting==0)
   {
      testR=1;
   }
   else
   {
      testR=0;
   }
    
   if( (headerMsg->deltaAbs(0)==testR) && (headerMsg->deltaAbs(1)==testR) ) 
   {
      // Va hacia el consumo. En el caso de multiples consumidores,
      // indicaremos en el canal el numero de consumidor al que debe
      // ir ( primero, ..., n-esimo).
         
      unsigned localPort = getCurrentLocalPort();
      TPortList* portList = new TPortList();
      portList->addAsLast(TUPair(_LocalNode_,localPort));
      msg->setPortList(portList);
      return true;
    }

   int deltaX = headerMsg->delta(0);
   int deltaY = headerMsg->delta(1);
   
   //
   //Miss routing Support
   //
   int missRoutingX = headerMsg->getMissRouting(0);
   int missRoutingY = headerMsg->getMissRouting(1);
   
   
         
   unsigned absDeltaX = headerMsg->deltaAbs(0);
   unsigned absDeltaY = headerMsg->deltaAbs(1);

   TPortList* portList = new TPortList();
   msg->setPortList(portList);
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();

//    funcion de seleccion aleatoria de verdad

//Orden normal
  if(!msg->getLastMissRouted())
  {     
    if((drand48()<0.50)){
       if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
      }
      else{
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
        if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
      }
   }
   else
      //
      //Se da la vuelta el orden de las peticiones
      //si no la cagamos 
      //El paquete vuelve por el mismo camino
      //
   {
      if(myDirection==_Yminus_ || myDirection==_Yplus_){
       if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
      }
      else{
        if( absDeltaY > testR )
        {
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),1));
        }
        if( absDeltaX > testR )
        {
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),1));
        }
      }
      
   }
            
   
      if( absDeltaX > testR )
      {
         if( controlAlgoritm(true,deltaX) ){
            portList->addAsLast(TUPair(((deltaX>0)?_Xplus_:_Xminus_),channelDor)); 
         }     
      }
      else if( (absDeltaX == testR)  && (absDeltaY > testR) )
      {
         if( controlAlgoritm(false,deltaY) ){ 
            portList->addAsLast(TUPair(((deltaY>0)?_Yplus_:_Yminus_),channelDor));
         }

      }
   
   
   //------------------------------------------------------
   //--------------==|Miss routing block|==----------------
   //Only Adaptive channels accept miss Routed Messages
   //Last option in Selection Function.
   //------------------------------------------------------
   
    if(myDirection==_Xminus_ || myDirection==_Xplus_ || myDirection==_LocalNode_)
    { 
        //Primero se realiza el Missrouting en la dimension superior a la que nos encontramos
        if(missRoutingY!=0)
        {
           if( absDeltaY!=0)
           {
               //portList->addAsLast(TUPair(((deltaY>0)?_Yminus_:_Yplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Yplus_,1));
                     portList->addAsLast(TUPair(_Yminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Yminus_,1));
                     portList->addAsLast(TUPair(_Yplus_,1));

                 }
           }
        }
        
        //No hago missrouting hacia atras
        if(missRoutingX!=0 )
        {
           if( absDeltaX != 0  )
           {
               //portList->addAsLast(TUPair(((deltaX>0)?_Xminus_:_Xplus_),1));
           }
           else
           {
                 if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Xplus_,1));
                     portList->addAsLast(TUPair(_Xminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Xminus_,1));
                     portList->addAsLast(TUPair(_Xplus_,1));

                 }
           }  
        }
      }
      else
      {
         //Estamos en una Y=> primero missrouting por X y despues por Y
         
         if(missRoutingX!=0)
        {
           if( absDeltaX != 0 )
           {
               //portList->addAsLast(TUPair(((deltaX>0)?_Xminus_:_Xplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Xplus_,1));
                     portList->addAsLast(TUPair(_Xminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Xminus_,1));
                     portList->addAsLast(TUPair(_Xplus_,1));

                 }
           }  
        } 

        if(missRoutingY!=0 )
        {
           if( absDeltaY != 0 )
           {
               //portList->addAsLast(TUPair(((deltaY>0)?_Yminus_:_Yplus_),1));
           }
           else
           {
              if(drand48()<0.5)
                 {
                     portList->addAsLast(TUPair(_Yplus_,1));
                     portList->addAsLast(TUPair(_Yminus_,1));
                 }
                 else
                 {
                     portList->addAsLast(TUPair(_Yminus_,1));
                     portList->addAsLast(TUPair(_Yplus_,1));

                 }
           }
        }
        
        
      }
   
   //-------------------------------------------------------------------
   //-------------------------------------------------------------------   
   

   #ifndef NO_TRAZA
      if( portList->numberOfElements() != 0)
      {
         TPortList::Cursor cursor(*portList);
         PRZString text = getComponent().asString() + "REQ: ";
         forCursor(cursor)
         {
            text += _routingType2string(przROUTINGTYPE(cursor.element().left())) +
                    "(" + PRZString(cursor.element().right()) + "), ";
         }
         PRZWRITE2LOG(text);
      }
      else
      {
         PRZString text = getComponent().asString() + "REQ: Nothing";
         PRZWRITE2LOG(text);   
      }
   #endif
   
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm(Boolean salidaX, int delta);
//
//  d: Este metodo se utiliza para verificar la condicion de la burbuja. Se
//     le llama siempre que el canal de salida elegido sea del tipo DOR;
//:
//*************************************************************************

Boolean PRZRoutingAdaptativeBurbleFlowMRVirtual :: controlAlgoritm( Boolean salidaX,
                                                           int delta )
{   // El valor de "salidaX" es TRUE cuando se sale por un canal X, y FALSE en
   // caso de salir por un canal Y.
	
   unsigned fifoID;
   Boolean isRequest = m_FlitsRx.element(1)->isRequest();
            
   PRZFifoMemory* fifo = 0;
   przROUTINGTYPE myDirection = getComponent().getRoutingDirection();
   przROUTINGTYPE newDirection;

   if(salidaX) newDirection = (delta>0) ? _Xplus_ : _Xminus_;
   else newDirection = (delta>0) ? _Yplus_ : _Yminus_;
   
   if( myDirection == _LocalNode_ )
   {                  
      // El Routing es de un canal de inyeccion, por lo que hay que verificar
      // que existe la condicion de la burbuja en cualquier caso.
      
      switch( newDirection )
      {
         case _Xplus_  : fifoID = 1; break;
         case _Xminus_ : fifoID = 2; break;
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;         
      }
      
   }

   else if( getComponent().getChannel() == 1 )  // Canal adaptativo -> DOR
   {
      // En este caso, se esta pasando de un canal Adaptativo a un canal DOR, por
      // lo que sebemos verificar que se cumple la condicion de la burbuja.
      
      switch( newDirection )
      {
         case _Xplus_  : fifoID = 1; break;
         case _Xminus_ : fifoID = 2; break;
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;         
      }
   }
   
   else   // Canal DOR -> DOR.
   {
      if( myDirection == newDirection ) return true;
      
      // Cuando estamos pasando de un canal DOR a otro DOR, solo debe verificarse la
      // condicion de la burbuja en el caso de que realicemos un cambio de dimension,
      // esto es, "myDirection" difiere de "newDirection".
      
      switch( newDirection )
      {
         case _Xplus_  : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Xminus_ : EXIT_PROGRAM(getComponent().asString()+ " Error de direccionamiento en el Routing DOR adaptativo");
         case _Yplus_  : fifoID = 3; break;
         case _Yminus_ : fifoID = 4; break;
      }      
   }
   
   if(isRequest)
   {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpReq; break;
         case 2: fifo = m_BufferXmReq; break;
         case 3: fifo = m_BufferYpReq; break;
         case 4: fifo = m_BufferYmReq; break;
       }
    }
    
    else
    {
      switch (fifoID)
      {
         case 1: fifo = m_BufferXpRep; break;
         case 2: fifo = m_BufferXmRep; break;
         case 3: fifo = m_BufferYpRep; break;
         case 4: fifo = m_BufferYmRep; break;
      }
   }
   return fifo->bubbleReady();
}

//*************************************************************************


// fin del fichero
