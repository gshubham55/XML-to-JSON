#include "catch_with_main.hpp"

TEST_CASE( "stupid/1=2", "Prove that one equals 2" )
{
    int one = 2;
    REQUIRE( one == 2 );
}