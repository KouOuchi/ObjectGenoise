#pragma once
#include "schema.h"

namespace soci
{
class transaction;
}

namespace og
{
namespace core
{

class session;

class transaction
{

public:
  OG_COREAPI transaction();
  OG_COREAPI transaction(session& _session);
  virtual ~transaction();
  OG_COREAPI void rollback();
  OG_COREAPI void commit();

private:
  scoped_ptr<soci::transaction> soci_tran_;
};

}// namespace core;
}// namespace og;