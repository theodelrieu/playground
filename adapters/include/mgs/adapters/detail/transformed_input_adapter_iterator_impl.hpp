#pragma once

#include <memory>

#include <mgs/adapters/transformed_input_adapter_iterator.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename TransformedInputAdapter>
transformed_input_adapter_iterator<TransformedInputAdapter>::
    transformed_input_adapter_iterator(TransformedInputAdapter const& e)
  : _adapter(e)
{
}

template <typename TransformedInputAdapter>
auto transformed_input_adapter_iterator<TransformedInputAdapter>::operator*()
    const -> reference
{
  return _adapter.get();
}

template <typename TransformedInputAdapter>
auto transformed_input_adapter_iterator<TransformedInputAdapter>::operator-> ()
    const -> pointer
{
  return std::addressof(**this);
}

template <typename TransformedInputAdapter>
auto transformed_input_adapter_iterator<TransformedInputAdapter>::operator++()
    -> transformed_input_adapter_iterator<TransformedInputAdapter>&
{
  _adapter.seek_forward(1);
  return *this;
}

template <typename TransformedInputAdapter>
auto transformed_input_adapter_iterator<TransformedInputAdapter>::operator++(
    int) -> transformed_input_adapter_iterator<TransformedInputAdapter>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename TransformedInputAdapter>
bool operator==(
    transformed_input_adapter_iterator<TransformedInputAdapter> const& lhs,
    transformed_input_adapter_iterator<TransformedInputAdapter> const&
        rhs) noexcept
{
  return lhs._adapter == rhs._adapter;
}

template <typename TransformedInputAdapter>
bool operator!=(
    transformed_input_adapter_iterator<TransformedInputAdapter> const& lhs,
    transformed_input_adapter_iterator<TransformedInputAdapter> const&
        rhs) noexcept
{
  return !(lhs == rhs);
}
}
}
}
