#pragma once

#include <algorithm>

#include <mgs/codecs/default_input_source.hpp>
#include <mgs/codecs/detail/input_reader.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S>
default_input_source<I, S>::default_input_source(I begin, S end)
  : _current(begin), _end(end)
{
}

template <typename I, typename S>
template <typename OutputIterator, typename SFINAE>
meta::ssize_t default_input_source<I, S>::read(OutputIterator out,
                                               meta::ssize_t n)
{
  return detail::input_reader<I, S>::read(_current, _end, out, n);
}

template <typename I, typename S>
template <typename S2, typename SFINAE>
meta::ssize_t default_input_source<I, S>::max_remaining_size() const
{
  return _end - _current;
}
}
}
