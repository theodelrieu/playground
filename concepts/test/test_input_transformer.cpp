#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/concepts/input_transformer.hpp>
#include <mgs/meta/concepts/iterator.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel.hpp>
#include <mgs/meta/concepts/sized_sentinel.hpp>
#include <mgs/meta/concepts/strict_totally_ordered.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs;

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

struct no_underlying_iterator_transformer
{
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_underlying_iterator_transformer();
  no_underlying_iterator_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  char const*& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct no_underlying_sentinel_transformer
{
  using underlying_iterator = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_underlying_sentinel_transformer();
  no_underlying_sentinel_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  char const*& get_sentinel() const;
};

struct non_iterator_sentinel_constructible_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  non_iterator_sentinel_constructible_transformer();

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct no_buffer_type_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;

  no_buffer_type_transformer();
  no_buffer_type_transformer(char const* begin, char const* end);

  void operator()(std::vector<std::uint8_t>& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct no_function_call_operator_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_function_call_operator_transformer();
  no_function_call_operator_transformer(char const* begin, char const* end);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct no_get_iterator_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_get_iterator_transformer();
  no_get_iterator_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_sentinel const& get_sentinel() const;
};

struct no_get_sentinel_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  no_get_sentinel_transformer();
  no_get_sentinel_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
};

struct invalid_underlying_sentinel_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = std::istreambuf_iterator<char>;
  using buffer_type = std::vector<std::uint8_t>;

  invalid_underlying_sentinel_transformer();
  invalid_underlying_sentinel_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct invalid_underlying_iterator_transformer
{
  using underlying_iterator = int;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<std::uint8_t>;

  invalid_underlying_iterator_transformer();
  invalid_underlying_iterator_transformer(int begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};

struct invalid_buffer_type_transformer
{
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::list<std::uint8_t>;

  invalid_buffer_type_transformer();
  invalid_buffer_type_transformer(char const* begin, char const* end);

  void operator()(buffer_type& out);

  underlying_iterator const& get_iterator() const;
  underlying_sentinel const& get_sentinel() const;
};
}

TEST_CASE("InputTransformer", "[concepts]")
{
  static_assert(is_input_transformer<valid_transformer>::value, "");

  static_assert(!is_input_transformer<struct incomplete>::value, "");
  static_assert(!is_input_transformer<void>::value, "");
  static_assert(!is_input_transformer<void*>::value, "");

  static_assert(
      !is_input_transformer<non_default_constructible_transformer>::value, "");
  static_assert(
      !is_input_transformer<no_underlying_iterator_transformer>::value, "");
  static_assert(
      !is_input_transformer<no_underlying_sentinel_transformer>::value, "");
  static_assert(!is_input_transformer<
                    non_iterator_sentinel_constructible_transformer>::value,
                "");
  static_assert(!is_input_transformer<no_buffer_type_transformer>::value, "");
  static_assert(
      !is_input_transformer<no_function_call_operator_transformer>::value, "");
  static_assert(!is_input_transformer<no_get_iterator_transformer>::value, "");
  static_assert(!is_input_transformer<no_get_sentinel_transformer>::value, "");

  static_assert(
      !is_input_transformer<invalid_underlying_sentinel_transformer>::value,
      "");

  static_assert(
      !is_input_transformer<invalid_underlying_iterator_transformer>::value,
      "");

  static_assert(!is_input_transformer<invalid_buffer_type_transformer>::value,
                "");

  test_helpers::generate_failed_requirements_tests<
      is_input_transformer<non_default_constructible_transformer>,
      std::tuple<
          meta::is_semiregular<non_default_constructible_transformer>>>();

  test_helpers::generate_failed_requirements_tests<
      is_input_transformer<invalid_underlying_sentinel_transformer>,
      std::tuple<
          meta::is_sentinel<
              invalid_underlying_sentinel_transformer::underlying_sentinel,
              invalid_underlying_sentinel_transformer::underlying_iterator>,
          meta::is_weakly_equality_comparable_with<
              invalid_underlying_sentinel_transformer::underlying_sentinel,
              invalid_underlying_sentinel_transformer::underlying_iterator>>>();
}
