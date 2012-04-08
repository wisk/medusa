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
  this->editorBackground->setColor(Settings::instance().value(EDITOR_EDITOR_BACKGROUND_PATH, EDITOR_EDITOR_BACKGROUND_DEFAULT).toString());
  this->instructionColor->setColor(Settings::instance().value(EDITOR_INSTRUCTION_INSTRUCTION_PATH, EDITOR_INSTRUCTION_INSTRUCTION_DEFAULT).toString());
  this->registerColor->setColor(Settings::instance().value(EDITOR_INSTRUCTION_REGISTER_PATH, EDITOR_INSTRUCTION_REGISTER_DEFAULT).toString());
  this->immediateColor->setColor(Settings::instance().value(EDITOR_INSTRUCTION_IMMEDIATE_PATH, EDITOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString());
  this->commentColor->setColor(Settings::instance().value(EDITOR_INSTRUCTION_COMMENT_PATH, EDITOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  this->selectionColor->setColor(Settings::instance().value(EDITOR_INSTRUCTION_SELECTION_PATH, EDITOR_INSTRUCTION_SELECTION_DEFAULT).toString());
  this->addressBackground->setColor(Settings::instance().value(EDITOR_ADDRESS_BACKGROUND_PATH, EDITOR_ADDRESS_BACKGROUND_DEFAULT).toString());

  // Font
  QFont  font;

  font.fromString(Settings::instance().value(EDITOR_FONT_INSTRUCTION, EDITOR_FONT_INSTRUCTION_DEFAULT).toString());
  this->instructionFont->setFont(font);
  font.fromString(Settings::instance().value(EDITOR_FONT_REGISTER, EDITOR_FONT_REGISTER_DEFAULT).toString());
  this->registerFont->setFont(font);
  font.fromString(Settings::instance().value(EDITOR_FONT_IMMEDIATE, EDITOR_FONT_IMMEDIATE_DEFAULT).toString());
  this->immediateFont->setFont(font);
  font.fromString(Settings::instance().value(EDITOR_FONT_COMMENT, EDITOR_FONT_COMMENT_DEFAULT).toString());
  this->commentFont->setFont(font);
}

void    SettingsDialog::saveSettings()
{
  // Color
  Settings::instance().setValue(EDITOR_EDITOR_BACKGROUND_PATH, this->editorBackground->color().name());
  Settings::instance().setValue(EDITOR_INSTRUCTION_INSTRUCTION_PATH, this->instructionColor->color().name());
  Settings::instance().setValue(EDITOR_INSTRUCTION_REGISTER_PATH, this->registerColor->color().name());
  Settings::instance().setValue(EDITOR_INSTRUCTION_IMMEDIATE_PATH, this->immediateColor->color().name());
  Settings::instance().setValue(EDITOR_INSTRUCTION_COMMENT_PATH, this->commentColor->color().name());
  Settings::instance().setValue(EDITOR_INSTRUCTION_SELECTION_PATH, this->selectionColor->color().name());
  Settings::instance().setValue(EDITOR_ADDRESS_BACKGROUND_DEFAULT, this->addressBackground->color().name());

  // Font
  Settings::instance().setValue(EDITOR_FONT_INSTRUCTION, this->instructionFont->font().toString());
  Settings::instance().setValue(EDITOR_FONT_REGISTER, this->registerFont->font().toString());
  Settings::instance().setValue(EDITOR_FONT_IMMEDIATE, this->immediateFont->font().toString());
  Settings::instance().setValue(EDITOR_FONT_COMMENT, this->commentFont->font().toString());

  Settings::instance().saveChanges();
}