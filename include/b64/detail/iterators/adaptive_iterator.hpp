#pragma once

#include <iterator>
#include <type_traits>
#include <vector>

namespace b64
{
namespace detail
{
template <typename Encoder, typename IteratorTag>
class adaptive_iterator;

// TODO single class, enable_if on iterator tag on methods. 
// TODO SeekableEncoder, RandomAccessEncoder
template <typename Encoder>
using adaptive_input_iterator =
    adaptive_iterator<Encoder, std::input_iterator_tag>;

template <typename Encoder>
using adaptive_forward_iterator =
    adaptive_iterator<Encoder, std::forward_iterator_tag>;

template <typename Encoder>
using adaptive_bidirectional_iterator =
    adaptive_iterator<Encoder, std::bidirectional_iterator_tag>;

template <typename Encoder>
using adaptive_random_access_iterator =
    adaptive_iterator<Encoder, std::random_access_iterator_tag>;

template <typename Encoder>
class adaptive_iterator<Encoder, std::input_iterator_tag>
{
public:
  using value_type = typename Encoder::value_type;
  using difference_type = typename Encoder::difference_type;

  // FIXME for that, maybe SFINAE on .get() return type (reference or not)
  // value_type const& is required for ForwardIterators, not Input
  // (e.g. istreambuf_iterator::reference == value_type)
  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;

  adaptive_iterator() = default;
  adaptive_iterator(Encoder const&);

  reference operator*() const;
  pointer operator->() const;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  template <typename T>
  friend bool operator==(adaptive_input_iterator<T> const&,
                         adaptive_input_iterator<T> const&) noexcept;

protected:
  Encoder _encoder;
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::forward_iterator_tag>
    : protected adaptive_input_iterator<Encoder>
{
  using base = adaptive_input_iterator<Encoder>;

public:
  using typename base::value_type;
  using typename base::reference;
  using typename base::const_reference;
  using typename base::pointer;
  using typename base::const_pointer;
  using typename base::difference_type;

  using iterator_category = std::forward_iterator_tag;

  using base::base;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  template <typename T>
  friend bool operator==(adaptive_forward_iterator<T> const&,
                         adaptive_forward_iterator<T> const&) noexcept;
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::bidirectional_iterator_tag>
    : protected adaptive_forward_iterator<Encoder>
{
  using base = adaptive_forward_iterator<Encoder>;

public:
  using typename base::value_type;
  using typename base::reference;
  using typename base::const_reference;
  using typename base::pointer;
  using typename base::const_pointer;
  using typename base::difference_type;

  using iterator_category = std::bidirectional_iterator_tag;

  using base::base;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  adaptive_iterator& operator--();
  adaptive_iterator operator--(int);

  template <typename T>
  friend bool operator==(adaptive_bidirectional_iterator<T> const&,
                         adaptive_bidirectional_iterator<T> const&) noexcept;
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::random_access_iterator_tag>
    : protected adaptive_bidirectional_iterator<Encoder>
{
  using base = adaptive_bidirectional_iterator<Encoder>;

public:
  using typename base::value_type;
  using typename base::reference;
  using typename base::const_reference;
  using typename base::pointer;
  using typename base::const_pointer;
  using typename base::difference_type;

  using iterator_category = std::random_access_iterator_tag;

  using base::base;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  adaptive_iterator& operator--();
  adaptive_iterator operator--(int);

  adaptive_iterator& operator+=(difference_type);
  adaptive_iterator& operator-=(difference_type);

  adaptive_iterator operator+(difference_type) const;
  adaptive_iterator operator-(difference_type) const;

  difference_type operator-(adaptive_iterator const&);

  // FIXME c.f. comment for operator*
  value_type operator[](difference_type) const;

  template <typename T>
  friend bool operator==(adaptive_random_access_iterator<T> const&,
                         adaptive_random_access_iterator<T> const&) noexcept;

  template <typename T>
  friend bool operator<(adaptive_random_access_iterator<T> const&,
                        adaptive_random_access_iterator<T> const&) noexcept;
};

template <typename Encoder>
adaptive_random_access_iterator<Encoder> operator+(
    typename adaptive_random_access_iterator<Encoder>::difference_type n,
    adaptive_random_access_iterator<Encoder> const& it);

template <typename T>
bool operator>(adaptive_random_access_iterator<T> const&,
               adaptive_random_access_iterator<T> const&) noexcept;

template <typename T>
bool operator>=(adaptive_random_access_iterator<T> const&,
                adaptive_random_access_iterator<T> const&) noexcept;

template <typename T>
bool operator>=(adaptive_random_access_iterator<T> const&,
                adaptive_random_access_iterator<T> const&) noexcept;
}
}

#include <b64/detail/iterators/adaptive_iterator_impl.hpp>
