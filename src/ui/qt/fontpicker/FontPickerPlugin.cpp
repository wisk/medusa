#include "FontPicker.h"

#include <QtCore/QtPlugin>
#include "FontPickerPlugin.h"


FontPickerPlugin::FontPickerPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void FontPickerPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
	if (initialized)
		return;

	initialized = true;
}

bool FontPickerPlugin::isInitialized() const
{
	return initialized;
}

QWidget *FontPickerPlugin::createWidget(QWidget *parent)
{
	return new FontPicker(parent);
}

QString FontPickerPlugin::name() const
{
	return "FontPicker";
}

QString FontPickerPlugin::group() const
{
	return "Medusa";
}

QIcon FontPickerPlugin::icon() const
{
	return QIcon();
}

QString FontPickerPlugin::toolTip() const
{
	return QString();
}

QString FontPickerPlugin::whatsThis() const
{
	return QString();
}

bool FontPickerPlugin::isContainer() const
{
	return false;
}

QString FontPickerPlugin::domXml() const
{
	return "<widget class=\"FontPicker\" name=\"fontPicker\">\n"
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

QString FontPickerPlugin::includeFile() const
{
	return "FontPicker.h";
}

Q_EXPORT_PLUGIN2(fontpicker, FontPickerPlugin)
