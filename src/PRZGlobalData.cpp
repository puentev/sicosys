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
// File: ../ATCSimul/src/PRZGlobalData.cpp
//
// Clase      : PRZGlobalData
//
// Hereda de  : None
//
// Notes:  
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZGlobalData.cpp,v $
//
// $Date: 2001/10/30 11:11:06 $
//
// $Revision: 1.2 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZGlobalData.cpp,v 1.2 2001/10/30 11:11:06 vpuente Exp $";

#include <PRZGlobalData.hpp>

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZGlobalData);

//*************************************************************************
//:
//  f: PRZGlobalData();
//
//  d:
//:
//*************************************************************************

PRZGlobalData :: PRZGlobalData()
               : m_cInitializated(false),
                 m_cInputsType(0),
                 m_cLocalPorts(0),
                 m_cOutputMux(0),
                 m_cOutputsType(0),
                 m_NumberOfHeaders(2),
                 m_rBufferControl(""),
                 m_rBufferSize(0),
                 m_rInitializated(false),
                 m_rInputsType(0),
                 m_rOutputsType(0),
                 m_rRoutingControl(""),
                 m_sInitializated(false),
		 m_sInputsType(0),
                 m_sLocalPorts(0),
                 m_sOutputMux(0),
		 m_sOutputsType(0),
                 m_tableUpdown(0),
                 m_tableUpdownAdap(0),
                 m_hamiltonTable(0),
                 m_tableTopol(0)
{
}


//*************************************************************************
//:
//  f: ~PRZGlobalData();
//
//  d:
//:
//*************************************************************************

PRZGlobalData :: ~PRZGlobalData()
{
   delete m_rInputsType;
   delete m_rOutputsType;
   delete m_cInputsType;
   delete m_cOutputsType;
   if(m_tableUpdown)     delete [] m_tableUpdown;
   if(m_tableUpdownAdap) delete [] m_tableUpdownAdap;
   if(m_hamiltonTable)   delete [] m_hamiltonTable;
   if(m_tableTopol)      delete [] m_tableTopol;
}
//*************************************************************************
//:
//  f:  void initializeTopol(PRZString filename);
//
//  d:  Initialize the topology table. Usseful to establish Hamilton cycle
//
//:
//*************************************************************************
void PRZGlobalData ::initializeTopol(PRZString filename)
{
    ifstream*      m_File;
    m_File = new ifstream(filename);
    PRZString line;
    suPAIR pareja;
    
    if( !m_File->good() )
    {
       PRZString err;
       err.sprintf(ERR_PRZTONET_005, (char*)filename ); 
       EXIT_PROGRAM(err);
    }  
    m_tableTopol=new PRZTableTopol[m_maxPorts*m_nodes];
    unsigned org,dst;
    while(!m_File->eof())
    {
         line = PRZString::lineFrom(*m_File);
         line = PRZString::stripBlanks(line);
         org = line.firstWord().asInteger();
	 line.removeFirstWord();
         unsigned port=0;
	 unsigned port_dest=0;
         while (line.size() != 0) 
         {
	  //Guardo toda la informacion del conexion
	  //puerto desde y hacia.
	    
           PRZString line_dst = line.firstWord();
	   PRZString line_dst2 = line_dst;

   	   line.removeFirstWord();
	   dst = PRZString::getStringLeftTo(line_dst,'/').asInteger();
           pareja=m_tableTopol[m_maxPorts*org+port];
	   pareja.setRight(dst);
	   m_tableTopol[m_maxPorts*org+port]=pareja;
	   port_dest = PRZString::getStringRightTo(line_dst2,'/').asInteger();
           pareja=m_tableTopol[m_maxPorts*dst+port_dest-1];
	   pareja.setLeft(org);
	   m_tableTopol[m_maxPorts*dst+port_dest-1]=pareja;
	              port++;

         }
    }
   
}


//*************************************************************************
//:
//  f:  void initializeTableUpdown(PRZString filename, PRZTableRouting* table);
//
//  d: I'm assuming this format for trace file
//     number_nodes  max_ports
//     / <-0 to 0 (comsuption)
//     |port_1 prioryty |port_2 prioryty|..... / <-end destiny 1 configuration
//     |port....
//     ----------  <-end node 0 configuration
//:
//*************************************************************************
 void PRZGlobalData::initializeTableUpdown(PRZString filename, PRZTableRouting** table)
 {
    ifstream*      m_File;
    m_File = new ifstream(filename);
    PRZTableRouting* tableTemp;
    Boolean final=false;
    PRZString line;
    int actualnode=0;
    int destinynode=0;
    if( !m_File->good() )
    {
       PRZString err;
       err.sprintf(ERR_PRZTONET_005, (char*)filename ); 
       EXIT_PROGRAM(err);
    }
    do
    { //Cleanning all lines before table
      line = PRZString::lineFrom(*m_File);
    } while(line!="START");
    
    if( m_File->eof() )
    {
       PRZString err;
       err.sprintf(ERR_PRZTONET_006, (char*)filename ); 
       EXIT_PROGRAM(err);
    }
    //The first line includes the number of nodes and the maximun number of ports per 
    //router.
       line = PRZString::lineFrom(*m_File);
       final = m_File->eof();
       line = PRZString::stripBlanks(line);
       m_nodes=line.firstWord().asInteger();
       line.removeFirstWord();
       int max_ports=line.firstWord().asInteger();
       setMaxPorts(max_ports);
       tableTemp=new PRZTableRouting[m_nodes*m_nodes*max_ports];
       for(int j=0;j<m_nodes*m_nodes*max_ports;j++)
          tableTemp[j]=0;
          
    while(!final)
    {
       if(destinynode>m_nodes)
       {
          PRZString err;
          err.sprintf(ERR_PRZTONET_006, (char*)filename ); 
          EXIT_PROGRAM(err);
       }
       line = PRZString::lineFrom(*m_File);
       final = m_File->eof();
       if(line=="EXIT") break;
       if(line.size()==0)continue; //Retornos de carro inutiles
       line = PRZString::stripBlanks(line);
       if(line.word(1)=='/')   //consumption
       { 
          
          destinynode++;
          continue;
       }
       if(line=="_______________________") //separator beetween nodes 
       {
          actualnode++;
          destinynode=0;
          continue;         
       }
       while(1)
       {
          line.removeFirstWord();
          tableTemp[line.word(1).asInteger()-1+ destinynode*m_maxPorts+actualnode*m_maxPorts*m_nodes]=
                (short unsigned)(line.word(2).asInteger()+1);
          line.removeFirstWord();
          line.removeFirstWord();
          if(line.word(1)=='/') break;
       }
       destinynode++; 
    }
//     cerr<<filename<<endl;
//     cerr<<"START"<<endl;
//     cerr<<m_nodes<<" "<<m_maxPorts<<endl;
//  for(int ii=0; ii<m_nodes;ii++)
//     {
//        for(int jj=0;jj<m_nodes;jj++)
//        {
//           for(int kk=0;kk<m_maxPorts;kk++)
//              if(tableTemp[kk+jj*m_maxPorts+ii*m_maxPorts*m_nodes]!=0)
//              cerr<<"| " <<kk+1<<" "<<tableTemp[kk+jj*m_maxPorts+ii*m_maxPorts*m_nodes]-1<< " ";
//                    cerr<<"/"<<endl;
//        }cerr<<"_______________________"<<endl;
//     }
     
    *table=tableTemp;

     return;
 }
//*************************************************************************
//:
//  f:  void initializeHamiltonCycle(PRZString filename, PRZHamiltonTable** table);
//
//  d:  The format is similar to Routing Tables but just n-nodes line
//      in each i-line we must include in a pair form the channels who
//      form part from the hamiltonian cycle (input and output) at node i.
//      If a node don't belongs to the hamiltonian cycle this list is empty.
//      Both adaptive and determinsitic routing needs only one table, then 
//      dont get through reference the pointer to the table.
//
//:
//*************************************************************************
void PRZGlobalData::initializeHamiltonCycle(PRZString filename)
{
    ifstream*     m_File;
    m_File = new ifstream(filename);
    Boolean final=false;
    PRZString line;
    int actualnode=0;
    if( !m_File->good() )
    {
       PRZString err;
       err.sprintf(ERR_PRZTONET_005, (char*)filename ); 
       EXIT_PROGRAM(err);
    }
    do
    { //Cleanning all lines before table
      line = PRZString::lineFrom(*m_File);
    } while(line!="START");
    if( m_File->eof() )
    {
       PRZString err;
       err.sprintf(ERR_PRZTONET_007, (char*)filename ); 
       EXIT_PROGRAM(err);
    }
    //The first line includes the number of nodes and the maximun number of ports per 
    //router.
       line = PRZString::lineFrom(*m_File);
       final = m_File->eof();
       line = PRZString::stripBlanks(line);
       m_nodes=line.firstWord().asInteger();
       
       line.removeFirstWord();
       m_hamiltonTable=new PRZHamiltonTable[m_nodes];
       for(int j=0;j<m_nodes;j++)
          m_hamiltonTable[j].removeAllElements();
          
    while(!final)
    {
       if(actualnode>m_nodes)
       {
          PRZString err;
          err.sprintf(ERR_PRZTONET_007, (char*)filename ); 
          EXIT_PROGRAM(err);
       }
       line = PRZString::lineFrom(*m_File);
       final = m_File->eof();
       if(line=="EXIT") break;
       if(line.size()==0)continue; //Retornos de carro inutiles
       line = PRZString::stripBlanks(line);
       if(line.word(1)=='/')   //the hamilton cycle don pass through this node
       { 
          
          actualnode++;
          continue;
       }

       while(1)
       {
          line.removeFirstWord();
          m_hamiltonTable[actualnode].enqueue(suPAIR(line.word(1).asInteger(),line.word(2).asInteger()));
          line.removeFirstWord();
          line.removeFirstWord();
          if(line.word(1)=='/') break;
       }
       actualnode++; 
    }
    m_File->close();
    if(actualnode!=m_nodes)
    {
          cerr<<actualnode;
          PRZString err;
          err.sprintf(ERR_PRZTONET_007, (char*)filename ); 
          EXIT_PROGRAM(err);
    }
     return;
}

//*************************************************************************
//:
//  f:        void getProfitableChannels(ProfitableChannels& list,unsigned source,unsigned destination, Boolean onlyMinimal=true);
//
//  d:  Return all profitable channel from the specified table
//:
//*************************************************************************  
void PRZGlobalData::getProfitableChannels( ProfitableChannels& list,PRZTableRouting* table, int source, int dest, Boolean onlyMinimal)
{
   short unsigned isProfitable;
   for(int i=0;i<getMaxPorts();i++)
   {
      isProfitable=table[i+dest*m_maxPorts+m_maxPorts*m_nodes*source];
      if(isProfitable==1 || (!onlyMinimal && isProfitable>1))
      {
         list.enqueue(i+1);
      }
   }
   return;
}
//*************************************************************************
//:
//  f:        void getProfitableChannels(ProfitableChannels& list,unsigned source,unsigned destination, Boolean onlyMinimal=true);
//
//  d:  Return all profitable channel from the specified table
//:
//*************************************************************************  
Boolean PRZGlobalData::addHamiltonChannels(ProfitableChannels& list,unsigned currentNode, unsigned currentChannel) const
{
   short unsigned oPort;
   suPAIR channelsPair;

   for(int i=0;i<list.numberOfElements();i++)
   {
      list.elementAt(i,oPort);
      for(int j=0;j<m_hamiltonTable[currentNode].numberOfElements();j++)
      {
         m_hamiltonTable[currentNode].elementAt(j,channelsPair);
         if( channelsPair.right()==oPort)
         {
            return false;
         }   
      }
   }
   for(int k=0;k<m_hamiltonTable[currentNode].numberOfElements();k++)
   {
       m_hamiltonTable[currentNode].elementAt(k,channelsPair);
       if(currentChannel==channelsPair.left())
       {
          list.enqueue(channelsPair.right());
          return true;
       }
   }
   unsigned chooseOne=((int)drand48()*1000)%m_hamiltonTable[currentNode].numberOfElements();
   m_hamiltonTable[currentNode].elementAt(chooseOne,channelsPair);
   list.enqueue(channelsPair.right());
   return true;
}

//*************************************************************************
//:
//  f:Boolean isNeddedBubble(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const;
//
//  d:Return if it's needed to apply bubble in the next channel (under 
//    Irregular bubble routing (aka Hamilton)
//:
//*************************************************************************  
Boolean PRZGlobalData::isNeddedBubble(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const
{
   suPAIR channelsPair;
   for(int i=0;i<m_hamiltonTable[currentNode].numberOfElements();i++)
   {
      m_hamiltonTable[currentNode].elementAt(i,channelsPair);
      if(channelsPair.left()!=currentChannel && channelsPair.right()==nextChannel)
      {
         //We  are not in the hamiltonian cicle and next channels yes
         return true;
      }   
   }
   if(m_hamiltonTable[currentNode].numberOfElements()==0) return false;
   return false;
}
//*************************************************************************
//:
//  f:short unsigned getBubble(unsigned currentNode, short unsigned nextChannel) const;
//
//  d: returns the channel number where we must apply bubble condition.
//     This number is procesed to acces to their fifo pointer
//:
//*************************************************************************        
short unsigned PRZGlobalData::getBubble(unsigned currentNode, short unsigned nextChannel) const
{
   suPAIR channelsPair;
   for(int i=0;i<m_hamiltonTable[currentNode].numberOfElements();i++)
   {
      m_hamiltonTable[currentNode].elementAt(i,channelsPair);
      if(channelsPair.right()==nextChannel)
      {
         return channelsPair.left();
      }   
   }
    
       PRZString err=PRZString("The bubble condition is NOT needed here!. currentnode=")+
             PRZString(currentNode)+PRZString(" next Channel=")+ PRZString(nextChannel) ;
       EXIT_PROGRAM(err);
       return 0;
}
//*************************************************************************
//:
//  f:Boolean  getOutHam(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const
//
//  d: if the packet get out the hamiltonian cycle then return true.
//:
//*************************************************************************        
Boolean  PRZGlobalData::getOutHam(unsigned currentNode, short unsigned currentChannel, short unsigned nextChannel) const
{
   suPAIR channelsPair; 
   for(int i=0;i<m_hamiltonTable[currentNode].numberOfElements();i++)
   {
      m_hamiltonTable[currentNode].elementAt(i,channelsPair);
      if(channelsPair.left()==currentChannel && channelsPair.right()!=nextChannel)
      {
         //We are in the hamiltonian cicle and next channels no
         return true;
      }   
   }
   if(m_hamiltonTable[currentNode].numberOfElements()==0) return false;
   return false;
}

//*************************************************************************
//:
//  f:short unsigned getBubble(unsigned currentNode, short unsigned nextChannel) const;
//
//  d: 
//
//:
//*************************************************************************        
short unsigned PRZGlobalData::followHam(unsigned currentNode, short unsigned currentChannel) const
{
   suPAIR channelsPair;
   for(int i=0;i<m_hamiltonTable[currentNode].numberOfElements();i++)
   {
      m_hamiltonTable[currentNode].elementAt(i,channelsPair);
      if(channelsPair.left()==currentChannel)
      {
         return channelsPair.right();
      }   
   }
   return 0;
}

//*************************************************************************
//:
//  f:  void initializeHamiltonRoutingTable(PRZTableRouting* table);
//
//  d: From Topologie structure and Hamilton cicle structures, this method
//     construct "safe" routing tables
//:
//*************************************************************************
void PRZGlobalData::initializeHamiltonRoutingTable(PRZTableRouting** table)
{
   //Table initialization
   PRZTableRouting* tableTemp;
   tableTemp=new PRZTableRouting[m_nodes*m_nodes*m_maxPorts];
   for(int j=0;j<m_nodes*m_nodes*m_maxPorts;j++)
      tableTemp[j]=0;
	  
   if(m_hamiltonTable==0 || m_tableTopol==0 || m_tableUpdownAdap==0)
   {
      EXIT_PROGRAM("Before construct hamilton routing Table, it must exist hamiltoncicle, adaptive routing table and topology!!!\n");
   }
   int node_from;
   int node_to;
   for(node_from=0;node_from<m_nodes;node_from++)
   {
      //cerr<<"From"<<node_from<<endl;
      for(node_to=0;node_to<m_nodes;node_to++)
      {

	 //El numero posible de caminos hamiltonianos
	 //se restringe al numero de veces que pasa por el
	 //nodo origen. Es independiente de lo que pase mas allá

	 
	 //El destino esta fuera del anillo 
	 //vuelvo para atras hasta alcanzar uno que lo este
	 //vuelvo por la tabla de rutas adaptativa hasta alcanzar un nodo
	 //dentro del hamiltoniano.
	 Boolean vecinos_sin_ciclo=false;  
	 int node_to_proximo=node_to;
	 int node_to_proximo_anterior=0;
	 //si el node_to_proximo es igual que el nodo origen en algun caso,
	 //entonces es un salto fuera del anillo... pasando de el
		   
	 while(m_hamiltonTable[node_to_proximo].numberOfElements()==0)
	 {
	    if(node_from==node_to_proximo)
	    {
	       vecinos_sin_ciclo=true;
	       break;
	    }
             for(int j=0;j<m_maxPorts;j++)
	     {
	       if(m_tableUpdownAdap[j+node_from*m_maxPorts+node_to_proximo*m_maxPorts*m_nodes]==1)
	       {
		  if( m_tableTopol[(m_maxPorts*node_to_proximo+j)].right()!=node_to_proximo_anterior)
		  {
		        node_to_proximo_anterior=node_to_proximo;
			node_to_proximo=m_tableTopol[m_maxPorts*node_to_proximo+j].right();
       		       if(node_from==node_to_proximo && 
			   m_hamiltonTable[node_to_proximo_anterior].numberOfElements()==0 )
		       {
	                  vecinos_sin_ciclo=true;
	                  break;
	               }
		  }

	       }

	     }
	    
	 }
	 		// if( vecinos_sin_ciclo==true)
// 			{
// 			    cerr<<"te pille2 "<<node_from<<" "<<node_to<<" "<<vecinos_sin_ciclo<<endl;
// 			      vecinos_sin_ciclo=true;
// 			}
	 //cerr<<node_from<<" "<<node_to<<"("<<node_to_proximo<<")" <<endl;
	 if(vecinos_sin_ciclo==false && node_from!=node_to)
	 { 
	 	 
         int minSaltos=999999;
	 int caminoSalida=0;
	 
	 //Si el origen esta fuera del anillo, tabla de rutas no tiene nada en estas entradas
	 //Se movera por el camino mas corto hasta alcanzar a un nodo que este detro
	 for(int i=0;i<m_hamiltonTable[node_from].numberOfElements();i++)
         {
	     int contadorDeSaltos=0;
	     suPAIR channelsPair;
             m_hamiltonTable[node_from].elementAt(i,channelsPair);
	     int next_node=m_tableTopol[m_maxPorts*node_from+channelsPair.right()-1].right();
	     int next_node_anterior=node_from;
	     contadorDeSaltos++;
	     // for(int h=0;h<m_nodes;h++)
// 	     {
// 	     	for(int t=0;t<m_maxPorts;t++)
// 	        {
// 	         cerr<<m_tableTopol[m_maxPorts*h+t].right()<<"/"<<m_tableTopol[m_maxPorts*h+t].left()<<" ";
// 	        }cerr<<endl;
// 	     }//exit(0);
	    // cerr<<"CAMINO "<<node_from<<"--> "<<node_to<<endl;
	     while(next_node!=node_to_proximo)
	     {
		 //En cada salto compruebo que sigo la direccion 
		 //correcta del anillo
		 Boolean compruebaCorrect=false;
		 
		 for(int j=0;j<m_hamiltonTable[next_node].numberOfElements();j++)
                 {
		    m_hamiltonTable[next_node].elementAt(j,channelsPair);
		      // cerr<<" Nodo actual:  " <<next_node<<" anterior nodo:"<<next_node_anterior<<" Nodo siguiente: "<<m_tableTopol[m_maxPorts*next_node+channelsPair.right()-1].left()<<endl;
		    if(m_tableTopol[m_maxPorts*next_node+channelsPair.left()-1].right()==next_node_anterior)
		    {
		       
		      // cerr<<"Objetivo alcanzado"<< next_node<<"-->"<<m_tableTopol[m_maxPorts*next_node+channelsPair.right()-1].left()<<endl;

		       contadorDeSaltos++;
		       next_node_anterior=next_node;
	               next_node=m_tableTopol[m_maxPorts*next_node+channelsPair.right()-1].left();
		       compruebaCorrect=true;
                       break;
		    }
		 }
		 if(compruebaCorrect==false)
		 { 
		    PRZString err="Al intentar seguir el ciclo hamiltoniano he encontrado una incongruencia\n";
		    err+="Seguramente el ciclo no corresponde con la topologia\n";
		    EXIT_PROGRAM(err);
		 }
		 
	     }
	     
	     m_hamiltonTable[node_from].elementAt(i,channelsPair);

	     if(contadorDeSaltos<minSaltos)
	     {
		for(int j=0;j<m_maxPorts;j++)
		{
		   tableTemp[j+ node_to_proximo*m_maxPorts+node_from*m_maxPorts*m_nodes]=0;
		}
		tableTemp[channelsPair.right()-1+ node_to*m_maxPorts+node_from*m_maxPorts*m_nodes]=1;
	        minSaltos=contadorDeSaltos;
	     }
	     if(contadorDeSaltos==minSaltos)
	     {
		tableTemp[channelsPair.right()-1+ node_to*m_maxPorts+node_from*m_maxPorts*m_nodes]=1;
	     }
	   }

	 }
      }
   }
   
//   for(int ii=0; ii<m_nodes;ii++)
//      {
//         for(int jj=0;jj<m_nodes;jj++)
//         {
//            for(int kk=0;kk<m_maxPorts;kk++)
//               if(tableTemp[kk+jj*m_maxPorts+ii*m_maxPorts*m_nodes]!=0)
//               cerr<<"| " <<kk+1<<" "<<tableTemp[kk+jj*m_maxPorts+ii*m_maxPorts*m_nodes]-1<< " ";
//                     cerr<<"/"<<endl;
//         }cerr<<"_______________________"<<endl;
//      }
   //Retorna le valor por referencia
    *table=tableTemp;
    return;
}
//*************************************************************************


// fin del fichero
