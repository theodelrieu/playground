#include <catch.hpp>

#include <mgs/meta/concepts/swappable.hpp>
#include <mgs/meta/static_asserts.hpp>

#include <test_helpers/requirements.hpp>

using namespace mgs::meta;
using namespace mgs::meta::concepts;

namespace prout {
	struct tag {};

	template<class ...T>
	tag swap(T&&...);

	template <typename ...T>
	using result_of_swap = decltype(swap(std::declval<T>()...));

	template <class ...T>
	struct would_call_std_swap_impl {
		static constexpr auto const value = mgs::meta::is_detected_exact<tag, result_of_swap, T...>::value;
	};

	/*template<typename ...T>
	struct would_call_std_swap_impl {

		template<typename ...U>
		static auto check(int)
			->std::integral_constant<bool, std::is_same<decltype(swap(std::declval<T>()...)), tag>::value>;

		template<typename>
		static std::false_type check(...);

		using type = decltype(check<T...>(0));
	};

	*/
	template<typename ...T>
	struct would_call_std_swap : would_call_std_swap_impl<T...>{ };
}

namespace
{
struct not_swappable
{
  not_swappable(not_swappable const&) = delete;
};

struct adl_swappable
{
  adl_swappable(adl_swappable const&) = delete;
};

void swap(adl_swappable&, adl_swappable&);

template <std::size_t N>
void swap(adl_swappable (&)[N], adl_swappable (&)[N]);
}

TEST_CASE("Swappable", "[meta][concepts][core]")
{
  static_assert(is_swappable<int>::value, "");
  static_assert(is_swappable<adl_swappable>::value, "");
  static_assert(is_swappable<int[4]>::value, "");
  static_assert(is_swappable<int(*)[4]>::value, "");
  static_assert(is_swappable<int(&)[4]>::value, "");
  static_assert(is_swappable<adl_swappable[4]>::value, "");
  static_assert(is_swappable<adl_swappable(*)[4]>::value, "");
  static_assert(is_swappable<adl_swappable(&)[4]>::value, "");

  static_assert(!is_swappable<void>::value, "");
  static_assert(!is_swappable<not_swappable>::value, "");
  static_assert(mgs::meta::detail3::would_call_std_swap<not_swappable&, not_swappable&>::value, "");
  static_assert(prout::would_call_std_swap<not_swappable&, not_swappable&>::value, "");

  test_helpers::generate_failed_requirements_tests<is_swappable<void>>();
}
