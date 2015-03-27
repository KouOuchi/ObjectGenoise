#include "og/net/stdafx.h"
#include "og/net/OGException.h"

namespace og
{
namespace net
{

OGException::OGException()
{
}

OGException::~OGException()
{
}

OGException::OGException(System::String^ _message) :
  Exception(_message)
{
}


}
}