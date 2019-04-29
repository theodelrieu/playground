#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/static_asserts.hpp>
#include <mgs/ranges/basic_transformed_input_range.hpp>

#include <test_helpers/noop_transformer.hpp>

using namespace mgs;
using namespace mgs::codecs;

namespace
{
template <typename Iterator, typename Sentinel = Iterator>
class noop_range : public mgs::ranges::basic_transformed_input_range<
                       test_helpers::noop_transformer<Iterator, Sentinel>>
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  using mgs::ranges::basic_transformed_input_range<
      test_helpers::noop_transformer<Iterator,
                                     Sentinel>>::basic_transformed_input_range;
};

struct invalid
{
};

struct valid
{
};
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid>
{
  template <typename U>
  static valid create(U);
};
}
}

TEST_CASE("CodecOutput", "[concepts]")
{
  // static_assert(
  //     is_codec_output<std::string, noop_range<char const*>>::value,
  //     "");
  // static_assert(
  //     is_codec_output<valid, noop_range<char const*>>::value, "");
  //
  // static_assert(
  //     !is_codec_output<invalid, noop_range<char const*>>::value, "");
  //
  // static_assert(
  //     !is_codec_output<void, noop_range<char const*>>::value,
  //     "");
  // static_assert(
  //     !is_codec_output<void*, noop_range<char const*>>::value,
  //     "");
  // static_assert(
  //     !is_codec_output<struct incomplete, noop_range<char const*>>::value,
  //     "");
  // static_assert(
  //     !is_codec_output<struct incomplete*, noop_range<char const*>>::value,
  //     "");
  meta::trigger_static_asserts<is_codec_output<std::string, noop_range<char const*>>>();
}
