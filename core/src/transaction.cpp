#include "og/core/transaction.h"
#include "og/core/session.h"

#include "soci.h"

namespace og
{
namespace core
{

transaction::transaction(session& _session) 
  : soci_tran_(new soci::transaction(*(_session.soci_session_.get())))
{ }

transaction::~transaction()
{ }

void transaction::rollback()
{
  soci_tran_->rollback();
}

void transaction::commit()
{
  soci_tran_->commit();
}

}// namespace core;
}// namespace og;