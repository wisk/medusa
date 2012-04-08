#ifndef __DISASM_VIEW_HPP__
# define __DISASM_VIEW_HPP__

# include <QtGui>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include <vector>

# include <medusa/address.hpp>
# include <medusa/database.hpp>

class DisassemblyView : public QAbstractScrollArea
{
  Q_OBJECT

public:
  DisassemblyView(QWidget * parent = 0);
  ~DisassemblyView(void);

  void goTo(medusa::Address const& addr);

  void setDatabase(medusa::Database const * db);

signals:

public slots:
  void setFont(QFont const & font);
  void listingUpdated(void);

protected:
  virtual void paintEvent(QPaintEvent * evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);
  virtual void mousePressEvent(QMouseEvent * evt);

private:
  enum LineType : unsigned char
  {
    UnknownLineType,
    CellLineType,
    MultiCellLineType,
    LabelLineType,
    MemoryAreaType,
    EmptyLineType
  };

  void updateScrollbars(void);

  medusa::Database const * _db;
  int _yOffset;
  int _xData, _yData;
  int _wChar, _hChar;
  int _xCursor, _yCursor;
  int _firstSelection, _lastSelection;
  int _lineNo, _lineLen;
};

#endif // !__DISASM_VIEW_HPP__