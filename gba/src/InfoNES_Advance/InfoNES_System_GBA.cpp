/*===================================================================*/
/*                                                                   */
/*  InfoNES_System_GBA.cpp : GBA specific File                       */
/*                                                                   */
/*  2002/04/01  InfoNES GBA Project                                  */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include <GBA.h>
#include <string.h>

#include "InfoNES.h"
#include "InfoNES_System.h"

#include "screenmode.h"
#include "keypad.h"

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

char szRomName[ 256 ];
char szSaveName[ 256 ];
int nSRAM_SaveFlag;

/*-------------------------------------------------------------------*/
/*  Function prototypes ( GBA specific )                             */
/*-------------------------------------------------------------------*/

#if 0
int LoadSRAM();
int SaveSRAM();
#endif

/*-------------------------------------------------------------------*/
/*  Variables ( GBA specific )                                       */
/*-------------------------------------------------------------------*/

/* Mode 4 specific */
WORD* videoBuffer;
WORD* paletteMem = (WORD*)0x5000000;

void PlotPixel(int x,int y, unsigned short int c) {videoBuffer[(y) * 120 + (x)] = (c);}

/* Flip back and front buffers */
void Flip(void)
{
	if (REG_DISPCNT & BACKBUFFER) //back buffer is the current buffer so we need to switch it to the font buffer
  { 
		REG_DISPCNT &= ~BACKBUFFER; //flip active buffer to front buffer by clearing back buffer bit
		videoBuffer = BackBuffer; //now we point our drawing buffer to the back buffer
	}
	else //front buffer is active so switch it to backbuffer
	{              
		REG_DISPCNT |= BACKBUFFER; //flip active buffer to back buffer by setting back buffer bit
		videoBuffer = FrontBuffer; //now we point our drawing buffer to the front buffer      
	}
}

/*-------------------------------------------------------------------*/
/*  Constants ( GBA specific )                                       */
/*-------------------------------------------------------------------*/

/* Palette data */
const WORD NesPalette[ 64 ] =
{
  0x39ce, 0x4464, 0x5400, 0x4c08, 0x3811, 0x0815, 0x0014, 0x002f,
  0x00a8, 0x0100, 0x0140, 0x08e0, 0x2ce3, 0x0000, 0x0000, 0x0000,
  0x5ef7, 0x75c0, 0x74e4, 0x7810, 0x5c17, 0x2c1c, 0x00bb, 0x0539,
  0x01d1, 0x0240, 0x02a0, 0x1e40, 0x4600, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x7ee7, 0x7e4b, 0x7e28, 0x7dfe, 0x59df, 0x31df, 0x1e7f,
  0x1efe, 0x0b50, 0x2769, 0x4feb, 0x6fa0, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x7f95, 0x7f58, 0x7f3a, 0x7f1f, 0x6f1f, 0x5aff, 0x577f,
  0x539f, 0x53fc, 0x5fd5, 0x67f6, 0x7bf3, 0x0000, 0x0000, 0x0000,
};

/* Rom image file system */
#include "Null_rom.cpp"

/*===================================================================*/
/*                                                                   */
/*                main() : Application main                          */
/*                                                                   */
/*===================================================================*/

int main()
{
  /*-------------------------------------------------------------------*/
  /*  Initialize                                                       */
  /*-------------------------------------------------------------------*/

	/* Set the display mode */
  SetMode( MODE_4 | BG2_ENABLE ); 

	/* Store the palette */ 
	for ( int i = 0; i < 64; i++ )
  {
		paletteMem[ i        ] = NesPalette[ i ]; 	
    paletteMem[ i + 0x80 ] = NesPalette[ i ];
  }

	/* Set up the back buffer */
	Flip();

  /*-------------------------------------------------------------------*/
  /*  Load ROM image                                                   */
  /*-------------------------------------------------------------------*/

  // Load cassette
  if ( InfoNES_Load( NULL ) == 0 )
  {	
#if 0
	  // Load SRAM
    LoadSRAM();
#endif

    // Create Emulation Thread
    InfoNES_Main();

  }  
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
int InfoNES_Menu()
{
/*
 *  Menu screen
 *
 *  Return values
 *     0 : Normally
 *    -1 : Exit InfoNES
 */

  /* Nothing to do here */
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*               InfoNES_ReadRom() : Read ROM image file             */
/*                                                                   */
/*===================================================================*/
int InfoNES_ReadRom( const char *pszFileName )
{
/*
 *  Read ROM image file
 *
 *  Parameters
 *    const char *pszFileName          (Read)
 *
 *  Return values
 *     0 : Normally
 *    -1 : Error
 */

  BYTE *pCursor = (BYTE *)szRomImage;

  /* Read ROM Header */
  memcpy( &NesHeader, pCursor, sizeof NesHeader );
  pCursor += sizeof NesHeader;

  if ( memcmp( NesHeader.byID, "NES\x1a", 4 ) != 0 )
  {
    /* not .nes file */
    return -1;
  }
  
  /* Clear SRAM */
  memset( SRAM, 0, SRAM_SIZE );

  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if ( NesHeader.byInfo1 & 4 )
  {
    memcpy( &SRAM[ 0x1000 ], pCursor, 512 );
    pCursor += 512;
  }

  /* Allocate Memory for ROM Image */
  ROM = pCursor;
  pCursor += NesHeader.byRomSize * 0x4000;

  if ( NesHeader.byVRomSize > 0 )
  {
    /* Allocate Memory for VROM Image */
    VROM = pCursor;
    pCursor += NesHeader.byVRomSize * 0x2000;
  }
  
  /* Successful */
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           InfoNES_ReleaseRom() : Release a memory for ROM         */
/*                                                                   */
/*===================================================================*/
void InfoNES_ReleaseRom()
{
/*
 *  Release a memory for ROM
 *
 */

  if ( ROM )
  {
    ROM = NULL;
  }

  if ( VROM )
  {
    VROM = NULL;
  }
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemoryCopy() : memcpy                         */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemoryCopy( void *dest, const void *src, int count )
{
/*
 *  memcpy
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the copied block's destination
 *
 *    const void *src                  (Read)
 *      Points to the starting address of the block of memory to copy
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to copy
 *
 *  Return values
 *    Pointer of destination
 */

#if 0
  memcpy( dest, src, count );
  CpuFastCopy( src, dest, count );
#else
	DmaCopy( 0, (char *)src, (char *)dest, count, 32 );
#endif
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_MemorySet() : memset                          */
/*                                                                   */
/*===================================================================*/
void *InfoNES_MemorySet( void *dest, int c, int count )
{
/*
 *  memset
 *
 *  Parameters
 *    void *dest                       (Write)
 *      Points to the starting address of the block of memory to fill
 *
 *    int c                            (Read)
 *      Specifies the byte value with which to fill the memory block
 *
 *    int count                        (Read)
 *      Specifies the size, in bytes, of the block of memory to fill
 *
 *  Return values
 *    Pointer of destination
 */

#if 0
  memset( dest, c, count);  
  CpuFastClear( c, dest, count );
#else
  DmaClear( 0, c, (char *)dest, count, 32 );
#endif 
  return dest;
}

/*===================================================================*/
/*                                                                   */
/*      InfoNES_LoadFrame() :                                        */
/*           Transfer the contents of work frame on the screen       */
/*                                                                   */
/*===================================================================*/
void InfoNES_LoadFrame()
{
/*
 *  Transfer the contents of work frame on the screen
 *
 */
	
	/* Flip the back and the front buffer */
	Flip();
}

/*===================================================================*/
/*                                                                   */
/*             InfoNES_PadState() : Get a joypad state               */
/*                                                                   */
/*===================================================================*/
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
/*
 *  Get a joypad state
 *
 *  Parameters
 *    DWORD *pdwPad1                   (Write)
 *      Joypad 1 State
 *
 *    DWORD *pdwPad2                   (Write)
 *      Joypad 2 State
 *
 *    DWORD *pdwSystem                 (Write)
 *      Input for InfoNES
 *
 */

  /* Transfer joypad state */
  *pdwPad1 = 
      ( KEY_DOWN( KEYA )      << 0 )     | // A 
			( KEY_DOWN( KEYB )      << 1 )     | // B 
			( KEY_DOWN( KEYSELECT ) << 2 )     | // Select
			( KEY_DOWN( KEYSTART )  << 3 )     | // Start
			( KEY_DOWN( KEYUP )     << 4 )     | // Up
			( KEY_DOWN( KEYDOWN )   << 5 )     | // Down 
			( KEY_DOWN( KEYLEFT )   << 6 )     | // Left
			( KEY_DOWN( KEYRIGHT )  << 7 );      // Right

	*pdwPad2   = 0;
	*pdwSystem = 0;
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundInit() : Sound Emulation Initialize           */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundInit( void ) 
{

}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundOpen() : Sound Open                           */
/*                                                                   */
/*===================================================================*/
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate ) 
{

  /* Successful */
  return 1;
}

/*===================================================================*/
/*                                                                   */
/*        InfoNES_SoundClose() : Sound Close                         */
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundClose( void ) 
{

}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_SoundOutput4() : Sound Output 4 Waves          */           
/*                                                                   */
/*===================================================================*/
void InfoNES_SoundOutput4( int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4 ) 
{
 
}

/*===================================================================*/
/*                                                                   */
/*            InfoNES_Wait() : Wait Emulation if required            */
/*                                                                   */
/*===================================================================*/
void InfoNES_Wait() 
{

}

/*
 * End of InfoNES_System_GBA.cpp
 */
