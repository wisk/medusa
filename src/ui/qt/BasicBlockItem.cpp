#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>
#include "Settings.hpp"

BasicBlockItem::BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses)
  : _parent(parent)
  , medusa::DisassemblyView(core, medusa::FormatDisassembly::ShowAddress, addresses.front())
  , _width(0.0), _height(0.0), _adLen(0.0)
  , _isPress(false)
  , _core(core)
  , _z(zValue()), _fx(new QGraphicsDropShadowEffect(this))
  , _needRepaint(true)
{
  setFlags(ItemIsMovable | ItemIsSelectable);
  _fx->setBlurRadius(25.0);
  setGraphicsEffect(_fx);
  setZValue(10.0);

  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  _font.fromString(fontInfo);
  QFontMetrics fm(_font);

  m_Format(addresses, m_FormatFlags);

  medusa::u32 viewWidth, viewHeight;
  GetDimension(viewWidth, viewHeight);
  _width  = viewWidth  * fm.width('M');
  _height = viewHeight * fm.height();
  _adLen  = (addresses.front().ToString().length() + 1) * fm.width('M');
}

void BasicBlockItem::OnDocumentUpdated(void)
{
  _needRepaint = true;
}

QRectF BasicBlockItem::boundingRect(void) const
{
  return QRectF(0, 0, _width, _height);
}

void BasicBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  if (_needRepaint == true || _width == 0.0 || _height == 0.0)
  {
    _cache = QPixmap(QSize(_width, _height));
    QPainter cachedPainter(&_cache);
    paintBackground(cachedPainter);
    cachedPainter.setRenderHint(QPainter::TextAntialiasing);
    paintText(cachedPainter);
    _needRepaint = false;
  }

  painter->drawPixmap(0, 0, _cache);
}

void BasicBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  _isPress = true;
  setZValue(1.0);
  update();
  QGraphicsItem::mousePressEvent(event);
}

void BasicBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseReleaseEvent(event);
  _isPress = false;
  setZValue(_z);
  update();
}

void BasicBlockItem::paintBackground(QPainter& p)
{
  QRectF bgRct = boundingRect();
  QRectF adRct = boundingRect();
  QColor bgClr = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  QColor adClr = QColor(Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());
  qreal opacity = 1.0;

  if (_isPress)
  {
    bgClr = Qt::darkBlue;
    opacity = 0.7;
  }

  bgRct.setX(bgRct.x() + _adLen);
  adRct.setWidth(_adLen);

  QBrush bgBrsh(bgClr);
  QBrush adBrsh(adClr);

  setOpacity(opacity);
  p.fillRect(bgRct, bgBrsh);
  p.fillRect(adRct, adBrsh);
  _fx->setColor(bgClr);
  p.drawRect(bgRct);
  p.drawRect(adRct);
}

void BasicBlockItem::paintText(QPainter& p)
{
  p.setFont(_font);
  QFontMetrics fm(_font);
  auto hChar = fm.height();
  auto wChar = fm.width('M');
  int Line = hChar - 5; // http://doc.qt.digia.com/qt-maemo/qpainter.html#drawText-12 (Note: The y-position is used as the baseline of the font.)
  QColor MarkClr(Qt::black);
  m_PrintData.ForEachLine([&](medusa::Address const& rAddr, std::string const& rText, medusa::Mark::List const& rMarks)
  {
    std::string::size_type TextOff = 0;
    for (auto const& rMark : rMarks)
    {
      auto MarkLen = rMark.GetLength();
      if (rMark.GetType() != medusa::Mark::UnprintableType)
      {
        switch (rMark.GetType())
        {
        case medusa::Mark::MnemonicType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC,  MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());  break;
        case medusa::Mark::KeywordType:   MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD,   MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());   break;
        case medusa::Mark::ImmediateType: MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString()); break;
        case medusa::Mark::OperatorType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR,  MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString());  break;
        case medusa::Mark::RegisterType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER,  MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());  break;
        case medusa::Mark::LabelType:     MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL,     MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());     break;
        case medusa::Mark::StringType:    MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING,    MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString());    break;
        case medusa::Mark::CommentType:   MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT,   MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());   break;
        default:                          MarkClr = QColor(Qt::black); break;
        };
        p.setPen(MarkClr);
        QString Text = QString::fromUtf8(rText.substr(TextOff, MarkLen).c_str());
        p.drawText(static_cast<int>(TextOff * wChar), Line, Text);
      }
      TextOff += MarkLen;
    }
    Line += hChar;
  });
}