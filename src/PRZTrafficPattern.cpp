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
// File: ../ATCSimul/src/PRZTrafficPattern.cpp
//
// Clase      : PRZTrafficPattern
//
// Hereda de  : None 
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZTrafficPattern.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZTrafficPattern.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZTrafficPattern.hpp>

#ifndef __PRZTag_HPP__
#include <PRZTag.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

#include <TUTTrafficPatternJump.hpp>

#ifndef NO_SCT
//#ifndef __PRZTrafficPatternTrace_HPP__
//#include <PRZTrafficPatternTrace.hpp>
//#endif

//#ifndef __PRZTrafficPatternTraceDualNet_HPP__
//#include <PRZTrafficPatternTraceDualNet.hpp>
//#endif

//#ifndef __PRZTrafficPatternTraceStatic_HPP__
//#include <PRZTrafficPatternTraceStatic.hpp>
//#endif
#endif

#include <math.h>
#include <fstream.h>

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternEmpty,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternRandom,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternRandomCP,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternRandomCP3D,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPermutation,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPermutationVirtual,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPerfectShuffle,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPerfectShuffleVirtual,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternBitReversal,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternBitReversalVirtual,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternLocal,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternHotSpot,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternFlash,PRZTrafficPattern);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternFile,PRZTrafficPattern);

IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternRandom3D,PRZTrafficPatternRandom);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPermutation3D,PRZTrafficPatternPermutation);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternPerfectShuffle3D,PRZTrafficPatternPerfectShuffle);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternBitReversal3D,PRZTrafficPatternBitReversal);
IMPLEMENT_RTTI_DERIVED(PRZTrafficPatternLocal3D,PRZTrafficPatternLocal);

//*************************************************************************
//:
//  f: virtual PRZString asString() const = 0;
//  d:
//
//  f: inline void setParameter1(double param);
//  d:
//
//  f: inline void setParameter2(double param);
//  d:
//
//  f: inline void setParameter3(double param);
//  d:
//
//  f: inline double getParameter1() const;
//  d:
//
//  f: inline double getParameter2() const;
//  d:
//
//  f: inline double getParameter3() const;
//  d:
//
//  f: inline PRZSimulation& getSimulation() const;
//  d:
//
//  f: inline void setProbBimodal(double prob);
//  d:
//
//  f: inline void setMessagesBimodal(unsigned nMsg);
//  d:
//
//  f: inline void setBimodal();
//  d:
//
//  f: inline Boolean isBimodal() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZTrafficPattern(PRZSimulation& simulation);
//
//  d:
//:
//*************************************************************************

PRZTrafficPattern :: PRZTrafficPattern(PRZSimulation& simul)
                   : m_Simulation(simul),
                     m_Parameter1(0.0),
                     m_Parameter2(0.0),
                     m_Parameter3(0.0),
                     m_ProbBimodal(0.0),
                     m_NumMsgBimodal(0),
                     m_Bimodal(false),
                     m_IsFile(false)
{
   m_SizeX = getSimulation().getNetwork()->getSizeX();
   m_SizeY = getSimulation().getNetwork()->getSizeY();
   m_SizeZ = getSimulation().getNetwork()->getSizeZ();
   m_TotalNodes = m_SizeX*m_SizeY*m_SizeZ;
}


//*************************************************************************
//:
//  f: void generateMessages(double q);
//
//  d:
//:
//*************************************************************************

void PRZTrafficPattern :: generateMessages(double q)
{
   for( register int i=0; i<m_SizeX; i++ )
      for( register int j=0; j<m_SizeY; j++ )
         for( register int k=0; k<m_SizeZ; k++ )
         {
            PRZPosition pos(i,j,k);
            if( drand48() < (getInjectFactor(pos)*q) )
            {
               injectMessage(pos);
            }
         }
}


//*************************************************************************
//:
//  f: PRZMessage generateBasicMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

PRZMessage* PRZTrafficPattern :: generateBasicMessage(const PRZPosition& source)
{
   PRZMessage* message=new PRZMessage;
   
   message->setPacketSize( getSimulation().getPacketLength() );
   
   if( isBimodal() && (drand48()<=m_ProbBimodal) )
   {
      unsigned mess_size= (unsigned)(m_NumMsgBimodal * getSimulation().getMessageLength());
      
      if(m_NumMsgBimodal>1)
      {
          message->setMessageSize( mess_size );

          
      }
      else
      {   //Todos los mensajes cortos los etiqueto como request
          //esto se empleara enlas redes virtuales de request y 
          //Reply
          //message.setRequest();
          message->setRequest(); 
          message->setMessageSize( getSimulation().getMessageLength() );
          message->setPacketSize( m_NumMsgBimodal*getSimulation().getPacketLength() );
      }
   }
   else
   {
      message->setMessageSize( getSimulation().getMessageLength() );
   }
 
   message->setSource(source);
   //if(drand48()>0.5)
   //{message.setRequest(); message.setPacketSize(2);}
   message->setGenerationTime(getSimulation().getCurrentTime());
   return message;
}


//*************************************************************************
//:
//  f: static PRZTrafficPattern* createTrafficPattern( const PRZTag& tag,
//                                                     PRZsimulation& simul);
//
//  d:
//:
//*************************************************************************

PRZTrafficPattern* PRZTrafficPattern :: createTrafficPattern(const PRZTag& tag,
                                                             PRZSimulation& simul )
{
   PRZString type;
      
   if( ! tag.getAttributeValueWithName(PRZ_TAG_TYPE, type) )
   {
      PRZString err;
      err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_TYPE );
      EXIT_PROGRAM(err);
   }

   PRZTrafficPattern* pattern = 0;
   //problematico cuando se trata de un solo ruter
   Boolean red3D = (simul.getNetwork()->getSizeZ()>1) ? true : false;
   
   if( type == PRZ_TAG_EMPTY )
   {
      pattern = new PRZTrafficPatternEmpty(simul);
   }
   else if( type == PRZ_TAG_RANDOM_CP )
   {
      pattern =  (red3D) ? new PRZTrafficPatternRandomCP3D(simul):
                           new PRZTrafficPatternRandomCP(simul);
   }
 
     else if( type == PRZ_TAG_RANDOM )
   {
      pattern = (red3D) ? new PRZTrafficPatternRandom3D(simul) :
                          new PRZTrafficPatternRandom(simul);
   }
   else if( type == PRZ_TAG_PERMUTATION )
   {
      pattern = (red3D) ? new PRZTrafficPatternPermutation3D(simul) :
                          new PRZTrafficPatternPermutation(simul);
   }
   else if( type == PRZ_TAG_PERMUTATION_VIRTUAL )
   {
      if(red3D)
      {
         PRZString err="This Traffic Pattern Only for Midimew Network\n";
         EXIT_PROGRAM(err);
      }
      else
      {
          pattern =   new PRZTrafficPatternPermutationVirtual(simul);
      }
   }
   else if( type == PRZ_TAG_PSUFFLE )
   {
      pattern = (red3D) ? new PRZTrafficPatternPerfectShuffle3D(simul) :
                          new PRZTrafficPatternPerfectShuffle(simul);
   }
   else if( type == PRZ_TAG_PSUFFLE_VIRTUAL )
   {
      if(red3D)
      {
                  PRZString err="This Traffic Pattern Only for Midimew Network\n";
         EXIT_PROGRAM(err);
      }
      else
      {
      pattern = new PRZTrafficPatternPerfectShuffleVirtual(simul);
      }
   }
   else if( type == PRZ_TAG_BREVERSAL )
   {
      pattern = (red3D) ? new PRZTrafficPatternBitReversal3D(simul) :
                          new PRZTrafficPatternBitReversal(simul);
   }
   else if( type == PRZ_TAG_BREVERSAL_VIRTUAL )
   {
      if(red3D)
      {
         PRZString err="This Traffic Pattern Only for Midimew Network\n";
         EXIT_PROGRAM(err);
      }
      else
      {
         pattern =new PRZTrafficPatternBitReversalVirtual(simul);
      }
   }
   else if( type == PRZ_TAG_LOCALTRAFFIC )
   {
      pattern = (red3D) ? new PRZTrafficPatternLocal3D(simul) :
                          new PRZTrafficPatternLocal(simul);
      PRZString sigma;
      if( tag.getAttributeValueWithName(PRZ_TAG_SIGMA, sigma) )
      {
         pattern -> setParameter1(sigma.asDouble());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM1, sigma) )
      {
         pattern -> setParameter1(sigma.asDouble());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_SIGMA );
         EXIT_PROGRAM(err);
      }
   }

   /** begin{TUTORIAL} *****************************************/
   else if( type == TUT_TAG_JUMP )
   {
      /* Create the traffic pattern object */
      TUTTrafficPatternJump* newPattern = new TUTTrafficPatternJump(simul);
      /* Read tag parameters */
      PRZString param;
      int x = 0, y = 0, z = 0;
      
      if( tag.getAttributeValueWithName(PRZ_TAG_PARAM1, param) )
         x = param.asInteger();
      if( tag.getAttributeValueWithName(PRZ_TAG_PARAM2, param) )
         y = param.asInteger();
      if( red3D && tag.getAttributeValueWithName(PRZ_TAG_PARAM3, param) )
         z = param.asInteger();
      
      /* Show error if parameters aren't valid */
      if( !x && !y && !z )
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_PARAM1 );
         EXIT_PROGRAM(err);
      }
      /* Set paraemter values in traffic pattern class */
      newPattern -> setJump(x,y,z);
      /* Set traffic pattern object */
      pattern=newPattern;
   } 
   /** end{TUTORIAL} *****************************************/

   else if( type == PRZ_TAG_HOTSPOT )
   {
      pattern = new PRZTrafficPatternHotSpot(simul);
      PRZString nodosHot, probHot, distHot;
      
      if( tag.getAttributeValueWithName(PRZ_TAG_NODOSHOT, nodosHot) )
      {
         pattern -> setParameter1(nodosHot.asInteger());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM1, nodosHot) )
      {
         pattern -> setParameter1(nodosHot.asInteger());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_NODOSHOT );
         EXIT_PROGRAM(err);
      }

      if( tag.getAttributeValueWithName(PRZ_TAG_PROBHOT, probHot) )
      {
         pattern -> setParameter2(probHot.asInteger());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM2, probHot) )
      {
         pattern -> setParameter2(probHot.asInteger());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_PROBHOT );
         EXIT_PROGRAM(err);
      }

      if( tag.getAttributeValueWithName(PRZ_TAG_DISTHOT, distHot) )
      {
         pattern -> setParameter3(distHot.asInteger());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM3, distHot) )
      {
         pattern -> setParameter3(distHot.asInteger());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_DISTHOT );
         EXIT_PROGRAM(err);
      }

   } 
   
   else if( type == PRZ_TAG_FLASH )
   {
      // El trafico de rafagas requiere de los siguientes parametros:
      //  parameter1 = p10, prob. de pasar del estado "1" al "0"
      //  parameter2 = alpha, cuantas veces mayor es p10 que p01.
      //  parameter3 = p01, se alcula a partir de p10=alpha*p01.

      pattern = new PRZTrafficPatternFlash(simul);
      PRZString p10, alpha;
      
      if( tag.getAttributeValueWithName(PRZ_TAG_P10_FLASH, p10) )
      {
         pattern -> setParameter1(p10.asDouble());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM1, p10) )
      {
         pattern -> setParameter1(p10.asDouble());
      } 
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_P10_FLASH );
         EXIT_PROGRAM(err);
      }
   

      if( tag.getAttributeValueWithName(PRZ_TAG_ALPHA_FLASH, alpha) )
      {
         pattern -> setParameter2(alpha.asDouble());
      }
      else if( tag.getAttributeValueWithName(PRZ_TAG_PARAM2, alpha) )
      {
         pattern -> setParameter2(alpha.asDouble());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_ALPHA_FLASH );
         EXIT_PROGRAM(err);
      }

      pattern->setParameter3(p10.asDouble()/alpha.asDouble());   // p01
   } 
   //Simulacion conducida por trazas
   else if( type == PRZ_TAG_TRACE )
   {
   #ifdef NO_SCT
         PRZString err;
         err=PRZString("This executable doesn't support Trace Driven Execution\n"); 
	 err+=PRZString("Please compile with USE_SCT (needs libz and librwtool)\n");
	 EXIT_PROGRAM(err);
   #else
      PRZString traceFile;
      PRZString flitSize; //in bytes
      PRZString compress;
      Boolean trazaCompresed=false;
      //only need the flit size for trace driven pattern Traffic
      
      
      if( tag.getAttributeValueWithName(PRZ_TAG_FLIT_SIZE, flitSize) )
      {
               simul.setFlitSize(flitSize.asInteger());
      }
      else
      {
               //4 bytes as default flit size
               //
              simul.setFlitSize(4);
      }
      
      //Trace file is compressed with gzip
      if(tag.getAttributeValueWithName(PRZ_TAG_COMPRESSED, compress))
      {
         trazaCompresed=true;
      }
      
      PRZString dual_net;
      Boolean doble_red;
      if( tag.getAttributeValueWithName(PRZ_TAG_DUAL_NET, dual_net) )
      {
          doble_red=true;
      }
      else
      {
         doble_red=false;
      }
      
      PRZString estatico;
      Boolean estructura_datos;
      if( tag.getAttributeValueWithName(PRZ_TAG_TRACE_STATIC, estatico) )
      {
               estructura_datos=true;
      }
      else
      {
         estructura_datos=false;
      }      

      //Set the relative speed beetwen network and processor. (The network is clock times slow than
      //processor
      PRZString clock;
      if( tag.getAttributeValueWithName(PRZ_TAG_TRACE_CLOCK, clock) )
      {
               simul.setClock((float)clock.asDouble());
      }
      else
      {
               //Network and processor have the same speed as default
              simul.setClock(1.0);
      }
      PRZString s_blistedFactor;
      unsigned blistedFactor;
      if( tag.getAttributeValueWithName(PRZ_TAG_BLISTED_FACTOR, s_blistedFactor) )
      {
               blistedFactor=clock.asInteger();
      }
      else
      {
               //Only a processor per router as default
              blistedFactor=1;
      }

      
      if( tag.getAttributeValueWithName(PRZ_TAG_TRACE_FILE, traceFile) )
      {
         		if(doble_red)
               		pattern = new PRZTrafficPatternTraceDualNet(traceFile,simul,trazaCompresed,estructura_datos,blistedFactor);
               else
               		pattern = new PRZTrafficPatternTrace(traceFile,simul,trazaCompresed,estructura_datos,blistedFactor);
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_P10_FLASH );
         EXIT_PROGRAM(err);
      }
      #endif
    } 
    
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZTRAFI_002, (char*)(type) );
      EXIT_PROGRAM(err);   
   }

   if( tag.tagId() == PRZ_TAG_BIMODAL )
   {
      pattern->setBimodal();
      PRZString prob, numMsg;
      if( tag.getAttributeValueWithName(PRZ_TAG_PROBBIMODAL, prob) )
      {
         pattern -> setProbBimodal(prob.asDouble());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_PROBBIMODAL );
         EXIT_PROGRAM(err);   
      }
      if( tag.getAttributeValueWithName(PRZ_TAG_MSGBIMODAL, numMsg) )
      {
         pattern -> setMessagesBimodal(numMsg.asDouble());
      }
      else
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_001, (char*)PRZ_TAG_MSGBIMODAL );
         EXIT_PROGRAM(err);   
      }
   }
   else if( tag.tagId() == PRZ_TAG_MODAL )
   {
   
   }
   else
   {
      PRZString err;
      err.sprintf( ERR_PRZTRAFI_003, (char*)(tag.tagId()) );
      EXIT_PROGRAM(err);   
   }


   return pattern;
}


//*************************************************************************
//:
//  f: static PRZTrafficPattern* createTrafficPattern( const PRZString& fileName,
//                                                     PRZsimulation& simul);
//
//  d:
//:
//*************************************************************************

PRZTrafficPattern* PRZTrafficPattern :: createTrafficPattern(const PRZString& fileName,
                                                             PRZSimulation& simul )
{
   PRZTrafficPattern* pattern = new PRZTrafficPatternFile(fileName,simul);
   return pattern;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternEmpty::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternEmpty :: injectMessage(const PRZPosition& source)
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternRandom::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternRandom :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y;
   PRZPosition destiny;

   do
   {
      x = rand() % getX();
      y = rand() % getY();
      destiny = PRZPosition(x,y,0);
   }
   while( source == destiny );
   msg->setDestiny(destiny);
   getSimulation().getNetwork()->sendMessage(msg);
   /*if(PRZPosition(4,0,0) == source)
   {
      msg->setDestiny(PRZPosition(1,0,0));
      getSimulation().getNetwork()->sendMessage(msg);
   }
   if(PRZPosition(7,0,0) == source)
   {
      msg->setDestiny(PRZPosition(1,0,0));
      getSimulation().getNetwork()->sendMessage(msg);
   }*/
   return true;
      
}


//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternRandomCP::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternRandomCP :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y ,z;
   PRZPosition destiny;

      x = (rand() % 100)-50;
      y = (rand() % 100)-50;
      z = (rand() % 100)-50;
      destiny = PRZPosition(x,y,z);
   
   msg->setDestiny(destiny);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}
//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternRandomCP3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternRandomCP3D :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y ,z;
   PRZPosition destiny;

      x = (rand() % 100)-50;
      y = (rand() % 100)-50;
      z = (rand() % 100)-50;
      destiny = PRZPosition(x,y,z);
   
   msg->setDestiny(destiny);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternRandom3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternRandom3D :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y, z;

   PRZPosition destiny;

   do
   {
      x = rand() % getX();
      y = rand() % getY();
      z = rand() % getZ();
      destiny = PRZPosition(x,y,z);
   }
   while( source == destiny );

   msg->setDestiny(destiny);
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPermutation::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternPermutation :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);

   if( x == y ) return false;
   msg->setDestiny(PRZPosition(y,x,0));
   
  // getSimulation().getNetwork()->sendMessage(msg);   
   //return true;
   
   //Existe una codificacion alternativa para este trafico: Ver duato pag: 406
   //CAda nodo an-1,an-2,...,a0 se comunica con an/2-1,an/2-2,.....,an/2
   //Esta codificacion me va a permitir el poder manejar este trafico en redes no 
   //dimensionales directamente. Solo lo hago en 2-D que es donde voy a utlizar la 
   //la midimew.
   //IMPORTANTE: redes con un numero impar de nodos estas notacion no es valida
   //
   //
   unsigned byteSize = ceil( log(numberOfNodes()) / log(2.0) );
                              
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = 0;

 
   unsigned kk=nodoOrigen<<((int)floor(byteSize/2));
     // cerr<<"LL:"<<kk<<endl;

   unsigned temp=nodoOrigen>>((int)ceil(byteSize/2))  ;
         nodoDestino=kk|temp;

//   if( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes()) ) return false;
   if( (nodoOrigen==nodoDestino)  ) return false;

   unsigned destX = nodoDestino % getX();
   unsigned destY = (nodoDestino / getY())%getY();
   
   //cerr<< x <<"  "<<y<<":"<<nodoOrigen<<":"<< nodoDestino<<"-->"<<destX<<" "<<destY<<endl;
   
   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;  
} 

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPermutationVirtual::injectMessage(const PRZPosition& source);
//
//  d: a emplear con la midimew. Incorpora un sistema de etiquetacion
//     de nodos virtual. Hay que obtener a partir del indice del nodo
//     la posicion real en la red. Desde el punto de vista del trafico
//     la numeracion de los nodos de la red es tal que todos estan distan 1
//:
//*************************************************************************

Boolean PRZTrafficPatternPermutationVirtual :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);

   if( x == y ) return false;
   msg->setDestiny(PRZPosition(y,x,0));
   
   //Mapeo virtual para las x
   x=MidimewIndex(x); 
   
   unsigned byteSize = ceil( log(numberOfNodes()) / log(2.0) );
                              
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = 0;

 
   unsigned kk=nodoOrigen<<((int)floor(byteSize/2));

   unsigned temp=nodoOrigen>>((int)ceil(byteSize/2))  ;
         nodoDestino=kk|temp;
	
   //Inversion del destino
   
   
   if( (nodoOrigen==nodoDestino)  ) return false;

   unsigned destX = nodoDestino % getX();
   unsigned destY = (nodoDestino / getY())%getY();
   
   destX=InvMidimewIndex(destX);
	
   if(destX ==x && destY==y)
   {
      return false;
   }   
   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);
   return true;  
} 

//*************************************************************************
//:
//  f: unsigned PRZTrafficPattern::MidimewIndex(unsigned posicion)const;
//  d: retorna el indice virtual del origen
//:
//*************************************************************************
unsigned PRZTrafficPattern :: InvMidimewIndex (unsigned posicion)const
{
  unsigned  b=(unsigned)ceil(sqrt(numberOfNodes()/2));
  int  resto=b;
  unsigned  cuenta=0;
  unsigned  numero;

     while(resto>0)
     {
        numero=(b-1)*(b-resto);
        while(numero<numberOfNodes())
        {
             if(posicion==cuenta){
                return numero;
              }
             cuenta++;
             numero=numero+b;
        }
        numero=resto;
        while(numero<(b-1)*(b-resto))
        { 
             if(posicion==cuenta){
                return numero;
              }
             cuenta++;
             numero=numero+b;
        }
        resto=resto-1;
     } 
     return 0; 
}
//*************************************************************************
//:
//  f: unsigned PRZTrafficPattern::InvMidimewIndex(unsigned posicion)const;
//  d: retorna el indice virtual del destino (inversa)
//:
//*************************************************************************
unsigned PRZTrafficPattern::MidimewIndex(unsigned posicion)const
{
  unsigned  b=(int)ceil(sqrt(numberOfNodes()/2));
  int  resto=b;
  unsigned cuenta=0;
  unsigned numero;
      while(resto>0)
      {
         numero=(b-1)*(b-resto);
         while(numero<numberOfNodes())
         {
           if(posicion==numero)
           {
              return cuenta;
           }
              cuenta++;
              numero=numero+b;
         }
         numero=resto;
         while(numero<(b-1)*(b-resto))
         {

           if(posicion==numero)
           {
              return cuenta;
           }
              cuenta++;
              numero=numero+b;
         }
         resto=resto-1;
      }
      return 0;
}
//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPermutation3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternPermutation3D :: injectMessage(const PRZPosition& source)
{

   PRZMessage* msg = generateBasicMessage(source);
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);
   unsigned z = source.valueForCoordinate(PRZPosition::Z);
   
   if( (x==y) && (y==z) ) return false;
   msg->setDestiny(PRZPosition(z,x,y));
   getSimulation().getNetwork()->sendMessage(msg);   

   return true;
}

//*************************************************************************
//*************************************************************************
//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPerfectShuffle::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternPerfectShuffle :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);      
    
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = nodoOrigen << 1;

   if( nodoDestino >= numberOfNodes() )
   {
      nodoDestino = nodoDestino - numberOfNodes() + 1;
   }
   
   if( nodoDestino == nodoOrigen ) return false;
   
   //Generando de esta forma los destinos 
   //no hay problema a la hora de evaluar 
   //redes no dimensionables en x-y
   //como las midimew. El indice del destino se retorna 
   //en X y el algoritmo de routing se encarga de 
   //manejar adecuadamente el dato en la red
   unsigned destX = nodoDestino % getX();
   unsigned destY = nodoDestino / getY();
   
   if (destY>getY()-1) destY=nodoDestino/getX(); //MIDIMEW PETE
      
   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);
   
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPerfectShuffleVirtual::injectMessage(const PRZPosition& source);
//
//  d: ver descripcion de  PRZTrafficPatternPermutationVirtual
//:
//*************************************************************************

Boolean PRZTrafficPatternPerfectShuffleVirtual :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   x=MidimewIndex(x);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);      
    
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = nodoOrigen << 1;

   if( nodoDestino >= numberOfNodes() )
   {
      nodoDestino = nodoDestino - numberOfNodes() + 1;
   }
   
   if( nodoDestino == nodoOrigen ) return false;
   
   //Generando de esta forma los destinos 
   //no hay problema a la hora de evaluar 
   //redes no dimensionables en x-y
   //como las midimew. El indice del destino se retorna 
   //en X y el algoritmo de routing se encarga de 
   //manejar adecuadamente el dato en la red
   unsigned destX = nodoDestino % getX();
   unsigned destY = nodoDestino / getY();
   
   destX=InvMidimewIndex(destX);
   
   if (destY>getY()-1) destY=nodoDestino/getX(); //MIDIMEW PETE
      
   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternPerfectShuffle3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternPerfectShuffle3D :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);
   unsigned z = source.valueForCoordinate(PRZPosition::Z);
   
   unsigned sizePlaneXY = getX() * getY();
   unsigned nodoOrigen = x + y*getX() + z*sizePlaneXY;
   unsigned nodoDestino = nodoOrigen << 1;

   if( nodoDestino >= numberOfNodes() )
   {
      nodoDestino = nodoDestino - numberOfNodes() + 1;
   }
   
   if( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes()) ) return false;

   unsigned destZ   = unsigned(nodoDestino/sizePlaneXY); 
   unsigned planeXY = nodoDestino - destZ*sizePlaneXY;
   unsigned destX   = planeXY % getX();
   unsigned destY   = planeXY / getY();
   msg->setDestiny(PRZPosition(destX,destY,destZ));
   getSimulation().getNetwork()->sendMessage(msg);
   
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternBitReversal::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternBitReversal :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);      

   unsigned byteSize = ceil( log(numberOfNodes()) / log(2.0) );
   static unsigned mask[] = { 0x0001, 0x0002, 0x0004, 0x0008,
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000 };
                              
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = 0;
   //Midimew Protect:
   if(getY()==1) nodoOrigen=x;
   
   for( int i=0; i<byteSize; i++ )
   {
      if( nodoOrigen & mask[i] )
      {
         nodoDestino |= mask[byteSize-i-1];
      }
   }

   
   unsigned destX = nodoDestino % getX();
   unsigned destY = nodoDestino / getY();
   
   if (destY>getY()-1) destY=nodoDestino/getX();  //MIDMEW PETE
      //cerr<<x<<"'"<<y<<" "<<nodoOrigen<<"  "<<nodoDestino<<" "<<byteSize<<"-->"<<destX<<","<<destY<<endl;

   if( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes()) ) return false;

   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}
//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternBitReversalVirtual::injectMessage(const PRZPosition& source);
//
//  d: Mapeo virtual Midimew
//:
//*************************************************************************

Boolean PRZTrafficPatternBitReversalVirtual :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   x=MidimewIndex(x);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);      

   unsigned byteSize = ceil( log(numberOfNodes()) / log(2.0) );
   static unsigned mask[] = { 0x0001, 0x0002, 0x0004, 0x0008,
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000 };
                              
   unsigned nodoOrigen  = x + y*getX();
   unsigned nodoDestino = 0;
   //Midimew Protect:
   if(getY()==1) nodoOrigen=x;
   
   for( int i=0; i<byteSize; i++ )
   {
      if( nodoOrigen & mask[i] )
      {
         nodoDestino |= mask[byteSize-i-1];
      }
   }

   
   unsigned destX = nodoDestino % getX();
   unsigned destY = nodoDestino / getY();
   destX=InvMidimewIndex(destX);
   if (destY>getY()-1) destY=nodoDestino/getX();  //MIDMEW PETE
      //cerr<<x<<"'"<<y<<" "<<nodoOrigen<<"  "<<nodoDestino<<" "<<byteSize<<"-->"<<destX<<","<<destY<<endl;

   if( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes()) ) return false;

   msg->setDestiny(PRZPosition(destX,destY,0));
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternBitReversal3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternBitReversal3D :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   unsigned x = source.valueForCoordinate(PRZPosition::X);
   unsigned y = source.valueForCoordinate(PRZPosition::Y);      
   unsigned z = source.valueForCoordinate(PRZPosition::Z);
   unsigned sizePlaneXY = getX() * getY();

   unsigned byteSize = ceil( log(numberOfNodes()) / log(2.0) );
   static unsigned mask[] = { 0x0001, 0x0002, 0x0004, 0x0008,
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000 };
                              
   unsigned nodoOrigen  = x + y*getX() + z*sizePlaneXY;
   unsigned nodoDestino = 0;

   for( int i=0; i<byteSize; i++ )
   {
      if( nodoOrigen & mask[i] )
      {
         nodoDestino |= mask[byteSize-i-1];
      }
   }

   if( (nodoOrigen==nodoDestino) || (nodoDestino>numberOfNodes()) ) return false;

   unsigned destZ   = unsigned(nodoDestino/sizePlaneXY); 
   unsigned planeXY = nodoDestino - destZ*sizePlaneXY;
   unsigned destX   = planeXY % getX();
   unsigned destY   = planeXY / getY();

   msg->setDestiny(PRZPosition(destX,destY,destZ));
   getSimulation().getNetwork()->sendMessage(msg);
   
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternLocal::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternLocal :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   double n1, n2, modulo2; 
   unsigned distancia;
   unsigned radio = getSimulation().getRadius();
   
   do
   {
      do
      {
         n1 = drand48();
         n2 = drand48();
         modulo2 = n1*n1 + n2*n2;
      }
      while( modulo2 > 1.0 );
  
      distancia = 1.0 + abs(getParameter1()*n1*sqrt(-2.0*log(modulo2)/modulo2));
   }
   while( distancia > unsigned(radio/2) );
   
   int signoX = ( drand48() < 0.5 ) ? 1 : -1;
   int signoY = ( drand48() < 0.5 ) ? 1 : -1;
   
   unsigned distX = rand()%(1+distancia);
   unsigned distY = distancia - distX;

   unsigned orgX = source.valueForCoordinate(PRZPosition::X);
   unsigned orgY = source.valueForCoordinate(PRZPosition::Y);      
   
   unsigned destX = ( distX*signoX + orgX + radio ) % radio;
   unsigned destY = ( distY*signoY + orgY + radio ) % radio;
   
   msg->setDestiny(PRZPosition(destX,destY,0));
   
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternLocal3D::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternLocal3D :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
   
   double n1, n2, modulo2; 
   unsigned distancia;
   unsigned radio = getSimulation().getRadius();
   do
   {
      do
      {
         n1 = drand48();
         n2 = drand48();
         modulo2 = n1*n1 + n2*n2;
      }
      while( modulo2 > 1.0 );
  
      distancia = 1.0 + abs(getParameter1()*n1*sqrt(-2.0*log(modulo2)/modulo2));
   }
   while( distancia > unsigned(radio/2) );
   
   int signoX = ( drand48() < 0.5 ) ? 1 : -1;
   int signoY = ( drand48() < 0.5 ) ? 1 : -1;
   int signoZ = ( drand48() < 0.5 ) ? 1 : -1;
   
   unsigned distX = rand() % (1+distancia);
   unsigned distY = rand() % (1+distancia-distX);
   unsigned distZ = rand() % (1+distancia-distX-distY);
   
   unsigned orgX = source.valueForCoordinate(PRZPosition::X);
   unsigned orgY = source.valueForCoordinate(PRZPosition::Y);      
   unsigned orgZ = source.valueForCoordinate(PRZPosition::Z);
   
   unsigned destX = ( distX*signoX + orgX + radio ) % radio;
   unsigned destY = ( distY*signoY + orgY + radio ) % radio;
   unsigned destZ = ( distZ*signoZ + orgZ + radio ) % radio;
   
   msg->setDestiny(PRZPosition(destX,destY,destZ));
   
   getSimulation().getNetwork()->sendMessage(msg);
   return true;
}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual void PRZTrafficPatternHotSpot::initialize();
//
//  d:
//:
//*************************************************************************

void PRZTrafficPatternHotSpot :: initialize()
{
   if( ! m_HotSpotList )
   {
      // Se inicializa la lista de nodos que van a ser "hot spot" de forma
      // aleatoria. Esta lista contiene a todos los nodos, de forma que los
      // que son hot, tiernen el valor de true, y los que no lo son, se 
      // inicializan con el valor "false".
      
      unsigned totalNodes = getX()*getY();
      if( totalNodes < getParameter1() )
      {
         PRZString err;
         err.sprintf(ERR_PRZTRAFI_004, (int)getParameter1() );
         EXIT_PROGRAM(err);
      }
      if( getParameter3() > getSimulation().getRadius() )
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_006, 
                      (int)getParameter3(),
                      getSimulation().getRadius() );
         EXIT_PROGRAM(err);      
      }
      if( (getParameter3() != 0.0) && (getParameter1()>2) )
      {
         PRZString err;
         err.sprintf( ERR_PRZTRAFI_007 );
         EXIT_PROGRAM(err);            
      }
      
      m_HotSpotList = new THotSpotList(totalNodes);
      m_HotSpotList -> initialize(false);
      
      if( getParameter1() == 2.0 && getParameter3() )
      {
         unsigned index  = rand() % totalNodes;
         unsigned xo, xi = index % getX();
         unsigned yo, yi = index / getY();
         m_HotSpotList->setValueAt(index+1,true);
         do
         {
            index = rand() % totalNodes;
            xo    = index % getX();
            yo    = index / getY();
         }
         while( getSimulation().getNetwork()->
                distance(PRZPosition(xi,yi),PRZPosition(xo,yo)) != 
                int(getParameter3()) );
                
         m_HotSpotList->setValueAt(index+1,true);
      }
      else for( int i=0; i<int(getParameter1()); i++ )
      {
         unsigned index;
         Boolean mark = false;
         do
         {
            index = (rand()%totalNodes) + 1;
            m_HotSpotList->valueAt(index,mark);
         }
         while(mark==true);
         m_HotSpotList->setValueAt(index,true);
      }
   }
}

//*************************************************************************
//:
//  f: virtual PRZString PRZTrafficPatternHotSpot::asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZTrafficPatternHotSpot :: asString() const
{
   PRZString rs =  Inhereited::asString() + "Hot spot: n=" +
                   PRZString(getParameter1()) + ", p=" + 
                   PRZString(getParameter2());
   
   for(int i=0; i<m_HotSpotList->numberOfElements(); i++)
   {
      Boolean isHotSpot;
      m_HotSpotList->valueAt(i+1,isHotSpot);
      if(isHotSpot)
      {
         rs += PRZString(" (") + PRZString(i%getX()) + "," +
               PRZString(i/getY()) + ")"; 
      }
   }
   return rs;

}

//*************************************************************************
//:
//  f: virtual double PRZTrafficPatternHotSpot::getInjectFactor(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

double PRZTrafficPatternHotSpot :: getInjectFactor(const PRZPosition& pos)
{
   if( ! m_HotSpotList )
      initialize();
  
   unsigned x = pos.valueForCoordinate(PRZPosition::X);
   unsigned y = pos.valueForCoordinate(PRZPosition::Y);      
      
   unsigned compose = x*getX() + y + 1;
   Boolean isHotSpot;
   m_HotSpotList->valueAt(compose,isHotSpot);

   return (isHotSpot) ? getParameter2() : 1.0;
}


//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternHotSpot::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternHotSpot :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y, z=0;
   PRZPosition destiny;
   
   do
   {
      x = rand() % getX();
      y = rand() % getY();
      destiny = PRZPosition(x,y,z);
   }
   while( source == destiny );

   msg->setDestiny(destiny);

   getSimulation().getNetwork()->sendMessage(msg);
   return true;

}

//*************************************************************************
//*************************************************************************

//*************************************************************************
//:
//  f: virtual PRZString PRZTrafficPatternFlash::asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZTrafficPatternFlash :: asString() const
{
   PRZString rs =  Inhereited::asString() + "Flash: p10=" +
                   PRZString(getParameter1()) + ", alpha=" +
                   PRZString(getParameter2());
      return rs;

}

//*************************************************************************
//:
//  f: virtual double PRZTrafficPatternFlash::getInjectFactor(const PRZPosition& pos);
//
//  d:
//:
//*************************************************************************

double PRZTrafficPatternFlash :: getInjectFactor(const PRZPosition& pos)
{
   if( ! m_StateList )
   {      
      m_StateList = new TFlashStateList(getX()*getY());
      m_StateList -> initialize(0);
   }      
  
   unsigned x = pos.valueForCoordinate(PRZPosition::X);
   unsigned y = pos.valueForCoordinate(PRZPosition::Y);      
      
   unsigned compose = x*getX() + y + 1;
   int nodeState;
   m_StateList->valueAt(compose,nodeState);


   double rc;
   
   
   if( nodeState == 0 )
   {
      rc = 0.0;
      if( drand48() < getParameter3() )    // P01
      {
         // Pasa al estado "1"
         m_StateList->setValueAt(compose,1);
      }
      
   }
   else
   {
      rc = 1.0 + getParameter2();
      if( drand48() < getParameter1() )   // P10
      {
         // Pasa al estado "0"
         m_StateList->setValueAt(compose,0);
      }
   }

   return rc;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternFlash::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternFlash :: injectMessage(const PRZPosition& source)
{
   PRZMessage* msg = generateBasicMessage(source);
    
   // El destino se elige aleatoriamente entre todos los posibles,
   // con la unica condicion de que origen != destino.
      
   unsigned x, y, z=0;
   PRZPosition destiny;
   
   do
   {
      x = rand() % getX();
      y = rand() % getY();
      destiny = PRZPosition(x,y,z);
   }
   while( source == destiny );

   msg->setDestiny(destiny);

   getSimulation().getNetwork()->sendMessage(msg);

   return true;

}

//*************************************************************************
//*************************************************************************


//*************************************************************************
//:
//  f: PRZTrafficPatternFile(const PRZString& fileName, PRZSimulation& s);
//
//  d:
//:
//*************************************************************************

PRZTrafficPatternFile :: PRZTrafficPatternFile( const PRZString& fileName, 
                                                PRZSimulation& s )
                       : PRZTrafficPattern(s),
                         m_FileName(fileName),
                         m_File(0)
{
   setFromFile(true);
   m_File = new ifstream(fileName);
   if( !m_File->good() )
   {
      PRZString err;
      err.sprintf(ERR_PRZTRAFI_005, (char*)m_FileName ); 
      EXIT_PROGRAM(err);
   }
   
   readNextMessage();
}

//*************************************************************************
//:
//  f: Boolean PRZTrafficPatternFile::readNextMessage();
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternFile :: readNextMessage()
{
   PRZString line;
   do
   {
      line = PRZString::lineFrom(*m_File);
      if( m_File->eof() ) return false;  // EOF 
      line = PRZString::stripBlanks(line);
   } while( line == "" );
   
   uTIME time     = line.word(1).asInteger();
   unsigned x_org = line.word(2).asInteger();
   unsigned y_org = line.word(3).asInteger();
   unsigned z_org = line.word(4).asInteger();

   unsigned x_dst = line.word(5).asInteger();
   unsigned y_dst = line.word(6).asInteger();
   unsigned z_dst = line.word(7).asInteger();
 
   unsigned size  = line.word(8).asInteger();

   m_Message.setGenerationTime(time);
   m_Message.setSource(PRZPosition(x_org,y_org,z_org));
   m_Message.setDestiny(PRZPosition(x_dst,y_dst,z_dst));
   m_Message.setPacketSize(getSimulation().getPacketLength());
   m_Message.setMessageSize(size);
   
   return true;
}

//*************************************************************************
//:
//  f: virtual Boolean PRZTrafficPatternFile::injectMessage(const PRZPosition& source);
//
//  d:
//:
//*************************************************************************

Boolean PRZTrafficPatternFile :: injectMessage(const PRZPosition& source)
{
   uTIME currentTime = getSimulation().getNetwork()->getCurrentTime();
   while( currentTime >= m_Message.generationTime() )
   {
      getSimulation().getNetwork()->sendMessage(m_Message);   
      if( ! readNextMessage() ) return false;
   }
   
   return true;
}

//*************************************************************************


// fin del fichero
