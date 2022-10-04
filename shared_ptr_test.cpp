#include <gtest/gtest.h>

#include "shared_ptr.hpp"

class SharedPtrTest : public ::testing::Test {
protected:
  template <typename T> const T *get_ptr(const seb::shared_ptr<T> &sp) {
    return sp._ptr;
  }
  template<typename T> int get_use_count(const seb::shared_ptr<T> &sp) {
    return sp._use_count;
  }
};

TEST_F(SharedPtrTest, IsConstructibleWithPointer) {
  seb::shared_ptr<int> sp(new int(123));
  EXPECT_TRUE(get_ptr(sp));
}

TEST_F(SharedPtrTest, MaintainsValueOnCopy) {
  seb::shared_ptr<int> sp1(new int(123));
  auto sp2{sp1};
  EXPECT_TRUE(get_ptr(sp1));
  EXPECT_TRUE(get_ptr(sp2));
  EXPECT_EQ(get_ptr(sp1), get_ptr(sp2));
  EXPECT_EQ(*sp1, 123);
}

TEST_F(SharedPtrTest, IsResetOnMove) {
  seb::shared_ptr<int> sp1(new int(123));
  auto sp2{std::move(sp1)};
  EXPECT_FALSE(get_ptr(sp1));
  EXPECT_TRUE(get_ptr(sp2));
  EXPECT_EQ(*sp2, 123);
}
