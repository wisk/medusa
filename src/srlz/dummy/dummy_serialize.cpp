#include "dummy_serialize.hpp"

#include <medusa/exception.hpp>

#include <iostream>

void DummySerialize::Open(std::string const& rFilename)
{
  m_Ofstrm.open(rFilename.c_str());
}

void DummySerialize::Load(void)
{
  throw medusa::Exception_NotImplemented(L"DummySerialize::Load");
}

void DummySerialize::Save(void)
{
  Visitor(m_spRootEntity, m_Ofstrm);
}

void DummySerialize::Visitor(medusa::SerializeEntity::SPtr spSrlzEtt, std::ofstream& rOfs)
{
  rOfs      << spSrlzEtt->GetName() << std::endl;

  for (medusa::SerializeEntity::SerializeFields::const_iterator It = spSrlzEtt->BeginField();
    It != spSrlzEtt->EndField(); ++It)
  {
    rOfs << "  " << It->first << ": ";
    boost::apply_visitor(OStreamVisitor(rOfs), It->second);
    rOfs << std::endl;
  }

  for (medusa::SerializeEntity::SPtrList::const_iterator It = spSrlzEtt->BeginSubEntities();
    It != spSrlzEtt->EndSubEntities(); ++It)
  {
    Visitor(*It, rOfs);
  }
}
