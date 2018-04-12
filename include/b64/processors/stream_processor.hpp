#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>

#include <b64/detail/meta/aliases.hpp>

#include <b64/detail/meta/concepts/iterable.hpp>
#include <b64/detail/meta/concepts/iterator.hpp>
#include <b64/detail/meta/concepts/sentinel.hpp>

namespace b64
{
namespace detail
{
template <typename Processor>
class input_source_iterator
{
public:
  using value_type = std::uint8_t;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  input_source_iterator() = default;
  input_source_iterator(Processor const&);

  reference operator*() const;
  pointer operator->() const;

  input_source_iterator& operator++();
  input_source_iterator operator++(int);

  template <typename T>
  friend bool operator==(input_source_iterator<T> const&,
                         input_source_iterator<T> const&);

private:
  Processor _source;
  value_type mutable _last_read{0};
  // TODO uint8_t mask instead of two bools
  bool mutable _read{false};
  bool mutable _end{true};
};

// Range-base for loop support

template <typename InputSource>
input_source_iterator<InputSource> begin(input_source_iterator<InputSource> it)
{
  return it;
}

template <typename InputSource>
input_source_iterator<InputSource> end(input_source_iterator<InputSource> const&)
{
  return input_source_iterator<InputSource>{};
}

// TODO put that in impl files
template <typename T>
input_source_iterator<T>::input_source_iterator(T const& encoding_source)
  : _source(encoding_source), _end(false)
{
}

template <typename T>
auto input_source_iterator<T>::operator*() const -> reference
{
  if (!_read)
  {
    _last_read = _source.next_char();
    _read = true;
  }
  return _last_read;
}

template <typename T>
auto input_source_iterator<T>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename T>
auto input_source_iterator<T>::operator++() -> input_source_iterator&
{
  _read = false;
  _end = _source.eof();
  return *this;
}

template <typename T>
auto input_source_iterator<T>::operator++(int) -> input_source_iterator
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename T>
bool operator==(input_source_iterator<T> const& lhs,
                input_source_iterator<T> const& rhs)
{
  if (lhs._end || rhs._end)
    return lhs._end == rhs._end;
  return lhs._source == rhs._source && lhs._last_read == rhs._last_read;
}

template <typename T>
bool operator!=(input_source_iterator<T> const& lhs,
                input_source_iterator<T> const& rhs)
{
  return !(lhs == rhs);
}
}
// TODO ns processors
// TODO fix name
// TODO iterator category specialization
template <typename Iterable,
          typename = std::enable_if_t<detail::is_iterable<Iterable>::value>>
class stream_processor
{
  using Iterator = typename detail2::result_of_begin<Iterable const&>::type;
  using Sentinel = typename detail2::result_of_end<Iterable const&>::type;

  static constexpr char const alphabet[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

public:
  using value_type = detail::value_type_t<std::iterator_traits<Iterator>>;

  stream_processor() = default;
  stream_processor(Iterable const&);

  value_type next_char() const;
  bool eof() const;

  auto begin() const
  {
    return detail::input_source_iterator<stream_processor>{*this};
  }

  auto end() const
  {
    return detail::input_source_iterator<stream_processor>{};
  }

  template <typename T, typename U>
  friend bool operator==(stream_processor<T, U> const&,
                         stream_processor<T, U> const& rhs);

private:
  void _encode_next_values() const;

  Iterator mutable _current_it{};
  Sentinel _end{};
  // FIXME should be value_type, char
  std::array<std::uint8_t, 4> mutable _last_encoded;
  int mutable _last_encoded_index{4};
};

template <typename Iterable, typename SFINAE>
char const stream_processor<Iterable, SFINAE>::alphabet[];

template <typename Iterable, typename SFINAE>
stream_processor<Iterable, SFINAE>::stream_processor(Iterable const& input)
  : _last_encoded_index(0)
{
  using std::begin;
  using std::end;

  _current_it = begin(input);
  _end = end(input);
}

template <typename Iterable, typename SFINAE>
void stream_processor<Iterable, SFINAE>::_encode_next_values() const
{
  assert(_last_encoded_index == 0);

  std::bitset<24> bits;
  int i = 0;
  for (; i < 3; ++i)
  {
    if (_current_it == _end)
      break;
    // FIXME require 1 byte value_type
    auto byte = static_cast<uint8_t>(*_current_it);
    ++_current_it;
    bits |= (byte << (16 - (8 * i)));
  }

  for (int j = 0; j < i + 1; ++j)
  {
    auto shift = (18 - (6 * j));
    auto mask = ((std::bitset<24>(0xFFFFFF) >> 18) << shift);
    auto l = bits & mask;
    l >>= shift;
    auto index = static_cast<uint8_t>(l.to_ulong());
    _last_encoded[j] = alphabet[index];
  }
  std::fill(std::next(_last_encoded.begin(), i + 1), _last_encoded.end(), '=');
}

template <typename Iterable, typename SFINAE>
auto stream_processor<Iterable, SFINAE>::next_char() const -> value_type
{
  if (!eof() && _last_encoded_index == 4)
    _last_encoded_index = 0;
  if (_last_encoded_index == 0)
    _encode_next_values();
  return _last_encoded[_last_encoded_index++];
}

template <typename Iterable, typename SFINAE>
bool stream_processor<Iterable, SFINAE>::eof() const
{
  return _last_encoded_index == 4 && _current_it == _end;
}

template <typename Iterable, typename SFINAE>
bool operator==(stream_processor<Iterable, SFINAE> const& lhs,
                stream_processor<Iterable, SFINAE> const& rhs)
{
  if (lhs._last_encoded_index == 4 &&
      lhs._last_encoded_index == rhs._last_encoded_index)
  {
    return true;
  }
  return lhs._current_it == rhs._current_it && lhs._end == rhs._end &&
         lhs._last_encoded_index == rhs._last_encoded_index;
}

template <typename Iterable, typename SFINAE>
bool operator!=(stream_processor<Iterable, SFINAE> const& lhs,
                stream_processor<Iterable, SFINAE> const& rhs)
{
  return !(lhs == rhs);
}
}
