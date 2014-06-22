#include <QFont>
#include "SettingsDialog.hpp"
#include "Settings.hpp"

#include <QColorDialog>
#include <QFontDialog>
#include <QKeySequenceEdit>

SettingsDialog::SettingsDialog(QWidget * parent)
  : QDialog(parent)
{
  this->setupUi(this);

  this->connect(this->ButtonBox,         SIGNAL(accepted()),         SLOT(saveSettings()));
  this->connect(this->ButtonBox,         SIGNAL(rejected()),         SLOT(close()       ));

  this->connect(this->BackgroundColor,   SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->BackgroundColor,   SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->MnemonicColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->RegisterColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->ImmediateColor,    SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->CommentColor,      SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->SelectionColor,    SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->OperatorColor,     SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->KeywordColor,      SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->LabelColor,        SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->StringColor,       SIGNAL(selectionChanged()), SLOT(setColor()    ));
  this->connect(this->AddressBackground, SIGNAL(selectionChanged()), SLOT(setColor()    ));

  this->connect(this->TextFont,          SIGNAL(selectionChanged()), SLOT(setFont()     ));

  auto pShortcutItem = new QTreeWidgetItem(ShortcutWidget);
  pShortcutItem->setText(0, "test");
  pShortcutItem->setText(1, "blabla");
  auto pKeySeqEdit = new QKeySequenceEdit(this);
  ShortcutWidget->setItemWidget(pShortcutItem, 2, pKeySeqEdit);
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
  setColor(this->BackgroundColor,   Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  setColor(this->MnemonicColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC, MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());
  setColor(this->RegisterColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER, MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());
  setColor(this->ImmediateColor,    Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString());
  setColor(this->CommentColor,      Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  setColor(this->SelectionColor,    Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());
  setColor(this->OperatorColor,     Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR, MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString());
  setColor(this->KeywordColor,      Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD, MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());
  setColor(this->LabelColor,        Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());
  setColor(this->StringColor,       Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING, MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString());
  setColor(this->AddressBackground, Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());

  // Font
  QFont  font;
  font.fromString(Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString());
  this->TextFont->setFont(font);
}

void    SettingsDialog::saveSettings()
{
  // Color
  Settings::instance().setValue(MEDUSA_COLOR_VIEW_BACKGROUND,       this->BackgroundColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_MNEMONIC,  this->MnemonicColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_REGISTER,  this->RegisterColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, this->ImmediateColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_COMMENT,   this->CommentColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_SELECTION, this->SelectionColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_OPERATOR,  this->OperatorColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_KEYWORD,   this->KeywordColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_LABEL,     this->LabelColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_INSTRUCTION_STRING,    this->StringColor->text());
  Settings::instance().setValue(MEDUSA_COLOR_ADDRESS_BACKGROUND,    this->AddressBackground->text());

  // Font
  Settings::instance().setValue(MEDUSA_FONT_TEXT, this->TextFont->font().toString());

  Settings::instance().saveChanges();

  done(QDialog::Accepted);
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
  {
    curLine->setText(font.toString());
    this->TextFont->setFont(font);
  }
}