#pragma once

#include <type_traits>

#include <mgs/codecs/detail/default_converter.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/codecs/concepts/input_source.hpp>

namespace mgs
{
namespace codecs
{
template <typename T, typename>
struct output_traits
{
  template <typename IS, typename U = T>
  static auto create(codecs::input_source<IS>& is)
      -> decltype(detail::default_converter<U>::create(is))
  {
    return detail::default_converter<U>::create(is);
  }
};
}
}
