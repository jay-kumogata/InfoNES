/*===================================================================*/
/*                                                                   */
/*  K6502.h : Header file for K6502                                  */
/*                                                                   */
/*  2002/04/01  InfoNES GBA Project                                  */
/*  1999/10/19  Racoon  New preparation                              */
/*                                                                   */
/*===================================================================*/

#ifndef K6502_H_INCLUDED
#define K6502_H_INCLUDED

// Type definition
#ifndef DWORD
typedef unsigned long  DWORD;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef BYTE
typedef unsigned char  BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif

/* 6502 Flags */
#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_R 0x20
#define FLAG_V 0x40
#define FLAG_N 0x80

/* Stack Address */
#define BASE_STACK   0x100

/* Interrupt Vectors */
#define VECTOR_NMI   0xfffa
#define VECTOR_RESET 0xfffc
#define VECTOR_IRQ   0xfffe

// NMI Request
#define NMI_REQ  NMI_State = 0;

// IRQ Request
#define IRQ_REQ  IRQ_State = 0;

// Emulator Operation
void K6502_Init();
void K6502_Reset();
void K6502_Set_Int_Wiring( BYTE byNMI_Wiring, BYTE byIRQ_Wiring );
void K6502_Step( register WORD wClocks );

// I/O Operation (User definition)
static inline BYTE K6502_Read( WORD wAddr);
static inline WORD K6502_ReadW( WORD wAddr );
static inline BYTE K6502_ReadZp( BYTE byAddr );
static inline WORD K6502_ReadZpW( BYTE byAddr );
static inline BYTE K6502_ReadAbsX();
static inline BYTE K6502_ReadAbsY();
static inline BYTE K6502_ReadIY();

static inline void K6502_Write( WORD wAddr, BYTE byData );
static inline void K6502_WriteW( WORD wAddr, WORD wData );

// I/O Operation Macro
#define K6502_ReadSp(a)				RAM[ BASE_STACK + (a) ]
#define K6502_WriteSp(a,d)		RAM[ BASE_STACK + (a) ] = (d)
#define K6502_WriteZp(a,d)		RAM[ (a) ] = (d)

// The state of the IRQ pin
extern BYTE IRQ_State;

// The state of the NMI pin
extern BYTE NMI_State;

#endif /* !K6502_H_INCLUDED */
