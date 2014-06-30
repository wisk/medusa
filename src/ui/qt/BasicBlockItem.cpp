#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>
#include <medusa/user_configuration.hpp>

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

  medusa::UserConfiguration UserCfg;
  QString fontInfo = QString::fromStdString(UserCfg.GetOption("font.listing"));
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
  medusa::UserConfiguration UserCfg;
  QRectF BgRct = boundingRect();
  QRectF AdRct = boundingRect();
  QColor BgClr = QColor(QString::fromStdString(UserCfg.GetOption("color.background_listing")));
  QColor AdClr = QColor(QString::fromStdString(UserCfg.GetOption("color.background_address")));
  qreal opacity = 1.0;

  if (_isPress)
  {
    BgClr = Qt::darkBlue;
    opacity = 0.7;
  }

  BgRct.setX(BgRct.x() + _adLen);
  AdRct.setWidth(_adLen);

  QBrush bgBrsh(BgClr);
  QBrush adBrsh(AdClr);

  setOpacity(opacity);
  p.fillRect(BgRct, bgBrsh);
  p.fillRect(AdRct, adBrsh);
  _fx->setColor(BgClr);
  p.drawRect(BgRct);
  p.drawRect(AdRct);
}

void BasicBlockItem::paintText(QPainter& p)
{
  p.setFont(_font);
  QFontMetrics fm(_font);
  auto hChar = fm.height();
  auto wChar = fm.width('M');
  int Line = hChar - 5; // http://doc.qt.digia.com/qt-maemo/qpainter.html#drawText-12 (Note: The y-position is used as the baseline of the font.)

  medusa::UserConfiguration UserCfg;
  QColor MnClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic")));
  QColor KwClr(QString::fromStdString(UserCfg.GetOption("color.keyword")));
  QColor ImClr(QString::fromStdString(UserCfg.GetOption("color.instruction_immediate")));
  QColor OpClr(QString::fromStdString(UserCfg.GetOption("color.operator")));
  QColor RgClr(QString::fromStdString(UserCfg.GetOption("color.instruction_register")));
  QColor LbClr(QString::fromStdString(UserCfg.GetOption("color.label")));
  QColor SzClr(QString::fromStdString(UserCfg.GetOption("color.string")));
  QColor CmClr(QString::fromStdString(UserCfg.GetOption("color.comment")));
  QColor DfClr(Qt::black);


  QColor MarkClr = DfClr;
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
        case medusa::Mark::MnemonicType:  MarkClr = MnClr; break;
        case medusa::Mark::KeywordType:   MarkClr = KwClr; break;
        case medusa::Mark::ImmediateType: MarkClr = ImClr; break;
        case medusa::Mark::OperatorType:  MarkClr = OpClr; break;
        case medusa::Mark::RegisterType:  MarkClr = RgClr; break;
        case medusa::Mark::LabelType:     MarkClr = LbClr; break;
        case medusa::Mark::StringType:    MarkClr = SzClr; break;
        case medusa::Mark::CommentType:   MarkClr = CmClr; break;
        default:                          MarkClr = DfClr; break;
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