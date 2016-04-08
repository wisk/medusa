#ifndef QMEDUSA_PROXY_HPP
#define QMEDUSA_PROXY_HPP

#include "MainWindow.hpp"
#include <medusa/event_handler.hpp>
#include <medusa/cell_action.hpp>

class AddDisassemblyViewAction : public medusa::Action
{
public:
  AddDisassemblyViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<AddDisassemblyViewAction>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.add_disassembly_view"; }

  virtual std::string GetName(void) const
  { return "Add disassembly view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new disassembly view"; }

  virtual std::string GetIconName(void) const
  { return "view-disassembly.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void);
};

class AddSemanticViewAction : public medusa::Action
{
public:
  AddSemanticViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<AddSemanticViewAction>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.add_semantic_view"; }

  virtual std::string GetName(void) const
  { return "Add semantic view"; }

  virtual std::string GetDescription(void) const
  { return "Add a new semantic view"; }

  virtual std::string GetIconName(void) const
  { return "view-semantic.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void);
};

class AddGraphViewAction : public medusa::Action
{
public:
  AddGraphViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  {
    return std::make_shared<AddGraphViewAction>(rCore, pView);
  }

  static char const* GetBindingName(void)
  {
    return "action.add_graph_view";
  }

  virtual std::string GetName(void) const
  {
    return "Add graph view";
  }

  virtual std::string GetDescription(void) const
  {
    return "Add a new graph view";
  }

  virtual std::string GetIconName(void) const
  {
    return "view-graph.png";
  }

  virtual bool IsCompatible(void) const
  {
    return true;
  }

  virtual void Do(void);
};

//class AddControlFlowGraphViewAction : public medusa::Action
//{
//public:
//  AddControlFlowGraphViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}
//
//  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
//  { return std::make_shared<AddControlFlowGraphViewAction>(rCore, pView); }
//
//  static char const* GetBindingName(void)
//  { return "action.add_control_flow_graph_view"; }
//
//  virtual std::string GetName(void) const
//  { return "Add CFG view"; }
//
//  virtual std::string GetDescription(void) const
//  { return "Add a new control flow graph view"; }
//
//  virtual std::string GetIconName(void) const
//  { return "view-graph.png"; }
//
//  virtual bool IsCompatible(void) const
//  { return true; }
//
//  virtual void Do(void);
//};

class ShowLabelDialog : public medusa::Action
{
public:
  ShowLabelDialog(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<ShowLabelDialog>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.show_label_dialog"; }

  virtual std::string GetName(void) const
  { return "Show label dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a label"; }

  virtual std::string GetIconName(void) const
  { return "label.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void);
};

class ShowCommentDialog : public medusa::Action
{
public:
  ShowCommentDialog(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<ShowCommentDialog>(rCore, pView); }

  static char const* GetBindingName(void)
  { return "action.show_comment_dialog"; }

  virtual std::string GetName(void) const
  { return "Show comment dialog"; }

  virtual std::string GetDescription(void) const
  { return "Add or edit a comment"; }

  virtual std::string GetIconName(void) const
  { return "comment.png"; }

  virtual bool IsCompatible(void) const
  { return true; }

  virtual void Do(void);
};

void AddUiActions(medusa::Action::MapType& rActions);

#endif // !QMEDUSA_PROXY_HPP