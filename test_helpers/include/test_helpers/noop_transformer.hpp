#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace test_helpers
{
template <typename Iterator, typename Sentinel>
class noop_transformer
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;
  using buffer_type = std::vector<std::uint8_t>;

  noop_transformer() = default;

  noop_transformer(Iterator begin, Sentinel end)
    : _current(std::move(begin)), _end(std::move(end))
  {
  }

  void operator()(buffer_type& out)
  {
    out.clear();
    while (_current != _end)
      out.push_back(*_current++);
  }

  underlying_iterator const& get_iterator() const
  {
    return _current;
  }

  underlying_sentinel const& get_sentinel() const
  {
    return _end;
  }

private:
  Iterator _current{};
  Sentinel _end{};
};
}
