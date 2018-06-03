#pragma once

#include <type_traits>

#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterator.hpp>
#include <mgs/detail/meta/concepts/sentinel.hpp>

namespace mgs
{
template <typename Encoder,
          typename Decoder,
          typename = std::enable_if_t<detail::is_iterable<Encoder>::value &&
                                      detail::is_iterable<Decoder>::value>>
class basic_codec
{
  template <typename T>
  void f();
};
}
