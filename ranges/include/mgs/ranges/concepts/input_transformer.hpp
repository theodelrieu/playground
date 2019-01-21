#pragma once

#include <tuple>

#include <mgs/ranges/detail/detected/types/buffer_type.hpp>
#include <mgs/ranges/detail/detected/types/underlying_iterator.hpp>
#include <mgs/ranges/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/call_std/begin.hpp>
#include <mgs/meta/call_std/end.hpp>
#include <mgs/meta/concepts/iterator/iterable.hpp>
#include <mgs/meta/concepts/iterator/random_access_iterator.hpp>
#include <mgs/meta/concepts/iterator/sized_sentinel.hpp>
#include <mgs/meta/concepts/object/regular.hpp>
#include <mgs/meta/detected/operators/function_call.hpp>

// template <typename T>
// concept InputTransformer = Regular<T> &&
//   requires Iterator<typename T::underlying_iterator> &&
//   requires Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator> &&
//   requires Iterable<typename T::buffer_type> &&
//   requires RandomAccessIterator<meta::result_of_begin<typename T::buffer_type const&>> &&
//   requires SizedSentinel<meta::result_of_end<typename T::buffer_type const&>, meta::result_of_begin<typename T::buffer_type const&>> &&
//   requires Constructible<typename T::underlying_iterator, typename T::underlying_sentinel> &&
//   requires(T& v, typename T::buffer_type& b) {
//     { v(b) } -> void;
//   }

namespace mgs
{
inline namespace v1
{
namespace ranges
{
namespace concepts
{
template <typename T>
struct is_input_transformer
{
private:
  using I = meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using S = meta::detected_t<detail::detected::types::underlying_sentinel, T>;
  using Buffer = meta::detected_t<detail::detected::types::buffer_type, T>;
  using BufferI = meta::detected_t<meta::result_of_begin, Buffer const&>;
  using BufferS = meta::detected_t<meta::result_of_end, Buffer const&>;

  static constexpr auto const has_function_call_op =
      meta::is_detected_exact<void,
                              meta::detected::operators::function_call,
                              T&,
                              Buffer&>::value;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      std::is_constructible<T, I, S>::value;

public:
  using requirements = std::tuple<meta::concepts::object::is_regular<T>>;

  static constexpr auto const value =
      meta::concepts::object::is_regular<T>::value &&
      meta::concepts::iterator::is_iterator<I>::value &&
      meta::concepts::iterator::is_sentinel<S, I>::value &&
      meta::concepts::iterator::is_iterable<Buffer>::value &&
      meta::concepts::iterator::is_random_access_iterator<BufferI>::value &&
      meta::concepts::iterator::is_sized_sentinel<BufferS, BufferI>::value &&
      is_constructible_from_iterator_sentinel && has_function_call_op;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an InputTransformer");
    static_assert(meta::concepts::iterator::is_iterator<I>::value,
                  "T::underlying_iterator must be an Iterator");
    static_assert(
        meta::concepts::iterator::is_sentinel<S, I>::value,
        "T::underlying_sentinel must be a Sentinel<T::underlying_iterator>");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(meta::concepts::iterator::is_iterable<Buffer>::value,
                  "T::buffer_type must be Iterable");
    static_assert(
        meta::concepts::iterator::is_random_access_iterator<BufferI>::value,
        "begin(T::buffer_type) must return a RandomAccessIterator");
    static_assert(
        meta::concepts::iterator::is_sized_sentinel<BufferS, BufferI>::value,
        "end(T::buffer_type) must return a SizedSentinel");
    static_assert(
        has_function_call_op,
        "Invalid or missing function: 'void T::operator()(T::buffer_type&)'");
    return 1;
  }
};

template <typename T,
          typename = std::enable_if_t<is_input_transformer<T>::value>>
using InputTransformer = T;
}
}
}
}
