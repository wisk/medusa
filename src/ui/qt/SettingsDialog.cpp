#include <QFont>
#include "SettingsDialog.hpp"
#include "Settings.hpp"

#include <QColorDialog>
#include <QFontDialog>

SettingsDialog::SettingsDialog(QWidget * parent)
  : QDialog(parent)
{
  this->setupUi(this);

  this->connect(this->buttonBox,         SIGNAL(accepted()),         SLOT(saveSettings()));
  this->connect(this->buttonBox,         SIGNAL(rejected()),         SLOT(close()       ));

  this->connect(this->medusaBackground,  SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->medusaBackground,  SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->mnemonicColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->registerColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->immediateColor,    SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->commentColor,      SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->selectionColor,    SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->operatorColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->keywordColor,      SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->labelColor,        SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->stringColor,       SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->addressBackground, SIGNAL(selectionChanged()), SLOT(setColor()    ));

  this->connect(this->textFont,          SIGNAL(selectionChanged()), SLOT(setFont()     ));
}

SettingsDialog::~SettingsDialog()
{
}

void    SettingsDialog::showEvent(QShowEvent * e)
{
  QDialog::showEvent(e);

  auto setColor = [](QLineEdit *lineEdit, QColor const& clr)
  {
    if (!clr.isValid())
      return;
    QPalette palette(lineEdit->palette());
    palette.setColor(QPalette::Base, clr);
    lineEdit->setPalette(palette);
    lineEdit->setText(clr.name().toUpper());
  };

  // Color
  setColor(this->medusaBackground,  Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  setColor(this->mnemonicColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC, MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());
  setColor(this->registerColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER, MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());
  setColor(this->immediateColor,    Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString());
  setColor(this->commentColor,      Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  setColor(this->selectionColor,    Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());
  setColor(this->operatorColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR, MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString());
  setColor(this->keywordColor,      Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD, MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());
  setColor(this->labelColor,        Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());
  setColor(this->stringColor,       Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING, MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString());
  setColor(this->addressBackground, Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());

  // Font
  QFont  font;
  font.fromString(Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString());
  this->textFont->setFont(font);
}

void    SettingsDialog::saveSettings()
{
  // Color
  Settings::instance().setValue(MEDUSA_COLOR_VIEW_BACKGROUND,       this->medusaBackground->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_MNEMONIC,  this->mnemonicColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_REGISTER,  this->registerColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, this->immediateColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_COMMENT,   this->commentColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_SELECTION, this->selectionColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_OPERATOR,  this->operatorColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_KEYWORD,   this->keywordColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_LABEL,     this->labelColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_STRING,    this->stringColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_ADDRESS_BACKGROUND,    this->addressBackground->text());

  // Font
  Settings::instance().setValue(MEDUSA_FONT_TEXT, this->textFont->font().toString());

  Settings::instance().saveChanges();

  close();
}

void SettingsDialog::setColor(void)
{
  auto curLine = dynamic_cast<QLineEdit*>(qApp->widgetAt(QCursor::pos()));
  if (curLine == nullptr)
    return;
  auto clr = QColorDialog::getColor(curLine->text(), this, "Select color", QColorDialog::DontUseNativeDialog);
  if (!clr.isValid())
    return;
  QPalette palette(curLine->palette());
  palette.setColor(QPalette::Base, clr);
  curLine->setPalette(palette);
  curLine->setText(clr.name().toUpper());
}

void SettingsDialog::setFont(void)
{
  auto curLine = dynamic_cast<QLineEdit*>(qApp->widgetAt(QCursor::pos()));
  if (curLine == nullptr)
    return;
  bool ok;
  auto font = QFontDialog::getFont(&ok, curLine->text(), this, "Select font", QFontDialog::DontUseNativeDialog);
  if (ok)
    curLine->setText(font.toString());
}