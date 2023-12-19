#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;

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
    // Arrange
    NiceMock<ShapeMock> shape;
    ON_CALL(shape, Area())
        .WillByDefault(Return(10));

    // Act-Assert
    EXPECT_THAT(shape.Area(), Eq(10));
}
