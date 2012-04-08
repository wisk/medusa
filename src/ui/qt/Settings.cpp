#include "Settings.hpp"

Settings::Settings()
  : QSettings(QSettings::IniFormat, QSettings::UserScope, "Medusa", "Medusa Qt")
{
}

void        Settings::saveChanges()
{
  emit settingsChanged();
}

Settings &      Settings::instance()
{
  static Settings  instance;

  return (instance);
}