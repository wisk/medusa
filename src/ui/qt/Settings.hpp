#ifndef __SETTINGS_H__
# define __SETTINGS_H__

# include <QSettings>

/***********************
* Path
************************/

// Generic
# define WINDOW_LAYOUT                               "settings/window/layout"
# define WINDOW_GEOMETRY                             "settings/window/geometry"

// Color
# define MEDUSA_COLOR_VIEW_BACKGROUND                "settings/listing/listing/background"
# define MEDUSA_COLOR_INSTRUCTION_MNEMONIC           "settings/listing/instruction/mnemonic"
# define MEDUSA_COLOR_INSTRUCTION_REGISTER           "settings/listing/instruction/register"
# define MEDUSA_COLOR_INSTRUCTION_IMMEDIATE          "settings/listing/instruction/immediate"
# define MEDUSA_COLOR_INSTRUCTION_COMMENT            "settings/listing/instruction/comment"
# define MEDUSA_COLOR_INSTRUCTION_SELECTION          "settings/listing/instruction/selection"
# define MEDUSA_COLOR_INSTRUCTION_OPERATOR           "settings/listing/instruction/operator"
# define MEDUSA_COLOR_INSTRUCTION_KEYWORD            "settings/listing/instruction/keyword"
# define MEDUSA_COLOR_INSTRUCTION_LABEL              "settings/listing/instruction/label"
# define MEDUSA_COLOR_INSTRUCTION_STRING             "settings/listing/instruction/string"
# define MEDUSA_COLOR_ADDRESS_BACKGROUND             "settings/listing/address/background"

// Font
# define MEDUSA_FONT_TEXT                            "settings/listing/font/text"

/***********************
* Default
************************/

// Generic
# define WINDOW_LAYOUT_DEFAULT                       ""
# define WINDOW_GEOMETRY_DEFAULT                     ""

// Color
# define MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT        "#1e1e1e"
# define MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT   "#9a86d6"
# define MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT   "#00aa7f"
# define MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT  "#ffaa00"
# define MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT    "#55aa00"
# define MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT  "#760000"
# define MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT   "#ff0000"
# define MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT    "#55aaff"
# define MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT      "#aaaa7f"
# define MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT     "#bd72ff"
# define MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT     "#333333"

// Font
# define MEDUSA_FONT_TEXT_DEFAULT                    "Consolas,10,-1,5,50,0,0,0,0,0"

class Settings : public QSettings
{
  Q_OBJECT

private:
  Settings();

public:
  void        saveChanges();

public:
  static Settings &  instance();

signals:
  void        settingsChanged();
};

#endif // !__SETTINGS_H__