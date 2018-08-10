#include <mgs/adapters/aliases/types/underlying_iterator.hpp>

struct A
{
  using underlying_iterator = char const*;
};

int main(int argc, char const* argv[])
{
  mgs::adapters::type_aliases::underlying_iterator<A> it = "";
}
