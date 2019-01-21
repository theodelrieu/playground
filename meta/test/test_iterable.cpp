#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/meta/concepts/comparison/weakly_equality_comparable_with.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/static_asserts.hpp>

#include "meta_test_helpers.hpp"

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;
namespace comparison_concepts = concepts::comparison;

namespace
{
struct non_iterable
{
};

struct invalid_iterable
{
};

struct invalid_sentinel{};

char* begin(invalid_iterable&);
invalid_sentinel end(invalid_iterable&);

struct iterable
{
};

char* begin(iterable&);
char* end(iterable&);

struct sentinel
{
};

bool operator==(char*, sentinel);
bool operator!=(char*, sentinel);
bool operator==(sentinel, char*);
bool operator!=(sentinel, char*);

struct sentinel_iterable
{
};

char* begin(sentinel_iterable&);
sentinel end(sentinel_iterable&);
}

TEST_CASE("Iterable", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_iterable<char [1]>::value, "");
  static_assert(iterator_concepts::is_iterable<char const[1]>::value, "");
  static_assert(iterator_concepts::is_iterable<std::string>::value, "");
  static_assert(iterator_concepts::is_iterable<std::vector<int>>::value, "");
  static_assert(iterator_concepts::is_iterable<iterable>::value, "");
  static_assert(iterator_concepts::is_iterable<sentinel_iterable>::value, "");

  static_assert(!iterator_concepts::is_iterable<char *>::value, "");
  static_assert(!iterator_concepts::is_iterable<non_iterable>::value, "");
  static_assert(!iterator_concepts::is_iterable<invalid_iterable>::value, "");

  static_assert(!iterator_concepts::is_iterable<struct incomplete>::value, "");
  static_assert(!iterator_concepts::is_iterable<void>::value, "");

  generate_failed_requirements_tests<
      iterator_concepts::is_iterable<non_iterable>>();

  generate_failed_requirements_tests<
      iterator_concepts::is_iterable<invalid_iterable>,
      std::tuple<iterator_concepts::is_sentinel<invalid_sentinel, char*>,
                 comparison_concepts::is_weakly_equality_comparable_with<
                     invalid_sentinel,
                     char*>>>();
}
