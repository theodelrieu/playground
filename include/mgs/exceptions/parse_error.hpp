#pragma once

#include <mgs/exceptions/exception.hpp>

namespace mgs
{
class parse_error : public exception
{
public:
  using exception::exception;
};
}
