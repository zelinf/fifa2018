#include <catch/catch.hpp>
#include <fifa2018/RandomGen.h>

#define REQUIRE_10_TIMES(expr)\
do {\
    for (int i = 0; i < 10; i++) {\
        REQUIRE((expr));\
    }\
} while(false)

TEST_CASE("RandomGen") {
    fifa2018::RandomGen &gen = fifa2018::RandomGen::getDefaultRandomGen();

    REQUIRE_NOTHROW(gen.next(0, 1));
    REQUIRE_THROWS_AS(gen.next(1, 1), std::invalid_argument &);
    REQUIRE_10_TIMES(0 == gen.next(0, 1));

    REQUIRE_10_TIMES(gen.next(3, 7) < 7);
    REQUIRE_10_TIMES(gen.next(4, 8) >= 4);
}
