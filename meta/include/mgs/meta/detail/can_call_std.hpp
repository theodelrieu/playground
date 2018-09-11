#pragma once

#include <mgs/meta/detected.hpp>

// source: https://stackoverflow.com/a/26745591

#define CAN_CALL_STD_FUNC_IMPL(std_name)                                      \
  namespace detail2                                                           \
  {                                                                           \
  using std::std_name;                                                        \
                                                                              \
  template <typename... T>                                                    \
  using result_of_##std_name = decltype(std_name(std::declval<T>()...));      \
  }                                                                           \
                                                                              \
  template <typename... T>                                                    \
  using result_of_##std_name = detail2::result_of_##std_name<T...>;           \
                                                                              \
  namespace detail3                                                           \
  {                                                                           \
  struct std_name##_tag                                                       \
  {                                                                           \
  };                                                                          \
                                                                              \
  template <typename... T>                                                    \
  std_name##_tag std_name(T&&...);                                            \
                                                                              \
  template <typename... T>                                                    \
  struct would_call_std_##std_name                                            \
  {                                                                           \
    static constexpr auto const value =                                       \
        is_detected_exact<std_name##_tag, result_of_##std_name, T...>::value; \
  };                                                                          \
  }                                                                           \
                                                                              \
  template <typename... T>                                                    \
  struct would_call_std_##std_name : detail3::would_call_std_##std_name<T...> \
  {                                                                           \
  }
