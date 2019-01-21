#include <catch.hpp>

#include <mgs/meta/concepts/core/swappable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace core_concepts = concepts::core;

namespace 
{
struct not_swappable
{
  not_swappable(not_swappable const&) = delete;
};

struct adl_swappable
{
  adl_swappable(adl_swappable const&) = delete;
};

void swap(adl_swappable&, adl_swappable&);

template <std::size_t N>
void swap(adl_swappable (&)[N], adl_swappable (&)[N]);
}

TEST_CASE("Swappable", "[meta][concepts][core]")
{
  static_assert(core_concepts::is_swappable<int>::value, "");
  static_assert(core_concepts::is_swappable<adl_swappable>::value, "");
  static_assert(core_concepts::is_swappable<int[4]>::value, "");
  static_assert(core_concepts::is_swappable<int(*)[4]>::value, "");
  static_assert(core_concepts::is_swappable<int(&)[4]>::value, "");
  static_assert(core_concepts::is_swappable<adl_swappable[4]>::value, "");
  static_assert(core_concepts::is_swappable<adl_swappable(*)[4]>::value, "");
  static_assert(core_concepts::is_swappable<adl_swappable(&)[4]>::value, "");

  static_assert(!core_concepts::is_swappable<void>::value, "");
  static_assert(!core_concepts::is_swappable<not_swappable>::value, "");

  test_helpers::generate_failed_requirements_tests<core_concepts::is_swappable<void>>();
}
