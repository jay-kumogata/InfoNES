/*===================================================================*/
/*                                                                   */
/* zinfones.cpp : A Qt-specific class implementation                 */
/*                                                                   */
/*  2004/06/26  InfoNES Project                                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include "zinfones.h"

/*-------------------------------------------------------------------*/
/*  Constructor                                                      */
/*-------------------------------------------------------------------*/
zinfones::zinfones( WORD *wf )
{
  /* FrameBuffer */
  QDirectPainter dq( this );
  pFb = (WORD *)dq.frameBuffer();

  pWf = wf;
  dx = dy = 0;

  /* To center */   
#if 0
  /* 1.5 magnification */
  setGeometry(320 - ( NES_DISP_WIDTH * 3 / 4 ), \
	      240 - ( NES_DISP_HEIGHT * 3 / 4 ), \
	      NES_DISP_WIDTH * 3 / 2, NES_DISP_HEIGHT * 3 / 2);
#else
  setGeometry(320 - ( NES_DISP_WIDTH / 2 ), \
	      240 - ( NES_DISP_HEIGHT / 2 ), \
	      NES_DISP_WIDTH, NES_DISP_HEIGHT);
#endif

}

/* Repaint */
void zinfones::paintEvent( QPaintEvent * e )
{
  /* To center */   
#if 0
  /* 1.5 magnification */
  setGeometry(320 - ( NES_DISP_WIDTH * 3 / 4 ), \
	      240 - ( NES_DISP_HEIGHT * 3 / 4 ), \
	      NES_DISP_WIDTH * 3 / 2, NES_DISP_HEIGHT * 3 / 2);
#else
  setGeometry(320 - ( NES_DISP_WIDTH / 2 ), \
	      240 - ( NES_DISP_HEIGHT / 2 ), \
	      NES_DISP_WIDTH, NES_DISP_HEIGHT);
#endif
}

/* Load Frame */
void zinfones::loadFrame()
{
  /* If not active, doesn't draw */
  if ( !isActiveWindow() )
    return;

  /* Draw a screen */ 
  for ( register unsigned int x = 0; x < NES_DISP_WIDTH; x++ )
  {
    for ( register unsigned int y = 0; y < NES_DISP_HEIGHT; y++ )
    {  
      /* Exchange 15-bit to 16-bit  */
      register WORD wColor = pWf[ ( y << 8 ) + x ];
      wColor = ((wColor& 0x7fe0)<<1)|(wColor&0x001f);	
#if 0
      /* 1.5 magnification */
      register int p = (((x+x+x)>>1)+dx) * 480 - (((y+y+y)>>1)+dy);

                        pFb[ p     ] = wColor;
      if (y&1)          pFb[ p-  1 ] = wColor;
      if (x&1)          pFb[ p+480 ] = wColor;
      if ((x&1)&&(y&1)) pFb[ p+479 ] = wColor;
#else
      /* 1 magnification */
      pFb[ (x + dx) * 480 - (y + dy) ] = wColor;
#endif
    }
  }
}

/* Key press */
void zinfones::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Key_Right:
    dwKeyPad1 |= (1<<7);
    break;
  case Key_Left:
    dwKeyPad1 |= (1<<6);
    break;
  case Key_Down:
    dwKeyPad1 |= (1<<5);
    break;
  case Key_Up:
    dwKeyPad1 |= (1<<4);
    break;
  case Key_S:
    dwKeyPad1 |= (1<<3);
    break;
  case Key_A:
    dwKeyPad1 |= (1<<2);
    break;
  case Key_Z:
    dwKeyPad1 |= (1<<1);
    break;
  case Key_X:
    dwKeyPad1 |= (1<<0);
    break;

  /* extra */	
  case Key_C:
    PPU_UpDown_Clip = ( PPU_UpDown_Clip ? 0 : 1);
    break;
  case Key_M:
    APU_Mute = ( APU_Mute ? 0 : 1 );
    break;
  case Key_D:
    FrameSkip = (FrameSkip == 0 ? 0 : FrameSkip - 1);
    break;
  case Key_U:
    FrameSkip++;
    break;
  }
}

/* Key release */
void zinfones::keyReleaseEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Key_Right:
    dwKeyPad1 &= ~(1<<7);
    break;
  case Key_Left:
    dwKeyPad1 &= ~(1<<6);
    break;
  case Key_Down:
    dwKeyPad1 &= ~(1<<5);
    break;
  case Key_Up:
    dwKeyPad1 &= ~(1<<4);
    break;
  case Key_S:
    dwKeyPad1 &= ~(1<<3);
    break;
  case Key_A:
    dwKeyPad1 &= ~(1<<2);
    break;
  case Key_Z:
    dwKeyPad1 &= ~(1<<1);
    break;
  case Key_X:
    dwKeyPad1 &= ~(1<<0);
    break;
  }
}

/* Move */
void zinfones::moveEvent( QMoveEvent *e )
{
  dx = e->pos().x();
  dy = e->pos().y();
}

/* End of zinfones.cpp */
