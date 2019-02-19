#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/concepts/sized_input_transformer.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::concepts;

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
  std::size_t max_transformed_size() const;

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct no_max_transformed_size_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_max_transformed_size_transformer();
  no_max_transformed_size_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct invalid_return_type_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  invalid_return_type_transformer();
  invalid_return_type_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);
  int max_transformed_size() const;

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct non_const_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  non_const_transformer();
  non_const_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);
  std::size_t max_transformed_size();

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};
}

TEST_CASE("SizedInputTransformer", "[concepts]")
{
  static_assert(is_sized_input_transformer<valid_transformer>::value, "");

  static_assert(!is_sized_input_transformer<struct incomplete>::value, "");
  static_assert(!is_sized_input_transformer<void>::value, "");
  static_assert(!is_sized_input_transformer<void*>::value, "");

  static_assert(
      !is_sized_input_transformer<no_max_transformed_size_transformer>::value,
      "");
  static_assert(
      !is_sized_input_transformer<invalid_return_type_transformer>::value, "");
  static_assert(!is_sized_input_transformer<non_const_transformer>::value, "");
}
