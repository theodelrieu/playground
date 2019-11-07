#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/concepts/totally_ordered.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/static_asserts.hpp>

using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct valid_input_source
{
  using element_type = char;

  int read(char*, int);
};

struct non_copyable_input_source
{
  using element_type = char;

  non_copyable_input_source(non_copyable_input_source const&) = delete;

  int read(char*, int);
};

struct no_element_type_input_source
{
  int read(char*, int);
};

struct invalid_read_input_source
{
  using element_type = char;

  std::string read(char*, int);
};

struct invalid_element_type_input_source
{
  using element_type = double;

  std::string read(char*, int);
};
}

TEST_CASE("input_source")
{
  static_assert(is_input_source<valid_input_source>::value, "");

  static_assert(!is_input_source<struct incomplete>::value, "");
  static_assert(!is_input_source<void>::value, "");
  static_assert(!is_input_source<void*>::value, "");

  static_assert(!is_input_source<non_copyable_input_source>::value, "");
  static_assert(!is_input_source<no_element_type_input_source>::value, "");
  static_assert(
      !is_input_source<invalid_read_input_source>::value, "");
  static_assert(!is_input_source<invalid_element_type_input_source>::value, "");
}
