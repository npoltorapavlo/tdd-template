#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Return;

struct IShape {
    virtual ~IShape() = default;
    virtual uint32_t Area() const = 0;
};

class ShapeMock : public IShape {
public:
    virtual ~ShapeMock() override = default;
    MOCK_METHOD(uint32_t, Area, (), (const, override));
};

TEST(AShape, ReturnsArea)
{
    // NiceMock tracks interactions only for methods on which expectations exist
    NiceMock<ShapeMock> shape;
    // Use ON_CALL by default, and only use EXPECT_CALL when intend to verify that the call is made
    ON_CALL(shape, Area())
        .WillByDefault(Return(10));

    // One Assert per Test
    EXPECT_THAT(shape.Area(), Eq(10));
}
