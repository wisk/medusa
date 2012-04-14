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
# define MEDUSA_FONT_MNEMONIC                        "settings/listing/font/mnemonic"
# define MEDUSA_FONT_REGISTER                        "settings/listing/font/register"
# define MEDUSA_FONT_IMMEDIATE                       "settings/listing/font/immediate"
# define MEDUSA_FONT_COMMENT                         "settings/listing/font/comment"

/***********************
* Default
************************/

// Generic
# define WINDOW_LAYOUT_DEFAULT                       ""
# define WINDOW_GEOMETRY_DEFAULT                     ""

// Color
# define MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT        "#FFFFFF"
# define MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT   "#000000"
# define MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT   "#000000"
# define MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT  "#000000"
# define MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT    "#000000"
# define MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT  "#FF0000"
# define MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT   "#000000"
# define MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT    "#000000"
# define MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT      "#000000"
# define MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT     "#000000"
# define MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT     "#333333"

// Font
# define MEDUSA_FONT_MNEMONIC_DEFAULT                "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_REGISTER_DEFAULT                "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_IMMEDIATE_DEFAULT               "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_COMMENT_DEFAULT                 "Arial,10,-1,5,50,0,0,0,0,0"

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