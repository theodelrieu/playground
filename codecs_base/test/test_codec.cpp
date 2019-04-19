#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/codecs/basic_codec.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/concepts/codec.hpp>
#include <mgs/meta/static_asserts.hpp>
#include <mgs/ranges/basic_transformed_input_range.hpp>

#include <test_helpers/noop_transformer.hpp>

using namespace mgs;

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

struct noop_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename Iterator, typename Sentinel>
  static noop_range<Iterator, Sentinel> make_encoder(Iterator begin, Sentinel end);

  template <typename Iterator, typename Sentinel>
  static noop_range<Iterator, Sentinel> make_decoder(Iterator begin, Sentinel end);
};

using noop_codec = mgs::codecs::basic_codec<noop_codec_traits>;

struct no_encode_char_ptr_codec : mgs::codecs::basic_codec<noop_codec_traits>
{
  using mgs::codecs::basic_codec<noop_codec_traits>::basic_codec;

  using mgs::codecs::basic_codec<noop_codec_traits>::encode;
  static mgs::codecs::basic_codec<noop_codec_traits>::default_encoded_output encode(char const*, char const*) = delete;
};
}

TEST_CASE("Codec", "[concepts]")
{
  static_assert(concepts::is_codec<noop_codec>::value, "");
  static_assert(concepts::is_codec<no_encode_char_ptr_codec>::value, "");
  static_assert(concepts::is_codec<no_encode_char_ptr_codec,
                                   unsigned char const(&)[1]>::value,
                "");
  static_assert(concepts::is_codec<noop_codec,
                                   std::string,
                                   std::vector<char>,
                                   std::string,
                                   std::string,
                                   std::istreambuf_iterator<char>,
                                   char*>::value,
                "");
  static_assert(
      !concepts::is_codec<no_encode_char_ptr_codec, char const(&)[1]>::value,
      "");

  static_assert(concepts::is_codec<noop_codec>::value, "");
}
