/*===================================================================*/
/*                                                                   */
/*  InfoNES_System.h : The function which depends on a system        */
/*                                                                   */
/*  2002/04/01  InfoNES GBA Project                                  */
/*  1999/11/03  Racoon  New preparation                              */
/*                                                                   */
/*===================================================================*/

#ifndef INFONES_SYSTEM_H_INCLUDED
#define INFONES_SYSTEM_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include "InfoNES_Types.h"

/*-------------------------------------------------------------------*/
/*  Palette data                                                     */
/*-------------------------------------------------------------------*/
extern const WORD NesPalette[];

/*-------------------------------------------------------------------*/
/*  Constants ( GBA specific )                                       */
/*-------------------------------------------------------------------*/

/* GBA display size */
#define GBA_DISP_WIDTH      240
#define GBA_DISP_HEIGHT     160

/* Mode 4 specific*/
#define FrontBuffer (u16*)0x6000000
#define BackBuffer (u16*)0x600A000 //start of back buffer in Mode 4 

/* For GCC */
#define IN_EWRAM __attribute__ ((section (".text")))
#define CODE_IN_IWRAM __attribute__ ((section (".bss"), long_call))

/*-------------------------------------------------------------------*/
/*  Variables ( GBA specific )                                       */
/*-------------------------------------------------------------------*/

/* Pointer to the current back buffer */
extern WORD* videoBuffer;

/*-------------------------------------------------------------------*/
/*  Function prototypes                                              */
/*-------------------------------------------------------------------*/

/* Menu screen */
int InfoNES_Menu();

/* Read ROM image file */
int InfoNES_ReadRom( const char *pszFileName );

/* Release a memory for ROM */
void InfoNES_ReleaseRom();

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame();

/* Get a joypad state */
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem );

/* memcpy */
void *InfoNES_MemoryCopy( void *dest, const void *src, int count );

/* memset */
void *InfoNES_MemorySet( void *dest, int c, int count );

/* Print debug message */
void InfoNES_DebugPrint( const char *pszMsg );

/* Print debug message */
void InfoNES_Debug();

#endif /* !INFONES_SYSTEM_H_INCLUDED */
