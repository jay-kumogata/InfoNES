//keypad.h

#ifndef __KEYPAD__
#define __KEYPAD__

#define KEYA			1
#define KEYB			2
#define KEYSELECT		4
#define KEYSTART		8
#define KEYRIGHT		16
#define KEYLEFT			32
#define KEYUP			64
#define KEYDOWN			128
#define KEYR			256
#define KEYL			512

//This is already in gba.h, but it has a much less reader-friendly name (REG_P1)
#define KEYS             (*(volatile u16*)0x4000130)

#define KEY_DOWN(k)       ( ! ( ( KEYS ) & k ) )

#endif
