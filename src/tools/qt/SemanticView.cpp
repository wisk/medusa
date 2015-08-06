#include "SemanticView.hpp"

SemanticView::SemanticView(QWidget *parent, medusa::Medusa& core, medusa::Address const& funcAddr)
  : QTextEdit(parent)
{
  setReadOnly(true);

  auto const& doc = core.GetDocument();

  auto func = dynamic_cast<medusa::Function const*>(doc.GetMultiCell(funcAddr));
  if (func == nullptr)
    return;

  medusa::ControlFlowGraph cfg(doc);
  if (!core.BuildControlFlowGraph(funcAddr, cfg))
    return;

  cfg.ForEachInstruction([&](medusa::Address const& addr)
  {
    QString addrStr = QString::fromStdString(addr.ToString()) + ": ";
    auto insn = std::dynamic_pointer_cast<medusa::Instruction const>(core.GetCell(addr));
    if (insn == nullptr)
    {
      append(addrStr + "(not an instruction)\n");
      return;
    }

    auto const& semList = insn->GetSemantic();
    if (semList.empty())
    {
      append(addrStr + "(no semantic available)\n");
      return;
    }

    for (auto sem = std::begin(semList); sem != std::end(semList); ++sem)
    {
      append(addrStr + QString::fromStdString((*sem)->ToString()));
    }
  });
}