---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: home
---

# Overview

Mgs is an open-source C++14 library, aiming to be a collection of codecs.

Its main design goals are:

* **Ease of use**: Basic usage must be a no-brainer. Advanced usage must be possible.
* **Extensibility**: New codecs must be simple to add in future versions.
* **API genericity**: Emulating [Concepts](https://en.cppreference.com/w/cpp/concepts) to define correct and generic APIs.
* **Package managers friendly**: Easy to package, one can choose to only install specific codecs.

## Ease of use

```cpp
#include <iostream>

#include <mgs/base64.hpp>

int main()
{
  std::cout << mgs::base64::encode("Hello, World!") << std::endl;
  std::cout << mgs::base64::decode("SGVsbG8sIFdvcmxkIQ==") << std::endl;
}
```

## Extensibility

Every codec is located inside the top-level `mgs` directory, here is the [list of supported codecs]().
Supporting a new codec will simply require a header to include.

## API genericity

```cpp
#include <string>

#include <mgs/base64.hpp>

using namespace std::string_literals;

int main()
{
  auto const encoded = mgs::base64::encode<std::vector<unsigned char>>("Hello, World!"s);
  auto const decoded = mgs::base64::decode<std::array<char, 13>>(encoded);
}
```

Almost every [`Iterable`]() can be passed as a parameter.
TODO link to relevant sections

## Package managers friendly

The library is split into multiple CMake components, which helps re-packaging it.

Conan is supported out of the box. If you use a different package manager, check out if it provides `mgs`!

If not, feel free to add it :)

Please note that Conan will remain the only out of the box package manager, in order to avoid cluttering the GitHub repository and keeping the release process manageable.


## Credits

The library's design is heavily based on [cppcodec](https://github.com/tplgy/cppcodec).
