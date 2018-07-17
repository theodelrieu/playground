#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/input_adapter.hpp>

namespace mgs
{
inline namespace v1
{
namespace detail
{
// TODO put in aliases.hpp
template <typename T>
using is_bidirectional_tag =
    is_derived_from<T, std::bidirectional_iterator_tag>;

template <typename T>
using is_random_access_tag =
    is_derived_from<T, std::random_access_iterator_tag>;

template <typename Adapter, typename IteratorTag>
class adaptive_iterator
{
  // cannot use is_iterable_input_adapter, since adaptive_iterator
  // is used during the definition of input transformers.
  static_assert(is_input_adapter<Adapter>::value,
                "Adapter is not an InputAdapter");

public:
  using value_type = typename Adapter::value_type;
  using difference_type = typename Adapter::difference_type;

  using reference = decltype(std::declval<Adapter const&>().get());
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = IteratorTag;

  adaptive_iterator() = default;
  explicit adaptive_iterator(Adapter const&);

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
  Adapter _encoder;
};

template <typename Adapter, typename Tag>
bool operator!=(adaptive_iterator<Adapter, Tag> const&,
                adaptive_iterator<Adapter, Tag> const&) noexcept;

template <typename Adapter, typename Tag>
adaptive_iterator<Adapter, std::random_access_iterator_tag> operator+(
    typename adaptive_iterator<Adapter, std::random_access_iterator_tag>::
        difference_type,
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const& it);

template <typename Adapter>
bool operator>(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&,
    adaptive_iterator<Adapter,
                      std::random_access_iterator_tag> const&) noexcept;

template <typename Adapter>
bool operator>=(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&,
    adaptive_iterator<Adapter,
                      std::random_access_iterator_tag> const&) noexcept;

template <typename Adapter>
bool operator<=(
    adaptive_iterator<Adapter, std::random_access_iterator_tag> const&,
    adaptive_iterator<Adapter,
                      std::random_access_iterator_tag> const&) noexcept;
}
}
}

#include <mgs/detail/iterators/adaptive_iterator_impl.hpp>
