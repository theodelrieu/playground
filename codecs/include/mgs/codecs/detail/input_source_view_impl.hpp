#pragma once

#include <algorithm>

#include <mgs/codecs/detail/input_reader.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S>
input_source_view<I, S>::input_source_view(I begin, S end)
  : _current(begin), _end(end)
{
}

template <typename I, typename S>
I input_source_view<I, S>::begin() const
{
  return _current;
}

template <typename I, typename S>
S input_source_view<I, S>::end() const
{
  return _end;
}

template <typename I, typename S>
template <typename OutputIterator, typename SFINAE>
meta::ssize_t input_source_view<I, S>::read(OutputIterator out,
                                               meta::ssize_t n)
{
  return detail::input_reader<I, S>::read(_current, _end, out, n);
}

template <typename I, typename S>
template <typename S2, typename SFINAE>
meta::ssize_t input_source_view<I, S>::max_remaining_size() const
{
  return _end - _current;
}
}
}
