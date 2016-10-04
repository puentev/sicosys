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
// File: ../ATCSimul/src/PRZMultiportFifoFlow.cpp
//
// Clase      : PRZMultiportFifoFlow
//
// Hereda de  : PRZFlow
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZMultiportFifoFlow.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZMultiportFifoFlow.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZMultiportFifoFlow.hpp>

#ifndef __PRZMultiportFifo_HPP__
#include <PRZMultiportFifo.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZFicheroLog_HPP__
#include <PRZFicheroLog.hpp>
#endif

#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZMultiportFifoFlow,PRZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize() const;
//  d:
//
//
//  f: inline unsigned bufferHoles() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZMultiportFifoFlow(PRZComponent& component);
//
//  d:
//:
//*************************************************************************

PRZMultiportFifoFlow :: PRZMultiportFifoFlow(PRZComponent& component)
                   : PRZFlow(component),
                     m_Size(0),
                     m_inputs(1),
                     m_SendStop(false),
                     m_memory(0),
                     m_outLast(0),
                     m_payload(0),
                     m_lpack(0)
                     
{
   
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifoFlow :: initialize()
{

   m_Size = ((PRZMultiportFifo&)getComponent()).getBufferSize(); 
   m_inputs= ((PRZMultiportFifo&)getComponent()).numberOfInputs();
   m_memory=new QueueFlits[m_inputs];   
   Inhereited::initialize();
}

PRZMultiportFifoFlow :: ~PRZMultiportFifoFlow()
{
   delete[] m_memory;
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading();
//
//  d: Lectura desde varios puertos de entrada
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: inputReading()
{ 
for(int i=1;i<m_inputs+1;i++)
{ 
   if( inputInterfaz(i)->isReadyActive() )
   {
      if( ! bufferHoles() )
      {
         PRZString err;
         err.sprintf(ERR_PRZFFLOW_001,(char*)getComponent().asString());
         err+="\n Superada la capacidad de la memoria";
         EXIT_PROGRAM(err);
      }

      PRZMessage* msg;
      inputInterfaz(i)->getData(&msg);    
      m_memory[i-1].enqueue(msg);
      
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Rx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles());
         PRZWRITE2LOG( texto );
         
         PRZWRITE2LOG(getComponent().asString() + " Enqueue:" + PRZString(i)+ " -> " + PRZString(m_memory[i-1].numberOfElements()));
      #endif
   }
}

   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: stateChange()
{
   // El metodo virtual "controlAlgoritm()" se sobrecarga en las clases
   // hijas. de esta forma, se da un comportamiento WH o CT segun la
   // clase a la cual pertenezca el objeto que hemos instanciado.

   if( controlAlgoritm(false,m_inputs)==false ) m_SendStop=true;
   else m_SendStop=false;

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: outputWriting()
{
   PRZMessage* flitnow;
   outputInterfaz()->clearData();
            
   if( ! outputInterfaz()->isStopActive() )
   {            
       //Sigw sacando datos del ultimo puerto de entrada
     if(m_payload==1)
     {
        if(m_memory[m_outLast].numberOfElements()!=0 )
        {
           m_memory[m_outLast].dequeue(flitnow);
PRZWRITE2LOG(getComponent().asString() + " Dequeue:" + PRZString(m_outLast+1)+ " -> " + PRZString(m_memory[m_outLast].numberOfElements()));
           if(flitnow->isTail())
           {
              //Ultimo flit del paquete. El siguiente a buscarlo en otro sitio
              m_payload=0;
           }
           sendFlit(flitnow);
        }
        //else
        //{
        //  cerr<<"Multiport Bloqueada (sin comida)"<<endl;
        //}
      }
     else
     {//Busca un nuevo candidato a salir.

         //Hay priblemas con el avance simultaneo de todos los flits.
         //En el criterior de selección incorporar la posibilidad de 
         //que si un paquete esta completo en el buffer de salida
         //se seleccione antes que los incompletos.
         //Para que esto funcione siempre el numero de paquetes que puede almacenar el buffer 
         //ha de ser igual o superior al numero de puertos de entrada.
        
        Boolean flag=true;
       //Busco paquetes completos
            m_lpack= ((PRZSimulation*)getComponent().getSimulation())->getPacketLength((PRZNetwork*)getOwnerRouter().getOwner()); 
        
      for(int i=0; (i<m_inputs)&&1; i++)
      {
         
         unsigned index=i;//(i+m_outLast+1)%(m_inputs);
         //cerr<<index<<"  ";
         if(m_memory[index].numberOfElements()!=0)
         {
          //  PRZWRITE2LOG(getComponent().asString() + "Elements =" + PRZString(m_memory[index].numberOfElements()));
            m_outLast=index;
            m_payload=1;
            m_memory[index].dequeue(flitnow);

         #ifndef NO_TRAZA
            PRZWRITE2LOG(getComponent().asString() + " Dequeue:" + PRZString(m_outLast+1)+ " -> " + PRZString(m_memory[m_outLast].numberOfElements()));            
         #endif
            sendFlit(flitnow);
            break;
         }         
       }
     } 
   }     
      
      
   if(!controlAlgoritm(false,m_inputs+1))
   {
      for(int index=1;index<m_inputs+1;index++)
         {
            inputInterfaz(index)->sendStop();
         }
      #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         PRZString texto = getComponent().asString() + ": Buffer lleno->Stop ";
         texto += PRZString(time);
         PRZWRITE2LOG(texto);
      #endif
   }
   else //if ( ! outputInterfaz()->isStopActive())
   { 
      //Este planteamiento evita el que cuando se llene y rearanque la fifo no se quede colgado el 
      //mensaje que estaba saliendo... que sin la restriccion de l tamaño>=n_inputs*LPACK podia 
      //producir un cuelge. 
      unsigned spacio=m_Size-bufferElements();
      if(m_payload==1  && spacio>m_inputs+1 && spacio<2*m_inputs+1 )
      {

         inputInterfaz(m_outLast+1)->clearStop();
            
//          #ifndef NO_TRAZA
//           unsigned x=m_outLast+1;
//                    
//           uTIME time = getOwnerRouter().getCurrentTime();
//           PRZString texto = getComponent().asString() + ": Clear Stop Independiente i= "+PRZString(x);
//           texto += PRZString(time);
//           PRZWRITE2LOG(texto);
//          #endif
          
         for(int index=1;index<m_inputs+1;index++)
         {  if(index!=m_outLast+1)
            inputInterfaz(index)->sendStop();
         }
         
      }
      else
      {
         for(int index=1;index<m_inputs+1;index++)
         {
            inputInterfaz(index)->clearStop(); 
         }
      }  
   }
   return true;   
}



//*************************************************************************
//:
//  f: virtual Boolean propagateStop();
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: propagateStop()
{
   //if( controlAlgoritm() )
   //{
      // En este caso, el buffer dispone de espacio para mas flits,
      // sea la que sea la politica de control de flujo, ya que se ha 
      // sobrecargado el metodo "controlAlgoritm()" convenientemente.
   //   setStopped(false);
   //   return false;
   //}
   //setStopped(true);   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   
   
   
   return true;
//   return Inhereited::onStopUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
//
//  d:
//:
//*************************************************************************

Boolean PRZMultiportFifoFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
  // if( controlAlgoritm() ) 
  // {
  //    Hay plazas en el buffer, por lo que se puede propagar la señal de
  //    stop, sin ningun peligro de desbordar el buffer.
  //    return Inhereited::onStopDown(interfaz,cv);
  //}
   
   // En este caso, no existe espacio en el buffer, por lo que esta parado.
   // Cuando pueda alamcenar algun flit, ya se encargara el mismo de bajar
   // la señal de Stop.
   
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit(const PRZMessage & msg);
//
//  d:
//:
//*************************************************************************

void PRZMultiportFifoFlow :: sendFlit(PRZMessage* msg)
{
   outputInterfaz()->sendData(msg);
    #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         PRZString texto = getComponent().asString() + " Flit Tx (" +
                           PRZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + PRZString(bufferHoles());
         PRZWRITE2LOG( texto );
      #endif
}

//*************************************************************************
//:
//  f:       virtual Boolean bubbleReady() const;   
//  d:       semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean PRZMultiportFifoFlow ::bubbleReady()const
 {     
    return true;
 }   
//*************************************************************************
unsigned PRZMultiportFifoFlow::bufferElements() const
{ 
         unsigned kaka=0;
         for(int index=0; index<m_inputs;index++)
         {
            kaka=m_memory[index].numberOfElements()+kaka;
         }
         return kaka; 
      }
// fin del fichero
