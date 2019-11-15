#pragma once

#include <cstring>
#include <utility>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/priority_tag.hpp>
#include <mgs/meta/sentinel_t.hpp>
#include <mgs/meta/static_asserts.hpp>

namespace mgs
{
namespace codecs
{
template <typename CodecTraits>
class basic_codec
{
public:
  using traits = CodecTraits;

  using default_encoded_output = typename traits::default_encoded_output;
  using default_decoded_output = typename traits::default_decoded_output;

private:
  template <typename IS>
  using encoder = meta::
      detected_t<detected::static_member_functions::make_encoder, traits, IS>;

  template <typename IS>
  using decoder = meta::
      detected_t<detected::static_member_functions::make_decoder, traits, IS>;

  template <typename T = default_encoded_output, typename IS>
  static codecs::codec_output<T, encoder<IS>> encode_impl(
      codecs::input_source<IS> is, meta::priority_tag<2>)
  {
    return output_traits<T>::create(traits::make_encoder(std::move(is)));
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      encoder<
          iterator_sentinel_source<meta::iterator_t<Range>, meta::sentinel_t<Range>>>>
  encode_impl(U& it, meta::priority_tag<1>)
  {
    return output_traits<T>::create(
        traits::make_encoder(make_iterator_sentinel_source(it)));
  }

  template <typename T = default_encoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      encoder<iterator_sentinel_source<meta::iterator_t<Range const>,
                                meta::sentinel_t<Range const>>>>
  encode_impl(U const& it, meta::priority_tag<0>)
  {
    return output_traits<T>::create(
        traits::make_encoder(make_iterator_sentinel_source(it)));
  }

  template <typename T = default_decoded_output, typename IS>
  static codecs::codec_output<T, decoder<IS>> decode_impl(
      codecs::input_source<IS> is, meta::priority_tag<2>)
  {
    return output_traits<T>::create(traits::make_decoder(std::move(is)));
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      decoder<
          iterator_sentinel_source<meta::iterator_t<Range>, meta::sentinel_t<Range>>>>
  decode_impl(U& it, meta::priority_tag<1>)
  {
    return output_traits<T>::create(
        traits::make_decoder(make_iterator_sentinel_source(it)));
  }

  template <typename T = default_decoded_output,
            typename U,
            typename Range = mgs::meta::input_range<U>>
  static mgs::codecs::codec_output<
      T,
      decoder<iterator_sentinel_source<meta::iterator_t<Range const>,
                                meta::sentinel_t<Range const>>>>
  decode_impl(U const& it, meta::priority_tag<0>)
  {
    return output_traits<T>::create(
        traits::make_decoder(make_iterator_sentinel_source(it)));
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
    static mgs::codecs::codec_output<T, encoder<iterator_sentinel_source<I, S>>>
    encode(meta::input_iterator<I> it, meta::sentinel_for<S, I> sent)
    {
      return output_traits<T>::create(traits::make_encoder(
          make_iterator_sentinel_source(std::move(it), std::move(sent))));
    }

    template <typename T = default_decoded_output, typename U>
    static auto decode(U&& val)
        -> decltype(decode_impl<T>(std::forward<U>(val),
                                   meta::priority_tag<2>{}))
    {
      return decode_impl<T>(std::forward<U>(val), meta::priority_tag<2>{});
    }

    template <typename T = default_decoded_output, typename I, typename S>
    static mgs::codecs::codec_output<T, decoder<iterator_sentinel_source<I, S>>>
    decode(meta::input_iterator<I> it, meta::sentinel_for<S, I> sent)
    {
      return output_traits<T>::create(traits::make_decoder(
          make_iterator_sentinel_source(std::move(it), std::move(sent))));
    }
};
}
}
