#ifndef QMEDUSA_DISASSEMBLY_VIEW_HPP
#define QMEDUSA_DISASSEMBLY_VIEW_HPP

# include <QtGui>
# include <QMenu>
# include <QScrollBar>
# include <QAbstractScrollArea>
# include <QPainter>
# include <QResizeEvent>
# include <QPaintEvent>

# include "Action.hpp"
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
  ~DisassemblyView();

  void OnDocumentUpdated(medusa::Address::Vector ErasedCellAddresses) override;

  bool goTo(medusa::Address const& address);

signals:
  void DisassemblyViewAdded(medusa::Address const& startAddr);
  void SemanticViewAdded(medusa::Address const& funcAddr);
  void GraphViewAdded(medusa::Address const& funcAddr);
  void ControlFlowGraphViewAdded(medusa::Address const& funcAddr);
  void viewportUpdated();
  void cursorAddressUpdated(medusa::Address const& addr);

public slots:
  void setFont();
  void viewUpdated();
  void horizontalScrollBarChanged(int n);
  void listingUpdated();
  void updateCursor();
  void showContextMenu(QPoint const& pos);
  void OnUiActionTriggered(medusa::Action::SPType spAction);

protected:
  void paintEvent(QPaintEvent * evt) override;
  void mouseMoveEvent(QMouseEvent * evt) override;
  void mousePressEvent(QMouseEvent * evt) override;
  void mouseDoubleClickEvent(QMouseEvent * evt) override;
  void keyPressEvent(QKeyEvent * evt) override;
  void resizeEvent(QResizeEvent * evt) override;
  void wheelEvent(QWheelEvent * evt) override;
  void actionEvent(QActionEvent * evt) override;

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

  void _UpdateActions();

  bool             _needRepaint;
  medusa::Medusa * _core;
  int              _wChar, _hChar;
  int              _addrLen;
  int              _lastVertPos;
  QTimer           _cursorTimer;
  bool             _cursorBlink;
  QPixmap          _cache;
};

#endif // !QMEDUSA_DISASSEMBLY_VIEW_HPP
