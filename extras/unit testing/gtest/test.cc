#include "gtest/gtest.h"

int abs(int i)
{
	return i;
}

TEST(StandardTest, equality)
{
	EXPECT_EQ(1,abs(1));
}