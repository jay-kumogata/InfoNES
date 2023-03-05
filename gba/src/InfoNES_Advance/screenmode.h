/****************************************
 screenmode.h 
 by Dovoto 
****************************************/

#ifndef SCREENMODE_H
#define SCREENMODE_H

///// REG_DISPCNT defines

#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

#define BACKBUFFER 0x10
#define H_BLANK_OAM 0x20

#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40

#define FORCE_BLANK 0x80

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200 
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000 
#define WIN1_ENABLE 0x2000 
#define WIN2_ENABLE 0x4000
#define WINOBJ_ENABLE 0x8000

//this define is not really necessary as it is defined in gba.h (in fact it will probably give you an error)
#define REG_DISPCNT * (u16 *)0x4000000 //define that address as a pointer and dereference it

///////SetMode Macro

#define SetMode(mode) REG_DISPCNT = (mode) 

#endif
