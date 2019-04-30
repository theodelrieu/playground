#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

#include <mgs/meta/concepts/strict_totally_ordered.hpp>

using namespace mgs::meta;

namespace
{
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

struct non_weakly_equality_comparable
{
};

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
}

TEST_CASE("Sentinel", "[meta][concepts][iterator]")
{
  static_assert(is_sentinel<char*, char*>::value, "");
  static_assert(is_sentinel<pointer_sentinel, char*>::value, "");

  static_assert(!is_sentinel<non_weakly_equality_comparable, char*>::value, "");
  static_assert(!is_sentinel<pointer_sentinel, void*>::value, "");
  static_assert(!is_sentinel<non_semiregular, char*>::value, "");

  static_assert(!is_sentinel<void, void>::value, "");
  static_assert(!is_sentinel<struct incomplete, incomplete>::value, "");

  test_helpers::generate_failed_requirements_tests<
      is_sentinel<non_weakly_equality_comparable, char*>,
      std::tuple<
          is_weakly_equality_comparable_with<non_weakly_equality_comparable,
                                             char*>>>();

  test_helpers::generate_failed_requirements_tests<
      is_sentinel<pointer_sentinel, void*>,
      std::tuple<is_iterator<void*>,
                 is_dereferenceable<void*>,
                 is_weakly_incrementable<void*>>>();

  test_helpers::generate_failed_requirements_tests<
      is_sentinel<non_semiregular, char*>,
      std::tuple<is_semiregular<non_semiregular>>>();
}
