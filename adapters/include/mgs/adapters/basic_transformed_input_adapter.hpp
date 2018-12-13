#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/adapters/concepts/input_transformer.hpp>
#include <mgs/adapters/transformed_input_adapter_iterator.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename InputTransformer>
class basic_transformed_input_adapter : private InputTransformer
{
  static_assert(concepts::is_input_transformer<InputTransformer>::value,
                "Template parameter must be an InputTransformer");

public:
  using underlying_iterator = typename InputTransformer::underlying_iterator;
  using underlying_sentinel = typename InputTransformer::underlying_sentinel;

private:
  using buffer_t = typename InputTransformer::buffer_type;
  using buffer_iterator = meta::result_of_begin<buffer_t>;

public:
  using iterator = transformed_input_adapter_iterator<basic_transformed_input_adapter>;
  using difference_type = std::streamoff;
  using value_type = typename std::iterator_traits<buffer_iterator>::value_type;

  basic_transformed_input_adapter() = default;
  basic_transformed_input_adapter(underlying_iterator begin,
                                  underlying_sentinel end);

  value_type const& get() const;
  void seek_forward(difference_type n);

  template <typename OutputIterator>
  std::size_t write(OutputIterator out, std::size_t n);

  template <typename OutputIterator>
  std::size_t write(OutputIterator out);

  iterator begin() const;
  iterator end() const;

private:
  buffer_t _buffer{};
  typename std::iterator_traits<buffer_iterator>::difference_type _index{};

  void _transform_input();

  template <typename T>
  friend bool operator==(basic_transformed_input_adapter<T> const& lhs,
                         basic_transformed_input_adapter<T> const& rhs);
};

template <typename T>
bool operator!=(basic_transformed_input_adapter<T> const& lhs,
                basic_transformed_input_adapter<T> const& rhs);
}
}
}

#include <mgs/adapters/detail/basic_transformed_input_adapter_impl.hpp>
