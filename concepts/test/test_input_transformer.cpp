#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/concepts/input_transformer.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/concepts/object/semiregular.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs;

namespace object_concepts = mgs::meta::concepts::object;

namespace
{
struct valid_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  valid_transformer();
  valid_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct non_default_constructible_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  non_default_constructible_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};
}

TEST_CASE("InputTransformer", "[concepts]")
{
  static_assert(concepts::is_input_transformer<valid_transformer>::value, "");

  static_assert(!concepts::is_input_transformer<struct incomplete>::value, "");
  static_assert(!concepts::is_input_transformer<void>::value, "");

  static_assert(!concepts::is_input_transformer<
                    non_default_constructible_transformer>::value,
                "");

  test_helpers::generate_failed_requirements_tests<
      concepts::is_input_transformer<non_default_constructible_transformer>,
      std::tuple<object_concepts::is_semiregular<
          non_default_constructible_transformer>>>();
}
