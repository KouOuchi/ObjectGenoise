#pragma once
#include <string>
#include <boost/shared_ptr.hpp>
#include "og/og_session.h"
#include "og/core/session.h"

namespace og
{
namespace core
{
class transaction;
}
using namespace boost;
using namespace std;


class og_transaction
{
public:
  og_transaction(og_session& _session);
  virtual ~og_transaction();

  void rollback();
  void commit();

private:
  og_transaction();
  boost::scoped_ptr<og::core::transaction> transaction_;
};

}// namespace og
