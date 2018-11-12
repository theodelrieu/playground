#pragma once

#include <ios>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/base16/detail/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base16
{
template <typename Iterator, typename Sentinel = Iterator>
class encoder
  : public adapters::transformer_adapter<
        binary_to_text::
            basic_encoder<Iterator, Sentinel, detail::encoding_traits<>>>
{
public:
  using adapters::transformer_adapter<binary_to_text::basic_encoder<
      Iterator,
      Sentinel,
      detail::encoding_traits<>>>::transformer_adapter;
};
}
}
}
}
