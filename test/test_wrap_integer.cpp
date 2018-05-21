#include <type_traits>

#include <catch.hpp>

#include <b64/detail/wrap_integer.hpp>

using namespace b64::detail;

TEST_CASE("wrap_integer", "[wrap_integer]")
{
  SECTION("default construction")
  {
    wrap_integer<0, 4> i;
    wrap_integer<2, 5> j;
    wrap_integer<-3, -1> k;

    CHECK(i == 0);
    CHECK(j == 2);
    CHECK(k == -3);
  }

  SECTION("constraints")
  {
    static_assert(!std::is_constructible<wrap_integer<0, 0>>::value, "");
    static_assert(!std::is_constructible<wrap_integer<1, 0>>::value, "");
  }

  SECTION("checks")
  {
    SECTION("positive bounds")
    {
      wrap_integer<0, 3> wi;

      SECTION("no wrapping around")
      {
        for (auto i = 0; i < 4; ++i)
        {
          wi = i;
          CHECK(wi == i);
        }
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 4; i < 8; ++i)
          {
            wi = i;
            CHECK(wi == i - 4);
          }
        }

        SECTION("lower bound")
        {
          for (auto i = -1; i > -4; --i)
          {
            wi = i;
            CHECK(wi == i + 4);
          }

          wi = -1024;
          CHECK(wi == 0);
        }
      }
    }

    SECTION("negative lower bound")
    {
      wrap_integer<-4, 1> wi;
      CHECK(wi == -4);

      SECTION("no wrapping around")
      {
        for (auto i = 1; i > -4; --i)
        {
          wi = i;
          CHECK(wi == i);
        }
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 2; i < 8; ++i)
          {
            wi = i;
            CHECK(wi == i - 6);
          }
          wi = 8;
          CHECK(wi == -4);
        }

        SECTION("lower bound")
        {
          for (auto i = -5; i > -11; --i)
          {
            wi = i;
            CHECK(wi == i + 6);
          }
        }
      }
    }

    SECTION("0 upper bound")
    {
      wrap_integer<-4, 0> wi;

      SECTION("no wrapping around")
      {
        for (auto i = 0; i > -4; --i)
        {
          wi = i;
          CHECK(wi == i);
        }
      }

      SECTION("wrapping")
      {
        SECTION("upper bound")
        {
          for (auto i = 1; i < 6; ++i)
          {
            wi = i;
            CHECK(wi == i - 5);
          }
        }

        SECTION("lower bound")
        {
          for (auto i = -5; i > -9; --i)
          {
            wi = i;
            CHECK(wi == i + 5);
          }
        }
      }
    }
  }

  SECTION("operators")
  {
    wrap_integer<-2, 4> i;
    CHECK(i == -2);

    SECTION("operator+=")
    {
      i += -2;
      CHECK(i == 3);
      i += 10;
      CHECK(i == -1);
      i += -10;
      CHECK(i == 3);
    }

    SECTION("operator-=")
    {
      i -= -2;
      CHECK(i == 0);
      i -= -10;
      CHECK(i == 3);
      i -= 3;
      CHECK(i == 0);
      i -= 9;
      CHECK(i == -2);
    }

    SECTION("operator++")
    {
      CHECK(i++ == -2);
      CHECK(i == -1);

      CHECK(++i == 0);
      i = 4;
      CHECK(++i == -2);
    }

    SECTION("operator--")
    {
      CHECK(i-- == -2);
      CHECK(i == 4);
      CHECK(--i == 3);
    }
  }
}
