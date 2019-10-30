#pragma once

#include <type_traits>

#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S = I>
class default_input_source
{
  static_assert(meta::is_input_iterator<I>::value,
                "I does not model meta::input_iterator");
  static_assert(meta::is_sentinel_for<S, I>::value,
                "S does not model meta::sentinel_for<I>");

public:
  using element_type = meta::iter_value_t<I>;

  default_input_source(I begin, S end);

  template <typename OutputIterator,
            typename = std::enable_if_t<
                meta::is_output_iterator<OutputIterator, element_type>::value>>
  meta::ssize_t read(OutputIterator out, meta::ssize_t n);

  template <
      typename S2 = S,
      typename = std::enable_if_t<meta::is_sized_sentinel_for<S2, I>::value>>
  meta::ssize_t max_remaining_size() const;

private:
  I _current;
  S _end;
};
}
}

#include <mgs/codecs/detail/default_input_source_impl.hpp>
