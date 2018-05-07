#pragma once

#include <type_traits>
#include <vector>

#include <b64/detail/meta/aliases.hpp>
#include <b64/detail/meta/concepts/iterable.hpp>

namespace b64
{
namespace detail
{
template <typename Encoder, typename IteratorTag>
class adaptive_iterator;

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
private:
  using underlying_iterator = typename detail2::result_of_begin<Encoder>::type;

public:
  using value_type = typename Encoder::value_type;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;

  using difference_type =
      typename std::iterator_traits<underlying_iterator>::iterator_category;

  adaptive_iterator() = default;
  adaptive_iterator(Encoder const&);

  reference operator*() const;
  pointer operator->() const;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  template <typename T>
  friend bool operator==(adaptive_input_iterator<T> const&,
                         adaptive_input_iterator<T> const&) noexcept;

private:
  // TODO add back eof()
  Encoder _encoder;
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::forward_iterator_tag>
    : public adaptive_input_iterator<Encoder>
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

  using base::adaptive_input_iterator;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::bidirectional_iterator_tag>
    : public adaptive_forward_iterator<Encoder>
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

  using base::adaptive_forward_iterator;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  adaptive_iterator& operator--();
  adaptive_iterator operator--(int);
};

template <typename Encoder>
class adaptive_iterator<Encoder, std::random_access_iterator_tag>
    : public adaptive_bidirectional_iterator<Encoder>
{
  using base = adaptive_bidirectional_iterator<Encoder>;

public:
  using typename base::value_type;
  using typename base::reference;
  using typename base::const_reference;
  using typename base::pointer;
  using typename base::const_pointer;
  using typename base::difference_type;

  using iterator_category = std::bidirectional_iterator_tag;

  using base::adaptive_bidirectional_iterator;

  using base::operator*;
  using base::operator->;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  adaptive_iterator& operator--();
  adaptive_iterator operator--(int);

  adaptive_iterator& operator+=(difference_type);
  adaptive_iterator& operator-=(difference_type);

  adaptive_iterator operator+(difference_type);
  adaptive_iterator operator-(difference_type);

  difference_type operator-(adaptive_iterator const&);

  const_reference operator[](difference_type) const;

  template <typename T>
  friend bool operator<(adaptive_random_access_iterator<T> const&,
                        adaptive_random_access_iterator<T> const&) noexcept;
};

// TODO should be Encoder/Decoder (Coder)?
// template <typename Encoder>
// class adaptive_iterator : select_adaptive_iterator_t<Encoder>
// {
//   using underlying_iterator = typename detail2::result_of_begin<Encoder>::type;
//
// public:
//   using value_type = typename Encoder::value_type;
//
//   using reference = value_type const&;
//   using const_reference = value_type const&;
//
//   using pointer = value_type const*;
//   using const_pointer = value_type const*;
//
//   using iterator_category =
//       iterator_category_t<std::iterator_traits<underlying_iterator>>;
//   using difference_type =
//       typename std::iterator_traits<underlying_iterator>::iterator_category;
//
//   input_source_iterator() = default;
//   input_source_iterator(Encoder const&);
//
//   reference operator*() const;
//   pointer operator->() const;
//
//   input_source_iterator& operator++();
//   input_source_iterator operator++(int);
//
//   template <typename T>
//   friend bool operator==(input_source_iterator<T> const&,
//                          input_source_iterator<T> const&);
//
// private:
//   Encoder _source;
//   value_type mutable _last_read{0};
//   // TODO uint8_t mask instead of two bools
//   bool mutable _read{false};
//   bool mutable _end{true};
// };
}
}
