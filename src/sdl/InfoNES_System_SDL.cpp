/*
 *	InfoNES
 *		SDL ports by mata      03/04/19
 *              Modified by Jay        06/02/25
 * 
 * 	Start Date: 2003.04.19
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "../InfoNES.h"
#include "../InfoNES_System.h"
#include "../InfoNES_pAPU.h"

/*-------------------------------------------------------------------*/
/*  ROM image file information                                       */
/*-------------------------------------------------------------------*/

char szRomName[ 256 ];
char szSaveName[ 256 ];
int nSRAM_SaveFlag;

/*-------------------------------------------------------------------*/
/*  Constants ( Linux specific )                                     */
/*-------------------------------------------------------------------*/

#define VBOX_SIZE 7 
static const char * VERSION = "InfoNES v0.97J RC1";

/*-------------------------------------------------------------------*/
/*  Global Variables ( SDL specific )                                */
/*-------------------------------------------------------------------*/

bool quit = false;

/* for video */
SDL_Surface *screen;

/* For Sound Emulation */
SDL_AudioSpec audio_spec;
BYTE final_wave[2048];
int waveptr;
int wavflag;
int wavdone;

/* Pad state */
DWORD dwPad1;
DWORD dwPad2;
DWORD dwSystem;

/*-------------------------------------------------------------------*/
/*  Function prototypes ( SDL specific )                             */
/*-------------------------------------------------------------------*/

int start_application(char *filename);
void poll_event(void);
int LoadSRAM();
int SaveSRAM();

/* Palette data */ 
#if 0
WORD NesPalette[ 64 ] =
{
  0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
  0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
  0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
  0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
  0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
  0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
  0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000
};
#else
WORD NesPalette[ 64 ];
#endif

BYTE NesPaletteRGB[64][3] = {
   112, 112, 112,     32,  24, 136,      0,   0, 168,     64,   0, 152,
   136,   0, 112,    168,   0,  16,    160,   0,   0,    120,   8,   0,
    64,  40,   0,      0,  64,   0,      0,  80,   0,      0,  56,  16,
    24,  56,  88,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   184, 184, 184,      0, 112, 232,     32,  56, 232,    128,   0, 240,
   184,   0, 184,    224,   0,  88,    216,  40,   0,    200,  72,   8,
   136, 112,   0,      0, 144,   0,      0, 168,   0,      0, 144,  56,
     0, 128, 136,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   248, 248, 248,     56, 184, 248,     88, 144, 248,     64, 136, 248,
   240, 120, 248,    248, 112, 176,    248, 112,  96,    248, 152,  56,
   240, 184,  56,    128, 208,  16,     72, 216,  72,     88, 248, 152,
     0, 232, 216,      0,   0,   0,      0,   0,   0,      0,   0,   0,
   248, 248, 248,    168, 224, 248,    192, 208, 248,    208, 200, 248,
   248, 192, 248,    248, 192, 216,    248, 184, 176,    248, 216, 168,
   248, 224, 160,    224, 248, 160,    168, 240, 184,    176, 248, 200,
   152, 248, 240,      0,   0,   0,      0,   0,   0,      0,   0,   0,
};

/*===================================================================*/
/*                                                                   */
/*                main() : Application main                          */
/*                                                                   */
/*===================================================================*/

/* Application main */
int main(int argc, char **argv)
{
  unsigned int 	i,vmode;

  /* Screen initialization */
  if (SDL_Init(SDL_INIT_VIDEO|
	       SDL_INIT_AUDIO|
	       SDL_INIT_JOYSTICK))
  {
    /* Failure initialization of SDL */
    fprintf(stderr, "SDL_Init() failed.\n");
    return 1;
  }
  vmode = SDL_HWPALETTE | SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
  screen = SDL_SetVideoMode(NES_DISP_WIDTH+VBOX_SIZE,
			    NES_DISP_HEIGHT+VBOX_SIZE,8,vmode);
  if(!screen)
  {
    fprintf(stderr, "SDL_SetVideoMode() failed.\n");
    return 1;
  }
  SDL_ShowCursor(0);
  SDL_WM_SetCaption(VERSION,VERSION);

  if(SDL_NumJoysticks() > 0)
  {
    SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);
  }

  for(i=0; i<64; ++i)
  {
    NesPalette[i] = SDL_MapRGB(screen->format,
			       NesPaletteRGB[i][0],
			       NesPaletteRGB[i][1],
			       NesPaletteRGB[i][2]);
  }
	
  /* If a rom name specified, start it */
  if (argc>=2) 
  {
    if ( start_application(argv[1]) )
    {
      /* MainLoop */
      InfoNES_Main();
      
      /* End */
      SaveSRAM();
    }
    else
    {
      /* Not a NES format file */
      fprintf(stderr, "%s isn't a NES format file.\n", argv[1] );
    }
  }
  else
  {
    /* Help Message */
    fprintf(stderr, "Copyright (C) 1998-2006 Jay's Factory, ");
    fprintf(stderr, "SDL Ports by mata.\n");
    fprintf(stderr, "%s. Usage: %s [NES format file]\n", VERSION, argv[0] );
  }
	    
  /* Finalization of SDL*/
  SDL_FreeSurface(screen);    
  SDL_Quit();
}

/* Start application */
int start_application(char *filename)
{
  /* Set a ROM image name */
  strcpy( szRomName, filename );

  /* Load cassette */
  if(InfoNES_Load(szRomName)==0) { 
    /* Load SRAM */
    LoadSRAM();

    /* Success */
    return 1;
  }
  /* Failure */
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           LoadSRAM() : Load a SRAM                                */
/*                                                                   */
/*===================================================================*/
int LoadSRAM()
{
/*
 *  Load a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be read
 */

  FILE *fp;
  unsigned char pSrcBuf[ SRAM_SIZE ];
  unsigned char chData;
  unsigned char chTag;
  int nRunLen;
  int nDecoded;
  int nDecLen;
  int nIdx;

  // It doesn't need to save it
  nSRAM_SaveFlag = 0;

  // It is finished if the ROM doesn't have SRAM
  if ( !ROM_SRAM )
    return 0;

  // There is necessity to save it
  nSRAM_SaveFlag = 1;

  // The preparation of the SRAM file name
  strcpy( szSaveName, szRomName );
  strcpy( strrchr( szSaveName, '.' ) + 1, "srm" );

  /*-------------------------------------------------------------------*/
  /*  Read a SRAM data                                                 */
  /*-------------------------------------------------------------------*/

  // Open SRAM file
  fp = fopen( szSaveName, "rb" );
  if ( fp == NULL )
    return -1;

  // Read SRAM data
  fread( pSrcBuf, SRAM_SIZE, 1, fp );

  // Close SRAM file
  fclose( fp );

  /*-------------------------------------------------------------------*/
  /*  Extract a SRAM data                                              */
  /*-------------------------------------------------------------------*/

  nDecoded = 0;
  nDecLen = 0;

  chTag = pSrcBuf[ nDecoded++ ];

  while ( nDecLen < 8192 )
  {
    chData = pSrcBuf[ nDecoded++ ];

    if ( chData == chTag )
    {
      chData = pSrcBuf[ nDecoded++ ];
      nRunLen = pSrcBuf[ nDecoded++ ];
      for ( nIdx = 0; nIdx < nRunLen + 1; ++nIdx )
      {
        SRAM[ nDecLen++ ] = chData;
      }
    }
    else
    {
      SRAM[ nDecLen++ ] = chData;
    }
  }

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*           SaveSRAM() : Save a SRAM                                */
/*                                                                   */
/*===================================================================*/
int SaveSRAM()
{
/*
 *  Save a SRAM
 *
 *  Return values
 *     0 : Normally
 *    -1 : SRAM data couldn't be written
 */

  FILE *fp;
  int nUsedTable[ 256 ];
  unsigned char chData;
  unsigned char chPrevData;
  unsigned char chTag;
  int nIdx;
  int nEncoded;
  int nEncLen;
  int nRunLen;
  unsigned char pDstBuf[ SRAM_SIZE ];

  if ( !nSRAM_SaveFlag )
    return 0;  // It doesn't need to save it

  /*-------------------------------------------------------------------*/
  /*  Compress a SRAM data                                             */
  /*-------------------------------------------------------------------*/

  memset( nUsedTable, 0, sizeof nUsedTable );

  for ( nIdx = 0; nIdx < SRAM_SIZE; ++nIdx )
  {
    ++nUsedTable[ SRAM[ nIdx++ ] ];
  }
  for ( nIdx = 1, chTag = 0; nIdx < 256; ++nIdx )
  {
    if ( nUsedTable[ nIdx ] < nUsedTable[ chTag ] )
      chTag = nIdx;
  }

  nEncoded = 0;
  nEncLen = 0;
  nRunLen = 1;

  pDstBuf[ nEncLen++ ] = chTag;

  chPrevData = SRAM[ nEncoded++ ];

  while ( nEncoded < SRAM_SIZE && nEncLen < SRAM_SIZE - 133 )
  {
    chData = SRAM[ nEncoded++ ];

    if ( chPrevData == chData && nRunLen < 256 )
      ++nRunLen;
    else
    {
      if ( nRunLen >= 4 || chPrevData == chTag )
      {
        pDstBuf[ nEncLen++ ] = chTag;
        pDstBuf[ nEncLen++ ] = chPrevData;
        pDstBuf[ nEncLen++ ] = nRunLen - 1;
      }
      else
      {
        for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
          pDstBuf[ nEncLen++ ] = chPrevData;
      }

      chPrevData = chData;
      nRunLen = 1;
    }

  }
  if ( nRunLen >= 4 || chPrevData == chTag )
  {
    pDstBuf[ nEncLen++ ] = chTag;
    pDstBuf[ nEncLen++ ] = chPrevData;
    pDstBuf[ nEncLen++ ] = nRunLen - 1;
  }
  else
  {
    for ( nIdx = 0; nIdx < nRunLen; ++nIdx )
      pDstBuf[ nEncLen++ ] = chPrevData;
  }

  /*-------------------------------------------------------------------*/
  /*  Write a SRAM data                                                */
  /*-------------------------------------------------------------------*/

  // Open SRAM file
  fp = fopen( szSaveName, "wb" );
  if ( fp == NULL )
    return -1;

  // Write SRAM data
  fwrite( pDstBuf, nEncLen, 1, fp );

  // Close SRAM file
  fclose( fp );

  // Successful
  return 0;
}

/*===================================================================*/
/*                                                                   */
/*                  InfoNES_Menu() : Menu screen                     */
/*                                                                   */
/*===================================================================*/
int InfoNES_Menu(){
  if(quit) return -1;
  return 0;
}

/* Read ROM image file */
int InfoNES_ReadRom( const char *pszFileName ){
  FILE *fp;

  /* Open ROM file */
  fp=fopen(pszFileName,"rb");
  if(fp==NULL) return -1;

  /* Read ROM Header */
  fread( &NesHeader, sizeof NesHeader, 1, fp );
  if( memcmp( NesHeader.byID, "NES\x1a", 4 )!=0){
    /* not .nes file */
    fclose( fp );
    return -1;
  }

  /* Clear SRAM */
  memset( SRAM, 0, SRAM_SIZE );

  /* If trainer presents Read Triner at 0x7000-0x71ff */
  if(NesHeader.byInfo1 & 4){
    fread( &SRAM[ 0x1000 ], 512, 1, fp );
  }

  /* Allocate Memory for ROM Image */
  ROM = (BYTE *)malloc( NesHeader.byRomSize * 0x4000 );

  /* Read ROM Image */
  fread( ROM, 0x4000, NesHeader.byRomSize, fp );

  if(NesHeader.byVRomSize>0){
    /* Allocate Memory for VROM Image */
    VROM = (BYTE *)malloc( NesHeader.byVRomSize * 0x2000 );

    /* Read VROM Image */
    fread( VROM, 0x2000, NesHeader.byVRomSize, fp );
  }

  /* File close */
  fclose( fp );

  /* Successful */
  return 0;

}

/* Release a memory for ROM */
void InfoNES_ReleaseRom(){
  if(ROM){
    free(ROM);
    ROM=NULL;
  }

  if(VROM){
    free(VROM);
    VROM=NULL;
  }
}

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame(){
  int	x, y;
  BYTE *p,*pl;
  WORD *pw;	

  SDL_LockSurface(screen);
  pl=(BYTE *)screen->pixels; pw = WorkFrame;
  for(y=0; y<NES_DISP_HEIGHT; y++){
    p=pl;
    for(x=0; x<NES_DISP_WIDTH; x++){  
      *p++ = (BYTE)*pw++;
    }
    pl+=screen->pitch; pw+=(0x100-NES_DISP_WIDTH);
  }
  SDL_UnlockSurface(screen);	 
  SDL_Flip(screen);
}

/* Get a joypad state */
void InfoNES_PadState( DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem )
{
  poll_event();
  *pdwPad1 = dwPad1;
  *pdwPad2 = dwPad2;
  *pdwSystem = dwSystem;
}

static const int joy_commit_range = 3276;

void poll_event(void)
{
  SDL_Event	e;
  static signed char	x_joy=0, y_joy=0;
		
  while(SDL_PollEvent(&e))
  {
    switch(e.type)
    {
    case SDL_QUIT:
      dwSystem|=PAD_SYS_QUIT;
      quit=1;
      break;
      
    case SDL_KEYDOWN:
      switch(e.key.keysym.sym)
      {
      case SDLK_RETURN:
	if( !(e.key.keysym.mod & KMOD_ALT)) break;
	SDL_WM_ToggleFullScreen(screen);
	break;

      case SDLK_ESCAPE:
	dwSystem|=PAD_SYS_QUIT;
	quit=1;
	break;
	
      case SDLK_RIGHT:   dwPad1 |= (1<<7);break;
      case SDLK_LEFT:    dwPad1 |= (1<<6);break;
      case SDLK_DOWN:    dwPad1 |= (1<<5);break;
      case SDLK_UP:      dwPad1 |= (1<<4);break;
      case SDLK_s:	 dwPad1 |= (1<<3);break;    /* Start */
      case SDLK_a:	 dwPad1 |= (1<<2);break;    /* Select */
      case SDLK_z:	 dwPad1 |= (1<<1);break;    /* 'B' */
      case SDLK_x:	 dwPad1 |= (1<<0);break;    /* 'A' */
      case SDLK_m:       
	/* Toggle of sound mute */	
	APU_Mute = ( APU_Mute ? 0 : 1 );break;      
      case SDLK_c:
	/* Toggle up and down clipping */   
	PPU_UpDown_Clip = ( PPU_UpDown_Clip ? 0 : 1 ); break;
      } /* keydown */
      break;		

    case SDL_KEYUP:
      switch(e.key.keysym.sym)
      {
      case SDLK_RIGHT:   dwPad1 &=~(1<<7);break;
      case SDLK_LEFT:    dwPad1 &=~(1<<6);break;
      case SDLK_DOWN:	 dwPad1 &=~(1<<5);break;
      case SDLK_UP:	 dwPad1 &=~(1<<4);break;
      case SDLK_s:	 dwPad1 &=~(1<<3);break;   /* Start */
      case SDLK_a:	 dwPad1 &=~(1<<2);break;   /* Select */
      case SDLK_z:	 dwPad1 &=~(1<<1);break;   /* 'B' */
      case SDLK_x:	 dwPad1 &=~(1<<0);break;   /* 'A' */
      } /* keyup */
      break;
      
    case SDL_JOYAXISMOTION:
      switch(e.jaxis.axis){
      case 0:	/* X axis */
	if(e.jaxis.value >  joy_commit_range){
	  if(x_joy > 0) break;	
	  if(x_joy < 0) dwPad1 &=~(1<<6);
	  dwPad1 |= (1<<7); x_joy=+1; break; }
	if(e.jaxis.value < -joy_commit_range){
	  if(x_joy < 0) break;
	  if(x_joy > 0) dwPad1 &=~(1<<7);
	  dwPad1 |= (1<<6); x_joy=-1; break; }
	if     (x_joy < 0) dwPad1 &=~(1<<6);
	else if(x_joy > 0) dwPad1 &=~(1<<7);
	x_joy= 0; break;
      case 1: /* Y asis */
	if(e.jaxis.value >  joy_commit_range){
	  if(y_joy > 0) break;
	  if(y_joy < 0) dwPad1 &=~(1<<4);
	  dwPad1 |= (1<<5); y_joy=+1; break; }
	if(e.jaxis.value < -joy_commit_range){
	  if(y_joy < 0) break;
	  if(y_joy > 0) dwPad1 &=~(1<<5);
	  dwPad1 |= (1<<4); y_joy=-1; break; }
	if      (y_joy < 0) dwPad1 &=~(1<<4);
	else if (y_joy > 0) dwPad1 &=~(1<<5);
	y_joy= 0; break;
      } /* joysxismotion */

    case SDL_JOYBUTTONUP:
      switch(e.jbutton.button){
      case 2: dwPad1 &=~(1<<0);break; /* A */
      case 1: dwPad1 &=~(1<<1);break; /* B */
      case 8: dwPad1 &=~(1<<2);break; /* select */
      case 9: dwPad1 &=~(1<<3);break; /* start */
      } break;	
    case SDL_JOYBUTTONDOWN:
      switch(e.jbutton.button){
      case 2: dwPad1 |= (1<<0);break; /* A */
      case 1: dwPad1 |= (1<<1);break; /* B */
      case 8: dwPad1 |= (1<<2);break; /* select */
      case 9: dwPad1 |= (1<<3);break; /* start */
      } break;
    }
  }
}

/* memcpy */
void *InfoNES_MemoryCopy( void *dest, const void *src, int count ){
  memcpy( dest, src, count );
  return dest;
}

/* memset */
void *InfoNES_MemorySet( void *dest, int c, int count ){
  memset( dest, c, count);  
  return dest;
}

/* Print debug message */
void InfoNES_DebugPrint( char *pszMsg ) {
  fprintf(stderr,"%s\n", pszMsg);
}

/* Wait */
void InfoNES_Wait(){}

/* Sound Initialize */
void InfoNES_SoundInit( void ){}

void waveout(void *udat,BYTE *stream,int len){
  if ( !wavdone )
  {
    /* we always expect that len is 1024 */
    memcpy( stream, &final_wave[(wavflag - 1) << 10], len );
    wavflag = 0; wavdone = 1;
  }
}

/* Sound Open */
int InfoNES_SoundOpen( int samples_per_sync, int sample_rate ){
  SDL_AudioSpec asp;

  asp.freq=sample_rate;
  asp.format=AUDIO_U8;
  asp.channels=1;
  asp.samples=1024; 
  asp.callback=waveout;
		
  if(SDL_OpenAudio(&asp,&audio_spec)<0){
    fprintf(stderr,"Can't Open SDL Audio\n");
    return -1;
  } 
  waveptr = wavflag = 0; wavdone = 1;
  SDL_PauseAudio(0);
	
  /* Successful */
  return 1;
}

/* Sound Close */
void InfoNES_SoundClose( void ){
  SDL_CloseAudio();
}
  
/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise. 1 DPCM */
void InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5){
  int i;

  for (i = 0; i < samples; i++)
  {
    final_wave[waveptr] = 
      ( wave1[i] + wave2[i] + wave3[i] + wave4[i] + wave5[i] ) / 5; 

    waveptr++;
    if ( waveptr == 2048 ) 
    {
      waveptr = 0;
      wavflag = 2; wavdone=0;
    }
    else if ( waveptr == 1024 )
    {
      wavflag = 1; wavdone=0;
    }
    while (!wavdone) SDL_Delay(0);
  }
}

/* Print system message */
void InfoNES_MessageBox(char *pszMsg, ...){}

/* End of InfoNES_System_SDL.cpp */
