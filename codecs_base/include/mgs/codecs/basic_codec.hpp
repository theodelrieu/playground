#pragma once

#include <utility>

#include <mgs/adapters/concepts/iterable_input_adapter.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
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
template <
    template <typename Iterator, typename Sentinel, typename...> class Encoder,
    template <typename Iterator, typename Sentinel, typename...> class Decoder>
class basic_codec
{
public:
  template <
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value>>
  static Encoder<Iterator, Sentinel> make_encoder(Iterator begin, Sentinel end)
  {
    return {std::move(begin), std::move(end)};
  }

  template <
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value>>
  static Decoder<Iterator, Sentinel> make_decoder(Iterator begin, Sentinel end)
  {
    return {std::move(begin), std::move(end)};
  }

  template <
      typename T,
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
          adapters::concepts::is_iterable_input_adapter<
              Encoder<Iterator, Sentinel>>::value &&
          concepts::is_codec_output<
              T,
              meta::result_of_begin_t<Encoder<Iterator, Sentinel>>>::value>>
  static auto encode(Iterator it, Sentinel sent)
  {
    auto enc = make_encoder(it, sent);
    return output_traits<T>::create(enc.begin(), enc.end());
  }

  template <
      typename T,
      typename Iterator,
      typename Sentinel,
      typename = std::enable_if_t<
          meta::concepts::iterator::is_input_iterator<Iterator>::value &&
          meta::concepts::iterator::is_sentinel<Sentinel, Iterator>::value &&
          adapters::concepts::is_iterable_input_adapter<
              Decoder<Iterator, Sentinel>>::value &&
          concepts::is_codec_output<
              T,
              meta::result_of_begin_t<Decoder<Iterator, Sentinel>>>::value>>
  static auto decode(Iterator it, Sentinel sent)
  {
    auto dec = make_decoder(it, sent);
    return output_traits<T>::create(dec.begin(), dec.end());
  }

  template <typename T,
            typename Iterable,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterable<Iterable>::value>>
  static auto encode(Iterable const& it) -> decltype(basic_codec::encode<T>(
      std::declval<meta::result_of_begin_t<Iterable const&>>(),
      std::declval<meta::result_of_end_t<Iterable const&>>()))
  {
    using std::begin;
    using std::end;
    return basic_codec::encode<T>(begin(it), end(it));
  }

  template <typename T,
            typename Iterable,
            typename = std::enable_if_t<
                meta::concepts::iterator::is_iterable<Iterable>::value>>
  static auto decode(Iterable const& it) -> decltype(basic_codec::decode<T>(
      std::declval<meta::result_of_begin_t<Iterable const&>>(),
      std::declval<meta::result_of_end_t<Iterable const&>>()))
  {
    using std::begin;
    using std::end;
    return basic_codec::decode<T>(begin(it), end(it));
  }
};
}
}
}
