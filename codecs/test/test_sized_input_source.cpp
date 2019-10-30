#include <catch2/catch.hpp>

#include <mgs/meta/ssize_t.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::codecs;

namespace
{
struct valid_sized_input_source
{
  using element_type = char;

  int read(char*, int);
  int max_remaining_size() const;
};

struct no_max_remaining_size_source
{
  using element_type = char;

  int read(char*, int);
};

struct invalid_return_type_source
{
  using element_type = char;

  int read(char*, int);
  char* max_remaining_size() const;
};

struct non_const_source
{
  using element_type = char;

  int read(char*, int);
  int max_remaining_size();
};
}

TEST_CASE("sized_input_source")
{
  static_assert(is_sized_input_source<valid_sized_input_source>::value, "");

  static_assert(!is_sized_input_source<struct incomplete>::value, "");
  static_assert(!is_sized_input_source<void>::value, "");
  static_assert(!is_sized_input_source<void*>::value, "");

  static_assert(!is_sized_input_source<no_max_remaining_size_source>::value,
                "");
  static_assert(!is_sized_input_source<invalid_return_type_source>::value, "");
  static_assert(!is_sized_input_source<non_const_source>::value, "");
}