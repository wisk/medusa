#include <QFont>
#include "SettingsDialog.hpp"
#include "Proxy.hpp"

#include <QPushButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QKeySequenceEdit>

#include "medusa/cell_action.hpp"
#include "medusa/user_configuration.hpp"

class QColorPicker : public QPushButton
{
public:
  QColorPicker(QWidget* pParent, QString const& rColorName)
    : QPushButton(pParent), m_Color(rColorName)
  {
    _UpdateColor();
    connect(this, &QColorPicker::clicked, [this]()
    {
      auto Color = QColorDialog::getColor(m_Color, this, "Select color", QColorDialog::DontUseNativeDialog);
      if (!Color.isValid())
        return;
      m_Color = Color;
      _UpdateColor();
    });
  }

  QString GetColorName(void) const
  { return m_Color.name(); }

private:
  void _UpdateColor(void)
  {
    QPalette Pal = palette();
    Pal.setColor(QPalette::Button, m_Color);
    setAutoFillBackground(true);
    setFlat(true);
    setPalette(Pal);
    update();
  }

  QColor m_Color;
};

class QFontPicker : public QPushButton
{
public:
  QFontPicker(QWidget* pParent, QString const& rFontName)
    : QPushButton(pParent)
  {
    m_Font.fromString(rFontName);
    _UpdateFont();
    connect(this, &QFontPicker::clicked, [this]()
    {
      bool Ok;
      auto Font = QFontDialog::getFont(&Ok, m_Font, this, "Select font", QFontDialog::DontUseNativeDialog);
      if (!Ok)
        return;
      m_Font = Font;
      _UpdateFont();
    });
  }

  QString GetFontName(void) const
  { return m_Font.toString(); }

private:
  void _UpdateFont(void)
  {
    setText(m_Font.toString());
  }

  QFont m_Font;
};

SettingsDialog::SettingsDialog(QWidget* pParent, medusa::Medusa& rCore)
  : QDialog(pParent), m_rCore(rCore)
{
  this->setupUi(this);

  connect(ButtonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::SaveSettings);
  connect(ButtonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::close       );

  medusa::UserConfiguration UserCfg;

  auto Fonts = medusa::Appearance::GetFonts();
  for (auto const& rFontPair : Fonts)
  {
    auto pFontItem = new QTreeWidgetItem(AppearanceWidget);
    auto const& rFontInfo = rFontPair.second;
    pFontItem->setText(0, rFontInfo.m_pName);
    pFontItem->setData(0, Qt::UserRole, QVariant(QString::fromStdString(rFontPair.first)));

    QString Value;
    std::string Opt;
    if (!UserCfg.GetOption(rFontPair.first, Opt))
      continue;
    Value = QString::fromStdString(Opt);

    AppearanceWidget->setItemWidget(pFontItem, 1, new QFontPicker(this, Value));
  }

  auto Colors = medusa::Appearance::GetColors();
  for (auto const& rColorPair : Colors)
  {
    auto pColorItem = new QTreeWidgetItem(AppearanceWidget);
    auto const& rColorInfo = rColorPair.second;
    pColorItem->setText(0, rColorInfo.m_pName);
    pColorItem->setData(0, Qt::UserRole, QString::fromStdString(rColorPair.first));

    QString Value;
    std::string Opt;
    if (!UserCfg.GetOption(rColorPair.first, Opt))
      continue;
    Value = QString::fromStdString(Opt);

    AppearanceWidget->setItemWidget(pColorItem, 1, new QColorPicker(this, Value));
  }

  auto Actions = medusa::Action::GetMap();
  AddUiActions(Actions);
  for (auto const& rActionPair : Actions)
  {
    auto pShortcutItem = new QTreeWidgetItem(ShortcutWidget);
    auto spCurAct = rActionPair.second(m_rCore);
    pShortcutItem->setText(0, QString::fromStdString(spCurAct->GetName()));
    pShortcutItem->setText(1, QString::fromStdString(spCurAct->GetDescription()));
    pShortcutItem->setData(0, Qt::UserRole, QString::fromStdString(rActionPair.first));

    std::string Opt;
    if (!UserCfg.GetOption(rActionPair.first, Opt))
      medusa::Log::Write("ui_qt") << "no option for: " << rActionPair.first << medusa::LogEnd;
    QKeySequence KeySeq(QString::fromStdString(Opt), QKeySequence::PortableText);
    auto pKeySeqEdit = new QKeySequenceEdit(KeySeq, this);
    ShortcutWidget->setItemWidget(pShortcutItem, 2, pKeySeqEdit);
  }
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::SaveSettings()
{
  medusa::UserConfiguration UserCfg;
  
  for (QTreeWidgetItemIterator itAppearance(AppearanceWidget); *itAppearance; ++itAppearance)
  {
    std::string OptName = (*itAppearance)->data(0, Qt::UserRole).toString().toStdString();
    std::string Value;
    auto pWidget = AppearanceWidget->itemWidget(*itAppearance, 1);
    auto pColorPicker = dynamic_cast<QColorPicker*>(pWidget);
    auto pFontPicker = dynamic_cast<QFontPicker*>(pWidget);
    if (pColorPicker != nullptr)
      Value = pColorPicker->GetColorName().toStdString();
    else if (pFontPicker != nullptr)
      Value = pFontPicker->GetFontName().toStdString();
    if (Value.empty())
      continue;
    if (!UserCfg.SetOption(OptName, Value))
      medusa::Log::Write("ui_qt") << "unable to set user option: " << OptName << ", " << Value << medusa::LogEnd;
  }

  for (QTreeWidgetItemIterator itShortcut(ShortcutWidget); *itShortcut; ++itShortcut)
  {
    std::string OptName = (*itShortcut)->data(0, Qt::UserRole).toString().toStdString();
    auto pKeySeqEdit = dynamic_cast<QKeySequenceEdit*>(ShortcutWidget->itemWidget(*itShortcut, 2));
    if (pKeySeqEdit == nullptr)
      continue;
    auto Value = pKeySeqEdit->keySequence().toString().toStdString();
    if (!UserCfg.SetOption(OptName, Value))
      medusa::Log::Write("ui_qt") << "unable to set user option: " << OptName << ", " << Value << medusa::LogEnd;
  }

  done(QDialog::Accepted);
}