#**************************************************************************
#
# Fichero    : Makefile
#
# Comentarios: Fichero make para xsimul
#
# Autor      : Jose Miguel Prellezo Gutierrez
#
# $RCSfile: Makefile,v $
#
# $Date: 2001/10/08 05:43:08 $
#
# $Revision: 1.1.1.1 $
#
# $Log: Makefile,v $
# Revision 1.1.1.1  2001/10/08 05:43:08  vpuente
# Revision previa a SC2001
#
# Revision 1.1.1.1  2001/10/07 17:28:18  vpuente
# Nuevo repositorio en marrajo 
#
# Revision 1.1.1.1.4.2  2001/06/07 20:14:56  esteban
# Nuevo Makefile mejorado para QT
# Modificaciones para Qt2.0
#
# Revision 1.1.1.1.4.1  2001/06/04 13:59:54  esteban
# Stephen haciendo los deberes desde Alemania:
#   - Añadido BView y xbuffer
#   - Compatibilidad con Linux de xbuffer y xsimul
#   - Ejemplo del tutorial
#   - y alguna que otra cosilla...
#
# Revision 1.1.1.1  2000/11/18 17:43:27  esteban
# Al principio todo era un Caos...
#
#**************************************************************************
IRIX=Yes
ifdef IRIX
PATH_QT      = /usr/local/qt/
PATH_PRZ_INC = $(PATH_PRZ)/inc -I/usr/freeware/include
else
PATH_QT      = /usr/lib/qt2
PATH_PRZ_INC = $(PATH_PRZ)/inc 
endif
PATH_PRZ_LIB = $(PATH_PRZ)/mak -L../../lib
PATH_PRZ     = ../..
PATH_INC     = ../inc
PATH_SRC     = ../src
PATH_OBJ     = ./OBJS
TARGET       = xsimul

ifdef IRIX
CC = CC
CFLAGS = -n32 -c -O2 -woff 1174 -I$(PATH_QT)/include -I$(PATH_PRZ_INC) -I$(PATH_INC)
LFLAGS = -n32 -O2 -L$(PATH_QT)/lib -L$(PATH_PRZ_LIB)  
MOC      = $(PATH_QT)/bin/moc
else
CC = g++
CFLAGS = -c -O2 -I$(PATH_QT)/include -I$(PATH_PRZ_INC) -I$(PATH_INC)
LFLAGS = -O2 -L$(PATH_QT)/lib -L$(PATH_PRZ_LIB)  
MOC      = moc2
endif

MAKE_DEPEND = $(CC) -P -M -I$(PATH_INC) -I$(PATH_PRZ_INC) -I$(PATH_QT)/include
MOC      = $(PATH_QT)/bin/moc
COMPILER = $(CC) $(CFLAGS)
LINKER   = $(CC) $(LFLAGS)

####### Files

QTCLASSES =  przdlg1     \
             przdlg2     \
             xsimul

MODULES := $(QTCLASSES)
MODULES := $(MODULES) $(addsuffix .moc, $(QTCLASSES))
OBJS = $(addprefix $(PATH_OBJ)/,$(addsuffix .o, $(MODULES)))

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJS) $(PATH_PRZ)/mak/libatcs.a
	$(LINKER) -o $(TARGET) $(OBJS) -latcs -lqt -lm  -lz #-lrwtool

clean:
	rm -f $(PATH_OBJ)/*.o 
	rm -f $(TARGET)
	rm -f $(addprefix $(PATH_SRC)/,$(addsuffix .moc.cpp, $(QTCLASSES)))
	rm -f -r $(PATH_OBJ)/ii_files


#moc:
#	$(MOC) $(PATH_INC)/xsimul.hpp  > $(PATH_SRC)/mxsimul.cpp
#	$(MOC) $(PATH_INC)/przdlg1.hpp > $(PATH_SRC)/mprzdlg1.cpp
#	$(MOC) $(PATH_INC)/przdlg2.hpp > $(PATH_SRC)/mprzdlg2.cpp

depend: $(addprefix $(PATH_SRC)/,$(addsuffix .moc.cpp, $(QTCLASSES)))
	$(MAKE_DEPEND) $(addprefix $(PATH_SRC)/,$(addsuffix .cpp,$(MODULES))) > Makedepend

#**************************************************************************

include Makedepend

#**************************************************************************
$(PATH_SRC)/%.moc.cpp : $(PATH_INC)/%.hpp
	$(MOC) $< > $@

$(PATH_OBJ)/%.o : $(PATH_SRC)/%.cpp
	$(COMPILER) $< -o $@

# fin del fichero
