#pragma once

#include <mgs/ranges/basic_transformed_input_range.hpp>

#include <test_helpers/noop_transformer.hpp>

namespace test_helpers
{
template <typename Iterator, typename Sentinel>
class noop_range : public mgs::ranges::basic_transformed_input_range<
                       test_helpers::noop_transformer<Iterator, Sentinel>>
{
public:
  using underlying_iterator = Iterator;
  using underlying_sentinel = Sentinel;

  using mgs::ranges::basic_transformed_input_range<
      test_helpers::noop_transformer<Iterator,
                                     Sentinel>>::basic_transformed_input_range;
};
}
