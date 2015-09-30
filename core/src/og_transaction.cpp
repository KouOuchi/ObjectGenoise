#include "og/stdafx.h"

#include "og/og_transaction.h"
#include "og/og_session.h"
#include "og/core/transaction.h"

namespace og
{

og_transaction::og_transaction(og_session& _o) :
transaction_(new og::core::transaction(*(_o.session_)))
{
}
og_transaction::og_transaction()
{
}

og_transaction::~og_transaction()
{
}

void og_transaction::rollback()
{
	transaction_->rollback();
}

void og_transaction::commit()
{
	transaction_->commit();
}

} // namespace og;

