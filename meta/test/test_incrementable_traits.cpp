#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/meta/incrementable_traits.hpp>
#include <mgs/meta/iter_difference_t.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct valid
{
};

unsigned int operator-(valid, valid);

struct both
{
  using difference_type = char;
};

unsigned int operator-(valid, valid);
}

TEST_CASE("incrementable_traits", "[meta]")
{
  static_assert(!is_detected<iter_difference_t, void>::value, "");
  static_assert(!is_detected<iter_difference_t, struct incomplete>::value, "");
  static_assert(!is_detected<iter_difference_t, void*>::value, "");

  static_assert(std::is_same<iter_difference_t<struct incomplete*>,
                             std::ptrdiff_t>::value,
                "");
  static_assert(
      std::is_same<iter_difference_t<std::vector<int>::iterator>,
                   std::iterator_traits<
                       std::vector<int>::iterator>::difference_type>::value,
      "");
  static_assert(std::is_same<iter_difference_t<int*>, std::ptrdiff_t>::value,
                "");
  static_assert(std::is_same<iter_difference_t<int const*>, std::ptrdiff_t>::value,
                "");
  static_assert(std::is_same<iter_difference_t<int const* const>, std::ptrdiff_t>::value,
                "");
  static_assert(std::is_same<iter_difference_t<valid>, int>::value, "");
  static_assert(std::is_same<iter_difference_t<both>, char>::value, "");
  static_assert(
      std::is_same<
          iter_difference_t<std::back_insert_iterator<std::vector<int>>>,
          void>::value,
      "");
}
