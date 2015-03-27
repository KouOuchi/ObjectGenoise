
#include "og/net/stdafx.h"

#include "og/net/OGSessionParameter.h"

//#include "og/og_session_parameter.h"

namespace og
{
namespace net
{
#ifndef net

OGSessionParameter::OGSessionParameter(OGSharedPtr<og::og_session_parameter>
                                       _p_ptr) :
  og_session_parameter_(_p_ptr)
{
}
OGSessionParameter::~OGSessionParameter()
{}
#endif
}
}

