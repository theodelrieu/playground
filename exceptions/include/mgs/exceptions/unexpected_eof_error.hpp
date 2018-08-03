#pragma once

#include <mgs/exceptions/decode_error.hpp>

namespace mgs
{
inline namespace v1
{
namespace exceptions
{
class unexpected_eof_error : public decode_error
{
public:
  using decode_error::decode_error;
};
}
}
}
