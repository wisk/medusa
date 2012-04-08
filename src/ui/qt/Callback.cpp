#include "Callback.hpp"
#include "MainWindow.hpp"

void  medusaLog(wchar_t const * text)
{
  MainWindow::log(text);
}