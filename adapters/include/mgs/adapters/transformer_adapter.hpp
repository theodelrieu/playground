#pragma once

#include <array>
#include <cstdlib>
#include <type_traits>

#include <mgs/iterators/adaptive_iterator.hpp>
#include <mgs/meta/concepts/byte_integral.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/input_transformer.hpp>
#include <mgs/meta/concepts/sentinel.hpp>

namespace mgs
{
namespace adapters
{
inline namespace v1
{
template <typename InputTransformer,
          typename UnderlyingIterator,
          typename Sentinel = UnderlyingIterator>
class transformer_adapter
{
  static_assert(meta::is_input_iterator<UnderlyingIterator>::value,
                "UnderlyingIterator is not an InputIterator");
  static_assert(meta::is_sentinel<Sentinel, UnderlyingIterator>::value,
                "Sentinel is not a Sentinel<UnderlyingIterator>");
  static_assert(meta::is_input_transformer<InputTransformer,
                                           UnderlyingIterator,
                                           Sentinel>::value,
                "InputTransformer is not an InputTransformer (or "
                "UnderlyingIterator/Sentinel are invalid)");

  using iterator = iterators::adaptive_iterator<transformer_adapter,
                                                std::input_iterator_tag>;

  using transformer_value_type = typename InputTransformer::value_type;
  using transformer_value_type_iterator =
      meta::result_of_begin_t<transformer_value_type>;

public:
  using underlying_iterator = UnderlyingIterator;
  using underlying_sentinel = Sentinel;

  using difference_type = std::streamoff;
  using value_type = typename std::iterator_traits<
      transformer_value_type_iterator>::value_type;

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
  meta::difference_type_t<std::iterator_traits<transformer_value_type_iterator>>
      _index{0};

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
