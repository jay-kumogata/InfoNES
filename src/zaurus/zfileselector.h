/*===================================================================*/
/*                                                                   */
/* zfileselector.cpp : A Qt-specific class implementation            */
/*                                                                   */
/*  2004/07/25  InfoNES Project                                      */
/*                                                                   */
/*===================================================================*/

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/
#ifndef ZFILESELECTOR_H
#define ZFILESELECTOR_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class SlFileSelector;

/*-------------------------------------------------------------------*/
/*  Class definition                                                 */
/*-------------------------------------------------------------------*/
class zfileselector : public QDialog
{
Q_OBJECT

public:
  zfileselector(const QString& dir, QWidget* parent = 0, 
		const char* name = 0, bool modal = FALSE, WFlags fl = 0);
  ~zfileselector();
  QString getFile();

private:
  QVBoxLayout * bl;
  SlFileSelector * sfs;
  QString m_sFile;

protected:
  void closeEvent( QCloseEvent * );

protected slots:
  void accept();
};
#endif /* ZFILESELECTOR_H */

/* End of zfileselector.h */
