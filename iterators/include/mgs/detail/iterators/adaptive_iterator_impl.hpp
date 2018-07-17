#pragma once

namespace mgs
{
inline namespace v1
{
namespace detail
{
// input iterator
template <typename Adapter, typename Tag>
adaptive_iterator<Adapter, Tag>::adaptive_iterator(Adapter const& e)
  : _encoder(e)
{
}

template <typename Adapter, typename Tag>
auto adaptive_iterator<Adapter, Tag>::operator*() const -> reference
{
  return _encoder.get();
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
  _encoder.seek_forward(1);
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
  return lhs._encoder == rhs._encoder;
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
  _encoder.seek_backward(-1);
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
    _encoder.seek_forward(n);
  else if (n < 0)
    _encoder.seek_backward(n);
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
  return _encoder.pos() - it._encoder.pos();
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
  return lhs._encoder.pos() < rhs._encoder.pos();
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
