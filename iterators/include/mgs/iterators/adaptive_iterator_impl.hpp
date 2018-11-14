#pragma once

#include <memory>

#include <mgs/iterators/adaptive_iterator.hpp>

namespace mgs
{
inline namespace v1
{
namespace iterators
{
// input iterator
template <typename Adapter, typename Tag>
adaptive_iterator<Adapter, Tag>::adaptive_iterator(Adapter const& e)
  : _adapter(e)
{
}

template <typename Adapter, typename Tag>
auto adaptive_iterator<Adapter, Tag>::operator*() const -> reference
{
  return _adapter.get();
}

template <typename Adapter, typename Tag>
auto adaptive_iterator<Adapter, Tag>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename Adapter, typename Tag>
auto adaptive_iterator<Adapter, Tag>::operator++()
    -> adaptive_iterator<Adapter, Tag>&
{
  _adapter.seek_forward(1);
  return *this;
}

template <typename Adapter, typename Tag>
auto adaptive_iterator<Adapter, Tag>::operator++(int)
    -> adaptive_iterator<Adapter, Tag>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Adapter, typename Tag>
bool operator==(adaptive_iterator<Adapter, Tag> const& lhs,
                adaptive_iterator<Adapter, Tag> const& rhs) noexcept
{
  return lhs._adapter == rhs._adapter;
}

template <typename Adapter, typename Tag>
bool operator!=(adaptive_iterator<Adapter, Tag> const& lhs,
                adaptive_iterator<Adapter, Tag> const& rhs) noexcept
{
  return !(lhs == rhs);
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator--()
    -> adaptive_iterator<Adapter, Tag>&
{
  _adapter.seek_backward(-1);
  return *this;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator--(int)
    -> adaptive_iterator<Adapter, Tag>
{
  auto ret = *this;
  --(*this);
  return ret;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator+=(difference_type n)
    -> adaptive_iterator<Adapter, Tag>&
{
  if (n > 0)
    _adapter.seek_forward(n);
  else if (n < 0)
    _adapter.seek_backward(n);
  return *this;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator-=(difference_type n)
    -> adaptive_iterator<Adapter, Tag>&
{
  return *this += -n;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator+(difference_type n) const
    -> adaptive_iterator<Adapter, Tag>
{
  auto it = *this;
  it += n;
  return it;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator-(difference_type n) const
    -> adaptive_iterator<Adapter, Tag>
{
  auto it = *this;
  it -= n;
  return it;
}

template <typename Adapter>
adaptive_iterator<Adapter, std::random_access_iterator_tag> operator+(
    typename adaptive_iterator<Adapter,
                               std::random_access_iterator_tag>::difference_type
        n,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& it)
{
  auto tmp = it;
  tmp += n;
  return tmp;
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator-(
    adaptive_iterator<Adapter, Tag> const& it) const -> difference_type
{
  return _adapter.pos() - it._adapter.pos();
}

template <typename Adapter, typename Tag>
template <typename, typename>
auto adaptive_iterator<Adapter, Tag>::operator[](difference_type n) const
    -> value_type
{
  return *(*this + n);
}

template <typename Adapter>
bool operator<(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return lhs._adapter.pos() < rhs._adapter.pos();
}

template <typename Adapter>
bool operator>(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return rhs < lhs;
}

template <typename Adapter>
bool operator>=(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return !(lhs < rhs);
}

template <typename Adapter>
bool operator<=(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return !(lhs > rhs);
}
}
}
}
