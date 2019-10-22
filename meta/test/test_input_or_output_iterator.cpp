#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/detail/iterator_traits.hpp>
#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;

namespace
{
struct output_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::output_iterator_tag;

  output_iterator& operator++();
  output_iterator& operator++(int);
  output_iterator& operator*();
};

struct non_dereferencable_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::output_iterator_tag;

  non_dereferencable_iterator& operator++();
  non_dereferencable_iterator& operator++(int);
  void operator*();
};

struct non_weakly_incrementable_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::output_iterator_tag;

  non_weakly_incrementable_iterator& operator*();
};
}

TEST_CASE("input_or_output_iterator")
{
  static_assert(is_input_or_output_iterator<char*>::value, "");
  static_assert(is_input_or_output_iterator<output_iterator>::value, "");
  static_assert(is_input_or_output_iterator<
                    std::back_insert_iterator<std::string>>::value,
                "");

  static_assert(!is_input_or_output_iterator<char>::value, "");
  static_assert(!is_input_or_output_iterator<void>::value, "");
  static_assert(!is_input_or_output_iterator<struct incomplete>::value, "");
  static_assert(!is_input_or_output_iterator<void*>::value, "");

  static_assert(
      !is_input_or_output_iterator<non_dereferencable_iterator>::value, "");
  static_assert(
      !is_input_or_output_iterator<non_weakly_incrementable_iterator>::value,
      "");

  test_helpers::generate_failed_requirements_tests<
      is_input_or_output_iterator<int>,
      std::tuple<is_dereferenceable<int>>>();

  test_helpers::generate_failed_requirements_tests<
      is_input_or_output_iterator<non_dereferencable_iterator>,
      std::tuple<is_dereferenceable<non_dereferencable_iterator>>>();

  test_helpers::generate_failed_requirements_tests<
      is_input_or_output_iterator<non_weakly_incrementable_iterator>,
      std::tuple<is_weakly_incrementable<non_weakly_incrementable_iterator>>>();
}
