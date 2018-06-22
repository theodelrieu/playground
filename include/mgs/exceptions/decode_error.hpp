#pragma once

#include <string>

#include <mgs/exceptions/exception.hpp>

namespace mgs
{
class decode_error : public exception
{
public:
  decode_error(char const* encoding_name, std::string const& msg)
    : exception(encoding_name + std::string{": "} + msg)
  {
  }
};
}
