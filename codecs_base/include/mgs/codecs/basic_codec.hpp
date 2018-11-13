#pragma once

#include <utility>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
inline namespace v1
{
namespace codecs
{
template <typename CodecTraits>
class basic_codec
{
public:
  template <typename Iterator, typename Sentinel = Iterator>
  using encoder = decltype(CodecTraits::make_encoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  // TODO SFINAE + detected + codec_traits concept
  template <typename Iterator, typename Sentinel = Iterator>
  using decoder = decltype(CodecTraits::make_decoder(std::declval<Iterator>(),
                                                     std::declval<Sentinel>()));

  using default_encoded_output = typename CodecTraits::default_encoded_output;
  using default_decoded_output = typename CodecTraits::default_decoded_output;

  template <
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value>>
  static auto make_encoder(Iterator begin, Sentinel end)
  {
    return CodecTraits::make_encoder(std::move(begin), std::move(end));
  }

  template <
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value>>
  static auto make_decoder(Iterator begin, Sentinel end)
  {
    return CodecTraits::make_decoder(std::move(begin), std::move(end));
  }

  template <
      typename T = default_encoded_output,
      typename Iterator = void,
      typename Sentinel = void,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
          adapters::concepts::is_iterable_input_adapter<
              encoder<Iterator, Sentinel>>::value &&
          concepts::is_codec_output<T, encoder<Iterator, Sentinel>>::value>>
  static auto encode(Iterator it, Sentinel sent)
  {
    using std::begin;
    using std::end;

    auto enc = CodecTraits::make_encoder(it, sent);
    return output_traits<T>::create(enc);
  }

  template <typename T = default_encoded_output,
            typename Iterable = void,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterable<Iterable>::value>>
  static auto encode(Iterable const& it) -> decltype(basic_codec::encode<T>(
      std::declval<meta::result_of_begin<Iterable const&>>(),
      std::declval<meta::result_of_end<Iterable const&>>()))
  {
    using std::begin;
    using std::end;

    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <
      typename T = default_decoded_output,
      typename Iterator = void,
      typename Sentinel = void,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
          adapters::concepts::is_iterable_input_adapter<
              decoder<Iterator, Sentinel>>::value &&
          concepts::is_codec_output<T, decoder<Iterator, Sentinel>>::value>>
  static auto decode(Iterator it, Sentinel sent)
  {
    using std::begin;
    using std::end;

    auto dec = CodecTraits::make_decoder(it, sent);
    return output_traits<T>::create(dec);
  }


  template <typename T = default_decoded_output,
            typename Iterable = void,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterable<Iterable>::value>>
  static auto decode(Iterable const& it) -> decltype(basic_codec::decode<T>(
      std::declval<meta::result_of_begin<Iterable const&>>(),
      std::declval<meta::result_of_end<Iterable const&>>()))
  {
    using std::begin;
    using std::end;

    return basic_codec::decode<T>(begin(it), end(it));
  }
};
}
}
}
