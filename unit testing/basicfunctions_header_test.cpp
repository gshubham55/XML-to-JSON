#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
using namespace std;
#include "../basicfunctions.h"


TEST_CASE( "check trimString", "[working]" ) {
	string s = " hello world! ";
    REQUIRE( trimString(s) == "hello world!" );
}

TEST_CASE( "check tab()", "[working]" ) {
	int i = 2;
    REQUIRE( tab(2) == "    " );
}

TEST_CASE( "check removeDeclarationAndComments()", "[working]" ) 
{
	string s = "<?xml varsion=\"1\"?>";
	removeDeclarationAndComments(s);
    REQUIRE(s[0] == '\0');
}

TEST_CASE( "check replaceFirstOccurance()", "[working]" ) 
{
	string s = "Hey! I like the word Hey.";
	string search = "Hey";
	string replace = "Hello";
	replaceFirstOccurance(s, search, replace );
    REQUIRE(s == "Hello! I like the word Hey.");
}

TEST_CASE( "check removeSlashes()", "[working]" ) 
{
	string attr = "lang=\"en/us\" / ";
	removeSlashes(attr);
    REQUIRE(attr == "lang=\"en/us\" ");
}

TEST_CASE( "check brackets()", "[working]" ) 
{
	string s = "Hello";
    REQUIRE(brackets(s) == "\"Hello\"");
}
