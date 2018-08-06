#include <mgs/meta/concepts/iterator.hpp>

int main(int argc, char const* argv[])
{
  static_assert(mgs::meta::is_iterator<char*>::value, "");
}
