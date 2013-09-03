#ifndef __DISASSEMBLY_VIEW_HPP__
# define __DISASSEMBLY_VIEW_HPP__

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
# include <medusa/disassembly_view.hpp>

class DisassemblyView : public QAbstractScrollArea, public medusa::FullDisassemblyView
{
  Q_OBJECT

public:
  DisassemblyView(QWidget * parent, medusa::Medusa * core);
  ~DisassemblyView(void);

  virtual void OnDocumentUpdated(void);

  bool goTo(medusa::Address const& address);

signals:
  void DisassemblyViewAdded(medusa::Address const& startAddr);
  void SemanticViewAdded(medusa::Address const& funcAddr);
  void ControlFlowGraphViewAdded(medusa::Address const& funcAddr);
  void viewportUpdated(void);

public slots:
  void setFont(void);
  void viewUpdated(void);
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

  bool                         _needRepaint;
  medusa::Medusa *             _core;
  int                          _xOffset, _yOffset;
  int                          _wChar, _hChar;
  int                          _xCursor;
  medusa::Address              _cursorAddress;
  int                          _begSelection, _endSelection;
  int                          _begSelectionOffset, _endSelectionOffset;
  int                          _addrLen;
  int                          _lineNo, _lineLen;
  QTimer                       _cursorTimer; 
  bool                         _cursorBlink;
  std::vector<QString>         _visibleLines;
  medusa::Address              _curAddr;
  QPixmap                      _cache;

  /* Actions */
};

#endif // !__DISASSEMBLY_VIEW_HPP__