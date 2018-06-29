#pragma once

#include <mgs/exceptions/decode_error.hpp>

// TODO add prefix in message
namespace mgs
{
class unexpected_eof_error : public decode_error
{
public:
  using decode_error::decode_error;
};
}
