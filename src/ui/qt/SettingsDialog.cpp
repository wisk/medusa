#include <QFont>
#include "SettingsDialog.hpp"
#include "Settings.hpp"

SettingsDialog::SettingsDialog(QWidget * parent)
  : QDialog(parent)
{
  this->setupUi(this);

  this->connect(this, SIGNAL(accepted()), SLOT(saveSettings()));
}

SettingsDialog::~SettingsDialog()
{
}

void    SettingsDialog::showEvent(QShowEvent * e)
{
  QDialog::showEvent(e);

  // Color
  this->medusaBackground->setColor (Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  this->mnemonicColor->setColor    (Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC, MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());
  this->registerColor->setColor    (Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER, MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());
  this->immediateColor->setColor   (Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString());
  this->commentColor->setColor     (Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  this->selectionColor->setColor   (Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());
  this->addressBackground->setColor(Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());

  // Font
  QFont  font;

  font.fromString(Settings::instance().value(MEDUSA_FONT_MNEMONIC, MEDUSA_FONT_MNEMONIC_DEFAULT).toString());
  this->mnemonicFont->setFont(font);
  font.fromString(Settings::instance().value(MEDUSA_FONT_REGISTER, MEDUSA_FONT_REGISTER_DEFAULT).toString());
  this->registerFont->setFont(font);
  font.fromString(Settings::instance().value(MEDUSA_FONT_IMMEDIATE, MEDUSA_FONT_IMMEDIATE_DEFAULT).toString());
  this->mnemonicFont->setFont(font);
  font.fromString(Settings::instance().value(MEDUSA_FONT_COMMENT, MEDUSA_FONT_COMMENT_DEFAULT).toString());
  this->commentFont->setFont(font);
}

void    SettingsDialog::saveSettings()
{
  // Color
  Settings::instance().setValue(MEDUSA_COLOR_VIEW_BACKGROUND, this->medusaBackground->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_MNEMONIC, this->mnemonicColor->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_REGISTER, this->registerColor->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, this->immediateColor->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_COMMENT, this->commentColor->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_SELECTION, this->selectionColor->color().name());
  Settings::instance().setValue(MEDUSA_COLOR_ADDRESS_BACKGROUND, this->addressBackground->color().name());

  // Font
  Settings::instance().setValue(MEDUSA_FONT_MNEMONIC, this->mnemonicFont->font().toString());
  Settings::instance().setValue(MEDUSA_FONT_REGISTER, this->registerFont->font().toString());
  Settings::instance().setValue(MEDUSA_FONT_IMMEDIATE, this->immediateFont->font().toString());
  Settings::instance().setValue(MEDUSA_FONT_COMMENT, this->commentFont->font().toString());

  Settings::instance().saveChanges();
}