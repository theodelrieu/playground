#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch.hpp>

#include <mgs/meta/concepts/iterator/readable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;

TEST_CASE("Readable", "[meta][concepts][iterator]")
{
  static_assert(!iterator_concepts::is_readable<void>::value, "");
  static_assert(!iterator_concepts::is_readable<struct incomplete>::value, "");

  static_assert(!iterator_concepts::is_readable<void*>::value, "");
  static_assert(!iterator_concepts::is_readable<struct incomplete*>::value, "");
  static_assert(!iterator_concepts::is_readable<
                    std::back_insert_iterator<std::string>>::value,
                "");
  static_assert(
      !iterator_concepts::is_readable<std::ostreambuf_iterator<char>>::value,
      "");
  static_assert(!iterator_concepts::is_readable<std::unique_ptr<void>>::value,
                "");

  static_assert(iterator_concepts::is_readable<char*>::value, "");
  static_assert(iterator_concepts::is_readable<int const volatile*>::value, "");
  static_assert(iterator_concepts::is_readable<std::unique_ptr<char>>::value,
                "");
  static_assert(
      iterator_concepts::is_readable<std::istreambuf_iterator<char>>::value,
      "");
}
