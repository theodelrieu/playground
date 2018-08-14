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
template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator>
class transformer_adapter
{
  static_assert(
      meta::concepts::iterator::is_input_iterator<UnderlyingIterator>::value,
      "UnderlyingIterator is not an InputIterator");
  static_assert(
      meta::concepts::iterator::is_sentinel<Sentinel,
                                            UnderlyingIterator>::value,
      "Sentinel is not a Sentinel<UnderlyingIterator>");
  static_assert(concepts::is_input_transformer<InputTransformer,
                                               UnderlyingIterator,
                                               Sentinel>::value,
                "InputTransformer is not an InputTransformer (or "
                "UnderlyingIterator/Sentinel are invalid)");

  using transformer_value_type = typename InputTransformer::value_type;
  using transformer_value_type_iterator =
      meta::result_of_begin_t<transformer_value_type>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;
  using iterator = iterators::adaptive_iterator<transformer_adapter,
                                                std::input_iterator_tag>;

  using difference_type = std::streamoff;
  using value_type =
      meta::detected::types::value_type<transformer_value_type_iterator>;

  transformer_adapter() = default;
  transformer_adapter(UnderlyingIterator const& begin, Sentinel const& end);

  value_type const& get() const;
  void seek_forward(difference_type n);

  iterator begin() const;
  iterator end() const;

private:
  UnderlyingIterator _current{};
  Sentinel _end{};
  transformer_value_type _transformed{};
  typename std::iterator_traits<
      transformer_value_type_iterator>::difference_type _index{0};

  void _process_input();

  template <typename T, typename U, typename V>
  friend bool operator==(transformer_adapter<T, U, V> const& lhs,
                         transformer_adapter<T, U, V> const& rhs);
};

template <typename T, typename U, typename V>
bool operator!=(transformer_adapter<T, U, V> const& lhs,
                transformer_adapter<T, U, V> const& rhs);
}
}
}

#include <mgs/adapters/detail/transformer_adapter_impl.hpp>
