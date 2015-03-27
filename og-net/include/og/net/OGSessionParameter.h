#pragma once

#include "og/net/OGSchema.h"
#include "og/net/OGSharedPtr.h"

namespace og
{

class og_session_parameter;

namespace net
{
#ifndef net

public ref class OGSessionParameter
{
public:
  OGSessionParameter(OGSharedPtr<og::og_session_parameter> _p_ptr);
  virtual ~OGSessionParameter();



internal:
  OGSharedPtr<og::og_session_parameter> og_session_parameter_;

};
#endif
}
}
