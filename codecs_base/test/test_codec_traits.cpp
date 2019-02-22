#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch.hpp>

#include <mgs/codecs/output_traits.hpp>
#include <mgs/concepts/codec_traits.hpp>
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

struct valid_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_encoder(I, S);

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_decoder(I, S);
};

struct no_encoded_output_traits
{
  using default_decoded_output = std::string;

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_encoder(I, S);

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_decoder(I, S);
};

struct no_decoded_output_traits
{
  using default_encoded_output = std::string;

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_encoder(I, S);

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_decoder(I, S);
};

struct no_make_encoder_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_decoder(I, S);
};

struct no_make_decoder_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename I, typename S>
  static noop_range<char const*, char const*> make_encoder(I, S);
};

struct invalid_return_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename I, typename S>
  static int make_encoder(I, S);
  template <typename I, typename S>
  static noop_range<char const*, char const*> make_decoder(I, S);
};
}

TEST_CASE("CodecTraits", "[concepts]")
{
  static_assert(concepts::is_codec_traits<valid_traits>::value, "");
  static_assert(concepts::is_codec_traits<valid_traits,
                                          std::string,
                                          std::vector<unsigned char>,
                                          char*,
                                          unsigned char*>::value,
                "");

  static_assert(!concepts::is_codec_traits<no_encoded_output_traits>::value,
                "");
  static_assert(!concepts::is_codec_traits<no_decoded_output_traits>::value,
                "");
  static_assert(!concepts::is_codec_traits<no_make_encoder_traits>::value, "");
  static_assert(!concepts::is_codec_traits<no_make_decoder_traits>::value, "");
  static_assert(!concepts::is_codec_traits<invalid_return_traits>::value, "");
}
