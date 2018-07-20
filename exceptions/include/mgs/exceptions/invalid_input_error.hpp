#pragma once

#include <mgs/exceptions/decode_error.hpp>

namespace mgs
{
namespace exceptions
{
inline namespace v1
{
class invalid_input_error : public decode_error
{
public:
  using decode_error::decode_error;
};
}
}
}
