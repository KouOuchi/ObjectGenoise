#include "og/net/stdafx.h"
#include "og/net/OGTransaction.h"
#include "og/net/OGSession.h"

#include <og/og_transaction.h>

namespace og
{
namespace net
{

OGTransaction::OGTransaction(OGSession^ _sess) :
	og_transaction_(new og::og_transaction(*(_sess->og_session_.get())))
{
}

OGTransaction::OGTransaction()
{}

OGTransaction::~OGTransaction()
{
}

void OGTransaction::rollback()
{
  og_transaction_->rollback();
}

void OGTransaction::commit()
{
  og_transaction_->commit();
}

}; // namespace net
}; // namespace og
