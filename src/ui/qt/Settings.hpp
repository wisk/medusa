#ifndef __SETTINGS_H__
# define __SETTINGS_H__

# include <QSettings>

/***********************
* Path
************************/

// Generic
# define WINDOW_LAYOUT    "settings/window/layout"
# define WINDOW_GEOMETRY  "settings/window/geometry"

// Color
# define MEDUSA_VIEW_BACKGROUND_PATH      "settings/editor/editor/background"
# define MEDUSA_INSTRUCTION_INSTRUCTION_PATH  "settings/editor/instruction/instruction"
# define MEDUSA_INSTRUCTION_REGISTER_PATH    "settings/editor/instruction/register"
# define MEDUSA_INSTRUCTION_IMMEDIATE_PATH    "settings/editor/instruction/immediate"
# define MEDUSA_INSTRUCTION_COMMENT_PATH    "settings/editor/instruction/comment"
# define MEDUSA_INSTRUCTION_SELECTION_PATH    "settings/editor/instruction/selection"
# define MEDUSA_ADDRESS_BACKGROUND_PATH      "settings/editor/address/background"

// Font
# define MEDUSA_FONT_INSTRUCTION  "settings/editor/font/instruction"
# define MEDUSA_FONT_REGISTER    "settings/editor/font/register"
# define MEDUSA_FONT_IMMEDIATE    "settings/editor/font/immediate"
# define MEDUSA_FONT_COMMENT    "settings/editor/font/comment"

/***********************
* Default
************************/

// Generic
# define WINDOW_LAYOUT_DEFAULT    ""
# define WINDOW_GEOMETRY_DEFAULT  ""

// Color
# define MEDUSA_VIEW_BACKGROUND_DEFAULT    "#FFFFFF"
# define MEDUSA_INSTRUCTION_INSTRUCTION_DEFAULT "#000000"
# define MEDUSA_INSTRUCTION_REGISTER_DEFAULT  "#000000"
# define MEDUSA_INSTRUCTION_IMMEDIATE_DEFAULT  "#000000"
# define MEDUSA_INSTRUCTION_COMMENT_DEFAULT    "#000000"
# define MEDUSA_INSTRUCTION_SELECTION_DEFAULT  "#FF0000"
# define MEDUSA_ADDRESS_BACKGROUND_DEFAULT    "#333333"

// Font
# define MEDUSA_FONT_INSTRUCTION_DEFAULT  "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_REGISTER_DEFAULT    "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_IMMEDIATE_DEFAULT    "Arial,10,-1,5,50,0,0,0,0,0"
# define MEDUSA_FONT_COMMENT_DEFAULT    "Arial,10,-1,5,50,0,0,0,0,0"

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