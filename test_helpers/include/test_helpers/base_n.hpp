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

#include <mgs/detail/base_n/base_n.hpp>
#include <mgs/detail/meta/concepts/derived_from.hpp>
#include <mgs/detail/meta/concepts/iterable.hpp>
#include <mgs/detail/meta/concepts/iterable_input_adapter.hpp>

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
    mgs::detail::is_sentinel<sentinel, std::istreambuf_iterator<char>>::value,
    "");

static_assert(
    !mgs::detail::is_sentinel<sentinel, std::vector<char>::iterator>::value,
    "");

template <typename InputTransformer, typename Container, typename Iterable>
void base_n_checks_impl(Container const& source,
                        Iterable const& expected_output)
{
  using Iterator = mgs::detail2::result_of_begin_t<Container>;
  using Sentinel = mgs::detail2::result_of_end_t<Container>;

  using std::begin;
  using std::end;

  mgs::detail::basic_adapter<InputTransformer, Iterator, Sentinel> adapter(
      begin(source), end(source));

  std::vector<std::uint8_t> const output(adapter.begin(), adapter.end());
  std::vector<std::uint8_t> const expected(expected_output.begin(),
                                           expected_output.end());

  CHECK(output == expected);
}

template <typename InputTransformer,
          typename Container,
          typename Iterable,
          typename Iterable2>
void base_n_checks(Iterable const& source, Iterable2 const& expected)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));

  base_n_checks_impl<InputTransformer>(cont, expected);
}

template <typename InputTransformer,
          typename Container,
          typename Input,
          typename Output>
void base_n_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  REQUIRE(inputs.size() == outputs.size());

  for (auto i = 0; i < inputs.size(); ++i)
    base_n_checks<InputTransformer, Container>(inputs[i], outputs[i]);
}

template <typename InputTransformer, typename Input, typename Output>
void common_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  SECTION("Empty input")
  {
    using namespace std::string_literals;

    base_n_checks_impl<InputTransformer>(""s, ""s);
  }

  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base_n_checks<InputTransformer, std::string>(inputs, outputs);
    }

    SECTION("std::vector")
    {
      base_n_checks<InputTransformer, std::vector<char>>(inputs, outputs);
    }

    SECTION("std::deque")
    {
      base_n_checks<InputTransformer, std::deque<char>>(inputs, outputs);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base_n_checks<InputTransformer, std::list<char>>(inputs, outputs);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base_n_checks<InputTransformer, std::forward_list<char>>(inputs, outputs);
    }
  }
}

template <typename InputTransformer>
void sentinel_check(std::string const& input, std::string const& output)
{
  std::stringstream ss{input};

  mgs::detail::
      basic_adapter<InputTransformer, std::istreambuf_iterator<char>, sentinel>
      adapter(std::istreambuf_iterator<char>(ss), sentinel{});

  std::string s(adapter.begin(), adapter.end());
  CHECK(s == output);
}

template <typename InputTransformer>
void inception_check(std::string const& input,
                     std::string const& first_output,
                     std::string const& final_output)
{
  mgs::detail::basic_adapter<InputTransformer, std::string::const_iterator>
      first_adapter(input.begin(), input.end());

  mgs::detail::basic_adapter<InputTransformer, decltype(first_adapter.begin())>
      second_adapter(first_adapter.begin(), first_adapter.end());

  std::string s(second_adapter.begin(), second_adapter.end());
  CHECK(s == final_output);
}

template <typename Encoder, typename Decoder>
void back_and_forth_check(std::string const& input)
{
  mgs::detail::basic_adapter<Encoder, std::string::const_iterator> enc(
      input.begin(), input.end());

  mgs::detail::basic_adapter<Decoder, decltype(enc.begin())> dec(enc.begin(),
                                                                 enc.end());

  std::string s(dec.begin(), dec.end());
  CHECK(s == input);
}

template <typename InputTransformer>
void stream_check(std::istream& input, std::istream& output)
{
  stream_iterable_adapter iterable_input{input};
  stream_iterable_adapter iterable_output{output};

  base_n_checks_impl<InputTransformer>(iterable_input, iterable_output);
}

template <typename Decoder, typename Exception>
void invalid_input_checks(std::vector<std::string> const& inputs)
{
  using namespace std::string_literals;

  for (auto const& input : inputs)
    CHECK_THROWS_AS(base_n_checks_impl<Decoder>(input, ""s), Exception);
}
