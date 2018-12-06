#pragma once

#include <memory>

#include <mgs/adapters/input_adapter_iterator.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
// input iterator
template <typename InputAdapter>
input_adapter_iterator<InputAdapter>::input_adapter_iterator(
    InputAdapter const& e)
  : _adapter(e)
{
}

template <typename InputAdapter>
auto input_adapter_iterator<InputAdapter>::operator*() const -> reference
{
  return _adapter.get();
}

template <typename InputAdapter>
auto input_adapter_iterator<InputAdapter>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename InputAdapter>
auto input_adapter_iterator<InputAdapter>::operator++()
    -> input_adapter_iterator<InputAdapter>&
{
  _adapter.seek_forward(1);
  return *this;
}

template <typename InputAdapter>
auto input_adapter_iterator<InputAdapter>::operator++(int)
    -> input_adapter_iterator<InputAdapter>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename InputAdapter>
bool operator==(input_adapter_iterator<InputAdapter> const& lhs,
                input_adapter_iterator<InputAdapter> const& rhs) noexcept
{
  return lhs._adapter == rhs._adapter;
}

template <typename InputAdapter>
bool operator!=(input_adapter_iterator<InputAdapter> const& lhs,
                input_adapter_iterator<InputAdapter> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}
}
}
