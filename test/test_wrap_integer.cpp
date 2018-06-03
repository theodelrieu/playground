#include <type_traits>

#include <catch.hpp>

#include <mgs/detail/wrap_integer.hpp>
#include <mgs/detail/meta/detected.hpp>

using namespace mgs::detail;

TEST_CASE("wrap_integer", "[wrap_integer]")
{
  SECTION("checks")
  {
    SECTION("positive bounds")
    {
      SECTION("no wrapping around")
      {
        for (auto i = 0; i < 4; ++i)
          CHECK(wrap_integer<0, 3>(i) == i);
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 4; i < 8; ++i)
            CHECK(wrap_integer<0, 3>(i) == i - 4);
        }

        SECTION("lower bound")
        {
          for (auto i = -1; i > -4; --i)
            CHECK(wrap_integer<0, 3>(i) == i + 4);

          CHECK(wrap_integer<0, 3>(-1024) == 0);
        }
      }
    }

    SECTION("negative lower bound")
    {
      SECTION("no wrapping around")
      {
        for (auto i = 1; i > -4; --i)
          CHECK(wrap_integer<-4, 1>(i) == i);
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 2; i < 8; ++i)
            CHECK(wrap_integer<-4, 1>(i) == i - 6);
          CHECK(wrap_integer<-4, 1>(8) == -4);
        }

        SECTION("lower bound")
        {
          for (auto i = -5; i > -11; --i)
            CHECK(wrap_integer<-4, 1>(i) == i + 6);
        }
      }
    }

    SECTION("0 upper bound")
    {
      SECTION("no wrapping around")
      {
        for (auto i = 0; i > -4; --i)
          CHECK(wrap_integer<-4, 0>(i) == i);
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 1; i < 6; ++i)
            CHECK(wrap_integer<-4, 0>(i) == i - 5);
        }

        SECTION("lower bound")
        {
          for (auto i = -5; i > -9; --i)
            CHECK(wrap_integer<-4, 0>(i) == i + 5);
        }
      }
    }
  }
}
