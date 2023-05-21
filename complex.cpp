// https://www.hackerrank.com/challenges/class-1-dealing-with-complex-numbers/problem?

#include <gtest/gtest.h>
#include <cmath>
#include <iomanip>

struct Complex {
  float r;
  float i;

  Complex operator+(const Complex &other) const {
    return {r + other.r, i + other.i};
  }
  Complex operator-(const Complex &other) const {
    return {r - other.r, i - other.i};
  }
  Complex operator*(const Complex &other) const {
    return {r * other.r - i * other.i, r * other.i + i * other.r};
  }
  Complex operator/(const Complex &other) const {
    auto other_mod_2 = other.scalar_mod_2();
    Complex other_inv{other.r / other_mod_2, -other.i / other_mod_2};
    return (*this) * other_inv;
  }
  float scalar_mod_2() const { return r * r + i * i; };
  float scalar_mod() const { return std::sqrt(scalar_mod_2()); };
  Complex mod() const { return {scalar_mod(), 0}; }
};

// Testing operators
bool operator==(const Complex &a, const Complex &b) {
  return abs(a.r - b.r) < 0.01f && abs(a.i - b.i) < 0.01f;
}
std::ostream& operator<<(std::ostream& out, const Complex& c) {
  return out << std::setw(2) << c.r << " + " << c.i << "i";
}

TEST(ComplexTest, MathsOps) {
  Complex a{2, 1};
  Complex b{5, 6};
  Complex sum{7, 7};
  Complex diff{-3, -5};
  Complex prod{4, 17};
  Complex div{0.26f, -0.11f};
  Complex mod_a{2.24f, 0};
  Complex mod_b{7.81f, 0};
  EXPECT_EQ((a + b), sum);
  EXPECT_EQ((a - b), diff);
  EXPECT_EQ((a * b), prod);
  EXPECT_EQ((a / b), div);
  EXPECT_EQ(a.mod(), mod_a);
  EXPECT_EQ(b.mod(), mod_b);
}
