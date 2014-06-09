#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
using namespace std;
#include "../basicfunctions.h"
ofstream out;
#include "../classes.h"
#include "../secondaryfunctions.h"
#include "../attributesfunctions.h"
#include "../classfunctions.h"

string xml = "<a lang=\"en\" a=\"true\"><b><c mango=\"tasty\"/></b></a>";
chart c(xml);
node* root = c.root;

TEST_CASE( "check attributes", "[working]" ) 
{

	string val = trimString(root->attr->value);
	string key = trimString(root->attr->key);
	REQUIRE(key=="lang");
	REQUIRE(val=="\"en\"");
	key = root->attr->attr->key;
	REQUIRE(key=="a");
}

// TEST_CASE( "check ", "[working]" ) 
// {
	
//     REQUIRE( );
// }
