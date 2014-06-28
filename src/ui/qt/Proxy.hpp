#ifndef __MEDUSA_PROXY_HPP__
#define __MEDUSA_PROXY_HPP__

#include "MainWindow.hpp"
#include <medusa/event_handler.hpp>
#include <medusa/cell_action.hpp>

class AddDisassemblyViewAction : public medusa::Action
{
public:
  AddDisassemblyViewAction(medusa::Medusa& rCore) : Action(rCore) {}

  static SPtr Create(medusa::Medusa& rCore)
  { return std::make_shared<AddDisassemblyViewAction>(rCore); }

  static char const* GetBindingName(void)
  { return "action.add_disassembly_view"; }

  virtual std::string GetName(void) const
  { return "Add disassembly view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new disassembly view"; }

  virtual std::string GetIconName(void) const
  { return "view-disassembly.png"; }

  virtual bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
  { return true; }

  virtual void Do(medusa::Action::RangeAddress const& rRangeAddress);
};

class AddSemanticViewAction : public medusa::Action
{
public:
  AddSemanticViewAction(medusa::Medusa& rCore) : Action(rCore) {}

  static SPtr Create(medusa::Medusa& rCore)
  { return std::make_shared<AddSemanticViewAction>(rCore); }

  static char const* GetBindingName(void)
  { return "action.add_semantic_view"; }

  virtual std::string GetName(void) const
  { return "Add semantic view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new semantic view"; }

  virtual std::string GetIconName(void) const
  { return "view-semantic.png"; }

  virtual bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
  { return true; }

  virtual void Do(medusa::Action::RangeAddress const& rRangeAddress);
};

class AddControlFlowGraphViewAction : public medusa::Action
{
public:
  AddControlFlowGraphViewAction(medusa::Medusa& rCore) : Action(rCore) {}

  static SPtr Create(medusa::Medusa& rCore)
  { return std::make_shared<AddControlFlowGraphViewAction>(rCore); }

  static char const* GetBindingName(void)
  { return "action.add_control_flow_graph_view"; }

  virtual std::string GetName(void) const
  { return "Add CFG view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new control flow graph view"; }

  virtual std::string GetIconName(void) const
  { return "view-graph.png"; }

  virtual bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
  { return true; }

  virtual void Do(medusa::Action::RangeAddress const& rRangeAddress);
};

class ShowLabelDialog : public medusa::Action
{
public:
  ShowLabelDialog(medusa::Medusa& rCore) : Action(rCore) {}

  static SPtr Create(medusa::Medusa& rCore)
  { return std::make_shared<ShowLabelDialog>(rCore); }

  static char const* GetBindingName(void)
  { return "action.show_label_dialog"; }

  virtual std::string GetName(void) const
  { return "Show label dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a label"; }

  virtual std::string GetIconName(void) const
  { return "label.png"; }

  virtual bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
  { return true; }

  virtual void Do(medusa::Action::RangeAddress const& rRangeAddress);
};

class ShowCommentDialog : public medusa::Action
{
public:
  ShowCommentDialog(medusa::Medusa& rCore) : Action(rCore) {}

  static SPtr Create(medusa::Medusa& rCore)
  { return std::make_shared<ShowCommentDialog>(rCore); }

  static char const* GetBindingName(void)
  { return "action.show_comment_dialog"; }

  virtual std::string GetName(void) const
  { return "Show comment dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a comment"; }

  virtual std::string GetIconName(void) const
  { return "comment.png"; }

  virtual bool IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
  { return true; }

  virtual void Do(medusa::Action::RangeAddress const& rRangeAddress);
};

void AddUiActions(medusa::Action::MapType& rActions);

#endif // !__MEDUSA_PROXY_HPP__