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
// File: ../ATCSimul/src/PRZRouter.cpp
//
// Clase      : PRZRouter
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRouter.cpp,v $
//
// $Date: 2001/10/30 11:11:06 $
//
// $Revision: 1.2 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRouter.cpp,v 1.2 2001/10/30 11:11:06 vpuente Exp $";

#include <PRZRouter.hpp>

#ifndef __PRZConnection_HPP__
#include <PRZConnection.hpp>
#endif

#ifndef __PRZInjector_HPP__
#include <PRZInjector.hpp>
#endif

#ifndef __PRZConsumer_HPP__
#include <PRZConsumer.hpp>
#endif

#ifndef __PRZCrossbar_HPP__
#include <PRZCrossbar.hpp>
#endif

#ifndef __PRZInterfaz_HPP__
#include <PRZInterfaz.hpp>
#endif

#ifndef __PRZRouterBuilder_HPP__
#include <PRZRouterBuilder.hpp>
#endif

#ifndef __PRZFifoMemory_HPP__
#include <PRZFifoMemory.hpp>
#endif

#ifndef __PRZSimulation_HPP__
#include <PRZSimulation.hpp>
#endif

#ifndef __PRZConst_HPP__
#include <PRZConst.hpp>
#endif

#include <string.h>

    
//*************************************************************************

// Run time information
IMPLEMENT_RTTI_DERIVED(PRZRouter,PRZRunnableComponent);

//*************************************************************************
//:
//  f: inline unsigned getNumberOfBuffers() const;
//  d:
//
//  f: inline uTIME getCurrentTime() const;
//  d:
//
//  f: inline PRZPosition& getPosition() const;
//  d:
//
//  f: inline void setPosition(const PRZPosition& position);
//  d:
//
//  f: inline void setInjector(PRZComponent* component);
//  d:
//
//  f: inline void setConsumer(PRZComponent* component);
//  d:
//
//  f: inline PRZComponent* getInjector() const;
//  d:
//
//  f: inline PRZComponent* getConsumer() const;
//  d:
//
//  f: inline unsigned   valueForBufferSize();
//  d:
//
//  f: inline PRZString  valueForBufferControl();
//  d:
//
//  f: inline PRZString  valueForRoutingControl();
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZRouter(const PRZComponentId& id);
//
//  d:
//:
//*************************************************************************

PRZRouter :: PRZRouter(const PRZComponentId& id)
           : PRZRunnableComponent(id),
             m_CurrentTime(0),
             m_Position(0),
             m_InjectorRoundRobin(0),
             m_InjectionType(INJECT_ROUND_ROBIN),
             m_ExternalInfo(0),
	          m_ExtraPoolSize(0)
{
   
}


//*************************************************************************
//:
//  f: virtual ~PRZRouter();
//
//  d:
//:
//*************************************************************************

PRZRouter :: ~PRZRouter()
{
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      cursor.element()->setRouter(0);
   }
   
   PRZMessageList::Cursor cursor2(m_MessageList);
   forCursor(cursor2)
   {
      delete cursor2.element();
   }
   
   m_BufferList.removeAllElements();
   m_InjectorList.removeAllElements();
   m_ConsumerList.removeAllElements();
   m_MessageList.removeAllElements();
}

void PRZRouter :: terminate()
{
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      cursor.element()->terminate();
   }
}


//*************************************************************************
//:
//  f:  unsigned valueForBufferSize();
//
//  d:
//:
//*************************************************************************

unsigned PRZRouter :: valueForBufferSize()
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
   return gData.routerBufferSize();
}


//*************************************************************************
//:
//  f:  PRZString valueForBufferControl();
//
//  d:
//:
//*************************************************************************

PRZString PRZRouter :: valueForBufferControl()
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
   return gData.routerBufferControl();
}


//*************************************************************************
//:
//  f:  PRZString valueForRoutingControl();
//
//  d:
//:
//*************************************************************************

PRZString PRZRouter :: valueForRoutingControl()
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
   return gData.routerRoutingControl();
}

//*************************************************************************
//:
//  f: virtual void run(uTIME runTime);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: run(uTIME runTime)
{
   PRZComponentSet::Cursor  componentCursor(m_ComponentSet);
   PRZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
   PRZComponentSet::Cursor  preRunCursor(m_PreRunComponentSet);
   PRZComponentSet::Cursor  postRunCursor(m_PostRunComponentSet);
   
   m_CurrentTime = runTime;
   setExternalInfo(0);
      
   forCursor(preRunCursor)
   {
      preRunCursor.element()->preRun(runTime);
   }
      
   forCursor(componentCursor)
   {
      componentCursor.element()->run(runTime);
   }   
   
   forCursor(connectionCursor)
   {
      connectionCursor.element()->run(runTime);
   }

   forCursor(postRunCursor)
   {
      postRunCursor.element()->postRun(runTime);
   }   
}


//*************************************************************************
//:
//  f: virtual void initialize();
//
//  d:
//:
//*************************************************************************

void PRZRouter :: initialize()
{
   if( ! isInitializated() )
   {
      PRZComponentSet::Cursor componentCursor(m_ComponentSet);
      forCursor(componentCursor)
      {
         PRZComponent* current = componentCursor.element();
         current->setRouter(this);
         current->setOwner(this);
         current->initialize();

      }
   }
   setInitializated(true);
}


//*************************************************************************
//:
//  f: virtual void postInitialize();
//
//  d: Este metodo se invoca despues de inicializar el router, por es 
//     necesario que alguno de los componentes necesita que algun otro
//     ya este inicializado previamente;
//:
//*************************************************************************

void PRZRouter :: postInitialize()
{
   PRZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      cursor.element()->postInitialize();
   }
}


//*************************************************************************
//:
//  f: virtual void sendMessage(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: sendMessage(PRZMessage* msg)
{
   PRZInjector* injector = chooseInjector();
   injector->sendMessage(msg);
}


//*************************************************************************
//:
//  f: void onHeaderPacketReceived(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: onHeaderPacketReceived(const PRZMessage* msg)
{

   PRZNetwork* net = (PRZNetwork*)getOwner();
   setExternalInfo(msg->getExternalInfo());
   net->onHeaderPacketReceived(msg);
}


//*************************************************************************
//:
//  f: void onPacketReceived(const PRZMessage& msg);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: onPacketReceived(PRZMessage* msg)
{
   PRZNetwork* net = (PRZNetwork*)getOwner();
   net->onPacketReceived(msg);
   //Le indico a rsim la recepcion a la llegada de la cabeza
  //setExternalInfo(msg->getExternalInfo());
   if( msg->messageSize() == 1 )
   {
      net->onMessageReceived(msg);
   }
   else
   {

      PRZMessageList::Cursor cursor(m_MessageList);
      PRZMessage* ptrMessage = 0;
      Boolean found = false;
      forCursor(cursor)
      {
         ptrMessage = cursor.element();
         if( ptrMessage->getIdentifier() == msg->getIdentifier() )
         {
            found = true;
            break;
         }
      }
      
      if( ! found )
      {
         // Es el primer paquete del mensaje que se ha recibido.
         msg->setPacketNumber(1);
         m_MessageList.add(msg);
         return;
      }
      else 
      {
         // 'ptrMessage' apunta a un mensaje en la lista de recibidos.
         if( msg->packetNumber() == 1 )
         {
            ptrMessage->setPacketInjectionTime( msg->packetInjectionTime() );
         }
         unsigned newPacketNumber = ptrMessage->packetNumber() + 1;
         ptrMessage->setPacketNumber(newPacketNumber);
         if( newPacketNumber == ptrMessage->messageSize() )
         {
            net->onMessageReceived(ptrMessage);
            delete ptrMessage;
            cursor.remove();
         }
      }
   }
}

//*************************************************************************
//:
//  f: virtual unsigned getBufferOccupation( przROUTINGTYPE type,
//                                           unsigned channel=1 );
//
//  d:
//:
//*************************************************************************

unsigned PRZRouter :: getBufferOccupation( przROUTINGTYPE type,
                                           unsigned channel )
{
   PRZComponentId id( PRZComponentId::FifoMemory, type, "*", channel );
   PRZComponent* component = componentWithIdLike(id); 
   PRZFifoMemory* fifo = POINTERCAST(PRZFifoMemory,component);
   if( fifo )
   {
      return (fifo->getBufferSize() - fifo->bufferHoles());
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZRouter :: buildFlowControl()
{
   // Para evitar que sea una clase abstracta.
}
//*************************************************************************
//:
//  f: virtual void buildInputInterfaz(unsigned number =1 );
//
//  d:
//:
//*************************************************************************

void PRZRouter :: buildInputInterfaz(unsigned number)
{
   // La accion por defecto no vale
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz(unsigned number=1);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: buildOutputInterfaz(unsigned number)
{
   // La accion por defecto no vale.
}


//*************************************************************************
//:
//  f: void writeComponentStatus(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: writeComponentStatus(ostream& os)
{
   PRZComponent* component = componentWithName("CROSSBAR");
   if(component) os << component->getStatus() << endl;
   /*
   component = componentWithName("RTG12");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG21");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG22");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG31");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG32");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG41");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG42");
   if(component) os << component->getStatus() << endl;
   component = componentWithName("RTG5");
   if(component) os << component->getStatus() << endl;
   os << "********************************************" << endl;
   */
}

//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRouter :: asString() const
{
   PRZSimulation* simul = (PRZSimulation*)getSimulation();
   PRZString texto = "Router: id=";
   texto += getName();
   texto += PRZString(" routing=") + simul->globalData((PRZNetwork*)getOwner()).routerRoutingControl();
   texto += " buffer=";
   texto += PRZString(simul->globalData((PRZNetwork*)getOwner()).routerBufferSize()) + " (";
   texto += simul->globalData((PRZNetwork*)getOwner()).routerBufferControl() + ")";
   return texto;
}


//*************************************************************************
//:
//  f: virtual PRZInjector* chooseInjector();
//
//  d:
//:
//*************************************************************************

PRZInjector* PRZRouter :: chooseInjector()
{
   PRZComponent* rc = 0;
   unsigned injectorNumber = m_InjectorList.numberOfElements();
   
   if( injectorNumber == 1 )
   {
      m_InjectorList.elementAt(0,rc);   
   }
   else
   {
      if( m_InjectionType == INJECT_RANDOM )
      {
         unsigned index = rand() % injectorNumber;
         m_InjectorList.elementAt(index,rc);
      }
      else if( m_InjectionType == INJECT_ROUND_ROBIN )
      {
         m_InjectorList.elementAt(m_InjectorRoundRobin,rc);
         if( (++m_InjectorRoundRobin) >= injectorNumber ) 
            m_InjectorRoundRobin = 0;  
      }
   }
   
   return (PRZInjector*)rc;
}


//*************************************************************************
//:
//  f: void writeBufferInformation(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: writeBufferInformation(ostream& os)
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());
   unsigned short numeroBuffers = m_BufferList.numberOfElements();
   unsigned short sizeBuffers   = gData.routerBufferSize();
   
   os.write((char*)&numeroBuffers,sizeof(unsigned short));
   os.write((char*)&sizeBuffers,sizeof(unsigned short));
   
   PRZBufferList::Cursor cursor(m_BufferList);
   forCursor(cursor)
   {
      char nameBuffer[32];
      unsigned short bufferSize = cursor.element()->getBufferSize();
      for(register int j=0; j<32; j++) nameBuffer[j]='\0';
      strcpy(nameBuffer,cursor.element()->asString());
      os.write(nameBuffer,32);
      os.write((char*)&bufferSize,sizeof(unsigned short));
   }
}


//*************************************************************************
//:
//  f: void writeBufferStatus(ostream& os);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: writeBufferStatus(ostream& os)
{
   unsigned short x = getPosition().valueForCoordinate(PRZPosition::X);
   unsigned short y = getPosition().valueForCoordinate(PRZPosition::Y);
   unsigned short z = getPosition().valueForCoordinate(PRZPosition::Z);
   
   os.write((char*)&x,sizeof(unsigned short));
   os.write((char*)&y,sizeof(unsigned short));
   os.write((char*)&z,sizeof(unsigned short));
   PRZBufferList::Cursor cursor(m_BufferList);
   forCursor(cursor)
   {
      unsigned short occupation = cursor.element()->bufferOccupation();
      os.write((char*)&occupation,sizeof(unsigned short));
   }   
}


//*************************************************************************
//:
//  f: void addBuffer(PRZFifoMemory* fifo);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: addBuffer(PRZFifoMemory* fifo)
{
   if(fifo) m_BufferList.add(fifo);
}


//*************************************************************************
//:
//  f: void setBufferSize(unsigned size);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: setBufferSize(unsigned size)
{
   PRZBufferList::Cursor cursor(m_BufferList);
   forCursor(cursor)
   {
      cursor.element()->setBufferSize(size);
   }
}
unsigned PRZRouter :: getTotalBuffeSize() const
{
   unsigned sizeTotal=0;
   PRZBufferList::Cursor cursor(m_BufferList);
   forCursor(cursor)
   {
      sizeTotal+=cursor.element()->getBufferSize();
   }
   return sizeTotal+m_ExtraPoolSize;
}
//*************************************************************************
//:
//  f:       void setPoolSize(unsigned size);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: setPoolSize(unsigned size)
{

   PRZInjectorList::Cursor cursor(m_InjectorList);
   forCursor(cursor)
   {
      PRZInjector* inj = (PRZInjector*)(cursor.element());
      inj->getFlowControl()->initializePool(size*getTotalBuffeSize());
      return;
   }
}

//*************************************************************************
//:
//  f: void addInjector(PRZComponent* injector);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: addInjector(PRZComponent* injector)
{
   if(injector) m_InjectorList.add(injector);
}


//*************************************************************************
//:
//  f: void addConsumer(PRZComponent* consumer);
//
//  d:
//:
//*************************************************************************

void PRZRouter :: addConsumer(PRZComponent* consumer)
{
   if(consumer) m_ConsumerList.add(consumer);
}


//*************************************************************************
//:
//  f: unsigned getMessagesToSend() const;
//
//  d:
//:
//*************************************************************************

unsigned PRZRouter :: getMessagesToSend() const
{
   unsigned messages = 0;
   PRZInjectorList::Cursor cursor(m_InjectorList);
   forCursor(cursor)
   {
      PRZInjector* inj = (PRZInjector*)(cursor.element());
      messages += inj->getMessagesToSend();
   }
   return messages;
}


//*************************************************************************
//:
//  f: unsigned getInputWithType( przROUTINGTYPE type );
//
//  d:
//:
//*************************************************************************

unsigned PRZRouter :: getInputWithType( przROUTINGTYPE type )
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());

   for( int i=1; i<=gData.routerInputsType()->numberOfElements(); i++ )
   {
      przROUTINGTYPE tempType;
      if( gData.routerInputsType()->valueAt(i,tempType) )
         if( tempType==type ) return i; 
   }
   return 0;
}


//*************************************************************************
//:
//  f: unsigned getOutputWithType(przROUTINGTYPE type);
//
//  d:
//:
//*************************************************************************

unsigned PRZRouter :: getOutputWithType( przROUTINGTYPE type )
{
   PRZGlobalData& gData = ((PRZSimulation*)getSimulation())->globalData((PRZNetwork*)getOwner());

   for( int i=1; i<=gData.routerOutputsType()->numberOfElements(); i++ )
   {
      przROUTINGTYPE tempType;
 
      if( gData.routerOutputsType()->valueAt(i,tempType) )
         if( tempType==type ) return i; 
   }
   return 0;
}


//*************************************************************************
//:
//  f: static PRZRouter* newFrom( const PRZTag* tag, 
//                                PRZComponent* owner, 
//                                INDEX i );
//
//  d:
//:
//*************************************************************************

PRZRouter* PRZRouter :: newFrom( const PRZTag* tag, 
                                 PRZComponent* owner,
                                 INDEX& index)
{   
   PRZComponentId idRouter(PRZComponentId::Router, _LocalNode_, tag->tagId());
   PRZString bufferSize,  bufferControl, injectorControl, tableUpdown,
             tableUpdownAdap, routingControl, inputs, outputs,missRouting,
             hamiltonCycle,tableHamilton,tableAdap,fileTop;
   
   tag->getAttributeValueWithName(PRZ_TAG_BUFFER_SIZE, bufferSize);
   tag->getAttributeValueWithName(PRZ_TAG_BUFFER_CONTROL, bufferControl);
   tag->getAttributeValueWithName(PRZ_TAG_ROUTING_CONTROL, routingControl);
   tag->getAttributeValueWithName(PRZ_TAG_INPUTS, inputs);
   tag->getAttributeValueWithName(PRZ_TAG_OUTPUTS, outputs);
   tag->getAttributeValueWithName(PRZ_TAG_INJECT_CONTROL, injectorControl);
   tag->getAttributeValueWithName(PRZ_TAG_FILEUPDOWN, tableUpdown);
   tag->getAttributeValueWithName(PRZ_TAG_FILEUPDOWNADAP, tableUpdownAdap);
   tag->getAttributeValueWithName(PRZ_TAG_NONMINIMAL, missRouting);
   tag->getAttributeValueWithName(PRZ_TAG_FILEHAMILTON, tableHamilton);
   tag->getAttributeValueWithName(PRZ_TAG_FILEADAP, tableAdap);
   tag->getAttributeValueWithName(PRZ_TAG_CYCLEHAMILTON, hamiltonCycle);
   tag->getAttributeValueWithName(PRZ_TAG_FILETOP, fileTop);
 
   if( (bufferSize == "")     ||
       (bufferControl == "")  || 
       (routingControl == "") ||
       (inputs == "" )        ||
       (outputs == "" ) )
   {
      PRZString err;
      err.sprintf(ERR_PRZROUTE_001,(char*)(tag->tagId()));
      EXIT_PROGRAM(err);
   }
   
   if(routingControl==PRZ_TAG_UPDOWN && tableUpdown=="")
   {
       PRZString err;
       err.sprintf(ERR_PRZRTING_004, (char*)routingControl );
       EXIT_PROGRAM(err);
   }
   if(routingControl==PRZ_TAG_UPDOWNADAP &&( tableUpdown=="" || tableUpdownAdap=="" ))
   {
       PRZString err;
       err.sprintf(ERR_PRZRTING_005, (char*)routingControl );
       EXIT_PROGRAM(err);
   }
   if((routingControl==PRZ_TAG_HAMILTON || routingControl==PRZ_TAG_HAMILTONADAP) &&( hamiltonCycle==""  || tableAdap==""|| ( tableHamilton=="" && fileTop=="") ))
   {
       PRZString err;
       err.sprintf(ERR_PRZRTING_006, (char*)routingControl );
       EXIT_PROGRAM(err);
   }
   
   PRZRouter* router = new PRZRouter(idRouter);   
   router->setSimulation(owner->getSimulation());
   router->setOwner(owner);
   PRZString idForResults="Aricmetic Routing";
   PRZGlobalData& gData = ((PRZSimulation*)(owner->getSimulation()))->globalData((PRZNetwork*)owner);
   
   if( ! gData.routerIsInitializated() )
   {   
      gData.setMaxPorts(inputs.asInteger());
      gData.routerSetBufferSize(bufferSize.asInteger());
      gData.routerSetBufferControl(bufferControl);
      if(missRouting=="")
      {
         gData.notMiss();
      }
      else
      { 
         gData.Miss(missRouting.asInteger());
      }
      gData.routerSetRoutingControl(routingControl);
      gData.routerSetInputsType(new PRZTypeArray(inputs.asInteger()));
      gData.routerSetOutputsType(new PRZTypeArray(outputs.asInteger()));
      if(routingControl==PRZ_TAG_UPDOWN || routingControl==PRZ_TAG_UPDOWNADAP )
      {
         gData.setTableFile(tableUpdown);
	 PRZTableRouting* temporal;
         gData.initializeTableUpdown(tableUpdown,&(temporal));
	 gData.setTableUpdownPointer(temporal);
         idForResults=PRZString(" Up/Down: ")+tableUpdown;
      }
      if( routingControl==PRZ_TAG_UPDOWNADAP)
      {
         PRZTableRouting* temporal;
         gData.initializeTableUpdown(tableUpdownAdap,&(temporal));
         gData.setTableUpdownAdapPointer(temporal);
         idForResults+=PRZString(" Up/downAdap: ")+tableUpdownAdap;
      }
      if( routingControl==PRZ_TAG_HAMILTON || routingControl==PRZ_TAG_HAMILTONADAP)
      {
	 PRZTableRouting* temporal2;
	 cerr<<"Leyendo tabla adaptativa: "<<tableAdap<<endl;
         gData.initializeTableUpdown(tableAdap,&(temporal2));
         gData.setTableUpdownAdapPointer(temporal2);
	 
         PRZTableRouting* temporal;
	 cerr<<"Leyendo ciclo hamiltoniano:"<<hamiltonCycle<<endl;

	 gData.initializeHamiltonCycle(hamiltonCycle);
	 
	 //Hacerme yo mismo la tabla segura. Le tengo que pasar en el 
	 //constructor del router el fichero de topologia
	 if(fileTop!="")
	 {
	    cerr<<"Leyendo Topologia: "<<fileTop<<endl;
	    gData.initializeTopol(fileTop);
	    cerr<<"Construyendo tabla segura"<<endl;
            gData.initializeHamiltonRoutingTable(&(temporal));
	 }
	 else
	 {
	    //Utilizo los datos que me da fernando
	    cerr<<"Leyendo tabla segura: "<<tableHamilton<<endl;
	    gData.initializeTableUpdown(tableHamilton,&(temporal));
	 }
	 gData.setTableUpdownPointer(temporal);

         if(fileTop!="")
	 {
            idForResults=PRZString("AdapRTbl: ")+tableAdap+ PRZString(" TopFile:")+fileTop +PRZString( " cycleTa: ")+hamiltonCycle;
	 }
	 else
	 {
            idForResults=PRZString("AdapRTbl: ")+tableAdap+ PRZString(" HamTbl:")+tableHamilton +PRZString( " cycleTa: ")+hamiltonCycle;
	 }
      }
      gData.setStringId(idForResults);
         
   }
   

   if( injectorControl == PRZ_TAG_INJ_RANDOM )
   {
      router->setInjectorType(PRZRouter::INJECT_RANDOM);
   }
   else if( injectorControl == PRZ_TAG_INJ_RROBIN )
   {
      router->setInjectorType(PRZRouter::INJECT_ROUND_ROBIN);
   }
   else if( injectorControl == PRZ_TAG_INJ_FREE )
   {
      router->setInjectorType(PRZRouter::INJECT_FREE);
   }
   else
   {
      router->setInjectorType(PRZRouter::INJECT_ROUND_ROBIN);
   }

   PRZString endTag = PRZString("/") + tag->tagName();
   PRZTag* nextTag = PRZComponent::routerBuilder->getTagWithIndex(++index);
   
   PRZInterfaz** arrayInterfaz = new PRZInterfaz*[PRZ_MAX_NUMBER_OF_CV];
   //PRZInterfaz* arrayInterfaz[3];
   
   while( nextTag && nextTag->tagName()!=endTag )
   {
      if( nextTag->tagName() == PRZ_TAG_INPUT )
      {
         // Se trata de una entrada al router (un wrapper)
         
         unsigned inputNumber = nextTag->tagId().asInteger();
         PRZString type, wrapper;
         nextTag->getAttributeValueWithName(PRZ_TAG_WRAPPER, wrapper);
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         wrapper.replace(',',' ');
         PRZString tempWrapper;
         int i=1;

	      while( (tempWrapper=wrapper.word(i)) != "" )
         {
            PRZString wIzda = PRZString::getStringLeftTo(tempWrapper,'.');
            PRZString wDcha = PRZString::getStringRightTo(tempWrapper,'.');
            unsigned  wNumb = (wDcha=="") ? 1 : wDcha.asInteger();
            PRZComponent* comp = router->componentWithName(wIzda);
            if( ! comp )
            {
               PRZString err;
               err.sprintf(ERR_PRZROUTE_002, (char*)wrapper);
               EXIT_PROGRAM(err);
            }
            if( i > PRZ_MAX_NUMBER_OF_CV ) 
            {
               PRZString err;
               err.sprintf(ERR_PRZROUTE_003, i );
               EXIT_PROGRAM(err);
            }
            arrayInterfaz[i-1] = comp->getInputInterfaz(wNumb);
            i++;
         }
         
         PRZInterfaz* interfaz;
         interfaz = PRZInterfaz::createWrapperFor(arrayInterfaz,i-1);
         
         PRZString name = PRZString("I") + PRZString(inputNumber);
         interfaz->setName(name);
         router->addComponent(interfaz);
         
         if( ! gData.routerIsInitializated() )
            gData.routerInputsType()->setValueAt(inputNumber,_string2routingType(type));
      }
      else if( nextTag->tagName() == PRZ_TAG_OUTPUT )
      {
         // Se trata de una salida al router (un wrapper)
         
         unsigned outputNumber = nextTag->tagId().asInteger();
         PRZString type, wrapper;
         nextTag->getAttributeValueWithName(PRZ_TAG_WRAPPER, wrapper);
         nextTag->getAttributeValueWithName(PRZ_TAG_TYPE, type);
         wrapper.replace( ',' , ' ' );
         PRZString tempWrapper;
         int i=1;
//         PRZInterfaz* arrayInterfaz[PRZ_MAX_NUMBER_OF_CV];
         //PRZInterfaz* arrayInterfaz[3];
         
         while( (tempWrapper=wrapper.word(i)) != "" )
         {
            PRZString wIzda = PRZString::getStringLeftTo(tempWrapper,'.');
            PRZString wDcha = PRZString::getStringRightTo(tempWrapper,'.');
            unsigned  wNumb = (wDcha=="") ? 1 : wDcha.asInteger();
            
            PRZComponent* comp = router->componentWithName(wIzda);
            if( ! comp )
            {
               PRZString err;
               err.sprintf(ERR_PRZROUTE_002, (char*)wrapper);
               EXIT_PROGRAM(err);
            }
            if( i > PRZ_MAX_NUMBER_OF_CV )
            {
               PRZString err;
               err.sprintf(ERR_PRZROUTE_003, i );
               EXIT_PROGRAM(err);
            }

            arrayInterfaz[i-1] = comp->getOutputInterfaz(wNumb);
            i++;
         }
         
         PRZInterfaz* interfaz;
         interfaz = PRZInterfaz::createWrapperFor(arrayInterfaz,i-1);
         
         PRZString name = PRZString("O") + PRZString(outputNumber);
         interfaz->setName(name);
         router->addComponent(interfaz);
   
         if( ! gData.routerIsInitializated() )
            gData.routerOutputsType()->setValueAt(outputNumber,_string2routingType(type));
      }
      else
      {
         // Se trata de un componente
         
         PRZComponent* newComponent = PRZComponent::routerBuilder->
                       parseComponentDefinition(nextTag,router,index);
         if( newComponent ) 
         {
            newComponent->setOwner(router);
            newComponent->setRouter(router);
            newComponent->initialize();
            router->addComponent(newComponent);
         }
      }
      
      nextTag = PRZComponent::routerBuilder->getTagWithIndex(++index);

   } // while
   
   delete[] arrayInterfaz;


   gData.routerSetInitializated();
      
   router->initialize();   
   router->postInitialize();
   
   return router;
}


//*************************************************************************


// fin del fichero
