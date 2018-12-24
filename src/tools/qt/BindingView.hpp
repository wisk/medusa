#ifndef QMEDUSA_BINDING_VIEW_HPP
#define QMEDUSA_BINDING_VIEW_HPP

#include <medusa/medusa.hpp>
#include <medusa/binding.hpp>
#include <QWidget>
#include "ui_BindingView.h"

class BindingView : public QWidget, public Ui::BindingView
{
public:
  BindingView(medusa::Medusa& rCore);
  ~BindingView(void);

private:
  medusa::Medusa& m_rCore;
  medusa::Binding::SPType m_spCurBinding;
};

#endif // !QMEDUSA_BINDING_VIEW_HPP