#pragma once
#include <iostream>

#include <cassert>
#include <tuple>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename InputTransformer>
transformer_adapter<InputTransformer>::transformer_adapter(
    typename InputTransformer::underlying_iterator begin,
    typename InputTransformer::underlying_sentinel end)
  : InputTransformer(std::move(begin), std::move(end))
{
  _process_input();
}

template <typename InputTransformer>
auto transformer_adapter<InputTransformer>::get() const -> value_type const&
{
  using std::begin;

  return *(begin(_transformed) + _index);
}

template <typename InputTransformer>
void transformer_adapter<InputTransformer>::read_block()
{
  _process_input();
}

template <typename InputTransformer>
auto const& transformer_adapter<InputTransformer>::block() const
{
  return _transformed;
}

template <typename InputTransformer>
void transformer_adapter<InputTransformer>::seek_forward(difference_type n)
{
  assert(n > 0);

  using std::begin;
  using std::end;

  auto const end_it = end(_transformed);

  while (n-- > 0)
  {
    ++_index;
    if ((begin(_transformed) + _index) == end_it)
      _process_input();
  }
}

template <typename InputTransformer>
void transformer_adapter<InputTransformer>::_process_input()
{
  using std::begin;

  _transformed = static_cast<InputTransformer&>(*this)();
  _index = 0;
}

template <typename InputTransformer>
auto transformer_adapter<InputTransformer>::begin() const -> iterator
{
  return iterator{*this};
}

template <typename InputTransformer>
auto transformer_adapter<InputTransformer>::end() const -> iterator
{
  return iterator{{}};
}

template <typename T>
bool operator==(transformer_adapter<T> const& lhs,
                transformer_adapter<T> const& rhs)
{
  using std::begin;
  using std::end;

  auto const lhs_transformed_current = begin(lhs._transformed) + lhs._index;
  auto const rhs_transformed_current = begin(rhs._transformed) + rhs._index;
  auto const lhs_transformed_end = end(lhs._transformed);
  auto const rhs_transformed_end = end(rhs._transformed);

  // a bit similar to std::istreambuf_iterator::equal
  if (lhs_transformed_current == lhs_transformed_end ||
      rhs_transformed_current == rhs_transformed_end)
  {
    return lhs_transformed_current == lhs_transformed_end &&
           rhs_transformed_current == rhs_transformed_end;
  }
  return std::tie(lhs._index) == std::tie(rhs._index);
}

template <typename T>
bool operator!=(transformer_adapter<T> const& lhs,
                transformer_adapter<T> const& rhs)
{
  return !(lhs == rhs);
}
}
}
}
