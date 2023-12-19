#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Eq;
using ::testing::IsTrue;

TEST(A, B)
{
    ASSERT_THAT(true, IsTrue());
    EXPECT_THAT(0, Eq(0));
}
