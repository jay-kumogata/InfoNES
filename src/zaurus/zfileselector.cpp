/*===================================================================*/
/*                                                                   */
/* zinfones.cpp : A Qt-specific class implementation                 */
/*                                                                   */
/*  2004/07/25  InfoNES Project                                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#include "zfileselector.h"
#include <qfile.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qwhatsthis.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qpe/config.h>
#include <qpe/qpeapplication.h>
#include <sl/slfileselector.h>

/*-------------------------------------------------------------------*/
/*  Constructor                                                      */
/*-------------------------------------------------------------------*/
zfileselector::zfileselector(const QString& dir, QWidget* parent, 
			     const char* name, bool modal, WFlags fl)
     : QDialog( parent, name, modal, fl )
{
  setWState( WState_Reserved1 );

  if ( !name )	
  {
    setName( "zfileselector" );
  }
  setSizePolicy( QSizePolicy((QSizePolicy::SizeType)5, 
			     (QSizePolicy::SizeType)5,
			     sizePolicy().hasHeightForWidth() ) );

  QString caption(name);
  setCaption( caption );

  bl = new QVBoxLayout( this ); 
  bl->setSpacing( 6 );
  bl->setMargin( 11 );

  sfs = new SlFileSelector( dir, "*/*", this, "sfs" );
  sfs->setFocusMode( SlFileSelector::ByTab );
  bl->addWidget( sfs );

  showMaximized();
}

zfileselector::~zfileselector()
{
}

/*-------------------------------------------------------------------*/
/*  Event Process                                                    */
/*-------------------------------------------------------------------*/
void zfileselector::closeEvent( QCloseEvent *e )
{
  e->accept();
}

void zfileselector::accept()
{
  m_sFile = sfs->currentFile();
  emit done( Accepted );
}

QString zfileselector::getFile()
{
  return m_sFile;
}

/* End of zfileselector.cpp */
