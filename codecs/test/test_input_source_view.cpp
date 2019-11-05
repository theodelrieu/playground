#include <string>

#include <catch2/catch.hpp>

#include <mgs/codecs/input_source_view.hpp>
#include <mgs/config.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

using namespace mgs::codecs;
using namespace mgs::meta;

namespace
{
template <typename Ret = std::string, typename I, typename S>
Ret read_all(input_source_view<I, S>& src)
{
  Ret ret;

  while (src.read(std::back_inserter(ret), 4096))
    ;
  return ret;
}
}

TEST_CASE("codecs::input_source_view")
{
  SECTION("read")
  {
    SECTION("std::string")
    {
      std::string s("abcdefghijklmnopqrstuvwxyz");
      auto is = make_input_source_view(s);
      CHECK(read_all(is) == s);
    }

    SECTION("std::vector<char>")
    {
      std::vector<char> v{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_input_source_view(v);
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), v.begin(), v.end()));
    }

    SECTION("char []")
    {
      char const tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_input_source_view(tab);
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("int []")
    {
      int const tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                         'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                         's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_input_source_view(tab);
      auto const v = read_all<std::vector<int>>(is);
      CHECK(std::equal(v.begin(), v.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("iterators")
    {
      char const tab[] = "abc\0defghijklmnopqrstuvwxyz";
      auto is = make_input_source_view(std::begin(tab), std::end(tab));
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("string literal")
    {
      SECTION("char")
      {
        auto is = make_input_source_view("abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all(is);
        CHECK(s == "abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("wchar_t")
      {
        auto is = make_input_source_view(L"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::wstring>(is);
        CHECK(s == L"abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("char16_t")
      {
        auto is = make_input_source_view(u"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u16string>(is);
        CHECK(s == u"abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("char32_t")
      {
        auto is = make_input_source_view(U"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u32string>(is);
        CHECK(s == U"abcdefghijklmnopqrstuvwxyz");
      }

#ifdef MGS_HAS_CPP20
      SECTION("char8_t")
      {
        auto is = make_input_source_view(u8"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u8string>(is);
        CHECK(s == u8"abcdefghijklmnopqrstuvwxyz");
      }
#endif
    }
  }

  SECTION("range")
  {
    static_assert(is_range<input_source_view<char const*>>::value, "");
    static_assert(is_semiregular<input_source_view<char const*>>::value, "");

    auto source = make_input_source_view("000");
    CHECK(std::count(source.begin(), source.end(), '0') == 3);
  }
}
