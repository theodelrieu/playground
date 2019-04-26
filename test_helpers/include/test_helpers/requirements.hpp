#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/static_asserts.hpp>

namespace test_helpers
{
template <typename Requirement,
          typename ExpectedFailedRequirements = std::tuple<>>
void generate_failed_requirements_tests()
{
  using failed_requirements =
      typename mgs::detail::collect_failed_requirements<Requirement>::type;

  using combined = decltype(
      std::tuple_cat(std::tuple<Requirement>{}, ExpectedFailedRequirements{}));
  static_assert(std::is_same<combined, failed_requirements>::value,
                "Mismatch when comparing failed requirements");
}
}
