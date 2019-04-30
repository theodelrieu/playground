#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/readable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
template <typename T> struct S;
template <std::size_t N> struct S2;
TEST_CASE("Readable", "[meta][concepts][iterator]")
{
  static_assert(!is_readable<void>::value, "");
  static_assert(!is_readable<struct incomplete>::value, "");

  static_assert(!is_readable<void*>::value, "");
  static_assert(!is_readable<struct incomplete*>::value, "");
  static_assert(!is_readable<std::back_insert_iterator<std::string>>::value,
                "");
  static_assert(!is_readable<std::ostreambuf_iterator<char>>::value, "");
  static_assert(!is_readable<std::unique_ptr<void>>::value, "");

  static_assert(is_readable<char*>::value, "");
  static_assert(is_readable<int const *>::value, "");
	// has_common_reference<volatile&, volatile&&> does not work on MSVC, so the following fails
#ifndef _MSC_VER
  static_assert(is_readable<int const volatile*>::value, "");
#endif
  static_assert(is_readable<std::unique_ptr<char>>::value, "");
  static_assert(is_readable<std::istreambuf_iterator<char>>::value, "");
}
