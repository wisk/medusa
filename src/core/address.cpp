#include "medusa/address.hpp"

std::ostream& operator<<(std::ostream& rOstrm, medusa::Address const& rAddr)
{
  rOstrm << rAddr.ToString();
  return rOstrm;
}

std::wostream& operator<<(std::wostream& rOstrm, medusa::Address const& rAddr)
{
  rOstrm << rAddr.ToWString();
  return rOstrm;
}
