---
layout: default
title: InputTransformer
nav_order: 5
parent: Concepts
permalink: /docs/concepts/input_transformer
---

# mgs::InputTransformer

Defined in header `<mgs/concepts/input_transformer.hpp>`

```cpp
template <typename T>
concept InputTransformer =
  std::Semiregular<T> &&
  requires(T& v, T const& cv, typename T::buffer_type& b) {
    std::Iterator<typename T::underlying_iterator>;
    std::Sentinel<typename T::underlying_sentinel, typename T::underlying_iterator>;
    Iterable<typename T::buffer_type>;
    std::Semiregular<typename T::buffer_type>;
    std::Constructible<T, typename T::underlying_iterator, typename T::underlying_sentinel>;

    { begin(b) } -> std::RandomAccessIterator;
    { end(b) } -> std::SizedSentinel<decltype(begin(b))>;
    { v(b) } -> std::Same<void>;
    { cv.iterator() } -> std::Same<typename T::underlying_iterator>;
    { cv.sentinel() } -> std::Same<typename T::underlying_sentinel>;
  };
```

An `InputTransformer` is a building block used in most [`TransformedInputView`](transformed_input_view)s.

It is responsible for the input transformation.

## Model of

* [`std::Semiregular`]()

## Associated types

| Member type              | Definition                     | Constraints                                 |
|--------------------------+--------------------------------+---------------------------------------------|
| `T::buffer_type`         | Type storing transformed input | [`Iterable`](), [`std::Semiregular`]()      |
| `T::underlying_iterator` | Input range iterator type      | [`std::InputIterator`]()                    |
| `T::underlying_sentinel` | Input range sentinel type      | [`std::Sentinel<T::underlying_iterator>`]() |

## Notation

* `v` - value of type `T&`
* `cv` - value of type `T const&`
* `b` - value of type `typename T::buffer_type&`
* `i` - value of type `typename T::underlying_iterator`
* `s` - value of type `typename T::underlying_sentinel`

## Valid expressions

In addition to the expressions defined by the modeled concepts, the following expressions must be valid.

| Expression                   | Return type                                  |
|------------------------------+----------------------------------------------|
| `T(i, s)`                    | `T`                                          |
| `using std::begin; begin(b)` | [`std::RandomAccessIterator`]()              |
| `using std::end; end(b)`     | [`std::SizedSentinel<decltype(begin(b))>`]() |
| `v(b)`                       | `void`                                       |
| `cv.iterator()`              | `T::underlying_iterator`                     |
| `cv.sentinel()`              | `T::underlying_sentinel`                     |

## Expression semantics

| Expression      | Precondition | Semantics                                | Postcondition                                                                                                         |
|-----------------+--------------+------------------------------------------+-----------------------------------------------------------------------------------------------------------------------|
| `v(b)`          |              | Transforms input and store it in `b`     | The buffer is overwritten with some transformed input, or is empty (i.e. distance between iterator and sentinel is 0) |
| `cv.iterator()` |              | Returns the input range current iterator |                                                                                                                       |
| `cv.sentinel()` |              | Returns the input range sentinel         |                                                                                                                       |

## Example

```cpp
#include <mgs/concepts/input_transformer.hpp>

using namespace mgs;

class noop_transformer {
public:
  using underlying_iterator = char const*;
  using underlying_sentinel = char const*;
  using buffer_type = std::vector<char>;

  noop_transformer() = default;
  noop_transformer(underlying_iterator begin, underlying_sentinel end):
    _current(begin), _end(end) {}

  underlying_iterator iterator() const {
    return _current;
  }

  underlying_sentinel sentinel() const {
    return _end;
  }

  void operator()(buffer_type& b) {
    b.clear();
    _current = std::copy(std::back_inserter(b), _current, _end);
  }

private:
  underlying_iterator _current{};
  underlying_sentinel _end{};
};

static_assert(concepts::is_input_transformer<noop_transformer>::value, "");
```
