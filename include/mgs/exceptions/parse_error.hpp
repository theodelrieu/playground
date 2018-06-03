#pragma once

#include <mgs/exceptions/exception.hpp>

namespace mgs
{
class parse_error : exception
{
public:
  using exception::exception;
};
}
