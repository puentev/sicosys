#**************************************************************************
#
#  SICOSYS MAKEFILE
#
#  1.- gmake is required to compile the simulator
#  2.- gmake depend
#  3.- gmake 
#      Star to using the simulator. Is the executable in ATCSimul/mak/ directory
#      See doc/ directory for more info...
#
#*************************************************************************** 

# To compile with gcc disable this line
#IRIX = YES
#DEBUG = YES

#Trace driven simulation for message passing or ccNUMA. Traffic not providen in this 
#distribution. This feature requires RWare library (is Not free )
#USE_SCT = YES

ifdef USE_SCT
   INC1 = /usr/freeware/include
   LIBS = -lz -lrwtool
else
   EXCL_SCT= -DNO_SCT
endif
LIBS += -lm
PATH_INC  = ../inc  $(INC1) 
PATH_SRC  = ../src
PATH_LIB  = . -L../lib
PATH_OBJ  = ./OBJS
TARGET    = ATCSimul 
ATC_DLL   = libatcs.so
ATC_LIB   = libatcs.a

TRAZA     = -DNO_TRAZA   #trace off 

SGI_OPT = -n32 -r12000 -Ofast=ip27 #optimizacion agresiva

# *************************************************************************
VPATH = $(PATH_OBJ)
# *************************************************************************
ifdef IRIX
CC = CC
CDEBUG_OPTS = -n32  -c -g $(EXCL_SCT) $(TRAZA) $(NO_POOL) $(UNIROUTER) $(UNPAQ) -I$(PATH_INC) -woff 1682 -woff 1174 -o $@
LDEBUG_OPTS = -n32  -g -L$(PATH_LIB) -o $(TARGET)  #-lmalloc_cv
DDEBUG_OPTS = -n32  -shared -g -o $(ATC_DLL)

CFAST_OPTS  = -c $(SGI_OPT)  -I$(PATH_INC) $(EXCL_SCT) $(TRAZA) $(NO_POOL) $(UNPAQ) $(UNIROUTER) -woff 1174 -woff 1682 -o $@
LFAST_OPTS  = -IPA $(SGI_OPT)  -woff all -L$(PATH_LIB) -o $(TARGET)
DFAST_OPTS  =  -v -shared $(SGI_OPT) -woff all -o $(ATC_DLL)
MAKE_DEPEND = $(CC) -M -I$(PATH_INC) 
else
CC = g++-3.4
CDEBUG_OPTS = -c -g $(EXCL_SCT) $(TRAZA) $(NO_POOL) $(UNIROUTER) $(UNPAQ) -I$(PATH_INC) -Wno-deprecated -o $@
LDEBUG_OPTS = -g -L$(PATH_LIB) -o $(TARGET)   #-lmalloc_cv
DDEBUG_OPTS = -shared -g -o $(ATC_DLL)

CFAST_OPTS  = -c -O3 -I$(PATH_INC) $(EXCL_SCT) $(NO_POOL) $(TRAZA) $(UNPAQ) $(UNIROUTER) -Wno-deprecated -o $@
LFAST_OPTS  = -O3 -IPA -L$(PATH_LIB) -o $(TARGET)  
DFAST_OPTS  = -v -shared -O3 all -o $(ATC_DLL)
MAKE_DEPEND = $(CC) -P -M -I$(PATH_INC) 
endif


# *************************************************************************
ifndef DEBUG
   # Compilacion Optimizada
   COMPILER   = $(CC) $(CFAST_OPTS)
   LINKER     = $(CC) $(LFAST_OPTS)
   DLL_LINKER = $(CC) $(DFAST_OPTS)
else
   # Compilacion para debugger
   COMPILER   = $(CC) $(CDEBUG_OPTS)
   LINKER     = $(CC) $(LDEBUG_OPTS)
   DLL_BUILD  = $(CC) $(DDEBUG_OPTS)
endif
# *************************************************************************

LIB_BUILD   = $(CC) -ar -o $(ATC_LIB)

# *************************************************************************
MODULES = ATCSimulator \
          PRZAttribute  \
          PRZBase  \
          PRZBuilder  \
          PRZCrossbarCTFlow \
          PRZCrossbarFlow \
          PRZCompositeComponent \
          PRZConsumerFlow \
          PRZComponentId \
          PRZComponent \
          PRZConnection \
          PRZConst \
          PRZConsumer \
          PRZCrossbar \
          PRZCTFifoMemoryFlow \
          PRZDelayConnection \
          PRZError \
          PRZEvent \
          PRZFifoMemory  \
          PRZFicheroLog \
          PRZFlow  \
          PRZFifoMemoryFlow \
          PRZGauge \
          PRZGlobalData \
          PRZIniFile   \
          PRZInjectorCTFlow \
          PRZInjectorFlow \
          PRZInjector \
          PRZInterfaz \
          PRZInputPort \
          PRZMessage  \
          PRZNetworkBuilder \
          PRZNetwork   \
          PRZNotifier \
          PRZObserver \
          PRZOutputPort \
          PRZOptions   \
          PRZPort  \
          PRZPosition   \
          PRZRoutingBurbleFlow \
          PRZRoutingBurbleFlowH1 \
          PRZRouterBuilder \
          PRZRoutingFlow \
          PRZRouter \
          PRZRouting \
          PRZRunTimeInformation  \
          PRZRunnableComponent \
          PRZSimulationBuilder \
          PRZSimulation \
          PRZSimulationDual \
          PRZStringGz \
          PRZTag   \
          PRZTorusNetwork \
          PRZMeshNetwork \
          PRZNullNetwork \
          PRZIrregularNetwork \
          PRZTrafficPattern \
          PRZString \
          PRZWHFifoMemoryFlow \
          PRZRoutingCVFlow \
          PRZMultiplexorCV \
          PRZMultiplexorCVFlow \
          PRZRoutingAdaptativeBurbleH1Flow \
          PRZRoutingFlowUpdown \
          PRZRoutingFlowUpdownAdap \
          PRZRoutingFlowHamilton \
          PRZRoutingFlowHamiltonAdap \
          PRZCrossbarMuxFlow \
          PRZRoutingBurbleFlowH1Virtual \
          PRZCrossbarCTFlowMux \
          PRZRoutingAdaptativeBurbleH1VirtualFlow \
          PRZRoutingAdapCV1RequestFlow \
          PRZCrossbarMRequestSingleWHFlow \
          PRZFifoMuxed \
          PRZFifoMuxedFlow \
          PRZCTFifoMuxedFlow \
          PRZRoutingMuxed \
          PRZRoutingAdaptativeBurbleFlowMuxedFullVirtual \
          PRZRoutingAdaptativeBurbleFlowMuxed \
          PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits \
          PRZRoutingAdaptativeBurbleFlowMRVirtual \
          PRZRoutingAdaptativeBurbleFlowMR \
          PRZMultiportFifo \
          PRZMultiportFifoFlow \
          PRZMultiportFifoNoFragFlow \
          PRZCrossbarMRequestFlowCTOutBuffResumedVirtual \
          PRZCrossbarMRequestFlowCTOutBuffResumed \
          PRZMultiplexorCVFlowCT \
	  PRZMidimewNetwork \
	  PRZSquareMidimewNetwork \
	  SFSSimpleRouter \
	  SFSSimpleRouterFlow \
	  SFSSimpleRouterAdaptativeBurbleFlowMRVirtualMaxCredits \
	  TUTTrafficPatternJump
       
ifdef USE_SCT
MODULES +=	\
          PRZTrafficPatternTrace \
          PRZStringGz \
          PRZTrafficPatternTraceDualNet 
endif	

OBJS = $(addsuffix .o, $(MODULES)) 

# *************************************************************************

$(TARGET): PRZMain.o $(ATC_LIB)
	@echo
	@echo Building executable...
	$(LINKER) $(PATH_OBJ)/PRZMain.o -latcs $(LIBS)
	@echo
	@echo Good luck !
	@echo

$(ATC_DLL): $(OBJS) PRZMain.o
	@echo 
	@echo Building shared library...
	$(DLL_BUILD) $(addprefix $(PATH_OBJ), $(OBJS))
	@echo Building executable...
	$(LINKER) $(PATH_OBJ)/PRZMain.o $(ATC_DLL) $(LIBS)
	@echo
	@echo Good luck !
	@echo


ifdef IRIX
$(ATC_LIB): $(OBJS)  
	@echo 
	@echo Building static library...
	$(LIB_BUILD) $(addprefix $(PATH_OBJ)/,$(OBJS)) 
else
$(ATC_LIB): $(OBJS)  
	@echo 
	@echo Building static library...
	rm -f $(ATC_LIB)
	ar clq $(ATC_LIB) $(addprefix $(PATH_OBJ)/, $(OBJS))
	ranlib $(ATC_LIB)
endif
dll: $(ATC_DLL)

lib: $(TARGET)

depend:
	$(MAKE_DEPEND) $(addprefix $(PATH_SRC)/,$(addsuffix .cpp,$(MODULES))) > Makedepend
	
# *************************************************************************

include Makedepend

# *************************************************************************

%.o : $(PATH_SRC)/%.cpp
	$(COMPILER) -o $(PATH_OBJ)/$@ $<

# *************************************************************************

xsimul: $(ATC_LIB)
	cd ../xsimul/mak; rm xsimul; make;cp xsimul ../../mak;

# *************************************************************************

clean:
	rm -f -r $(PATH_OBJ)/ii_files
	rm -f $(PATH_OBJ)/*.o $(PATH_OBJ)/PRZMain.o
	rm -f $(TARGET)                  
	rm -f $(ATC_DLL)
	rm -f $(ATC_LIB)

# *************************************************************************


# fin del fichero
