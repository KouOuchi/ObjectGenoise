#pragma once
#include <boost/exception/all.hpp>
#include <exception>

namespace og
{
namespace core
{

class exception : public boost::exception, public std::exception
{};

typedef boost::error_info<struct tag_errmsg, std::string>
  exception_message; // error message

} //namespace core
} //namespace og
