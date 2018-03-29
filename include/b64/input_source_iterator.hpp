#pragma once

#include <b64/detail/meta/concepts/input_source.hpp>

#include <ios>
#include <iterator>
#include <type_traits>

namespace b64
{
template <typename InputSource,
          typename =
              std::enable_if_t<detail::is_input_source<InputSource>::value>>
class input_source_iterator
{
public:
  using value_type = typename InputSource::value_type;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  input_source_iterator() = default;
  input_source_iterator(InputSource const&);

  reference operator*() const;
  pointer operator->() const;

  input_source_iterator& operator++();
  input_source_iterator operator++(int);

  template <typename T, typename U>
  friend bool operator==(input_source_iterator<T, U> const&,
                         input_source_iterator<T, U> const&);

private:
  InputSource _source;
  value_type mutable _last_read{0};
  // TODO uint8_t mask instead of two bools
  bool mutable _read{false};
  bool mutable _end{true};
};

template <typename T, typename U>
input_source_iterator<T, U>::input_source_iterator(T const& encoding_source)
  : _source(encoding_source), _end(false)
{
}

template <typename T, typename U>
auto input_source_iterator<T, U>::operator*() const -> reference
{
  if (!_read)
  {
    _last_read = _source.next_char();
    _read = true;
  }
  return _last_read;
}

template <typename T, typename U>
auto input_source_iterator<T, U>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename T, typename U>
auto input_source_iterator<T, U>::operator++() -> input_source_iterator&
{
  _read = false;
  _end = _source.eof();
  return *this;
}

template <typename T, typename U>
auto input_source_iterator<T, U>::operator++(int) -> input_source_iterator
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename T, typename U>
bool operator==(input_source_iterator<T, U> const& lhs,
                input_source_iterator<T, U> const& rhs)
{
  if (lhs._end || rhs._end)
    return lhs._end == rhs._end;
  return lhs._source == rhs._source && lhs._last_read == rhs._last_read;
}

template <typename T, typename U>
bool operator!=(input_source_iterator<T, U> const& lhs,
                input_source_iterator<T, U> const& rhs)
{
  return !(lhs == rhs);
}
} // namespace b64
