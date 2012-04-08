#ifndef __COLORPICKER_H__
# define __COLORPICKER_H__

# include <QLineEdit>
# include <QString>
# include <QColor>
# include <QtDesigner/QDesignerExportWidget>

class QMouseEvent;

class QDESIGNER_WIDGET_EXPORT ColorPicker : public QLineEdit
{
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor);

public:
  ColorPicker(QWidget * parent = 0);
  ~ColorPicker();

public:
  void    setColor(QColor const & color);
  QColor    color() const;

protected:
  void    mousePressEvent(QMouseEvent * event);

protected:
  QColor    _color;
};

#endif // !__COLORPICKER_H__