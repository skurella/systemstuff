#include <string>

#include <gtest/gtest.h>

#include "tuple.hpp"

TEST(TupleTest, IsConstructible) {
  seb::tuple<int, std::string> t(10, "asdf");
}

TEST(TupleTest, CanReadElements) {
  seb::tuple<int, std::string> t(10, "asdf");
  EXPECT_EQ(t.get<0>(), 10);
  EXPECT_EQ(t.get<1>(), "asdf");
}

TEST(TupleTest, CanWriteElements) {
  seb::tuple<int, std::string> t(10, "asdf");
  EXPECT_EQ(t.get<0>(), 10);
  EXPECT_EQ(t.get<1>(), "asdf");
  t.get<0>() = 11;
  t.get<1>() = "qwerty";
  EXPECT_EQ(t.get<0>(), 11);
  EXPECT_EQ(t.get<1>(), "qwerty");
}