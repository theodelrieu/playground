#include <istream>
#include <iterator>
#include <string>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/core/common_reference.hpp>
#include <mgs/meta/concepts/iterator/dereferenceable.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/weakly_incrementable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
namespace iterator_concepts = concepts::iterator;
namespace core_concepts = concepts::core;

namespace
{
struct output_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = void;
  using iterator_category = std::output_iterator_tag;

  output_iterator& operator++();
  void operator++(int);
  void operator*();
};

struct invalid_reference_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = std::string;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  invalid_reference_iterator& operator++();
  invalid_reference_iterator operator++(int);
  reference operator*();
};

struct valid_input_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  valid_input_iterator& operator++();
  valid_input_iterator operator++(int);
  reference operator*();
};
}

TEST_CASE("InputIterator", "[meta][concepts][iterator]")
{
  static_assert(iterator_concepts::is_input_iterator<char*>::value, "");
  static_assert(iterator_concepts::is_input_iterator<
                    std::istreambuf_iterator<char>>::value,
                "");
  static_assert(
      iterator_concepts::is_input_iterator<valid_input_iterator>::value, "");

  static_assert(!iterator_concepts::is_input_iterator<
                    std::back_insert_iterator<std::string>>::value,
                "");
  static_assert(!iterator_concepts::is_input_iterator<output_iterator>::value,
                "");
  static_assert(
      !iterator_concepts::is_input_iterator<invalid_reference_iterator>::value,
      "");
  static_assert(!iterator_concepts::is_input_iterator<void>::value, "");
  static_assert(!iterator_concepts::is_input_iterator<struct incomplete>::value, "");

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_input_iterator<int>,
      std::tuple<iterator_concepts::is_iterator<int>,
                 iterator_concepts::is_dereferenceable<int>,
                 iterator_concepts::is_readable<int>>>();

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_input_iterator<invalid_reference_iterator>,
      std::tuple<
          iterator_concepts::is_readable<invalid_reference_iterator>,
          core_concepts::has_common_reference<std::string&&, int&>,
          core_concepts::has_common_reference<std::string&&, int const&>>>();

  test_helpers::generate_failed_requirements_tests<
      iterator_concepts::is_input_iterator<void*>,
      std::tuple<iterator_concepts::is_iterator<void*>,
                 iterator_concepts::is_dereferenceable<void*>,
                 iterator_concepts::is_weakly_incrementable<void*>,
                 iterator_concepts::is_readable<void*>>>();
}
