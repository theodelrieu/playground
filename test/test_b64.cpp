#include <algorithm>
#include <fstream>
#include <vector>

#include <catch.hpp>

#include <b64/encode_iterator.hpp>
#include <b64/processors/stream_processor.hpp>

using namespace std::string_literals;

extern std::vector<std::string> testFilePaths;

namespace
{
template <typename T>
struct test_input_source{
  using value_type = typename T::value_type;

  using iterator = typename T::const_iterator;
  test_input_source() = default;
  test_input_source(iterator begin, iterator end) : current(begin), end(end)
  {
  }

  //FIXME remove const
  value_type next_char() const
  {
    return *current++;
  }

  bool eof() const
  {
    return current == end;
  }

  iterator mutable current{};
  iterator end{};
};

struct test_stream_input_source
{
  using value_type = char;

  using iterator = std::istreambuf_iterator<char>; 
  test_stream_input_source() = default;
  test_stream_input_source(iterator begin) : current(begin)
  {
  }

  //FIXME remove const
  value_type next_char() const
  {
    return *current++;
  }

  bool eof() const
  {
    return current == end;
  }

  iterator mutable current{};
  iterator end{};
};

bool operator==(test_stream_input_source const& lhs,
                test_stream_input_source const& rhs)
{
  return lhs.current == rhs.current && lhs.end == rhs.end;
}

bool operator!=(test_stream_input_source const& lhs,
                test_stream_input_source const& rhs)
{
  return !(lhs == rhs);
}

template <typename T>
bool operator==(test_input_source<T> const& lhs,
                test_input_source<T> const& rhs)
{
  return lhs.current == rhs.current && lhs.end == rhs.end;
}

template <typename T>
bool operator!=(test_input_source<T> const& lhs,
                test_input_source<T> const& rhs)
{
  return !(lhs == rhs);
}
}

TEST_CASE("b64 encode", "[b64]")
{
  SECTION("Two bytes of padding")
  {
    auto const text = "abcd"s;
    test_input_source<std::string> input(text.begin(), text.end());
    b64::stream_processor<decltype(input)> pr(input);
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZA==");
  }

  SECTION("One byte of padding")
  {
    auto const text = "abcde"s;
    test_input_source<std::string> input(text.begin(), text.end());
    b64::stream_processor<decltype(input)> pr(input);
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGU=");
  }

  SECTION("No padding")
  {
    auto const text = "abcdef"s;
    test_input_source<std::string> input(text.begin(), text.end());
    b64::stream_processor<decltype(input)> pr(input);
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::string s(it, end);
    CHECK(s == "YWJjZGVm");
  }

  SECTION("huge file")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0]);
    std::ifstream b64_random_data(testFilePaths[1]);

    test_stream_input_source input{std::istreambuf_iterator<char>(random_data)};
    b64::stream_processor<decltype(input)> pr(input);
    b64::encoding_iterator<decltype(pr)> it(pr);
    decltype(it) end;

    std::istreambuf_iterator<char> expectedB64It(b64_random_data);
    std::istreambuf_iterator<char> expectedEnd;
    CHECK(std::equal(expectedB64It, expectedEnd, it, end));
  }
}
