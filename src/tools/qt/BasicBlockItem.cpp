#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>
#include <medusa/user_configuration.hpp>

BasicBlockItem::BasicBlockItem(QObject* pParent, medusa::Medusa& rCore, medusa::Address::List const& rAddresses)
  : m_pParent(pParent)
  , medusa::DisassemblyView(rCore, medusa::FormatDisassembly::ShowAddress, rAddresses.front())
  , m_Addresses(rAddresses)
  , m_Width(0.0), m_Height(0.0), m_AddrLen(0.0)
  , m_IsSelected(false)
  , m_rCore(rCore)
  , m_Z(zValue()), m_Fx(new QGraphicsDropShadowEffect(this))
  , m_NeedRepaint(true)
{
  medusa::UserConfiguration UserCfg;
  m_BackgroundColor = QColor(QString::fromStdString(UserCfg.GetOption("color.background_node")));
  setFlags(ItemIsMovable | ItemIsSelectable);
  _Update();
}

void BasicBlockItem::OnDocumentUpdated(void)
{
  m_NeedRepaint = true;
}

QRectF BasicBlockItem::boundingRect(void) const
{
  return QRectF(0, 0, m_Width, m_Height);
}

void BasicBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  if (m_NeedRepaint == true || m_Width == 0.0 || m_Height == 0.0)
  {
    m_Cache = QPixmap(QSize(m_Width, m_Height));
    QPainter cachedPainter(&m_Cache);
    paintBackground(cachedPainter);
    cachedPainter.setRenderHint(QPainter::TextAntialiasing);
    paintText(cachedPainter);
    m_NeedRepaint = false;
  }

  painter->drawPixmap(0, 0, m_Cache);
}

void BasicBlockItem::SetBackgroundColor(QColor const& rBgClr)
{
  m_BackgroundColor = rBgClr;
  m_NeedRepaint = true;
}

QVariant BasicBlockItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
  switch (change)
  {
  case ItemSelectedHasChanged:
    m_IsSelected = value.toBool();
    m_NeedRepaint = true;
    update();
    break;
  default:
    break;
  }
  return value;
}

void BasicBlockItem::paintBackground(QPainter& p)
{
  medusa::UserConfiguration UserCfg;
  QRectF BgRct = boundingRect();
  QRectF AdRct = boundingRect();
  QColor AdClr = QColor(QString::fromStdString(UserCfg.GetOption("color.background_address")));
  QColor FxClr = m_BackgroundColor;

  if (m_IsSelected)
    FxClr = QColor(QString::fromStdString(UserCfg.GetOption("color.selection")));

  BgRct.setX(BgRct.x() + m_AddrLen);
  AdRct.setWidth(m_AddrLen);

  QBrush bgBrsh(m_BackgroundColor);
  QBrush adBrsh(AdClr);

  p.fillRect(BgRct, bgBrsh);
  p.fillRect(AdRct, adBrsh);
  m_Fx->setColor(FxClr);
  p.drawRect(BgRct);
  p.drawRect(AdRct);
}

void BasicBlockItem::paintText(QPainter& p)
{
  p.setFont(m_Font);
  QFontMetrics fm(m_Font);
  auto hChar = fm.height();
  auto wChar = fm.width('M');
  int Line = hChar - 5; // http://doc.qt.digia.com/qt-maemo/qpainter.html#drawText-12 (Note: The y-position is used as the baseline of the font.)

  medusa::UserConfiguration UserCfg;
  QColor MnClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic")));
  QColor PfClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic_prefix")));
  QColor SfClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic_suffix")));
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
      auto MarkLen = rMark.GetSize();
      if (rMark.GetType() != medusa::Mark::UnprintableType)
      {
        switch (rMark.GetType())
        {
        case medusa::Mark::MnemonicType:       MarkClr = MnClr; break;
        case medusa::Mark::MnemonicPrefixType: MarkClr = PfClr; break;
        case medusa::Mark::MnemonicSuffixType: MarkClr = SfClr; break;
        case medusa::Mark::KeywordType:        MarkClr = KwClr; break;
        case medusa::Mark::ImmediateType:      MarkClr = ImClr; break;
        case medusa::Mark::OperatorType:       MarkClr = OpClr; break;
        case medusa::Mark::RegisterType:       MarkClr = RgClr; break;
        case medusa::Mark::LabelType:          MarkClr = LbClr; break;
        case medusa::Mark::StringType:         MarkClr = SzClr; break;
        case medusa::Mark::CommentType:        MarkClr = CmClr; break;
        default:                               MarkClr = DfClr; break;
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

void BasicBlockItem::_Update(void)
{
  m_Fx->setBlurRadius(25.0);
  setGraphicsEffect(m_Fx);
  setZValue(10.0);

  medusa::UserConfiguration UserCfg;
  QString fontInfo = QString::fromStdString(UserCfg.GetOption("font.listing"));
  m_Font.fromString(fontInfo);
  QFontMetrics fm(m_Font);

  m_Format(m_Addresses, m_FormatFlags);

  medusa::u32 viewWidth, viewHeight;
  GetDimension(viewWidth, viewHeight);
  m_Width  = viewWidth  * fm.width('M');
  m_Height = viewHeight * fm.height();
  m_AddrLen  = (m_Addresses.front().ToString().length() + 1) * fm.width('M');
  m_NeedRepaint = true;
}
