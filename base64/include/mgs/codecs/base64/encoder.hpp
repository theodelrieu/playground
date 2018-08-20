#pragma once

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/codecs/base64/detail/encoding_traits.hpp>
#include <mgs/codecs/binary_to_text/basic_encoder.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace base64
{
template <typename Iterator, typename Sentinel = Iterator>
class encoder
  : public adapters::transformer_adapter<
        binary_to_text::
            basic_encoder<Iterator, Sentinel, detail::encoding_traits<3, 4>>>
{
public:
  using adapters::transformer_adapter<binary_to_text::basic_encoder<
      Iterator,
      Sentinel,
      detail::encoding_traits<3, 4>>>::transformer_adapter;
};
}
}
}
}
