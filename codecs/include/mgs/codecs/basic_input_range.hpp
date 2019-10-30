#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>
#include <mgs/codecs/detail/basic_input_range_iterator.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/iter_difference_t.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename InputSource>
class basic_input_range
{
  static_assert(
      is_input_source<InputSource>::value,
      "basic_input_range's parameter must model codecs::input_source");

  friend detail::basic_input_range_iterator<basic_input_range>;
  using iterator = detail::basic_input_range_iterator<basic_input_range>;

public:
  using element_type = typename InputSource::element_type;

  explicit basic_input_range(InputSource const&);
  explicit basic_input_range(InputSource&&);

  iterator begin();
  iterator end() const;

  template <typename OutputIterator,
            typename = std::enable_if_t<
                meta::is_output_iterator<OutputIterator, element_type>::value>>
  meta::ssize_t read(OutputIterator out, meta::ssize_t n) const;

  template <typename T = InputSource,
            typename = std::enable_if_t<is_sized_input_source<T>::value>>
  meta::ssize_t max_remaining_size() const;

private:
  InputSource _input_source;
};
}
}

#include <mgs/codecs/detail/basic_input_range_impl.hpp>
