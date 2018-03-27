#pragma once

#include <b64/detail/meta/concepts/input_processor.hpp>

#include <ios>
#include <iterator>
#include <type_traits>

namespace b64
{
template <typename InputProcessor,
          typename = std::enable_if_t<
              detail::is_input_processor<InputProcessor>::value>>
class encoding_iterator
{
public:
  // TODO different value_type depending on EncodingProcess
  using value_type = typename InputProcessor::value_type;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  encoding_iterator() = default;
  encoding_iterator(InputProcessor const&);
  encoding_iterator(encoding_iterator const&) = default;
  encoding_iterator(encoding_iterator&&) = default;
  encoding_iterator& operator=(encoding_iterator const&) = default;
  encoding_iterator& operator=(encoding_iterator&&) = default;

  reference operator*() const;
  pointer operator->() const;

  encoding_iterator& operator++();
  encoding_iterator operator++(int);

  template <typename T, typename U>
  friend bool operator==(encoding_iterator<T, U> const&,
                         encoding_iterator<T, U> const&);

private:
  InputProcessor _processor;
  value_type mutable _last_read{0};
  // uint8_t mask instead of two bools
  bool mutable _read{false};
  bool mutable _end{true};
};

template <typename T, typename U>
encoding_iterator<T, U>::encoding_iterator(T const& encoding_processor)
  : _processor(encoding_processor), _end(false)
{
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator*() const -> reference
{
  if (!_read)
  {
    _last_read = _processor.next_char();
    _read = true;
  }
  return _last_read;
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator++() -> encoding_iterator&
{
  _read = false;
  _end = _processor.eof();
  return *this;
}

template <typename T, typename U>
auto encoding_iterator<T, U>::operator++(int) -> encoding_iterator
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename T, typename U>
bool operator==(encoding_iterator<T, U> const& lhs,
                encoding_iterator<T, U> const& rhs)
{
  if (lhs._end || rhs._end)
    return lhs._end == rhs._end;
  return lhs._processor == rhs._processor && lhs._last_read == rhs._last_read;
}

template <typename T, typename U>
bool operator!=(encoding_iterator<T, U> const& lhs,
                encoding_iterator<T, U> const& rhs)
{
  return !(lhs == rhs);
}
}
