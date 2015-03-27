#pragma once

namespace og
{
namespace net
{

using System::Exception;

public ref class OGException : public Exception
{
public:
  OGException();
  virtual ~OGException();

  OGException(System::String^ _message);
};

}
}
