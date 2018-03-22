#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>

template <typename InputIterator, typename Sentinel = InputIterator>
class base64_encoding_iterator
{
public:
  using value_type = char;

  using reference = value_type const&;
  using const_reference = value_type const&;

  using pointer = value_type const*;
  using const_pointer = value_type const*;

  using iterator_category = std::input_iterator_tag;
  using difference_type = std::streamoff;

  base64_encoding_iterator() = default;
  base64_encoding_iterator(InputIterator begin, Sentinel end);
  base64_encoding_iterator(base64_encoding_iterator const&) = default;
  base64_encoding_iterator(base64_encoding_iterator&&) = default;
  base64_encoding_iterator& operator=(base64_encoding_iterator const&) =
      default;
  base64_encoding_iterator& operator=(base64_encoding_iterator&&) = default;

  reference operator*() const;
  pointer operator->() const;

  base64_encoding_iterator& operator++();
  base64_encoding_iterator operator++(int);

  template <typename T, typename U>
  friend bool operator==(base64_encoding_iterator<T, U>,
                         base64_encoding_iterator<T, U>) noexcept;

private:
  InputIterator mutable _current_it;
  Sentinel _end;
  value_type mutable _last_read;
};

template <typename T, typename U>
base64_encoding_iterator<T, U>::base64_encoding_iterator(T begin, U end)
  : _current_it(begin), _end(end), _last_read{}
{
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator*() const -> reference
{
  _last_read = static_cast<value_type>(*_current_it);
  return _last_read;
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator->() const -> pointer
{
  return std::addressof(**this);
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator++() -> base64_encoding_iterator&
{
  ++_current_it;
  return *this;
}

template <typename T, typename U>
auto base64_encoding_iterator<T, U>::operator++(int) -> base64_encoding_iterator
{
  auto ret = *this;
  ++_current_it;
  return ret;
}

template <typename T, typename U>
bool operator==(base64_encoding_iterator<T, U> lhs,
                base64_encoding_iterator<T, U> rhs) noexcept
{
  return lhs._current_it == rhs._current_it && lhs._end == rhs._end;
}

template <typename T, typename U>
bool operator!=(base64_encoding_iterator<T, U> lhs,
                base64_encoding_iterator<T, U> rhs) noexcept
{
  return !(lhs == rhs);
}

int main(int argc, char const* argv[])
{

  std::ifstream ifs("./t.cpp");
  base64_encoding_iterator<std::istreambuf_iterator<char>> it{
      std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
  decltype(it) end;

  while (it != end){
    std::cout << *it;
    ++it;
  }
}
