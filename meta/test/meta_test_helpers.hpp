#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/static_asserts.hpp>

template <typename Requirement, typename... ExpectedFailedRequirements>
void generate_failed_requirements_tests(
    std::tuple<ExpectedFailedRequirements...>)
{
  using failed_requirements =
      typename mgs::meta::detail::collect_failed_requirements<
          Requirement>::type;

  static_assert(
      std::is_same<std::tuple<ExpectedFailedRequirements..., Requirement>,
                   failed_requirements>::value,
      "Mismatch when comparing failed requirements");
}
