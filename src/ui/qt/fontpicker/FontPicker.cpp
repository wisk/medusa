#include <QFontDialog>
#include <QPalette>
#include <QFontMetrics>
#include "FontPicker.h"

FontPicker::FontPicker(QWidget *parent)
  : QLineEdit(parent)
{
  this->setReadOnly(true);
}

FontPicker::~FontPicker()
{
}

void      FontPicker::setFont(QFont const & font)
{
  QLineEdit::setFont(font);

  QFontMetrics  fontMetrics(font);

  this->setText(font.family());
  this->resize(this->width(), fontMetrics.size(Qt::TextSingleLine, font.family()).height());
}

void      FontPicker::mousePressEvent(QMouseEvent * event)
{
  bool    ok;
  QFont    font = QFontDialog::getFont(&ok, this->font(), this, QObject::tr("Select font"), QFontDialog::DontUseNativeDialog);

  if (ok)
    this->setFont(font);
}