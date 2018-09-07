#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/comparison/equality_comparable.hpp>
#include <mgs/meta/concepts/core/swappable.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace object_concepts = concepts::object;
namespace core_concepts = concepts::core;
namespace comparison_concepts = concepts::comparison;

namespace
{
struct almost_semiregular
{
  almost_semiregular() = delete;
};

bool operator==(almost_semiregular const&, almost_semiregular const&);
bool operator!=(almost_semiregular const&, almost_semiregular const&);

struct semiregular
{
};

struct almost_regular
{
};

bool operator==(almost_regular const&, almost_regular const&);

struct regular
{
};

bool operator==(regular const&, regular const&);
bool operator!=(regular const&, regular const&);
}

TEST_CASE("Semiregular", "[meta][concepts][object]")
{
  static_assert(object_concepts::is_regular<int>::value, "");
  static_assert(!object_concepts::is_regular<semiregular>::value, "");
  static_assert(object_concepts::is_regular<regular>::value, "");
  static_assert(!object_concepts::is_regular<almost_regular>::value, "");

  static_assert(!object_concepts::is_regular<void>::value, "");
  static_assert(!object_concepts::is_regular<struct incomplete>::value, "");

  generate_failed_requirements_tests<
      object_concepts::is_regular<almost_regular>>(
      std::tuple<
          comparison_concepts::is_equality_comparable<almost_regular>>{});

  generate_failed_requirements_tests<
      object_concepts::is_regular<almost_semiregular>>(
      std::tuple<object_concepts::is_semiregular<almost_semiregular>>{});
}
