#include "ColorPicker.h"

#include <QtCore/QtPlugin>
#include "ColorPickerPlugin.h"

ColorPickerPlugin::ColorPickerPlugin(QObject *parent)
  : QObject(parent)
{
  initialized = false;
}

void ColorPickerPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
  if (initialized)
    return;

  initialized = true;
}

bool ColorPickerPlugin::isInitialized() const
{
  return initialized;
}

QWidget *ColorPickerPlugin::createWidget(QWidget *parent)
{
  return new ColorPicker(parent);
}

QString ColorPickerPlugin::name() const
{
  return "ColorPicker";
}

QString ColorPickerPlugin::group() const
{
  return "Medusa";
}

QIcon ColorPickerPlugin::icon() const
{
  return QIcon();
}

QString ColorPickerPlugin::toolTip() const
{
  return QString();
}

QString ColorPickerPlugin::whatsThis() const
{
  return QString();
}

bool ColorPickerPlugin::isContainer() const
{
  return false;
}

QString ColorPickerPlugin::domXml() const
{
  return "<widget class=\"ColorPicker\" name=\"colorPicker\">\n"
    " <property name=\"geometry\">\n"
    "  <rect>\n"
    "   <x>0</x>\n"
    "   <y>0</y>\n"
    "   <width>100</width>\n"
    "   <height>100</height>\n"
    "  </rect>\n"
    " </property>\n"
    "</widget>\n";
}

QString ColorPickerPlugin::includeFile() const
{
  return "ColorPicker.h";
}

Q_EXPORT_PLUGIN2(colorpicker, ColorPickerPlugin)