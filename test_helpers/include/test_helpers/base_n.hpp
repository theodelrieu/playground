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

template <typename EncodingTraits, typename Container, typename Iterable>
void base_n_checks_impl(Container const& source,
                        Iterable const& expected_output)
{
  using Iterator = mgs::detail2::result_of_begin_t<Container>;
  using Sentinel = mgs::detail2::result_of_end_t<Container>;

  using std::begin;
  using std::end;

  mgs::detail::base_n_transformer<EncodingTraits, Iterator, Sentinel>
      transformer(begin(source), end(source));

  std::vector<std::uint8_t> const output(transformer.begin(),
                                         transformer.end());
  std::vector<std::uint8_t> const expected(expected_output.begin(),
                                           expected_output.end());

  CHECK(output == expected);
}

template <typename EncodingTraits,
          typename Container,
          typename Iterable,
          typename Iterable2>
void base_n_checks(Iterable const& source, Iterable2 const& expected)
{
  using std::begin;
  using std::end;

  Container cont(begin(source), end(source));

  base_n_checks_impl<EncodingTraits>(cont, expected);
}

template <typename EncodingTraits,
          typename Container,
          typename Input,
          typename Output>
void base_n_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  REQUIRE(inputs.size() == outputs.size());

  for (auto i = 0; i < inputs.size(); ++i)
    base_n_checks<EncodingTraits, Container>(inputs[i], outputs[i]);
}

template <typename EncodingTraits, typename Input, typename Output>
void common_checks(std::vector<Input> const& inputs,
                   std::vector<Output> const& outputs)
{
  SECTION("Empty input")
  {
    using namespace std::string_literals;

    base_n_checks_impl<EncodingTraits>(""s, ""s);
  }

  SECTION("RandomAccessIterator")
  {
    SECTION("std::string")
    {
      base_n_checks<EncodingTraits, std::string>(inputs, outputs);
    }

    SECTION("std::vector")
    {
      base_n_checks<EncodingTraits, std::vector<char>>(inputs, outputs);
    }

    SECTION("std::deque")
    {
      base_n_checks<EncodingTraits, std::deque<char>>(inputs, outputs);
    }
  }

  SECTION("BidirectionalIterator")
  {
    SECTION("std::list")
    {
      base_n_checks<EncodingTraits, std::list<char>>(inputs, outputs);
    }
  }

  SECTION("ForwardIterator")
  {
    SECTION("std::forward_list")
    {
      base_n_checks<EncodingTraits, std::forward_list<char>>(inputs, outputs);
    }
  }
}

template <typename EncodingTraits>
void sentinel_check(std::string const& input, std::string const& output)
{
  std::stringstream ss{input};

  mgs::detail::base_n_transformer<EncodingTraits,
                                  std::istreambuf_iterator<char>,
                                  sentinel>
  transformer(std::istreambuf_iterator<char>(ss), sentinel{});

  std::string s(transformer.begin(), transformer.end());
  CHECK(s == output);
}

template <typename EncodingTraits>
void inception_check(std::string const& input,
                     std::string const& first_output,
                     std::string const& final_output)
{
  mgs::detail::base_n_transformer<EncodingTraits, std::string::const_iterator>
      first_transformer(input.begin(), input.end());

  mgs::detail::base_n_transformer<EncodingTraits,
                                  decltype(first_transformer.begin())>
      second_transformer(first_transformer.begin(), first_transformer.end());

  std::string s(second_transformer.begin(), second_transformer.end());
  CHECK(s == final_output);
}

template <typename EncoderTraits, typename DecoderTraits>
void back_and_forth_check(std::string const& input)
{
  mgs::detail::base_n_transformer<EncoderTraits, std::string::const_iterator>
      enc(input.begin(), input.end());

  mgs::detail::base_n_transformer<DecoderTraits, decltype(enc.begin())> dec(
      enc.begin(), enc.end());

  std::string s(dec.begin(), dec.end());
  CHECK(s == input);
}

template <typename EncodingTraits>
void stream_check(std::istream& input, std::istream& output)
{
  stream_iterable_adapter iterable_input{input};
  stream_iterable_adapter iterable_output{output};

  base_n_checks_impl<EncodingTraits>(iterable_input, iterable_output);
}

template <typename DecoderTraits, typename Exception>
void invalid_input_checks(std::vector<std::string> const& inputs)
{
  using namespace std::string_literals;

  for (auto const& input : inputs)
    CHECK_THROWS_AS(base_n_checks_impl<DecoderTraits>(input, ""s), Exception);
}
