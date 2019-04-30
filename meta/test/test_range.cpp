#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct non_range
{
};

struct invalid_range
{
};

struct invalid_sentinel
{
};

char* begin(invalid_range&);
invalid_sentinel end(invalid_range&);

struct range
{
};

char* begin(range&);
char* end(range&);

struct sentinel
{
};

bool operator==(char*, sentinel);
bool operator!=(char*, sentinel);
bool operator==(sentinel, char*);
bool operator!=(sentinel, char*);

struct sentinel_range
{
};

char* begin(sentinel_range&);
sentinel end(sentinel_range&);
}

TEST_CASE("Range", "[concepts]")
{
  static_assert(is_range<char[1]>::value, "");
  static_assert(is_range<char const[1]>::value, "");
  static_assert(is_range<std::string>::value, "");
  static_assert(is_range<std::vector<int>>::value, "");
  static_assert(is_range<range>::value, "");
  static_assert(is_range<sentinel_range>::value, "");

  static_assert(!is_range<char*>::value, "");
  static_assert(!is_range<non_range>::value, "");
  static_assert(!is_range<invalid_range>::value, "");

  static_assert(!is_range<struct incomplete>::value, "");
  static_assert(!is_range<void>::value, "");

  test_helpers::generate_failed_requirements_tests<is_range<non_range>>();

  test_helpers::generate_failed_requirements_tests<
      is_range<invalid_range>,
      std::tuple<
          is_sentinel<invalid_sentinel, char*>,
          is_weakly_equality_comparable_with<invalid_sentinel, char*>>>();
}
