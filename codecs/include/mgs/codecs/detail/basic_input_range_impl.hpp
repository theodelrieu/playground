#pragma once

#include <utility>

namespace mgs
{
namespace codecs
{
template <typename InputSource>
basic_input_range<InputSource>::basic_input_range(InputSource& input_source)
  : _input_source(std::addressof(input_source))
{
}

template <typename InputSource>
auto basic_input_range<InputSource>::begin() const -> iterator
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
