#pragma once

// source: https://stackoverflow.com/a/26745591

#define CAN_CALL_STD_FUNC_IMPL(std_name)                               \
  namespace detail2                                                    \
  {                                                                    \
  using std::std_name;                                                 \
                                                                       \
  template <typename... T>                                             \
  struct can_call_##std_name##_impl                                    \
  {                                                                    \
    template <typename... U>                                           \
    static auto check(int)                                             \
        -> decltype(std_name(std::declval<U>()...), std::true_type()); \
                                                                       \
    template <typename...>                                             \
    static std::false_type check(...);                                 \
                                                                       \
    using type = decltype(check<T...>(0));                             \
  };                                                                   \
                                                                       \
  template <typename... T>                                             \
  struct can_call_##std_name : can_call_##std_name##_impl<T...>::type  \
  {                                                                    \
  };                                                                   \
                                                                       \
  /* not SFINAE friendly */                                            \
  template <typename... T>                                             \
  struct result_of_##std_name                                          \
  {                                                                    \
    using type = decltype(std_name(std::declval<T>()...));             \
  };                                                                   \
  }                                                                    \
                                                                       \
  namespace detail3                                                    \
  {                                                                    \
  struct std_name##tag                                                 \
  {                                                                    \
  };                                                                   \
                                                                       \
  template <class... T>                                                \
  std_name##tag std_name(T&&...);                                      \
                                                                       \
  template <typename... T>                                             \
  struct would_call_std_##std_name##_impl                              \
  {                                                                    \
    template <typename... U>                                           \
    static auto check(int) -> std::integral_constant<                  \
        bool,                                                          \
        std::is_same<decltype(std_name(std::declval<U>()...)),         \
                     std_name##tag>::value>;                           \
                                                                       \
    template <typename...>                                             \
    static std::false_type check(...);                                 \
                                                                       \
    using type = decltype(check<T...>(0));                             \
  };                                                                   \
                                                                       \
  template <typename... T>                                             \
  struct would_call_std_##std_name                                     \
    : would_call_std_##std_name##_impl<T...>::type                     \
  {                                                                    \
  };                                                                   \
  }
