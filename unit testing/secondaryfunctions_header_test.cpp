#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
using namespace std;
#include "../basicfunctions.h"
ofstream out;
#include "../classes.h"
#include "../secondaryfunctions.h"

// TEST_CASE( "check ", "[working]" ) 
// {
	
//     REQUIRE( );
// }



TEST_CASE( "check getTagName()", "[working]" ) 
{
	string s = "  <tag>  ";
	string tag =getTagName(s);
    REQUIRE( tag == "tag");
}

TEST_CASE( "check stringHasChildren", "[working]" ) 
{
	string s = "<a><b/></a>";
	bool hasChildren = stringHasChildren(s);
    REQUIRE( hasChildren );
	s = "hello";
	hasChildren = stringHasChildren(s);
    REQUIRE( !hasChildren );

}

TEST_CASE( "check isOpeningtag()", "[working]" ) 
{	
	string s = "<a>";
	bool op = isOpeningtag(s,1);
    REQUIRE( op );
	op = "</a>";
	op = isOpeningtag(s,2);
    REQUIRE( !op );
}

TEST_CASE( "check isClosingtag()", "[working]" ) 
{
	string s = "<a>";
	bool cl = isClosingtag(s,1);
    REQUIRE( !cl );
	s = "</a>";
	cl = isClosingtag(s,2);
    REQUIRE( cl );
	
}

TEST_CASE( "check posOfEndOfApprTag()", "[working]" ) 
{
	string s = "<a>A</a><b></b><a></a><a/>";
	string tag = "a";
	int pos = posOfEndOfApprTag(s,tag);
    REQUIRE( pos==7);
}

TEST_CASE( "check checkIfNullDataSet()", "[working]" ) 
{
	string s = "<a>";
	int null = checkIfNullDataSet(s);
    REQUIRE( null ==0);
    s = "<a/>";
	null = checkIfNullDataSet(s); 
    REQUIRE( null ==2);
}

TEST_CASE( "check checkForTextBeforeTags()", "[working]" ) 
{
	string s="A<b/>";
	string text = checkForTextBeforeTags(s);
    REQUIRE( text == "A");
}



node* N = new node;
node* next = new node;

TEST_CASE( "check insert_after()", "[working]" ) 
{
	insert_after(N,next);
    REQUIRE( N->sibling==next);
}

TEST_CASE( "check deleteBwhichIsAfterA()", "[working]" ) 
{
	deleteBwhichIsAfterA(N,next);
    REQUIRE(N->sibling==0);
}
