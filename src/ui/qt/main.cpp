#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QTimer>
#include <QPixmap>
#include "MainWindow.hpp"

// HACK: In Windows, if we use the standard main function,
// VS sets the subsystem to console and then Windows will
// spawn a cmd.exe with the GUI.
// In order to avoid that, we must use the WinMain function
// instead of main.
#ifdef WIN32
# ifndef _DEBUG
#  include <Windows.h>
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
  // __argc and __argv are automatically set by msvcrt
  // this tip comes from this site
  // http://support.microsoft.com/kb/126571
  int     argc = __argc;
  char**  argv = __argv;

# else // DEBUG
#  ifdef WIN32
# pragma comment(linker, "/SUBSYSTEM:console")
#  endif
int main(int argc, char *argv[])
{
# endif //DEBUG
# else // WIN32
int main(int argc, char *argv[])
{
#endif // WIN32

  QApplication  a(argc, argv);
  MainWindow    window;
  QSplashScreen  screen(QPixmap(":/images/medusa_logo.png"), Qt::WindowStaysOnTopHint);
  QTimer      timer;

  /*screen.show();
  screen.showMessage(QObject::tr("Loading"));*/
    a.setWindowIcon(QIcon(":/images/medusa_logo.png"));
  /*
  screen.connect(&timer, SIGNAL(timeout()), SLOT(hide()));
  window.connect(&timer, SIGNAL(timeout()), SLOT(show()));
  timer.setSingleShot(true);
  timer.start(1000);*/
  window.show();
    return (a.exec());
}