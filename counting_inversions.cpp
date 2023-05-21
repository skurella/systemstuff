// https://www.hackerrank.com/challenges/ctci-merge-sort/problem

#include <gtest/gtest.h>

#include <iterator>
#include <vector>

using count_t = unsigned long long;
using iter_t = std::vector<int>::iterator;

// Merges sorted ranges (begin, middle), (middle, end) and returns the swap count.
count_t merge(iter_t begin, iter_t middle, iter_t end) {
  // std::cout << "merge: ";
  // std::copy(begin, middle, std::ostream_iterator<int>(std::cout, " "));
  // std::cout << " - ";
  // std::copy(middle, end, std::ostream_iterator<int>(std::cout, " "));
  // std::cout << std::endl;

  std::vector<int> res(std::distance(begin, end));
  auto left_iter = begin;
  auto right_iter = middle;
  count_t count = 0;
  for (auto i = 0; i < res.size(); ++i) {
    const auto use_left_iter = [&]() { res[i] = *left_iter++; };
    const auto use_right_iter = [&]() {
      count += std::distance(begin + i, right_iter);
      // std::cout << "count is now " << count << std::endl;
      res[i] = *right_iter++;
    };
    if (right_iter == end) { use_left_iter(); continue; }
    if (left_iter == middle) { use_right_iter(); continue; }
    if (*left_iter <= *right_iter) { use_left_iter(); continue; }
    else { use_right_iter(); continue; }
  }
  std::copy(res.begin(), res.end(), begin);
  return count;
}

void impl(count_t& count, iter_t a, iter_t b) {
  // std::cout << "impl: ";
  // std::copy(a, b, std::ostream_iterator<int>(std::cout, " "));
  // std::cout << std::endl;

  if (std::distance(a, b) < 2) return;
  auto mid = a + std::distance(a, b) / 2;
  impl(count, a, mid);
  impl(count, mid, b);
  count += merge(a, mid, b);
}

count_t count_inversions_merge(std::vector<int> arr) {
  count_t count = 0;
  impl(count, arr.begin(), arr.end());
  return count;
}

TEST(CountInversionsTest, Merge) {
  std::vector v{1, 3, 5, 2, 4};
  EXPECT_EQ(merge(v.begin(), v.begin() + 3, v.end()), 3);
}

long count_inversions_naive(std::vector<int> arr) {
  if (arr.size() < 2)
    return 0;

  auto count = 0ull;
  const auto impl = [&](decltype(arr)::iterator begin,
                        decltype(arr)::iterator end) {
    if (std::distance(begin, end) <= 1)
      return;

    auto left = begin;
    while (left != end) {
      auto right = std::next(left);
      int distance = 1;
      while (right != end) {
        if (*left > *right) {
          // Left can only become smaller.
          // We can continue looking for smaller numbers on the right.
          std::rotate(left, right, std::next(right));
          count += distance;
        }
        ++right;
        ++distance;
      }
      ++left;
      --distance;
    }
  };

  impl(arr.begin(), arr.end());

  return count;
}

// auto count_inversions = count_inversions_naive;
auto count_inversions = count_inversions_merge;

TEST(CountInversionsTest, TestCase0) {
  EXPECT_EQ(count_inversions({1, 1, 1, 2, 2}), 0);
  EXPECT_EQ(count_inversions({2, 1, 3, 1, 2}), 4);
}
