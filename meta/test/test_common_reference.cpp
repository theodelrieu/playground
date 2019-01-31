#include <iterator>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/type.hpp>

using namespace mgs::meta;

// Use a part of the range-v3 test suite for this specific trait:
namespace 
{
struct B {};
struct D : B
{
};
}

TEST_CASE("CommonReference", "[meta][concepts][core]")
{
  static_assert(!is_detected<detected::types::type, common_reference<>>::value,
                "");
  static_assert(std::is_same<common_reference<int>::type, int>::value, "");
  static_assert(std::is_same<common_reference<int, int>::type, int>::value, "");
  static_assert(std::is_same<common_reference<int, int, int>::type, int>::value, "");

  static_assert(std::is_same<common_reference<int const&, int volatile&>::type,
                             int const volatile&>::value,
                "");

    static_assert(std::is_same<common_reference_t<B &, D &>, B &>::value, "");
    static_assert(std::is_same<common_reference_t<B &, D const &>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B &, D const &, D &>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B const &, D &>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B &, D &, B &, D &>, B &>::value, "");

    static_assert(std::is_same<common_reference_t<B &&, D &&>, B &&>::value, "");
    static_assert(std::is_same<common_reference_t<B const &&, D &&>, B const &&>::value, "");
    static_assert(std::is_same<common_reference_t<B &&, D const &&>, B const &&>::value, "");

    static_assert(std::is_same<common_reference_t<B &, D &&>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B &, D const &&>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B const &, D &&>, B const &>::value, "");

    static_assert(std::is_same<common_reference_t<B &&, D &>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B &&, D const &>, B const &>::value, "");
    static_assert(std::is_same<common_reference_t<B const &&, D &>, B const &>::value, "");

    static_assert(std::is_same<common_reference_t<int, short>, int>::value, "");
    static_assert(std::is_same<common_reference_t<int, short &>, int>::value, "");
    static_assert(std::is_same<common_reference_t<int &, short &>, int>::value, "");
    static_assert(std::is_same<common_reference_t<int &, short>, int>::value, "");

    // tricky volatile reference case
    static_assert(std::is_same<common_reference_t<int &&, int volatile &>, int>::value, "");
    static_assert(std::is_same<common_reference_t<int volatile &, int &&>, int>::value, "");
    static_assert(std::is_same<common_reference_t<int const volatile &&, int volatile &&>, int const volatile &&>::value, "");
    static_assert(std::is_same<common_reference_t<int &&, int const &, int volatile &>, int const volatile &>(), "");

    // Array types?? Yup!
    static_assert(std::is_same<common_reference_t<int (&)[10], int (&&)[10]>, int const(&)[10]>::value, "");
    static_assert(std::is_same<common_reference_t<int const (&)[10], int volatile (&)[10]>, int const volatile(&)[10]>::value, "");
    static_assert(std::is_same<common_reference_t<int (&)[10], int (&)[11]>, int *>::value, "");
}
