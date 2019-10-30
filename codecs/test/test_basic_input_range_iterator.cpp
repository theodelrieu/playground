#include <forward_list>
#include <list>
#include <string>
#include <sstream>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/codecs/basic_input_range.hpp>
#include <mgs/meta/concepts/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/forward_iterator.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>

#include <mgs/codecs/basic_input_range.hpp>
// #include <test_helpers/noop_transformer.hpp>

using namespace mgs;
using namespace std::string_literals;

namespace
{
class noop_source
{
public:
  using element_type = char;

  explicit noop_source(std::string s) : _s(s)
  {
  }

  template <typename O>
  int read(O o, int n) const
  {
    auto const to_read = std::min<int>(n, _s.size() - _idx);
    o = std::copy_n(_s.data() + _idx, to_read, o);
    _idx += to_read;
    return to_read;
  }

private:
  std::string _s;
  int mutable _idx{0};
};

// FIXME replace
// template <typename I, typename S>
// using noop_encoder =
//     ranges::basic_transformed_input_range<test_helpers::noop_transformer<I, S>>;
}

TEST_CASE("transformed_input_range_iterator")
{
  SECTION("regular tests")
  {
    noop_source source("abcdefghijklmnopqrstuvwxyz");

    codecs::basic_input_range<noop_source> enc(std::move(source));
    auto current = enc.begin();
    auto end = enc.end();

    static_assert(meta::is_input_or_output_iterator<decltype(current)>::value,
                  "");
    static_assert(
        meta::is_sentinel_for<decltype(end), decltype(current)>::value, "");
    using encoder_tag = meta::detected::types::iterator_category<
        std::iterator_traits<decltype(current)>>;

    static_assert(meta::is_input_iterator<decltype(current)>::value, "");
    CHECK(*current == *current);
    CHECK(*current == 'a');
    CHECK(*++current == 'b');
    current++;
    CHECK(*current == 'c');
    CHECK(current != end);
    CHECK(current == current);
    CHECK(std::distance(current, end) == 24);
  }

  SECTION("empty range")
  {
    noop_source empty_source("");

    codecs::basic_input_range<noop_source> enc(std::move(empty_source));

    auto current = enc.begin();
    auto end = enc.end();

    CHECK(current == current);
    CHECK(current == end);
    CHECK(std::distance(current, end) == 0);
  }
}
