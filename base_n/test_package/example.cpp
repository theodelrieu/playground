#include <mgs/meta/concepts/is_iterator.hpp>

int main(int argc, char const *argv[])
{
  static_assert(mgs::detail::is_iterator<char*>::value, "");
}
