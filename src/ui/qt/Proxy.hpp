#ifndef __MEDUSA_PROXY_HPP__
#define __MEDUSA_PROXY_HPP__

#include "MainWindow.hpp"
#include <medusa/event_handler.hpp>
#include <medusa/cell_action.hpp>

class AddDisassemblyViewAction : public medusa::CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Add disassembly view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new disassembly view"; }

  virtual std::string GetIconName(void) const
  { return "view-disassembly.png"; }

  virtual bool IsCompatible(medusa::Cell const& rCell) const
  { return true; }

  virtual void Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList);
};

class AddSemanticViewAction : public medusa::CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Add semantic view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new semantic view"; }

  virtual std::string GetIconName(void) const
  { return "view-semantic.png"; }

  virtual bool IsCompatible(medusa::Cell const& rCell) const
  { return true; }

  virtual void Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList);
};

class AddControlFlowGraphViewAction : public medusa::CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Add CFG view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new control flow graph view"; }

  virtual std::string GetIconName(void) const
  { return "view-graph.png"; }

  virtual bool IsCompatible(medusa::Cell const& rCell) const
  { return true; }

  virtual void Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList);
};

class ShowLabelDialog : public medusa::CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Show label dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a label"; }

  virtual std::string GetIconName(void) const
  { return "label.png"; }

  virtual bool IsCompatible(medusa::Cell const& rCell) const
  { return true; }

  virtual void Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList);
};

class ShowCommentDialog : public medusa::CellAction
{
public:
  virtual std::string GetName(void) const
  { return "Show comment dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a comment"; }

  virtual std::string GetIconName(void) const
  { return "comment.png"; }

  virtual bool IsCompatible(medusa::Cell const& rCell) const
  { return true; }

  virtual void Do(medusa::Medusa& rCore, medusa::Address::List const& rAddrList);
};

#endif // !__MEDUSA_PROXY_HPP__