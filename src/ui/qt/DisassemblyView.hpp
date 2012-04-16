#ifndef __DISASM_VIEW_HPP__
# define __DISASM_VIEW_HPP__

# include <QtGui>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include "Settings.hpp"

# include <vector>

# include <medusa/address.hpp>
# include <medusa/database.hpp>
# include <medusa/instruction.hpp>

class DisassemblyView : public QAbstractScrollArea
{
  Q_OBJECT

public:
  DisassemblyView(QWidget * parent = 0);
  ~DisassemblyView(void);

  bool goTo(medusa::Database::View::LineInformation const & lineInfo);

  void setDatabase(medusa::Database const * db);

signals:

public slots:
  void setFont(QFont const & font);
  void listingUpdated(void);
  void updateCursor(void);

protected:
  virtual void paintEvent(QPaintEvent * evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);
  virtual void mousePressEvent(QMouseEvent * evt);
  virtual void mouseDoubleClickEvent(QMouseEvent * evt);

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
  bool convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr);

  medusa::Database const * _db;
  int _xOffset, _yOffset;
  int _xData, _yData;
  int _wChar, _hChar;
  int _xCursor, _yCursor;
  int _firstSelection, _lastSelection;
  int _addrLen;
  int _lineNo, _lineLen;
  QTimer _cursorTimer; bool _cursorBlink;
};

#endif // !__DISASM_VIEW_HPP__