#ifndef __FONTPICKER_H__
# define __FONTPICKER_H__

# include <QLineEdit>
# include <QString>
# include <QFont>
# include <QtDesigner/QDesignerExportWidget>

class QMouseEvent;

class QDESIGNER_WIDGET_EXPORT FontPicker : public QLineEdit
{
  Q_OBJECT

public:
  FontPicker(QWidget *parent = 0);
  ~FontPicker();

public:
  void    setFont(QFont const & font);

protected:
  void    mousePressEvent(QMouseEvent * event);
};

#endif // !__FONTPICKER_H__