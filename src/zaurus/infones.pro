TEMPLATE	= app
CONFIG		= qt warn_on release
#DESTDIR		= $(QPEDIR)/bin
DESTDIR		= .
HEADERS		= ../InfoNES.h \
		  ../K6502.h \
		  ../K6502_rw.h	\
		  ../InfoNES_Mapper.h \
		  ../InfoNES_pApu.h \
		  ../InfoNES_System.h \
		  ../InfoNES_Types.h \
		  zinfones.h \
		  zfileselector.h
SOURCES		= InfoNES_System_Zaurus.cpp \
		  ../InfoNES.cpp \
		  ../K6502.cpp \
		  ../InfoNES_Mapper.cpp \
		  ../InfoNES_pApu.cpp \
                  zinfones.cpp \
		  zfileselector.cpp
INCLUDEPATH += $(QPEDIR)/include
DEPENDPATH	+= $(QPEDIR)/include
LIBS            += -lzdtm -lsl -lqpe -pthread 
INTERFACES	= 
TARGET		= InfoNES

TRANSLATIONS = ../i18n/de/infones.ts
