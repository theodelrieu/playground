#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/adapters/input_adapter_iterator.hpp>
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
template <typename Impl>
class basic_input_adapter : private Impl
{
public:
  using underlying_iterator = typename Impl::underlying_iterator;
  using underlying_sentinel = typename Impl::underlying_sentinel;

private:
  using buffer_t = typename Impl::value_type;
  using buffer_iterator = meta::result_of_begin<buffer_t>;

public:
  using iterator = input_adapter_iterator<basic_input_adapter>;
  using difference_type = std::streamoff;
  using value_type = typename std::iterator_traits<buffer_iterator>::value_type;

  basic_input_adapter() = default;
  basic_input_adapter(underlying_iterator begin, underlying_sentinel end);

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

  void _process_input();

  template <typename T>
  friend bool operator==(basic_input_adapter<T> const& lhs,
                         basic_input_adapter<T> const& rhs);
};

template <typename T>
bool operator!=(basic_input_adapter<T> const& lhs,
                basic_input_adapter<T> const& rhs);
}
}
}

#include <mgs/adapters/detail/basic_input_adapter_impl.hpp>
