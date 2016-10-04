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
// File: ../ATCSimul/inc/PRZError.hpp
//
// Description:   fichero que contiene el texto de los mensajes de error
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZError.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZError_HPP__
#define __PRZError_HPP__


   #ifdef __PRZERROR_IMPLEMENT__
      #define _EXTERN
      #define _CONST
      #define _IS(value)  =value
   #else
      #define _EXTERN     extern
      #define _CONST      const
      #define _IS(value) 
   #endif
   

//*************************************************************************

   _EXTERN _CONST char ERR_ATCSIMUL_001[]
                  _IS("ATCSimulator: No se pueden correr mas de %d simulaciones a la vez");


   _EXTERN _CONST char ERR_PRZBLDR_001[]
                  _IS("PRZBuilder: No se puede abrir el fichero '%s'");
   _EXTERN _CONST char ERR_PRZBLDR_002[]
                  _IS("PRZBuilder: No existe un componentes Sgml con id=\"%s\"");

   _EXTERN _CONST char ERR_PRZCFLOC_001[]
                  _IS("PRZCrossbarCTFlow: %s Puerto de entrada=0; msg=%s");
   _EXTERN _CONST char ERR_PRZCFLOC_002[]
                  _IS("PRZCrossbarCTFlow: %s Puerto de salida=0; msg=%s");

   _EXTERN _CONST char ERR_PRZCFLOW_001[]
                  _IS("PRZCrossbarFlow: %s Puerto de salida no valido: (%s,%d)");
   _EXTERN _CONST char ERR_PRZCFLOW_002[]
                  _IS("PRZCrossbarFlow: %s Puerto de entrada = 0");
   _EXTERN _CONST char ERR_PRZCFLOW_003[]
                  _IS("PRZCrossbarFlow: %s Puerto de salida = 0");
   _EXTERN _CONST char ERR_PRZCFLOW_004[]
                  _IS("PRZCrossbarFlow: %s No existe evento de Request");
   _EXTERN _CONST char ERR_PRZCFLOW_005[]
                  _IS("PRZCrossbarFlow: %s Evento desconocido: %d");

   _EXTERN _CONST char ERR_PRZCMFLO_001[]
                  _IS("PRZCrossbarMuxFlow: %s Puerto de entrada = 0");
   _EXTERN _CONST char ERR_PRZCMFLO_002[]
                  _IS("PRZCrossbarMuxFlow: %s Puerto de salida = 0");
   _EXTERN _CONST char ERR_PRZCMFLO_003[]
                  _IS("PRZCrossbarMuxFlow: %s Puerto de salida no valido: (%s)");

   _EXTERN _CONST char ERR_PRZCMRFL_001[]
                  _IS("PRZCrossbarMRequestFlow: %s Puerto de entrada = 0");
   _EXTERN _CONST char ERR_PRZCMRFL_002[]
                  _IS("PRZCrossbarMRequestFlow: %s Puerto de salida = 0");
   _EXTERN _CONST char ERR_PRZCMRFL_003[]
                  _IS("PRZCrossbarMRequestFlow: %s No existe una lista de solicitudes");

   _EXTERN _CONST char ERR_PRZCOFLO_001[]
                  _IS("PRZConsumerFlow: %s Error de direccionamiento; msg=%s");

   _EXTERN _CONST char ERR_PRZCONEC_001[]
                  _IS("PRZConnection: Parametros invalidos; own=%p src=%p dst=%p");
   _EXTERN _CONST char ERR_PRZCONEC_002[]
                  _IS("PRZConnection: Interfaces con distinto num. de CV: %d, %d");
   _EXTERN _CONST char ERR_PRZCONEC_003[]
                  _IS("PRZConnection: El destino '%s' no es un puerto de entrada");
   _EXTERN _CONST char ERR_PRZCONEC_004[]
                  _IS("PRZConnection: El origen '%s' no es un puerto de salida");
   _EXTERN _CONST char ERR_PRZCONEC_005[]
                  _IS("PRZConnection: Falta el 'source' en la conexion '%s'");
   _EXTERN _CONST char ERR_PRZCONEC_006[]
                  _IS("PRZConnection: Falta el 'destiny' en la conexion '%s'");
   _EXTERN _CONST char ERR_PRZCONEC_007[]
                  _IS("PRZConnection: El 'source=%s' no existe para la conexion '%s'");
   _EXTERN _CONST char ERR_PRZCONEC_008[]
                  _IS("PRZConnection: El 'destiny=%s' no existe para la conexion '%s'");
                  
   _EXTERN _CONST char ERR_PRZCROSS_001[]
                  _IS("PRZCrossbar: %s No ha sido inicializado");
   _EXTERN _CONST char ERR_PRZCROSS_002[]
                  _IS("PRZCrossbar: Falta el tag '%s'");
   _EXTERN _CONST char ERR_PRZCROSS_003[]
                  _IS("PRZCrossbar: 'type=%s' no valido");
   _EXTERN _CONST char ERR_PRZCROSS_004[]
                  _IS("PRZCrossbar: Tag no valido: %s");

   _EXTERN _CONST char ERR_PRZDECON_001[]
                  _IS("PRZDelayConnection: Parametros invalidos; own=%p src=%p dst=%p");
   _EXTERN _CONST char ERR_PRZDECON_002[]
                  _IS("PRZDelayConnection: Interfaces con distinto num. de CV: %d, %d");
   _EXTERN _CONST char ERR_PRZDECON_003[]
                  _IS("PRZDelayConnection: El destino '%s' no es un puerto de entrada");
   _EXTERN _CONST char ERR_PRZDECON_004[]
                  _IS("PRZDelayConnection: El origen '%s' no es un puerto de salida");

   _EXTERN _CONST char ERR_PRZFFLOW_001[]
                  _IS("PRZFifoMemoryFlow: %s No hay espacio en el Buffer de entrada");

   _EXTERN _CONST char ERR_PRZFIFO_001[]
                  _IS("PRZFifoMemory: No se puede crear el flujo '%d'");
   _EXTERN _CONST char ERR_PRZFIFO_002[]
                  _IS("PRZFifoMemory: 'bufferControl=%s' no valido");

   _EXTERN _CONST char ERR_PRZFILOG_001[]
                  _IS("PRZFicheroLog: No se puede abrir el fichero '%s'");

   _EXTERN _CONST char ERR_PRZFLOW_001[]
                  _IS("PRZFlow: %s No se ha inicializado el interfaz de entrada");
   _EXTERN _CONST char ERR_PRZFLOW_002[]
                  _IS("PRZFlow: %s No se ha inicializado el interfaz de salida");
   _EXTERN _CONST char ERR_PRZFLOW_003[]
                  _IS("PRZFlow: %s Evento desconocido: %s");

   _EXTERN _CONST char ERR_PRZINJEC_001[]
                  _IS("PRZInjector: 'type=%s' no valido");

   _EXTERN _CONST char ERR_PRZINTER_001[]
                  _IS("PRZInterfaz: Canal '%d' no valido");

   _EXTERN _CONST char ERR_PRZNBLDR_001[]
                  _IS("PRZNetworkBuilder: Tag '%s' no valido");

   _EXTERN _CONST char ERR_PRZNET_001[]
                  _IS("PRZNetwork: Ya existe un router en %s");
   _EXTERN _CONST char ERR_PRZNET_002[]
                  _IS("PRZNetwork: No se puede inyectar el mensaje en %s");
   _EXTERN _CONST char ERR_PRZNET_003[]
                  _IS("PRZNetwork: No existe un router en (%d,%d)");

   _EXTERN _CONST char ERR_PRZRAFLO_001[]
                  _IS("PRZRoutingAdaptativeBurbleFlow: %s Buffer no encontrado");
    _EXTERN _CONST char ERR_PRZRAFLO_002[]
                  _IS("PRZRoutingFlowHamilton: %s Buffer no encontrado (id=%s)");
    
    
   _EXTERN _CONST char ERR_PRZRBAF3_001[]
                  _IS("PRZRoutingAdaptativeBurble3CvFlow: %s Buffer no encontrado");

   _EXTERN _CONST char ERR_PRZRBFLO_001[]
                  _IS("PRZRoutingBurbleFlow: %s Delta=0");
   _EXTERN _CONST char ERR_PRZRBFLO_002[]
                  _IS("PRZRoutingBurbleFlow: %s Buffer no encontrado: %s");

   _EXTERN _CONST char ERR_PRZRBLDR_001[]
                  _IS("PRZRouterBuilder: Tag '%s' no valido");

   _EXTERN _CONST char ERR_PRZRC2FL_001[]
                  _IS("PRZRoutingCV2Flow: %s Delta=0");

   _EXTERN _CONST char ERR_PRZRCAFL_001[]
                  _IS("PRZRoutingAdaptativeCVFlow: %s No puede almacenar mas flits");

   _EXTERN _CONST char ERR_PRZRCFLO_001[]
                  _IS("PRZRoutingCVFlow: %s Delta=0");

   _EXTERN _CONST char ERR_PRZRFLOW_001[]
                  _IS("PRZRoutingFlow: %s Delta=0");

   _EXTERN _CONST char ERR_PRZROUTE_001[]
                  _IS("PRZRouter: Faltan descriptores Sgml en el Router '%s'");
   _EXTERN _CONST char ERR_PRZROUTE_002[]
                  _IS("PRZRouter: Componente '%s' desconocido");
   _EXTERN _CONST char ERR_PRZROUTE_003[]
                  _IS("PRZRouter: Numero de CV's excesivo (%d)");

   _EXTERN _CONST char ERR_PRZRTING_001[]
                  _IS("PRZRouting: No se puede crear el flujo '%d'");
   _EXTERN _CONST char ERR_PRZRTING_002[]
                  _IS("PRZRouting: Falta el tag '%s'");
   _EXTERN _CONST char ERR_PRZRTING_003[]
                  _IS("PRZRouting: 'routingControl=%s' no valido");
   _EXTERN _CONST char ERR_PRZRTING_004[]
                  _IS("PRZRouting: 'routingControl=%s' require una tabla de rutas (tableUpdow=\"file\")");
   _EXTERN _CONST char ERR_PRZRTING_005[]
                  _IS("PRZRouting: 'routingControl=%s' require dos tablas de rutas (tableUpdow=\"file\" tableUpdowAdap=\"file\"");
   _EXTERN _CONST char ERR_PRZRTING_006[]
                  _IS("PRZRouting: 'routingControl=%s' require una tablas de rutas (tableHamilton=\"file\") y el ciclo PH(hamiltonCycle=\"file\")");
      
   _EXTERN _CONST char ERR_PRZRUNCO_001[]
                  _IS("PRZRunnableComponent: %s No tiene flujo asociado");

   _EXTERN _CONST char ERR_PRZSIMUL_001[]
                  _IS("PRZSimulation: El trafico bimodal necesita mas parametros");

   _EXTERN _CONST char ERR_PRZTONET_001[]
                  _IS("PRZTorusNetwork: La red tiene dimensiones no validas: (%d,%d,%d)");
   _EXTERN _CONST char ERR_PRZTONET_002[]
                  _IS("PRZTorusNetwork: No se puede crear el router con id='%s'");
   _EXTERN _CONST char ERR_PRZTONET_003[]
                  _IS("PRZTorusNetwork: Falta el tag '%s'");
   _EXTERN _CONST char ERR_PRZTONET_004[]
                  _IS("PRZTorusNetwork: Ninguna dimension puede ser 0: (x=%d, y=%d, z=%d)");
   _EXTERN _CONST char ERR_PRZTONET_005[]
                  _IS("PRZIrregular: No se puede abrir fichero %s");
   _EXTERN _CONST char ERR_PRZTONET_006[]
                  _IS("PRZIrregular: Error de formato en fichero de tabla de rutas%s");
   _EXTERN _CONST char ERR_PRZTONET_007[]
                  _IS("PRZIrregular: Error de formato en fichero de ciclo hamiltoniano%s");

   _EXTERN _CONST char ERR_PRZTRAFI_001[]
                  _IS("PRZTrafficPattern: Falta el tag '%s'");
   _EXTERN _CONST char ERR_PRZTRAFI_002[]
                  _IS("PRZTrafficPattern: 'type=%s' no valido");
   _EXTERN _CONST char ERR_PRZTRAFI_003[]
                  _IS("PRZTrafficPattern: Patron de trafico '%s' no valido");
   _EXTERN _CONST char ERR_PRZTRAFI_004[]
                  _IS("PRZTrafficPattern: Se han puesto demasidos hot-spots (%d)");
   _EXTERN _CONST char ERR_PRZTRAFI_005[]
                  _IS("PRZTrafficPattern: El fichero '%s' no se puede abrir");
   _EXTERN _CONST char ERR_PRZTRAFI_006[]
                  _IS("PRZTrafficPattern: 'dist=%d' es erroneo. Tiene que ser <= al radio de la red (%d)");
   _EXTERN _CONST char ERR_PRZTRAFI_007[]
                  _IS("PRZTrafficPattern: La opcion 'dist' solo se puede especificar para 2 hot-spots");
   _EXTERN _CONST char ERR_SFSPOOL_001[]
                  _IS("SFSPool: Not enough messages in pool");                  
//*************************************************************************


#endif


// fin del fichero
