#pragma once

#include <mgs/exceptions/parse_error.hpp>

namespace mgs
{
class invalid_input_error : public parse_error
{
public:
  using parse_error::parse_error;
};
}
