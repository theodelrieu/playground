#pragma once

#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch.hpp>

#include <mgs/adapters/transformer_adapter.hpp>
#include <mgs/base_n/base_n.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/iterable.hpp>
#include <mgs/meta/concepts/iterable_input_adapter.hpp>

// streams are not Iterable until C++20.
struct stream_iterable_adapter
{
  stream_iterable_adapter(std::istream& is) : _is(is)
  {
  }

  auto begin() const
  {
    return std::istreambuf_iterator<char>{_is};
  }

  auto end() const
  {
    return std::istreambuf_iterator<char>{};
  }

  std::istream& _is;
};

struct sentinel
{
};

bool operator==(std::istreambuf_iterator<char> lhs, sentinel) noexcept
{
  return lhs.equal({});
}

bool operator==(sentinel, std::istreambuf_iterator<char> rhs) noexcept
{
  return rhs.equal({});
}

bool operator!=(std::istreambuf_iterator<char> lhs, sentinel s) noexcept
{
  return !(lhs == s);
}

bool operator!=(sentinel s, std::istreambuf_iterator<char> rhs) noexcept
{
  return !(s == rhs);
}

static_assert(
    mgs::meta::is_sentinel<sentinel, std::istreambuf_iterator<char>>::value,
    "");

static_assert(
    !mgs::meta::is_sentinel<sentinel, std::vector<char>::iterator>::value, "");

template <template <typename ...> class Adapter, typename Container, typename Iterable>
void base_n_checks_impl(Container const& source,
                        Iterable const& expected_output)
{
  using Iterator = mgs::meta::result_of_begin_t<Container>;
  using Sentinel = mgs::meta::result_of_end_t<Container>;

  using std::begin;
  using std::end;

  Adapter<Iterator, Sentinel> adapter(begin(source), end(source));

  std::vector<std::uint8_t> const output(adapter.begin(), adapter.end());
  std::vector<std::uint8_t> const expected(expected_output.begin(),
                                           expected_output.end());

  CHECK(output == expected);
}

template <template <typename...> class Adapter,
          typename Container,
          typename Iterable,
          typename Iterable2>
void base_n_checks(Iterable const& source, Iterable2 const& expected)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));

  base_n_checks_impl<Adapter>(cont, expected);
}

template <template <typename...> class Adapter,
          typename Container,
          typename Input,
          typename Output>
void base_n_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  REQUIRE(inputs.size() == outputs.size());

  for (auto i = 0; i < inputs.size(); ++i)
    base_n_checks<Adapter, Container>(inputs[i], outputs[i]);
}

template <template <typename...> class Adapter, typename Input, typename Output>
void common_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  SECTION("Empty input")
  {
    using namespace std::string_literals;

    base_n_checks_impl<Adapter>(""s, ""s);
  }

  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base_n_checks<Adapter, std::string>(inputs, outputs);
    }

    SECTION("std::vector")
    {
      base_n_checks<Adapter, std::vector<char>>(inputs, outputs);
    }

    SECTION("std::deque")
    {
      base_n_checks<Adapter, std::deque<char>>(inputs, outputs);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base_n_checks<Adapter, std::list<char>>(inputs, outputs);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base_n_checks<Adapter, std::forward_list<char>>(inputs, outputs);
    }
  }
}

template <template <typename...> class Adapter>
void sentinel_check(std::string const& input, std::string const& output)
{
  std::stringstream ss{input};

  Adapter<std::istreambuf_iterator<char>, sentinel> adapter(
      std::istreambuf_iterator<char>(ss), sentinel{});

  std::string s(adapter.begin(), adapter.end());
  CHECK(s == output);
}

template <template <typename...> class Adapter>
void inception_check(std::string const& input,
                     std::string const& first_output,
                     std::string const& final_output)
{
  Adapter<std::string::const_iterator> first_adapter(input.begin(),
                                                     input.end());

  Adapter<decltype(first_adapter.begin())> second_adapter(first_adapter.begin(),
                                                          first_adapter.end());

  std::string s(second_adapter.begin(), second_adapter.end());
  CHECK(s == final_output);
}

template <template <typename...> class EncodingAdapter,
          template <typename...> class DecodingAdapter>
void back_and_forth_check(std::string const& input)
{
  EncodingAdapter<std::string::const_iterator> enc(input.begin(), input.end());

  DecodingAdapter<decltype(enc.begin())> dec(enc.begin(), enc.end());

  std::string s(dec.begin(), dec.end());
  CHECK(s == input);
}

template <template <typename...> class Adapter>
void stream_check(std::istream& input, std::istream& output)
{
  stream_iterable_adapter iterable_input{input};
  stream_iterable_adapter iterable_output{output};

  base_n_checks_impl<Adapter>(iterable_input, iterable_output);
}

template <template <typename...> class DecodingAdapter, typename Exception>
void invalid_input_checks(std::vector<std::string> const& inputs)
{
  using namespace std::string_literals;

  for (auto const& input : inputs)
    CHECK_THROWS_AS(base_n_checks_impl<DecodingAdapter>(input, ""s), Exception);
}
