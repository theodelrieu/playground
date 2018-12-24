---
layout: default
title: Installation and integration
nav_order: 2
---

# Installation and integration
{:.no_toc}

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

# Installation
## Package managers

### Conan 

Conan is supported out of the box:

1. `conan remote add theodelrieu https://api.bintray.com/conan/theodelrieu/conan-repo`
1. Add `mgs/0.1@mgs/testing` to your `conanfile.txt`

This will install the entire library, you can also install specific components, e.g. `mgs_base64`.

## Manual

Each release can be downloaded [here]().

# Integration

## CMake

CMake files are released to ease integration:

```cmake
set(CMAKE_PREFIX_PATH "/path/to/mgs/;${CMAKE_PREFIX_PATH}")
find_package(mgs 0.1 REQUIRED)

add_executable(foo main.cpp)
target_link_libraries(foo mgs::mgs)
```

Alternatively, you can use CMake components:

```cmake
set(CMAKE_PREFIX_PATH "/path/to/mgs/;${CMAKE_PREFIX_PATH}")
find_package(mgs 0.1 REQUIRED COMPONENTS base64 base64url)

add_executable(foo main.cpp)
target_link_libraries(foo mgs::base64 mgs::base64url)
```

## Manual

Simply add `-I/path/to/mgs/include` to your compiler invocation.
