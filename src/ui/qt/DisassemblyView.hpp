#ifndef __DISASM_VIEW_HPP__
# define __DISASM_VIEW_HPP__

# include <QtGui>
# include <QMenu>
# include <QScrollBar>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include "Settings.hpp"
# include "DisassemblyPrinter.hpp"

# include <vector>

# include <medusa/medusa.hpp>
# include <medusa/address.hpp>
# include <medusa/document.hpp>
# include <medusa/instruction.hpp>
# include <medusa/screen.hpp>

class DisassemblyView : public QAbstractScrollArea
{
  Q_OBJECT

public:
  DisassemblyView(QWidget * parent = 0);
  ~DisassemblyView(void);

  bool goTo(medusa::Address const& address);
  void bindMedusa(medusa::Medusa * core);
  void clear(void);

signals:
  void DisassemblyViewAdded(medusa::Address const& startAddr);
  void SemanticViewAdded(medusa::Address const& funcAddr);
  void ControlFlowGraphViewAdded(medusa::Address const& funcAddr);

public slots:
  void setFont(void);
  void viewUpdated(void);
  void verticalScrollBarChanged(int n);
  void horizontalScrollBarChanged(int n);
  void listingUpdated(void);
  void updateCursor(void);
  void showContextMenu(QPoint const& pos);

protected:
  virtual void paintEvent(QPaintEvent * evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);
  virtual void mousePressEvent(QMouseEvent * evt);
  virtual void mouseDoubleClickEvent(QMouseEvent * evt);
  virtual void keyPressEvent(QKeyEvent * evt);
  virtual void resizeEvent(QResizeEvent * evt);

private:
  enum LineType
  {
    UnknownLineType,
    CellLineType,
    MultiCellLineType,
    LabelLineType,
    MemoryAreaType,
    EmptyLineType
  };

  void paintBackground(QPainter& p);
  void paintSelection(QPainter& p);
  void paintText(QPainter& p);
  void paintCursor(QPainter& p);

  void setCursorPosition(QMouseEvent * evt);
  void setCursorPosition(int x, int y);
  void moveCursorPosition(int x, int y);

  void resetSelection(void);
  void setSelection(int x, int y);
  void moveSelection(int x, int y);

  void getSelectedAddresses(medusa::Address::List& addresses);
  void updateScrollbars(void);
  bool convertPositionToAddress(QPoint const & pos, medusa::Address & addr);
  bool convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr);
  void ensureCursorIsVisible(void);

  bool                 _needRepaint;
  medusa::Medusa *     _core;
  medusa::Document *   _db;
  int                  _xOffset, _yOffset;
  int                  _wChar, _hChar;
  int                  _xCursor, _yCursor;
  int                  _begSelection, _endSelection;
  int                  _begSelectionOffset, _endSelectionOffset;
  int                  _addrLen;
  int                  _lineNo, _lineLen;
  QTimer               _cursorTimer; 
  bool                 _cursorBlink;
  std::vector<QString> _visibleLines;
  medusa::Address      _curAddr;
  QPixmap              _cache;
  medusa::Screen*      _scr;
  DisassemblyPrinter*  _dp;

  /* Actions */
};

#endif // !__DISASM_VIEW_HPP__