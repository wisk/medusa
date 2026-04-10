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

  static char const* GetBindingName()
  { return "action.add_disassembly_view"; }

  std::string GetName() const override
  { return "Add disassembly view"; }

  std::string GetDescription() const override
  { return "Add a new disassembly view"; }

  std::string GetIconName() const override
  { return "view-disassembly.png"; }

  bool IsCompatible() const override
  { return true; }

  void Do() override;
};

class AddSemanticViewAction : public medusa::Action
{
public:
  AddSemanticViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<AddSemanticViewAction>(rCore, pView); }

  static char const* GetBindingName()
  { return "action.add_semantic_view"; }

  std::string GetName() const override
  { return "Add semantic view"; }

  std::string GetDescription() const override
  { return "Add a new semantic view"; }

  std::string GetIconName() const override
  { return "view-semantic.png"; }

  bool IsCompatible() const override
  { return true; }

  void Do() override;
};

class AddGraphViewAction : public medusa::Action
{
public:
  AddGraphViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  {
    return std::make_shared<AddGraphViewAction>(rCore, pView);
  }

  static char const* GetBindingName()
  {
    return "action.add_graph_view";
  }

  std::string GetName() const override
  {
    return "Add graph view";
  }

  std::string GetDescription() const override
  {
    return "Add a new graph view";
  }

  std::string GetIconName() const override
  {
    return "view-graph.png";
  }

  bool IsCompatible() const override
  {
    return true;
  }

  void Do() override;
};

//class AddControlFlowGraphViewAction : public medusa::Action
//{
//public:
//  AddControlFlowGraphViewAction(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}
//
//  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
//  { return std::make_shared<AddControlFlowGraphViewAction>(rCore, pView); }
//
//  static char const* GetBindingName()
//  { return "action.add_control_flow_graph_view"; }
//
//  std::string GetName() const override
//  { return "Add CFG view"; }
//
//  std::string GetDescription() const override
//  { return "Add a new control flow graph view"; }
//
//  std::string GetIconName() const override
//  { return "view-graph.png"; }
//
//  bool IsCompatible() const override
//  { return true; }
//
//  void Do() override;
//};

class ShowLabelDialog : public medusa::Action
{
public:
  ShowLabelDialog(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<ShowLabelDialog>(rCore, pView); }

  static char const* GetBindingName()
  { return "action.show_label_dialog"; }

  std::string GetName() const override
  { return "Show label dialog"; }

  std::string GetDescription() const override
  { return "Add or edit a label"; }

  std::string GetIconName() const override
  { return "label.png"; }

  bool IsCompatible() const override
  { return true; }

  void Do() override;
};

class ShowCommentDialog : public medusa::Action
{
public:
  ShowCommentDialog(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView) : Action(rCore, pView) {}

  static SPType Create(medusa::Medusa& rCore, medusa::FullDisassemblyView* pView)
  { return std::make_shared<ShowCommentDialog>(rCore, pView); }

  static char const* GetBindingName()
  { return "action.show_comment_dialog"; }

  std::string GetName() const override
  { return "Show comment dialog"; }

  std::string GetDescription() const override
  { return "Add or edit a comment"; }

  std::string GetIconName() const override
  { return "comment.png"; }

  bool IsCompatible() const override
  { return true; }

  void Do() override;
};

void AddUiActions(medusa::Action::MapType& rActions);

#endif // !QMEDUSA_PROXY_HPP