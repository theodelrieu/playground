#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/adapters/concepts/input_adapter.hpp>
#include <mgs/meta/concepts/core/derived_from.hpp>

namespace mgs
{
inline namespace v1
{
namespace iterators
{
namespace detail
{
template <typename T>
using is_bidirectional_tag =
    meta::concepts::core::is_derived_from<T, std::bidirectional_iterator_tag>;

template <typename T>
using is_random_access_tag =
    meta::concepts::core::is_derived_from<T, std::random_access_iterator_tag>;
}

template <typename Adapter, typename IteratorTag>
class adaptive_iterator
{
  static_assert(adapters::concepts::is_input_adapter<Adapter>::value,
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

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_bidirectional_tag<Tag>::value>>
  adaptive_iterator& operator--();

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_bidirectional_tag<Tag>::value>>
  adaptive_iterator operator--(int);

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  adaptive_iterator& operator+=(difference_type);

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  adaptive_iterator& operator-=(difference_type);

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  adaptive_iterator operator+(difference_type) const;

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  adaptive_iterator operator-(difference_type) const;

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  auto operator-(adaptive_iterator const&) const -> difference_type;

  template <
      typename Tag = IteratorTag,
      typename = std::enable_if_t<detail::is_random_access_tag<Tag>::value>>
  value_type operator[](difference_type) const;

  template <typename T, typename Tag>
  friend bool operator==(adaptive_iterator<T, Tag> const&,
                         adaptive_iterator<T, Tag> const&) noexcept;

  template <typename T>
  friend bool operator<(
      adaptive_iterator<T, std::random_access_iterator_tag> const&,
      adaptive_iterator<T, std::random_access_iterator_tag> const&) noexcept;

private:
  Adapter _adapter;
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

#include <mgs/iterators/adaptive_iterator_impl.hpp>
