#include <cstdlib>
#include <iostream>
#include <string>

#include <mgs/iterators/adaptive_iterator.hpp>
#include <mgs/meta/aliases.hpp>

namespace
{
template <typename UnderlyingIterator, typename Sentinel = UnderlyingIterator>
class noop_encoder
{
public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

private:
  using underlying_iterator_traits = std::iterator_traits<UnderlyingIterator>;
  using iterator =
      mgs::iterators::adaptive_iterator<noop_encoder, std::input_iterator_tag>;

  friend mgs::iterators::adaptive_iterator<
      noop_encoder,
      mgs::meta::iterator_category_t<underlying_iterator_traits>>;

public:
  using difference_type = typename underlying_iterator_traits::difference_type;
  using value_type = typename underlying_iterator_traits::value_type;

  noop_encoder() = default;
  noop_encoder(UnderlyingIterator it, Sentinel end)
    : _begin(it), _current_it(it), _end(end)
  {
  }

  iterator begin()
  {
    return iterator{*this};
  }

  iterator end() const
  {
    noop_encoder enc{{}, _end};
    enc._begin = _begin;
    enc._current_it = enc._end;
    return iterator{enc};
  }

  void seek_forward(difference_type n)
  {
    std::advance(_current_it, n);
  }

  value_type const& get() const
  {
    // be sure to return a reference
    _last_read = *_current_it;
    return _last_read;
  }

  template <typename T, typename U>
  friend bool operator==(noop_encoder<T, U> const& lhs,
                         noop_encoder<T, U> const& rhs) noexcept;

private:
  UnderlyingIterator _begin{};
  UnderlyingIterator _current_it{};
  Sentinel _end{};
  value_type mutable _last_read{};
};

template <typename T, typename U>
bool operator==(noop_encoder<T, U> const& lhs,
                noop_encoder<T, U> const& rhs) noexcept
{
  return std::tie(lhs._begin, lhs._current_it, lhs._end) ==
         std::tie(rhs._begin, rhs._current_it, rhs._end);
}

template <typename T, typename U>
bool operator!=(noop_encoder<T, U> const& lhs,
                noop_encoder<T, U> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}

int main(int argc, char const* argv[])
{
  mgs::iterators::adaptive_iterator<noop_encoder<char*>,
                                    std::input_iterator_tag>
      it;
}
