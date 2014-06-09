#define CATCH_CONFIG_MAIN
#include "catch.hpp"
using namespace std;
#include "../basicfunctions.h"
ofstream out;
#include "../classes.h"

TEST_CASE( "check node", "[working]" ) 
{
	node* n = new node;
	n->data="DATA";
	node* sib = new node;
	n->sibling = sib;
    REQUIRE( n->data == "DATA" );
    REQUIRE( n->sibling == sib );
    REQUIRE( n->child == NULL );
}

void chart::define(node* &N,string s)
{
	return;
}

TEST_CASE( "check chart", "[working]" ) 
{
	chart c("");
    REQUIRE( c.tabs == 0 );
    REQUIRE( c.root );
}
