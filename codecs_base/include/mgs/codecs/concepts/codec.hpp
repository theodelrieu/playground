#pragma once

#include <type_traits>

// template <typename T, typename Out, Iterable I>
// concept Codec = requires (result_of_begin_t<I> a, result_of_end_t<I> b, I const& c) {
//   // lazy, so only iterators to avoid lifetime issues.
//   InputAdapter<decltype(T::make_encoder(a, b))>;
//   InputAdapter<decltype(T::make_decoder(a, b))>;
//
//   CodecOutput<Out, decltype(T::make_encoder(a, b))>;
//   CodecOutput<Out, decltype(T::make_decoder(a, b))>;
//
//   // eager, user specifies return type.
//   Same<Out, decltype(T::encode<Out>(a, b))>;
//   Same<Out, decltype(T::encode<Out>(c))>;
//   Same<Out, decltype(T::decode<Out>(a, b))>;
//   Same<Out, decltype(T::decode<Out>(c))>;
// };

namespace mgs
{
inline namespace v1
{
namespace codecs
{
namespace concepts
{
template <typename T, typename Out, typename It, typename = void>
struct is_codec : std::false_type
{
};

// TODO
}
}
}
}
