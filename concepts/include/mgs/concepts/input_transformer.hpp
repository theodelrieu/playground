#pragma once

#include <tuple>

#include <mgs/concepts/detail/detected/member_functions/get_iterator.hpp>
#include <mgs/concepts/detail/detected/member_functions/get_sentinel.hpp>
#include <mgs/concepts/detail/detected/types/buffer_type.hpp>
#include <mgs/concepts/detail/detected/types/underlying_iterator.hpp>
#include <mgs/concepts/detail/detected/types/underlying_sentinel.hpp>
#include <mgs/meta/concepts/constructible.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sized_sentinel.hpp>
#include <mgs/meta/detected/operators/function_call.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>

// clang-format off
//
// template <typename T>
// concept InputTransformer = std::Semiregular<T> &&
// requires(T& v, T const& cv, typename T::buffer_type& b) {
//   InputIterator<typename T::underlying_iterator>;
//   Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
//   Range<typename T::buffer_type>;
//   Constructible<typename T::underlying_iterator, typename T::underlying_sentinel>;
//   { begin(b) } -> std::RandomAccessIterator;
//   { end(b) } -> std::SizedSentinel<decltype(begin(b))>;
//   { v(b) } -> void;
//   { cv.get_iterator() } -> std::Same<typename T::underlying_iterator const&>;
//   { cv.get_sentinel() } -> std::Same<typename T::underlying_sentinel const&>;
// };
//
// clang-format on

namespace mgs
{
inline namespace v1
{
namespace concepts
{
template <typename T>
struct is_input_transformer
{
private:
  using t_ref = std::add_lvalue_reference_t<T>;
  using t_const_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

  using I = meta::detected_t<detail::detected::types::underlying_iterator, T>;
  using S = meta::detected_t<detail::detected::types::underlying_sentinel, T>;
  using Buffer = meta::detected_t<detail::detected::types::buffer_type, T>;
  using BufferI = meta::detected_t<meta::iterator_t, Buffer const>;
  using BufferS = meta::detected_t<meta::sentinel_t, Buffer const>;

  static constexpr auto const has_function_call_op =
      meta::is_detected_exact<void,
                              meta::detected::operators::function_call,
                              t_ref,
                              Buffer&>::value;

  static constexpr auto const has_get_iterator =
      meta::is_detected_exact<I const&,
                              detail::detected::member_functions::get_iterator,
                              t_const_ref>::value;
  static constexpr auto const has_get_sentinel =
      meta::is_detected_exact<S const&,
                              detail::detected::member_functions::get_sentinel,
                              t_const_ref>::value;

  static auto constexpr const is_constructible_from_iterator_sentinel =
      meta::concepts::is_constructible<T, I, S>::value;

public:
  using requirements =
      std::tuple<meta::concepts::is_semiregular<T>,
                 meta::concepts::is_input_iterator<I>,
                 meta::concepts::is_sentinel<S, I>,
                 meta::concepts::is_random_access_iterator<BufferI>,
                 meta::concepts::is_sized_sentinel<BufferS, BufferI>,
                 meta::concepts::is_range<Buffer>>;

  static constexpr auto const value =
      meta::concepts::is_semiregular<T>::value && has_get_iterator &&
      has_get_sentinel && meta::concepts::is_iterator<I>::value &&
      meta::concepts::is_sentinel<S, I>::value &&
      meta::concepts::is_range<Buffer>::value &&
      meta::concepts::is_random_access_iterator<BufferI>::value &&
      meta::concepts::is_sized_sentinel<BufferS, BufferI>::value &&
      is_constructible_from_iterator_sentinel && has_function_call_op;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T is not an InputTransformer");
    static_assert(has_get_iterator,
                  "Invalid or missing function: 'underlying_iterator "
                  "T::get_iterator() const'");
    static_assert(has_get_sentinel,
                  "Invalid or missing function: 'underlying_sentinel "
                  "T::get_sentinel() const'");
    static_assert(is_constructible_from_iterator_sentinel,
                  "T is not Constructible from Iterator/Sentinel pair");
    static_assert(
        has_function_call_op,
        "Invalid or missing function: 'void T::operator()(T::buffer_type&)'");
    return 1;
  }
};

template <typename T>
constexpr auto is_input_transformer_v = is_input_transformer<T>::value;
}

template <typename T,
          typename = std::enable_if_t<concepts::is_input_transformer<T>::value>>
using InputTransformer = T;
}
}
