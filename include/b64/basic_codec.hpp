#pragma once

#include <type_traits>

#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>

namespace b64
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
