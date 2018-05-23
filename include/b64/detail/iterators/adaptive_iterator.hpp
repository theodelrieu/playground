#pragma once

#include <iterator>
#include <type_traits>

#include <b64/detail/meta/concepts/derived_from.hpp>

namespace b64
{
namespace detail
{
template <typename T>
using is_bidirectional_tag =
    is_derived_from<T, std::bidirectional_iterator_tag>;

template <typename T>
using is_random_access_tag =
    is_derived_from<T, std::random_access_iterator_tag>;

template <typename Encoder, typename IteratorTag>
class adaptive_iterator
{
public:
  using value_type = typename Encoder::value_type;
  using difference_type = typename Encoder::difference_type;

  // FIXME add stuff to encoder concept, + SFINAE
  using reference = decltype(std::declval<Encoder const&>().get());
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = IteratorTag;

  adaptive_iterator() = default;
  adaptive_iterator(Encoder const&);

  reference operator*() const;
  pointer operator->() const;

  adaptive_iterator& operator++();
  adaptive_iterator operator++(int);

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_bidirectional_tag<Tag>::value>>
  adaptive_iterator& operator--();

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_bidirectional_tag<Tag>::value>>
  adaptive_iterator operator--(int);

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  adaptive_iterator& operator+=(difference_type);

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  adaptive_iterator& operator-=(difference_type);

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  adaptive_iterator operator+(difference_type) const;

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  adaptive_iterator operator-(difference_type) const;

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  auto operator-(adaptive_iterator const&) const -> difference_type;

  template <typename Tag = IteratorTag,
            typename = std::enable_if_t<is_random_access_tag<Tag>::value>>
  value_type operator[](difference_type) const;

  template <typename T, typename Tag>
  friend bool operator==(adaptive_iterator<T, Tag> const&,
                         adaptive_iterator<T, Tag> const&) noexcept;

  template <typename T>
  friend bool operator<(
      adaptive_iterator<T, std::random_access_iterator_tag> const&,
      adaptive_iterator<T, std::random_access_iterator_tag> const&) noexcept;

private:
  Encoder _encoder;
};

template <typename Encoder, typename Tag>
bool operator!=(adaptive_iterator<Encoder, Tag> const&,
                adaptive_iterator<Encoder, Tag> const&) noexcept;

template <typename Encoder, typename Tag>
adaptive_iterator<Encoder, std::random_access_iterator_tag> operator+(
    typename adaptive_iterator<Encoder, std::random_access_iterator_tag>::
        difference_type,
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const& it);

template <typename Encoder>
bool operator>(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&,
    adaptive_iterator<Encoder,
                      std::random_access_iterator_tag> const&) noexcept;

template <typename Encoder>
bool operator>=(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&,
    adaptive_iterator<Encoder,
                      std::random_access_iterator_tag> const&) noexcept;

template <typename Encoder>
bool operator<=(
    adaptive_iterator<Encoder, std::random_access_iterator_tag> const&,
    adaptive_iterator<Encoder,
                      std::random_access_iterator_tag> const&) noexcept;
}
}

#include <b64/detail/iterators/adaptive_iterator_impl.hpp>
