#include <mgs/adapters/concepts/input_adapter.hpp>

int main(int argc, char const* argv[])
{
  static_assert(!mgs::adapters::concepts::is_input_adapter<int>::value, "");
}
