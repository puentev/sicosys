PATH_QT    = ../../../qt
PATH_ROOT  = ../
PATH_INC   = $(PATH_ROOT)/inc
PATH_SRC   = $(PATH_ROOT)/src
CFLAGS     = -n32 -c -O2 -woff all -I$(PATH_QT)/include -I$(PATH_INC)
LFLAGS     = -n32 -woff all -L$(PATH_QT)/lib

MOC        = $(PATH_QT)/bin/moc
COMPILER   = CC $(CFLAGS)
LINKER     = CC $(LFLAGS)

####### Files

OBJS    =   xbuffer.o   mxbuffer.o \
            xbview.o    mxbview.o  \
            xdlg1.o     mxdlg1.o

TARGET	=	xbuffer


####### Build rules

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINKER) -o $(TARGET) $(OBJS) -lqt
	

xbuffer.o: $(PATH_SRC)/xbuffer.cpp $(PATH_INC)/xbuffer.hpp
	$(COMPILER) $(PATH_SRC)/xbuffer.cpp 

mxbuffer.o: $(PATH_SRC)/mxbuffer.cpp $(PATH_INC)/xbuffer.hpp
	$(COMPILER) $(PATH_SRC)/mxbuffer.cpp 

xbview.o: $(PATH_SRC)/xbview.cpp $(PATH_INC)/xbview.hpp
	$(COMPILER) $(PATH_SRC)/xbview.cpp 

mxbview.o: $(PATH_SRC)/mxbview.cpp $(PATH_INC)/xbview.hpp
	$(COMPILER) $(PATH_SRC)/mxbview.cpp 

xdlg1.o: $(PATH_SRC)/xdlg1.cpp $(PATH_INC)/xdlg1.hpp
	$(COMPILER) $(PATH_SRC)/xdlg1.cpp 

mxdlg1.o: $(PATH_SRC)/mxdlg1.cpp $(PATH_INC)/xdlg1.hpp
	$(COMPILER) $(PATH_SRC)/mxdlg1.cpp 

clean:
	rm -f *.o *.bak *~ *% #*
	rm -f $(TARGET)
	rm -f -r ii_files
	
moc:
	$(MOC) $(PATH_INC)/xbuffer.hpp > $(PATH_SRC)/mxbuffer.cpp
	$(MOC) $(PATH_INC)/xbview.hpp  > $(PATH_SRC)/mxbview.cpp
	$(MOC) $(PATH_INC)/xdlg1.hpp   > $(PATH_SRC)/mxdlg1.cpp
install:
	mv $(TARGET) ../../../bin
	rm -f *.o *.bak *~ *% #*
	rm -f $(TARGET)
	rm -f -r ii_files
# fin del fichero
