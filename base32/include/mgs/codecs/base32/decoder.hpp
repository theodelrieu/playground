#pragma once

#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/base32/detail/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_decoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base32
{
template <typename Iterator, typename Sentinel = Iterator>
class decoder
  : public adapters::transformer_adapter<
        binary_to_text::
            basic_decoder<Iterator, Sentinel, detail::encoding_traits<8, 5>>>
{
public:
  using adapters::transformer_adapter<binary_to_text::basic_decoder<
      Iterator,
      Sentinel,
      detail::encoding_traits<8, 5>>>::transformer_adapter;
};
}
}
}
}
