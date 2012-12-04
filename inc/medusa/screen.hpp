#ifndef __MEDUSA_SCREEN_HPP__
#define __MEDUSA_SCREEN_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/printer.hpp"
#include "medusa/medusa.hpp"

MEDUSA_NAMESPACE_BEGIN

class Screen
{
public:
  Screen(Medusa& rCore, u16 Width, u16 Height);


private:
  Medusa& m_rCore;
  u16 m_Width, m_Height;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SCREEN_HPP__