#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include <mgs/adapters/concepts/transformed_input_adapter.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename TransformedInputAdapter>
class transformed_input_adapter_iterator
{
  static_assert(adapters::concepts::is_transformed_input_adapter<
                    TransformedInputAdapter>::value,
                "template parameter must be a TransformedInputAdapter");

public:
  using value_type = typename TransformedInputAdapter::value_type;
  using difference_type = typename TransformedInputAdapter::difference_type;

  using reference =
      decltype(std::declval<TransformedInputAdapter const&>().get());
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = std::input_iterator_tag;

  transformed_input_adapter_iterator() = default;
  explicit transformed_input_adapter_iterator(TransformedInputAdapter const&);

  reference operator*() const;
  pointer operator->() const;

  transformed_input_adapter_iterator& operator++();
  transformed_input_adapter_iterator operator++(int);

  template <typename T>
  friend bool operator==(transformed_input_adapter_iterator<T> const&,
                         transformed_input_adapter_iterator<T> const&) noexcept;

private:
  TransformedInputAdapter _adapter;
};

template <typename TransformedInputAdapter>
bool operator!=(
    transformed_input_adapter_iterator<TransformedInputAdapter> const&,
    transformed_input_adapter_iterator<
        TransformedInputAdapter> const&) noexcept;
}
}
}

#include <mgs/adapters/detail/transformed_input_adapter_iterator_impl.hpp>
