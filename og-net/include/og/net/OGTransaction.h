#pragma once

#include "og/net/OGSharedPtr.h"

namespace og
{

class og_transaction;

namespace net
{
using namespace System;
using namespace System::Collections::Generic;

ref class OGSession;

public ref class OGTransaction 
{
public:
  OGTransaction(OGSession^ _sess);
  virtual ~OGTransaction();

  void rollback();
  void commit();

internal:
  OGSharedPtr<og::og_transaction> og_transaction_;
  OGTransaction();

private:

};

}
}