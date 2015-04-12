#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QTimer>
#include <QPixmap>
#include "MainWindow.hpp"
#include <medusa/log.hpp>

// HACK: In Windows, if we use the standard main function,
// VS sets the subsystem to console and then Windows will
// spawn a cmd.exe with the GUI.
// In order to avoid that, we must use the WinMain function
// instead of main.
#ifdef WIN32
# ifndef _DEBUG
#  include <Windows.h>

/* Silly windows.h ... */
#undef max
#undef min

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

  QString FilePath = "";
  QString DbPath = "";

  if (argc >= 2)
    FilePath = argv[1];
  if (argc >= 3)
    DbPath = argv[2];

  MainWindow window(FilePath, DbPath);

  a.setWindowIcon(QIcon(":/logo/medusa.png"));
  if (QFontDatabase::addApplicationFont(":/fonts/DejaVuSansMono.ttf") == -1)
    medusa::Log::Write("ui_qt") << "unable to load default font" << medusa::LogEnd;
  window.show();
  return (a.exec());
}
