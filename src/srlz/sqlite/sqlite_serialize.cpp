#include "sqlite_serialize.hpp"

void SqliteSerialize::Open(std::string const& rFilename)
{
  std::cout << "== SqliteSerialize::Open ==" << std::endl;

  m_sDbName = rFilename;

  int rc = sqlite3_open(rFilename.c_str(), &m_pSqlite);
  if (rc != SQLITE_OK)
  {
    sqlite3_close(m_pSqlite);
    throw medusa::Exception(L"Can't open database: m_pSqlite");
  }

  //sqlite3_stmt *statement;

  const char *pSQL[4];

  pSQL[0] = "CREATE TABLE Root (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);";
  pSQL[1] = "CREATE TABLE SubEntities (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, owner INTEGER);";
  pSQL[2] = "PRAGMA synchronous = 0";
  pSQL[3] = "PRAGMA journal_mode = OFF";
  char *err = NULL;
 /*
  // execute all the sql statements
  for(int i = 0; i < 2; i++)
  {
    if(sqlite3_prepare_v2(m_pSqlite, pSQL[i], -1, &statement, 0) == SQLITE_OK)
    {
      sqlite3_step(statement);
    } 
    else
    {
      throw medusa::Exception(L"Error creating tables");
    }
  }
  std::cout << "Database created" << std::endl;

  sqlite3_finalize(statement);
  if(sqlite3_prepare_v2(m_pSqlite, "PRAGMA synchronous = 0", -1, &statement, 0) == SQLITE_OK)
    {
      sqlite3_step(statement);
    } 
    else
    {
      throw medusa::Exception(L"Error creating tables");
    }

  sqlite3_finalize(statement);
  std::cout << "PRAGMA synchronous = 0" << std::endl;
  if(sqlite3_prepare_v2(m_pSqlite, "PRAGMA journal_mode = OFF", -1, &statement, 0) == SQLITE_OK)
    {
      sqlite3_step(statement);
    } 
    else
    {
      throw medusa::Exception(L"Error creating tables");
    }
  std::cout << "PRAGMA journal_mode = OFF" << std::endl;
  */
  
  for(int i = 0; i < 4; i++)
  {
    if (sqlite3_exec(m_pSqlite, pSQL[i], NULL, NULL, &err) != SQLITE_OK)
    {
      std::cout << "Error while executing this query: " << pSQL[i] << " | " << err << std::endl;
      sqlite3_free(err);
      throw medusa::Exception(L"Error creating tables");
    }
    std::cout << pSQL[i] << " OK !" << std::endl;
  }
  std::cout << "Database created" << std::endl;

  std::cout << "PRAGMA synchronous = 0" << std::endl;
 
  std::cout << "PRAGMA journal_mode = OFF" << std::endl;
  
}

void SqliteSerialize::Load(void)
{
  throw medusa::Exception(L"SqliteSerialize::Load");
}

void SqliteSerialize::Save(void)
{
  std::cout << "Saving data..." << std::endl;
  Visitor(m_spRootEntity, m_pSqlite, -1);
}

void SqliteSerialize::Visitor(medusa::SerializeEntity::SPtr spSrlzEtt, sqlite3* rSqlite, sqlite3_int64 iOwner)
{
  //std::cout << "SqliteSerialize::Visitor " << spSrlzEtt->GetName() << std::endl;
  
  std::string statement;
  std::string sOwner;
  std::stringstream out;
  out << iOwner;
  sOwner = out.str();

  statement = (iOwner < 0) ? "INSERT INTO Root (id, name) VALUES (NULL, '" + m_sDbName + "')" : 
    "INSERT INTO SubEntities (id, name, owner) VALUES (NULL, '" + spSrlzEtt->GetName() + "', " + sOwner + ")";
  char *err = NULL;
  sqlite3_exec(m_pSqlite, statement.c_str(), NULL, NULL, &err);

  m_iId = sqlite3_last_insert_rowid(m_pSqlite);
  
  for (medusa::SerializeEntity::SPtrList::const_iterator It = spSrlzEtt->BeginSubEntities();
    It != spSrlzEtt->EndSubEntities(); ++It)
  {
    Visitor(*It, rSqlite, m_iId);
  }
}

void  SqliteSerialize::InsertIntoDB()
{

}