#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename InputAdapter>
class input_adapter_iterator
{
  static_assert(adapters::concepts::is_input_adapter<InputAdapter>::value,
                "InputAdapter is not an InputAdapter");

public:
  using value_type = typename InputAdapter::value_type;
  using difference_type = typename InputAdapter::difference_type;

  using reference = decltype(std::declval<InputAdapter const&>().get());
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = std::input_iterator_tag;

  input_adapter_iterator() = default;
  explicit input_adapter_iterator(InputAdapter const&);

  reference operator*() const;
  pointer operator->() const;

  input_adapter_iterator& operator++();
  input_adapter_iterator operator++(int);

  template <typename T>
  friend bool operator==(input_adapter_iterator<T> const&,
                         input_adapter_iterator<T> const&) noexcept;

private:
  InputAdapter _adapter;
};

template <typename InputAdapter>
bool operator!=(input_adapter_iterator<InputAdapter> const&,
                input_adapter_iterator<InputAdapter> const&) noexcept;

}
}
}

#include <mgs/adapters/detail/input_adapter_iterator_impl.hpp>
