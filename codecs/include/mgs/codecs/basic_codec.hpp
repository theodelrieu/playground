#pragma once

#include <cstring>
#include <utility>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/input_source_view.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/priority_tag.hpp>
#include <mgs/meta/sentinel_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename CodecTraits>
class basic_codec
{
  static_assert(is_codec_traits<CodecTraits>::value, "");

public:
  using traits = CodecTraits;

  using default_encoded_output = typename CodecTraits::default_encoded_output;
  using default_decoded_output = typename CodecTraits::default_decoded_output;

private:
  template <typename IS>
  using encoder =
      meta::detected_t<detected::static_member_functions::make_encoder,
                       traits,
                       std::add_lvalue_reference_t<IS>>;

  template <typename IS>
  using decoder =
      meta::detected_t<detected::static_member_functions::make_decoder,
                       traits,
                       std::add_lvalue_reference_t<IS>>;

  template <typename T = default_encoded_output, typename IS>
  static codecs::codec_output<T, encoder<IS>> encode_impl(
      codecs::input_source<IS>& is, meta::priority_tag<2>)
  {
    auto enc = traits::make_encoder(is);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      encoder<
          input_source_view<meta::iterator_t<Range>, meta::sentinel_t<Range>>>>
  encode_impl(U& it, meta::priority_tag<1>)
  {
    auto is = make_input_source_view(it);
    auto enc = traits::make_encoder(is);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      encoder<input_source_view<meta::iterator_t<Range const>,
                                meta::sentinel_t<Range const>>>>
  encode_impl(U const& it, meta::priority_tag<0>)
  {
    auto is = make_input_source_view(it);
    auto enc = traits::make_encoder(is);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_decoded_output, typename IS>
  static codecs::codec_output<T, decoder<IS>> decode_impl(
      codecs::input_source<IS>& is, meta::priority_tag<2>)
  {
    auto dec = CodecTraits::make_decoder(is);
    return output_traits<T>::create(dec);
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      decoder<
          input_source_view<meta::iterator_t<Range>, meta::sentinel_t<Range>>>>
  decode_impl(U& it, meta::priority_tag<1>)
  {
    auto is = make_input_source_view(it);
    auto dec = traits::make_decoder(is);
    return output_traits<T>::create(dec);
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      decoder<input_source_view<meta::iterator_t<Range const>,
                                meta::sentinel_t<Range const>>>>
  decode_impl(U const& it, meta::priority_tag<0>)
  {
    auto is = make_input_source_view(it);
    auto dec = traits::make_decoder(is);
    return output_traits<T>::create(dec);
  }

  public:
    template <typename T = default_encoded_output, typename U>
    static auto encode(U&& val)
        -> decltype(encode_impl<T>(std::forward<U>(val),
                                   meta::priority_tag<2>{}))
    {
      return encode_impl<T>(std::forward<U>(val), meta::priority_tag<2>{});
    }

    template <typename T = default_decoded_output, typename I, typename S>
    static mgs::codecs::codec_output<T, encoder<input_source_view<I, S>>>
    encode(meta::input_iterator<I> it, meta::sentinel_for<S, I> sent)
    {
      auto is = make_input_source_view(std::move(it), std::move(sent));
      auto enc = traits::make_encoder(is);
      return output_traits<T>::create(enc);
    }

    template <typename T = default_decoded_output, typename U>
    static auto decode(U&& val)
        -> decltype(decode_impl<T>(std::forward<U>(val),
                                   meta::priority_tag<2>{}))
    {
      return decode_impl<T>(std::forward<U>(val), meta::priority_tag<2>{});
    }

    template <typename T = default_decoded_output, typename I, typename S>
    static mgs::codecs::codec_output<T, decoder<input_source_view<I, S>>>
    decode(meta::input_iterator<I> it, meta::sentinel_for<S, I> sent)
    {
      auto is = make_input_source_view(std::move(it), std::move(sent));
      auto dec = traits::make_decoder(is);
      return output_traits<T>::create(dec);
    }
};
}
}
