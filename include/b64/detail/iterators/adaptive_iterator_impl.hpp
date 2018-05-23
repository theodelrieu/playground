#pragma once

namespace b64
{
namespace detail
{
// input iterator
template <typename Encoder, typename Tag>
adaptive_iterator<Encoder, Tag>::adaptive_iterator(Encoder const& e)
  : _encoder(e)
{
}

template <typename Encoder, typename Tag>
auto adaptive_iterator<Encoder, Tag>::operator*() const -> reference
{
  return _encoder.get();
}

template <typename Encoder, typename Tag>
auto adaptive_iterator<Encoder, Tag>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename Encoder, typename Tag>
auto adaptive_iterator<Encoder, Tag>::operator++()
    -> adaptive_iterator<Encoder, Tag>&
{
  _encoder.seek_forward(1);
  return *this;
}

template <typename Encoder, typename Tag>
auto adaptive_iterator<Encoder, Tag>::operator++(int)
    -> adaptive_iterator<Encoder, Tag>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Encoder, typename Tag>
bool operator==(adaptive_iterator<Encoder, Tag> const& lhs,
                adaptive_iterator<Encoder, Tag> const& rhs) noexcept
{
  return lhs._encoder == rhs._encoder;
}

template <typename Encoder, typename Tag>
bool operator!=(adaptive_iterator<Encoder, Tag> const& lhs,
                adaptive_iterator<Encoder, Tag> const& rhs) noexcept
{
  return !(lhs == rhs);
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator--()
    -> adaptive_iterator<Encoder, Tag>&
{
  _encoder.seek_backward(-1);
  return *this;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator--(int)
    -> adaptive_iterator<Encoder, Tag>
{
  auto ret = *this;
  --(*this);
  return ret;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator+=(difference_type n)
    -> adaptive_iterator<Encoder, Tag>&
{
  if (n > 0)
    _encoder.seek_forward(n);
  else if (n < 0)
    _encoder.seek_backward(n);
  return *this;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator-=(difference_type n)
    -> adaptive_iterator<Encoder, Tag>&
{
  return *this += -n;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator+(difference_type n) const
    -> adaptive_iterator<Encoder, Tag>
{
  auto it = *this;
  it += n;
  return it;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator-(difference_type n) const
    -> adaptive_iterator<Encoder, Tag>
{
  auto it = *this;
  it -= n;
  return it;
}

template <typename Encoder>
adaptive_iterator<Encoder, std::random_access_iterator_tag> operator+(
    typename adaptive_iterator<Encoder,
                               std::random_access_iterator_tag>::difference_type
        n,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& it)
{
  auto tmp = it;
  tmp += n;
  return tmp;
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator-(
    adaptive_iterator<Encoder, Tag> const& it) const -> difference_type
{
  return _encoder.pos() - it._encoder.pos();
}

template <typename Encoder, typename Tag>
template <typename, typename>
auto adaptive_iterator<Encoder, Tag>::operator[](difference_type n) const
    -> value_type
{
  return *(*this + n);
}

template <typename Encoder>
bool operator<(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return lhs._encoder.pos() < rhs._encoder.pos();
}

template <typename Encoder>
bool operator>(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return rhs < lhs;
}

template <typename Encoder>
bool operator>=(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return !(lhs < rhs);
}

template <typename Encoder>
bool operator<=(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& lhs,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&
        rhs) noexcept
{
  return !(lhs > rhs);
}
}
}
