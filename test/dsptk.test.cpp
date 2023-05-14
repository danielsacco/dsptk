#include <gtest/gtest.h>
#include "dsptk/dsptk.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
};

TEST(HelloTest, BasicAssertions2) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "montoto");
	// Expect equality.
	EXPECT_EQ(2 * 2, 4);
};

TEST(LibTest, Square) {
	EXPECT_EQ(square(3), 9);
}
