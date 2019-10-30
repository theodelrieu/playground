#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <tuple>
#include <utility>

namespace mgs
{
namespace codecs
{
template <typename InputSource>
basic_input_range<InputSource>::basic_input_range(
    InputSource const& input_source)
  : _input_source(input_source)
{
}

template <typename InputSource>
basic_input_range<InputSource>::basic_input_range(InputSource&& input_source)
  : _input_source(std::move(input_source))
{
}

template <typename InputSource>
template <typename OutputIterator, typename>
meta::ssize_t basic_input_range<InputSource>::read(OutputIterator out,
                                                   meta::ssize_t n) const
{
  return _input_source.read(std::move(out), n);
}

template <typename InputSource>
template <typename T, typename SFINAE>
meta::ssize_t basic_input_range<InputSource>::max_remaining_size() const
{
  return _input_source.max_remaining_size();
}

template <typename InputSource>
auto basic_input_range<InputSource>::begin() -> iterator
{
  return iterator{this};
}

template <typename InputSource>
auto basic_input_range<InputSource>::end() const -> iterator
{
  return iterator{nullptr};
}
}
}
