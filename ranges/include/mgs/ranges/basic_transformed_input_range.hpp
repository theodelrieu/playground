#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/concepts/input_transformer.hpp>
#include <mgs/concepts/sized_input_transformer.hpp>
#include <mgs/ranges/detail/transformed_input_range_iterator.hpp>

namespace mgs
{
inline namespace v1
{
namespace ranges
{
template <typename InputTransformer>
class basic_transformed_input_range : private InputTransformer
{
  static_assert(concepts::is_input_transformer<InputTransformer>::value,
                "Template parameter must be an InputTransformer");

public:
  using underlying_iterator = typename InputTransformer::underlying_iterator;
  using underlying_sentinel = typename InputTransformer::underlying_sentinel;

private:
  using buffer_t = typename InputTransformer::buffer_type;
  using buffer_iterator = meta::result_of_begin<buffer_t>;

  friend detail::transformed_input_range_iterator<basic_transformed_input_range>;

public:
  using iterator = detail::transformed_input_range_iterator<basic_transformed_input_range>;
  using value_type = typename std::iterator_traits<buffer_iterator>::value_type;

  basic_transformed_input_range() = default;
  basic_transformed_input_range(underlying_iterator begin,
                                underlying_sentinel end);

  template <typename OutputIterator>
  std::size_t read(OutputIterator out, std::size_t n);

  iterator begin() const;
  iterator end() const;

  template <typename T = InputTransformer,
            typename = std::enable_if_t<
                concepts::is_sized_input_transformer<T>::value>>
  std::size_t max_transformed_size() const;

private:
  buffer_t _buffer{};
  typename std::iterator_traits<buffer_iterator>::difference_type _index{};

  value_type const& _get() const;
  void _seek_forward();
  void _transform_input();
  std::size_t _buffer_size() const;
};
}
}
}

#include <mgs/ranges/detail/basic_transformed_input_range_impl.hpp>
