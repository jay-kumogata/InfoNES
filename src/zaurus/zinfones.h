/*===================================================================*/
/*                                                                   */
/* zinfones.h : A Qt-specific class implementation                   */
/*                                                                   */
/*  2004/06/26  InfoNES Project                                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#ifndef ZINFONES_H
#define ZINFONES_H
#include <qapplication.h>
#include <qwidget.h>
#include <qframe.h>
#include <qevent.h>
#include <qdirectpainter_qws.h> 
#include <qnamespace.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qstring.h>

#include "../InfoNES.h"

/*-------------------------------------------------------------------*/
/*  Class definition                                                 */
/*-------------------------------------------------------------------*/
class zinfones : public QWidget
{
public: 
  /* Constructor */
  zinfones( WORD * );

  /* Pad state */
  DWORD dwKeyPad1;
  DWORD dwKeyPad2;
  DWORD dwKeySystem;

  /* Frame Buffer */
  void loadFrame();
  WORD *pWf;

protected:
  virtual void paintEvent( QPaintEvent *e );
  virtual void keyPressEvent( QKeyEvent *e );
  virtual void keyReleaseEvent( QKeyEvent *e );
  virtual void moveEvent( QMoveEvent *e );

private:
  /* Window pos */
  int dx;
  int dy;
  WORD *pFb;

};
#endif /* ZINFONES_H */

/* End of zinfones.h */
