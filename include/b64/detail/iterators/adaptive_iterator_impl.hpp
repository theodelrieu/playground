#pragma once

namespace b64
{
namespace detail
{
// input iterator
template <typename Encoder>
adaptive_input_iterator<Encoder>::adaptive_iterator(Encoder const& e)
  : _encoder(e)
{
}

template <typename Encoder>
auto adaptive_input_iterator<Encoder>::operator*() const -> reference
{
  return _encoder.get();
}

template <typename Encoder>
auto adaptive_input_iterator<Encoder>::operator->() const -> pointer
{
  return std::addressof(**this);
}

template <typename Encoder>
auto adaptive_input_iterator<Encoder>::operator++()
    -> adaptive_input_iterator<Encoder>&
{
  _encoder.seek(1);
  return *this;
}

template <typename Encoder>
auto adaptive_input_iterator<Encoder>::operator++(int)
    -> adaptive_input_iterator<Encoder>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Encoder>
bool operator==(adaptive_input_iterator<Encoder> const& lhs,
                adaptive_input_iterator<Encoder> const& rhs) noexcept
{
  return lhs._encoder == rhs._encoder;
}

template <typename Encoder>
bool operator!=(adaptive_input_iterator<Encoder> const& lhs,
                adaptive_input_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs == rhs);
}

// forward iterator
template <typename Encoder>
auto adaptive_forward_iterator<Encoder>::operator++()
    -> adaptive_forward_iterator<Encoder>&
{
  base::operator++();
  return *this;
}

template <typename Encoder>
auto adaptive_forward_iterator<Encoder>::operator++(int)
    -> adaptive_forward_iterator<Encoder>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Encoder>
bool operator==(adaptive_forward_iterator<Encoder> const& lhs,
                adaptive_forward_iterator<Encoder> const& rhs) noexcept
{
  return lhs._encoder == rhs._encoder;
}

template <typename Encoder>
bool operator!=(adaptive_forward_iterator<Encoder> const& lhs,
                adaptive_forward_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs == rhs);
}

// bidirectional iterator
template <typename Encoder>
auto adaptive_bidirectional_iterator<Encoder>::operator++()
    -> adaptive_bidirectional_iterator<Encoder>&
{
  base::operator++();
  return *this;
}

template <typename Encoder>
auto adaptive_bidirectional_iterator<Encoder>::operator++(int)
    -> adaptive_bidirectional_iterator<Encoder>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Encoder>
auto adaptive_bidirectional_iterator<Encoder>::operator--()
    -> adaptive_bidirectional_iterator<Encoder>&
{
  // for some reason base:: has to be specified...
  base::_encoder.seek(-1);
  return *this;
}


template <typename Encoder>
auto adaptive_bidirectional_iterator<Encoder>::operator--(int)
    -> adaptive_bidirectional_iterator<Encoder>
{
  auto ret = *this;
  --(*this);
  return ret;
}

template <typename Encoder>
bool operator==(adaptive_bidirectional_iterator<Encoder> const& lhs,
                adaptive_bidirectional_iterator<Encoder> const& rhs) noexcept
{
  return lhs._encoder == rhs._encoder;
}

template <typename Encoder>
bool operator!=(adaptive_bidirectional_iterator<Encoder> const& lhs,
                adaptive_bidirectional_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs == rhs);
}

// random-access iterator
template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator++()
    -> adaptive_random_access_iterator<Encoder>&
{
  base::operator++();
  return *this;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator++(int)
    -> adaptive_random_access_iterator<Encoder>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator--()
    -> adaptive_random_access_iterator<Encoder>&
{
  base::operator++();
  return *this;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator--(int)
    -> adaptive_random_access_iterator<Encoder>
{
  auto ret = *this;
  --(*this);
  return ret;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator+=(difference_type n)
    -> adaptive_random_access_iterator<Encoder>&
{
  base::_encoder.seek(n);
  return *this;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator-=(difference_type n)
    -> adaptive_random_access_iterator<Encoder>&
{
  return *this += -n;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator+(difference_type n)
    -> adaptive_random_access_iterator<Encoder>
{
  auto it = *this;
  return it += n;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator-(difference_type n)
    -> adaptive_random_access_iterator<Encoder>
{
  auto it = *this;
  return it -= n;
}

template <typename Encoder>
auto operator+(
    typename adaptive_random_access_iterator<Encoder>::difference_type n,
    adaptive_random_access_iterator<Encoder>& it)
{
  auto tmp = it;
  return tmp += n;
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator-(
    adaptive_random_access_iterator<Encoder> const& it) -> difference_type
{
  return base::_encoder.pos() - it._encoder.pos();
}

template <typename Encoder>
auto adaptive_random_access_iterator<Encoder>::operator[](difference_type n) const
    -> const_reference
{
  return *(*this + n);
}

template <typename Encoder>
bool operator<(adaptive_random_access_iterator<Encoder> const& lhs,
               adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return lhs.pos() < rhs.pos();
}

template <typename Encoder>
bool operator>(adaptive_random_access_iterator<Encoder> const& lhs,
               adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return rhs < lhs;
}

template <typename Encoder>
bool operator>=(adaptive_random_access_iterator<Encoder> const& lhs,
                adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs < rhs);
}

template <typename Encoder>
bool operator<=(adaptive_random_access_iterator<Encoder> const& lhs,
                adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs > rhs);
}

template <typename Encoder>
bool operator==(adaptive_random_access_iterator<Encoder> const& lhs,
                adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return lhs._encoder == rhs._encoder;
}

template <typename Encoder>
bool operator!=(adaptive_random_access_iterator<Encoder> const& lhs,
                adaptive_random_access_iterator<Encoder> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}
}
