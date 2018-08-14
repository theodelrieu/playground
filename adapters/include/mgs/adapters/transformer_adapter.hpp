#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/adapters/concepts/input_transformer.hpp>
#include <mgs/iterators/adaptive_iterator.hpp>
#include <mgs/meta/concepts/iterator/input_iterator.hpp>
#include <mgs/meta/concepts/iterator/sentinel.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

namespace mgs
{
inline namespace v1
{
namespace adapters
{
template <typename InputTransformer>
class transformer_adapter : private InputTransformer
{
  static_assert(concepts::is_input_transformer<InputTransformer>::value,
                "InputTransformer is not an InputTransformer");

  using transformer_underlying_iterator =
      typename InputTransformer::underlying_iterator;
  using transformer_underlying_sentinel =
      typename InputTransformer::underlying_sentinel;
  using transformer_value_type = typename InputTransformer::value_type;
  using transformer_value_type_iterator =
      meta::result_of_begin_t<transformer_value_type>;

public:
  using iterator = iterators::adaptive_iterator<transformer_adapter,
                                                std::input_iterator_tag>;

  using difference_type = std::streamoff;
  using value_type = typename std::iterator_traits<
      transformer_value_type_iterator>::value_type;

  transformer_adapter() = default;
  transformer_adapter(transformer_underlying_iterator begin,
                      transformer_underlying_sentinel end);

  value_type const& get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  transformer_value_type _transformed{};
  typename std::iterator_traits<
      transformer_value_type_iterator>::difference_type _index{0};

  void _process_input();

  template <typename T>
  friend bool operator==(transformer_adapter<T> const& lhs,
                         transformer_adapter<T> const& rhs);
};

template <typename T>
bool operator!=(transformer_adapter<T> const& lhs,
                transformer_adapter<T> const& rhs);
}
}
}

#include <mgs/adapters/detail/transformer_adapter_impl.hpp>
