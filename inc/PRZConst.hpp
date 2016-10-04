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
// File: ../ATCSimul/inc/PRZConst.hpp
//
// Description:   fichero de variables globales constantes
//
// Notes:   
//
// $Author: vpuente $ 
//
// $RCSfile: PRZConst.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZConst_HPP__
#define __PRZConst_HPP__


   #ifdef __PRZCONST_IMPLEMENT__
      #define _EXTERN
      #define _CONST
      #define _IS(value)  =value
   #else
      #define _EXTERN     extern
      #define _CONST      const
      #define _IS(value) 
   #endif
   
   #ifndef __PRZString_HPP__
   #include "PRZString.hpp"
   #endif

//*************************************************************************

   _EXTERN _CONST PRZString PRZ_LOG_FILENAME          _IS("ATCSimul.log");

//*************************************************************************

   _EXTERN _CONST unsigned PRZ_PACKET_HEADER          _IS(2);
   _EXTERN _CONST unsigned PRZ_MAX_NUMBER_OF_CV       _IS(4);
   _EXTERN _CONST unsigned PRZ_MAX_NUMBER_SIMULATIONS _IS(10);
   
//******************************************** Tags para el fichero .ini **

   _EXTERN _CONST PRZString PRZ_TAG_INI_ROUTER      _IS("RouterFile");
   _EXTERN _CONST PRZString PRZ_TAG_INI_NETWORK     _IS("NetworkFile");
   _EXTERN _CONST PRZString PRZ_TAG_INI_SIMULATION  _IS("SimulationFile");

//******************************************* Tags para descripcion SGML **

   _EXTERN _CONST PRZString PRZ_TAG_ROUTER           _IS("Router");
   _EXTERN _CONST PRZString PRZ_TAG_BUFFER           _IS("Buffer");
   _EXTERN _CONST PRZString PRZ_TAG_MPBUFFER         _IS("MPBuffer");
   _EXTERN _CONST PRZString PRZ_TAG_FIFOMUXED        _IS("FifoMuxed");
   _EXTERN _CONST PRZString PRZ_TAG_ROUTING          _IS("Routing");
   _EXTERN _CONST PRZString PRZ_TAG_ROUTING_MUXED    _IS("RoutingMuxed");
   _EXTERN _CONST PRZString PRZ_TAG_CROSSBAR         _IS("Crossbar");
   _EXTERN _CONST PRZString PRZ_TAG_MUXCV            _IS("MultiplexorCV");
   _EXTERN _CONST PRZString PRZ_TAG_DEMUX            _IS("Demultiplexor");
   _EXTERN _CONST PRZString PRZ_TAG_CONNECTION       _IS("Connection");
   _EXTERN _CONST PRZString PRZ_TAG_INJECTOR         _IS("Injector");
   _EXTERN _CONST PRZString PRZ_TAG_CONSUMER         _IS("Consumer");
   _EXTERN _CONST PRZString PRZ_TAG_INPUT            _IS("Input");
   _EXTERN _CONST PRZString PRZ_TAG_OUTPUT           _IS("Output");
   _EXTERN _CONST PRZString PRZ_TAG_MESHNET          _IS("MeshNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_TORUSNET         _IS("TorusNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_MIDIMEWNET       _IS("MidimewNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_SQUAREMIDIMEWNET _IS("SquareMidimewNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_IRREGULARNET     _IS("IrregularNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_FILETOP          _IS("FileTop");
   _EXTERN _CONST PRZString PRZ_TAG_NULLNET          _IS("NullNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_SIMULATION       _IS("Simulation");
   _EXTERN _CONST PRZString PRZ_TAG_SIMULATION_DUAL  _IS("SimulationDual");
   _EXTERN _CONST PRZString PRZ_TAG_GAUGE            _IS("Gauge");
   _EXTERN _CONST PRZString PRZ_TAG_ROUTER_GAUGE     _IS("routerPos");
   _EXTERN _CONST PRZString PRZ_TAG_FILE_GAUGE       _IS("file");
   _EXTERN _CONST PRZString PRZ_TAG_SIMPLE_ROUTER    _IS("SimpleRouter");

   _EXTERN _CONST PRZString PRZ_TAG_XPLUS           _IS("X+");
   _EXTERN _CONST PRZString PRZ_TAG_XMINUS          _IS("X-");
   _EXTERN _CONST PRZString PRZ_TAG_YPLUS           _IS("Y+");
   _EXTERN _CONST PRZString PRZ_TAG_YMINUS          _IS("Y-");
   _EXTERN _CONST PRZString PRZ_TAG_ZPLUS           _IS("Z+");
   _EXTERN _CONST PRZString PRZ_TAG_ZMINUS          _IS("Z-");
   _EXTERN _CONST PRZString PRZ_TAG_LOCAL           _IS("Node");
   
   _EXTERN _CONST PRZString PRZ_TAG_SIZE            _IS("size");
   _EXTERN _CONST PRZString PRZ_TAG_SIZE_SHORT      _IS("sizeShort");
   _EXTERN _CONST PRZString PRZ_TAG_CONTROL         _IS("control");
   _EXTERN _CONST PRZString PRZ_TAG_ACKGRANUL       _IS("ackgranul");
   _EXTERN _CONST PRZString PRZ_TAG_INPUTS          _IS("inputs");
   _EXTERN _CONST PRZString PRZ_TAG_OUTPUTS         _IS("outputs");
   _EXTERN _CONST PRZString PRZ_TAG_BUFFER_SIZE     _IS("bufferSize");
   _EXTERN _CONST PRZString PRZ_TAG_BUFFER_CONTROL  _IS("bufferControl");
   _EXTERN _CONST PRZString PRZ_TAG_ROUTING_CONTROL _IS("routingControl");
   _EXTERN _CONST PRZString PRZ_TAG_SYNCHR_SIZE     _IS("syncSize");
   _EXTERN _CONST PRZString PRZ_TAG_WRAPPER         _IS("wrapper");
   _EXTERN _CONST PRZString PRZ_TAG_TYPE            _IS("type");
   _EXTERN _CONST PRZString PRZ_TAG_TYPE_INJECTOR   _IS("typeInj");
   _EXTERN _CONST PRZString PRZ_TAG_OUTPUTMUX       _IS("mux");
   _EXTERN _CONST PRZString PRZ_TAG_INJECT_CONTROL  _IS("injControl");
   _EXTERN _CONST PRZString PRZ_TAG_DMUX_CONTROL    _IS("control");
   _EXTERN _CONST PRZString PRZ_TAG_NUM_HEADERS     _IS("numHeaders");
   _EXTERN _CONST PRZString PRZ_TAG_MISS_ROUTING    _IS("maxMisses");
   _EXTERN _CONST PRZString PRZ_TAG_ISOLATED        _IS("isolated");
   _EXTERN _CONST PRZString PRZ_TAG_SF_THR          _IS("thrSF");

   
   _EXTERN _CONST PRZString PRZ_TAG_WORMHOLE           _IS("WH");
   _EXTERN _CONST PRZString PRZ_TAG_WORMHOLE_OB        _IS("WH-OB");
   _EXTERN _CONST PRZString PRZ_TAG_CUTHROUGH          _IS("CT");
   _EXTERN _CONST PRZString PRZ_TAG_CUTHROUGH_OUTBUFF  _IS("CT-OB");
   _EXTERN _CONST PRZString PRZ_TAG_CUTHROUGH_MUX      _IS("CTM");   
   _EXTERN _CONST PRZString PRZ_TAG_VIRTUAL_CT         _IS("VCT");
   _EXTERN _CONST PRZString PRZ_TAG_STOREFOR           _IS("SF");
   _EXTERN _CONST PRZString PRZ_TAG_VCT_NOFRAG         _IS("CT-NF");
   _EXTERN _CONST PRZString PRZ_TAG_INJ_RANDOM         _IS("RANDOM");
   _EXTERN _CONST PRZString PRZ_TAG_INJ_RROBIN         _IS("RROBIN");
   _EXTERN _CONST PRZString PRZ_TAG_INJ_FREE           _IS("FREE");
   
   //Crossbars types
   
   _EXTERN _CONST PRZString PRZ_TAG_MUX_ADAPT                       _IS("MUX-CT");
   _EXTERN _CONST PRZString PRZ_TAG_MUX_ADAPT_OB                    _IS("MUX-CT-OB");
   _EXTERN _CONST PRZString PRZ_TAG_MUX_ADAPT_SF                    _IS("MUX-CT-SF");
   _EXTERN _CONST PRZString PRZ_TAG_WH_OAC                          _IS("WH-OAC");   
   _EXTERN _CONST PRZString PRZ_TAG_WH_MULTIPLE_REQ                 _IS("WH-MREQ");
   _EXTERN _CONST PRZString PRZ_TAG_WH_MULTIPLE_REQ_OB              _IS("WH-MREQ-OB");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ                 _IS("CT-MREQ");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ_OB              _IS("CT-MREQ-OB");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ_OB_RE_VIRTUAL   _IS("CT-MREQ-OB-RE-VIRTUAL");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ_OB_RE           _IS("CT-MREQ-OB-RE");
   _EXTERN _CONST PRZString PRZ_TAG_WH_MULTIPLE_REQ_SIMPLE          _IS("WH-MSREQ");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ_SIMPLE          _IS("CT-MSREQ");
   _EXTERN _CONST PRZString PRZ_TAG_CT_MULTIPLE_REQ_SIMPLE_3D       _IS("CT-MSREQ3D");      
   
   
   //Routing Algorithms types
   _EXTERN _CONST PRZString PRZ_TAG_DORBURBLE                             _IS("DOR-BU");
   _EXTERN _CONST PRZString PRZ_TAG_DORBURBLE_H1                          _IS("DOR-BU-H1");
   _EXTERN _CONST PRZString PRZ_TAG_DORBURBLE_H1_VIRTUAL                  _IS("DOR-BU-H1-REQ-REP");  
   _EXTERN _CONST PRZString PRZ_TAG_DORBURBLE_OB                          _IS("DOR-BU-OB");   
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_3D                             _IS("BUR-3D");
   _EXTERN _CONST PRZString PRZ_TAG_DORCV                                 _IS("DOR-CV");
   _EXTERN _CONST PRZString PRZ_TAG_DORCV2                                _IS("DOR-CV2");
   _EXTERN _CONST PRZString PRZ_TAG_DORCV_3D                              _IS("DOR-CV-3D");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP                           _IS("BUR-ADAP");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_H1                        _IS("BUR-ADAP-H1");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_H1_REQUEST_REPLY          _IS("BUR-ADAP-H1-REQ-REP");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_H1_REQUEST_REPLY_FULL     _IS("BUR-ADAP-H1-REQ-REP-FULL");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_H1_DN                     _IS("BUR-ADAP-H1-DN");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR                        _IS("BUR-ADAP-MR");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR_RAND                   _IS("BUR-ADAP-MR-RAND");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR_RAND_REQ_REPLY         _IS("BUR-ADAP-MR-RAND-REQ-REP");  
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR_XY_REQ_REPLY         _IS("BUR-ADAP-MR-XY-REQ-REP");  
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MAXCREDITS_RAND_REQ_REPLY _IS("BUR-ADAP-MAXCREDITS-RAND-REQ-REP");  
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR_OB                     _IS("BUR-ADAP-MR-OB");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_MR_3D                     _IS("BUR-ADAP-MR-3D");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_1R                        _IS("BUR-ADAP-1R");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_1R_3D                     _IS("BUR-ADAP-1R-3D");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_3CV                       _IS("BUR-ADAP-3CV");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_3D                        _IS("BUR-ADAP-3D");
   _EXTERN _CONST PRZString PRZ_TAG_BURBLE_ADAP_3D_H1                     _IS("BUR-ADAP-3D-H1");
   _EXTERN _CONST PRZString PRZ_TAG_CV_ADAPT                              _IS("CV-ADAP");   
   _EXTERN _CONST PRZString PRZ_TAG_CV3D_ADAPT                            _IS("CV-ADAP-3D");
   _EXTERN _CONST PRZString PRZ_TAG_CV_ADAPT_1REQUEST                     _IS("CV-ADAP-1R");
   _EXTERN _CONST PRZString PRZ_TAG_CV3D_ADAPT_1REQUEST                   _IS("CV-ADAP3D-1R");
   _EXTERN _CONST PRZString PRZ_TAG_CV_OAC                                _IS("CV-ADAPT-OAC");
   //Irregular Network Routers (table lookup based)
   _EXTERN _CONST PRZString PRZ_TAG_UPDOWN                                _IS("UPDOWN");
   _EXTERN _CONST PRZString PRZ_TAG_FILEUPDOWN                            _IS("tableUpdown"); 
   _EXTERN _CONST PRZString PRZ_TAG_UPDOWNADAP                            _IS("UPDOWNADAP");
   _EXTERN _CONST PRZString PRZ_TAG_FILEUPDOWNADAP                        _IS("tableUpdownAdap"); 
   _EXTERN _CONST PRZString PRZ_TAG_NONMINIMAL                            _IS("noMinimal"); 
   _EXTERN _CONST PRZString PRZ_TAG_HAMILTON                              _IS("HAMILTON"); 
   _EXTERN _CONST PRZString PRZ_TAG_FILEADAP                              _IS("tableAdap"); 
   _EXTERN _CONST PRZString PRZ_TAG_FILEHAMILTON                          _IS("tableHamilton"); 
   _EXTERN _CONST PRZString PRZ_TAG_CYCLEHAMILTON                         _IS("cycleHamilton"); 
   _EXTERN _CONST PRZString PRZ_TAG_HAMILTONADAP                          _IS("HAMILTONADAP"); 

   
   //Routing Decision units multiplexed (AKA Crossbar muxed)
   _EXTERN _CONST PRZString PRZ_TAG_MUXED_BUBBLE_ADAP                     _IS("BUR-ADAP-MUX");
   _EXTERN _CONST PRZString PRZ_TAG_MUXED_BUBBLE_ADAP_FULL                _IS("BUR-ADAP-MUX-FULL");
   _EXTERN _CONST PRZString PRZ_TAG_MUXED_BUBBLE_ADAP_FULL_REQ_REP        _IS("BUR-ADAP-MUX-FULL-REQ-REP");

   _EXTERN _CONST PRZString PRZ_TAG_SOURCE          _IS("source");
   _EXTERN _CONST PRZString PRZ_TAG_DESTINY         _IS("destiny");
   
   _EXTERN _CONST PRZString PRZ_TAG_CHANNEL         _IS("channel");

   _EXTERN _CONST PRZString PRZ_TAG_SIZEX           _IS("sizeX");
   _EXTERN _CONST PRZString PRZ_TAG_SIZEY           _IS("sizeY");
   _EXTERN _CONST PRZString PRZ_TAG_SIZEZ           _IS("sizeZ");
   _EXTERN _CONST PRZString PRZ_TAG_REFROUTER       _IS("router");

   _EXTERN _CONST PRZString PRZ_TAG_NETWORK         _IS("Network");
   _EXTERN _CONST PRZString PRZ_TAG_REQUESTNETWORK  _IS("RequestNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_REPLYNETWORK    _IS("ReplyNetwork");
   _EXTERN _CONST PRZString PRZ_TAG_SIMULCYCLES     _IS("SimulationCycles");
   _EXTERN _CONST PRZString PRZ_TAG_STOPSIMUL       _IS("StopInjectionAfter");
   _EXTERN _CONST PRZString PRZ_TAG_PATTERN         _IS("TrafficPattern");
   _EXTERN _CONST PRZString PRZ_TAG_MESSAGELENGTH   _IS("MessageLength");
   _EXTERN _CONST PRZString PRZ_TAG_PACKETLENGTH    _IS("PacketLength");
   _EXTERN _CONST PRZString PRZ_TAG_SEED            _IS("Seed");
   _EXTERN _CONST PRZString PRZ_TAG_LOAD            _IS("Load");
   _EXTERN _CONST PRZString PRZ_TAG_PROB            _IS("Probability");

   _EXTERN _CONST PRZString PRZ_TAG_EMPTY           			_IS("EMPTY");
   _EXTERN _CONST PRZString PRZ_TAG_RANDOM          			_IS("RANDOM");
   _EXTERN _CONST PRZString PRZ_TAG_RANDOM_CP       			_IS("RANDOM_CP");
   _EXTERN _CONST PRZString PRZ_TAG_PERMUTATION     			_IS("PERMUTATION");
   _EXTERN _CONST PRZString PRZ_TAG_PERMUTATION_VIRTUAL     _IS("PERMUTATIONV");
   _EXTERN _CONST PRZString PRZ_TAG_MODAL           			_IS("MODAL");
   _EXTERN _CONST PRZString PRZ_TAG_BIMODAL         			_IS("BIMODAL");
   _EXTERN _CONST PRZString PRZ_TAG_PSUFFLE         			_IS("PERFECT-SUFFLE");
   _EXTERN _CONST PRZString PRZ_TAG_PSUFFLE_VIRTUAL         _IS("PERFECT-SUFFLEV");
   _EXTERN _CONST PRZString PRZ_TAG_BREVERSAL       			_IS("BIT-REVERSAL");
   _EXTERN _CONST PRZString PRZ_TAG_BREVERSAL_VIRTUAL       _IS("BIT-REVERSALV");

   _EXTERN _CONST PRZString PRZ_TAG_LOCALTRAFFIC    _IS("LOCAL");
   _EXTERN _CONST PRZString PRZ_TAG_FLASH           _IS("FLASH");
   _EXTERN _CONST PRZString PRZ_TAG_TRACE           _IS("TRACE");
   _EXTERN _CONST PRZString PRZ_TAG_PROBBIMODAL     _IS("prob");
   _EXTERN _CONST PRZString PRZ_TAG_MSGBIMODAL      _IS("numMsg");
   _EXTERN _CONST PRZString PRZ_TAG_SIGMA           _IS("sigma");
   _EXTERN _CONST PRZString PRZ_TAG_HOTSPOT         _IS("HOT-SPOT");
   _EXTERN _CONST PRZString PRZ_TAG_NODOSHOT        _IS("nodesHot");
   _EXTERN _CONST PRZString PRZ_TAG_PROBHOT         _IS("probHot");
   _EXTERN _CONST PRZString PRZ_TAG_DISTHOT         _IS("dist");
   _EXTERN _CONST PRZString PRZ_TAG_P10_FLASH       _IS("prob10");
   _EXTERN _CONST PRZString PRZ_TAG_ALPHA_FLASH     _IS("alpha");
   _EXTERN _CONST PRZString PRZ_TAG_PARAM1          _IS("p1");
   _EXTERN _CONST PRZString PRZ_TAG_PARAM2          _IS("p2");
   _EXTERN _CONST PRZString PRZ_TAG_PARAM3          _IS("p3");
   _EXTERN _CONST PRZString TUT_TAG_JUMP            _IS("JUMP");
   //Trace driven simulation parameters
   _EXTERN _CONST PRZString PRZ_TAG_TRACE_FILE      _IS("traceFile");
   _EXTERN _CONST PRZString PRZ_TAG_FLIT_SIZE       _IS("flitsz");
   _EXTERN _CONST PRZString PRZ_TAG_COMPRESSED      _IS("compress");
   _EXTERN _CONST PRZString PRZ_TAG_DUAL_NET        _IS("dualNet");
   _EXTERN _CONST PRZString PRZ_TAG_TRACE_CLOCK     _IS("clock");
   _EXTERN _CONST PRZString PRZ_TAG_TRACE_STATIC    _IS("static");
   _EXTERN _CONST PRZString PRZ_TAG_BLISTED_FACTOR  _IS("blistedFactor");

   
   _EXTERN _CONST PRZString PRZ_TAG_HEADER_DELAY    _IS("headerDelay");
   _EXTERN _CONST PRZString PRZ_TAG_DATA_DELAY      _IS("dataDelay");
   _EXTERN _CONST PRZString PRZ_TAG_DELAY           _IS("delay");

         
//*************************************************************************


#endif


// fin del fichero
