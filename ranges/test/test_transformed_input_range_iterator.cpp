#include <forward_list>
#include <list>
#include <sstream>
#include <tuple>

#include <catch.hpp>

#include <mgs/meta/concepts/core/derived_from.hpp>
#include <mgs/meta/concepts/iterator/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/iterator/forward_iterator.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterator.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

#include <mgs/ranges/basic_transformed_input_range.hpp>
#include <mgs/ranges/detail/transformed_input_range_iterator.hpp>
#include <test_helpers/noop_transformer.hpp>

using namespace mgs;
using namespace std::string_literals;

namespace
{
template <typename I, typename S>
using noop_encoder =
    ranges::basic_transformed_input_range<test_helpers::noop_transformer<I, S>>;
}

TEST_CASE("transformed_input_range_iterator", "[ranges][transformed_input_range_iterator]")
{
  std::stringstream ss("abcdefghijklmnopqrstuvwxyz");
  using Iterator = std::istreambuf_iterator<char>;
  using Sentinel = Iterator;

  Iterator it{ss};
  Sentinel sent{};

  noop_encoder<Iterator, Sentinel> encoder{it, sent};
  auto current = encoder.begin();
  auto end = encoder.end();

  static_assert(meta::concepts::iterator::is_iterator<decltype(current)>::value,
                "");
  static_assert(meta::concepts::iterator::is_sentinel<decltype(end),
                                                      decltype(current)>::value,
                "");
  using encoder_tag = meta::detected::types::iterator_category<
      std::iterator_traits<decltype(current)>>;
  using underlying_tag =
      meta::detected::types::iterator_category<std::iterator_traits<Iterator>>;
  static_assert(std::is_same<underlying_tag, encoder_tag>::value, "");

  static_assert(
      meta::concepts::iterator::is_input_iterator<decltype(current)>::value,
      "");
  CHECK(*current == *current);
  CHECK(*current == 'a');
  CHECK(*++current == 'b');
  current++;
  CHECK(*current == 'c');
  CHECK(current != end);
  CHECK(current == current);
  CHECK(std::distance(current, end) == 24);
}
