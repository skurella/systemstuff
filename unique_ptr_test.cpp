#include <gtest/gtest.h>

#include "unique_ptr.hpp"

class UniquePtrTest : public ::testing::Test {
protected:
  template <typename T> const T *get_ptr(const seb::unique_ptr<T> &up) {
    return up._ptr;
  }
};

TEST_F(UniquePtrTest, IsConstructibleWithPointer) {
  seb::unique_ptr<int> up(new int(123));
  EXPECT_TRUE(get_ptr(up));
}

TEST_F(UniquePtrTest, IsResetOnMove) {
  seb::unique_ptr<int> up1(new int(123));
  auto up2{std::move(up1)};
  EXPECT_FALSE(get_ptr(up1));
  EXPECT_TRUE(get_ptr(up2));
}
