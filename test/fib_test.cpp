#include "test.h"

#include "fib.h"

class FibTestFixture : public ::testing::Test {
  protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(FibTestFixture, HandlesZeroInput) {
    EXPECT_EQ(fib(0), 0);
}

TEST_F(FibTestFixture, HandlesOneInput) {
    EXPECT_EQ(fib(1), 1);
}

TEST_F(FibTestFixture, HandlesPositiveInput) {
    EXPECT_EQ(fib(2), 1);
    EXPECT_EQ(fib(3), 2);
    EXPECT_EQ(fib(4), 3);
    EXPECT_EQ(fib(5), 5);
    EXPECT_EQ(fib(10), 55);
}