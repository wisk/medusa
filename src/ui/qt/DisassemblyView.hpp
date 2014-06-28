#ifndef __DISASSEMBLY_VIEW_HPP__
# define __DISASSEMBLY_VIEW_HPP__

# include <QtGui>
# include <QMenu>
# include <QScrollBar>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include "Action.hpp"
# include "Settings.hpp"
# include "Proxy.hpp"

# include <vector>

# include <medusa/medusa.hpp>
# include <medusa/address.hpp>
# include <medusa/document.hpp>
# include <medusa/instruction.hpp>
# include <medusa/disassembly_view.hpp>
# include <medusa/cell_action.hpp>

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
  void cursorAddressUpdated(medusa::Address const& addr);

public slots:
  void setFont(void);
  void viewUpdated(void);
  void horizontalScrollBarChanged(int n);
  void listingUpdated(void);
  void updateCursor(void);
  void showContextMenu(QPoint const& pos);
  void OnUiActionTriggered(medusa::Action::SPtr spAction);

protected:
  virtual void paintEvent(QPaintEvent * evt);
  virtual void mouseMoveEvent(QMouseEvent * evt);
  virtual void mousePressEvent(QMouseEvent * evt);
  virtual void mouseDoubleClickEvent(QMouseEvent * evt);
  virtual void keyPressEvent(QKeyEvent * evt);
  virtual void resizeEvent(QResizeEvent * evt);
  virtual void wheelEvent(QWheelEvent * evt);
  virtual void actionEvent(QActionEvent * evt);

private:
  void paintBackground(QPainter& p);
  void paintSelection(QPainter& p);
  void paintText(QPainter& p);
  void paintCursor(QPainter& p);

  void setCursorPosition(QMouseEvent * evt);
  void setCursorPosition(int x, int y);
  void moveCursorPosition(int x, int y);

  bool convertPositionToAddress(QPoint const & pos, medusa::Address & addr);
  bool convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr);

  void _UpdateActions(void);

  bool             _needRepaint;
  medusa::Medusa * _core;
  int              _wChar, _hChar;
  int              _addrLen;
  int              _lastVertPos;
  QTimer           _cursorTimer;
  bool             _cursorBlink;
  QPixmap          _cache;
};

#endif // !__DISASSEMBLY_VIEW_HPP__
