#////////////////////////////////////////////////////////////////////////////#
#/ Makefile                                                                 /#
#////////////////////////////////////////////////////////////////////////////#

MYLIB=/dev/gba/devkitadv/Mylib
AGBINC=/dev/gba/devkitadv/NyLib
AGBLIB=/dev/gba/devkitadv/NyLib

#////////////////////////////////////////////////////////////////////////////#
#/ Title                                                                    /#
#////////////////////////////////////////////////////////////////////////////#
NAME		= InfoNES
TITLE		= InfoNES
GAMECODE	= Ifns
MAKERCODE	= DF
MULTIBOOT	= 1
BIN_PREFIX	= bin_
DEBUG		= 0

#////////////////////////////////////////////////////////////////////////////#
#/ Project settings                                                         /#
#////////////////////////////////////////////////////////////////////////////#
BINS		= # $(wildcard res/*.bin)
OBJS		= InfoNES.o InfoNES_Mapper.o K6502.o InfoNES_System_GBA.o  # InfoNES_pAPU.o
LIBS		= stdc++ c agbsyscall debug

#////////////////////////////////////////////////////////////////////////////#
#/ Includes                                                                 /#
#////////////////////////////////////////////////////////////////////////////#
include		$(MYLIB)/Makefile.1
