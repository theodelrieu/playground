#include <mgs/meta/concepts/iterator/iterator.hpp>

int main(int argc, char const* argv[])
{
  static_assert(mgs::meta::concepts::iterator::is_iterator<char*>::value, "");
}
