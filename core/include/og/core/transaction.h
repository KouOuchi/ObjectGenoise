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
  transaction();
  transaction(session& _session);
  virtual ~transaction();
  void rollback();
  void commit();

private:
  scoped_ptr<soci::transaction> soci_tran_;
};

}// namespace core;
}// namespace og;