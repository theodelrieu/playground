#include <iterator>
#include <tuple>
#include <iostream>

#include <catch.hpp>

#include <mgs/meta/concepts/bidirectional_iterator.hpp>

#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/concepts/sized_sentinel.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
using namespace mgs::meta::concepts;

namespace
{
struct non_semiregular
{
  non_semiregular() = delete;
};

template <typename T>
bool operator==(T*, non_semiregular);
template <typename T>
bool operator!=(T*, non_semiregular);
template <typename T>
bool operator==(non_semiregular, T*);
template <typename T>
bool operator!=(non_semiregular, T*);

struct pointer_sentinel
{
};

template <typename T>
bool operator==(T*, pointer_sentinel);
template <typename T>
bool operator!=(T*, pointer_sentinel);
template <typename T>
bool operator==(pointer_sentinel, T*);
template <typename T>
bool operator!=(pointer_sentinel, T*);

struct valid_sized_sentinel : pointer_sentinel
{
};

template <typename T>
std::ptrdiff_t operator-(valid_sized_sentinel, T*);

template <typename T>
std::ptrdiff_t operator-(T*, valid_sized_sentinel);

struct invalid_difference_type_sized_sentinel : valid_sized_sentinel
{
};

template <typename T>
void operator-(invalid_difference_type_sized_sentinel, T*);

template <typename T>
void operator-(T*, invalid_difference_type_sized_sentinel);
}

TEST_CASE("SizedSentinel", "[meta][concepts][iterator]")
{
  static_assert(is_sized_sentinel<char*, char*>::value, "");
  static_assert(is_sized_sentinel<valid_sized_sentinel, char*>::value, "");

  static_assert(!is_sized_sentinel<pointer_sentinel, void*>::value, "");
  static_assert(
      !is_sized_sentinel<invalid_difference_type_sized_sentinel, void*>::value,
      "");
  static_assert(!is_sized_sentinel<non_semiregular, char*>::value, "");

  static_assert(!is_sized_sentinel<void, void>::value, "");
  static_assert(!is_sized_sentinel<struct incomplete, incomplete>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_sized_sentinel<pointer_sentinel, char*>>();

  test_helpers::generate_failed_requirements_tests<
      is_sized_sentinel<invalid_difference_type_sized_sentinel, char*>>();

  test_helpers::generate_failed_requirements_tests<
      is_sized_sentinel<non_semiregular, char*>,
      std::tuple<is_sentinel<non_semiregular, char*>,
                 is_semiregular<non_semiregular>>>();
}
