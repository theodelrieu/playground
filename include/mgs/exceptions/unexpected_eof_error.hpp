#pragma once

#include <mgs/exceptions/parse_error.hpp>

// TODO add prefix in message
namespace mgs
{
class unexpected_eof_error : public parse_error
{
public:
  using parse_error::parse_error;
};
}
