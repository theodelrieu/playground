#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/common_reference.hpp>
#include <mgs/meta/detected.hpp>

using namespace mgs::meta;
using namespace mgs::meta::concepts;

// Use a part of the range-v3 test suite for this specific trait:
namespace
{
struct B
{
};

struct D : B
{
};
}

TEST_CASE("CommonReference", "[meta][concepts][core]")
{
	static_assert(!std::is_reference<void>::value, "WTF");
	static_assert(!mgs::meta::detail::common_reference_bullet_one<int, void>::value, "bullet one");
	static_assert(!mgs::meta::detail::common_reference_bullet_three<int, void>::value, "bullet three");
	static_assert(!mgs::meta::detail::common_reference_bullet_four<int, void>::value, "bullet four");

  static_assert(!is_detected<common_reference_t>::value, "");
  static_assert(!is_detected<common_reference_t, int, void const>::value, "");
  static_assert(!is_detected<common_reference_t, int, char(&)[]>::value, "");
  static_assert(
      !is_detected<common_reference_t, int, struct incomplete*>::value, "");

  static_assert(std::is_same<common_reference_t<int>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, int>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, int, int>, int>::value,
                "");
  static_assert(has_common_reference<int, int>::value, "");

  static_assert(std::is_same<common_reference_t<int const&, int volatile&>,
                             int const volatile&>::value,
                "");
  static_assert(has_common_reference<int const&, int volatile&>::value, "");

  static_assert(std::is_same<common_reference_t<B&, D&>, B&>::value, "");
  static_assert(has_common_reference<B&, D&>::value, "");
  static_assert(std::is_same<common_reference_t<B&, D const&>, B const&>::value,
                "");
  static_assert(has_common_reference<B&, D const&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&, D const&, D&>, B const&>::value, "");
  static_assert(std::is_same<common_reference_t<B const&, D&>, B const&>::value,
                "");
  static_assert(has_common_reference<B const&, D&>::value, "");
  static_assert(std::is_same<common_reference_t<B&, D&, B&, D&>, B&>::value,
                "");

  static_assert(std::is_same<common_reference_t<B&&, D&&>, B&&>::value, "");
  static_assert(has_common_reference<B&&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&&, D&&>, B const&&>::value, "");
  static_assert(has_common_reference<B const&&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&&, D const&&>, B const&&>::value, "");
  static_assert(has_common_reference<B&&, D const&&>::value, "");

  static_assert(std::is_same<common_reference_t<B&, D&&>, B const&>::value, "");
  static_assert(has_common_reference<B&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&, D const&&>, B const&>::value, "");
  static_assert(has_common_reference<B&, D const&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&, D&&>, B const&>::value, "");
  static_assert(has_common_reference<B const&, D&&>::value, "");

  static_assert(std::is_same<common_reference_t<B&&, D&>, B const&>::value, "");
  static_assert(has_common_reference<B&&, D&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&&, D const&>, B const&>::value, "");
  static_assert(has_common_reference<B&&, D const&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&&, D&>, B const&>::value, "");
  static_assert(has_common_reference<B const&&, D&>::value, "");

  static_assert(std::is_same<common_reference_t<int, short>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, short&>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int&, short&>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int&, short>, int>::value, "");
  static_assert(has_common_reference<int, short>::value, "");
  static_assert(has_common_reference<int, short&>::value, "");
  static_assert(has_common_reference<int&, short&>::value, "");
  static_assert(has_common_reference<int&, short>::value, "");

  // tricky volatile reference case
  static_assert(
      std::is_same<common_reference_t<int&&, int volatile&>, int>::value, "");
  static_assert(has_common_reference<int&&, int volatile&>::value, "");
  static_assert(
      std::is_same<common_reference_t<int volatile&, int&&>, int>::value, "");
  static_assert(has_common_reference<int volatile&, int&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<int const volatile&&, int volatile&&>,
                   int const volatile&&>::value,
      "");
  static_assert(
      has_common_reference<int const volatile&&, int volatile&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<int&&, int const&, int volatile&>,
                   int const volatile&>(),
      "");

  // Array types?? Yup!
  static_assert(std::is_same<common_reference_t<int(&)[10], int(&&)[10]>,
                             int const(&)[10]>::value,
                "");
  static_assert(has_common_reference<int(&)[10], int(&&)[10]>::value, "");
  static_assert(
      std::is_same<common_reference_t<int const(&)[10], int volatile(&)[10]>,
                   int const volatile(&)[10]>::value,
      "");
  static_assert(
      has_common_reference<int const(&)[10], int volatile(&&)[10]>::value, "");
  static_assert(
      std::is_same<common_reference_t<int(&)[10], int(&)[11]>, int*>::value,
      "");
  static_assert(has_common_reference<int(&)[10], int(&)[11]>::value, "");
}
