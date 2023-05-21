// https://www.hackerrank.com/contests/icebreak-1/challenges/finding-the-maximum-occurrence

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <sstream>

namespace {

class Algorithm {
  std::vector<int> sorted(const std::vector<int> &in) {
    std::vector<int> out = in;
    std::sort(out.begin(), out.end());
    return out;
  }
  const std::vector<int> arr; ///< sorted numbers
  const int k;                ///< increment tokens

  struct State {
    std::vector<int>::const_iterator head;
    std::vector<int>::const_iterator tail;
    int len() const { return std::distance(tail, head) + 1; }
    // A state is better if the length is higher. When lengths are equal, the
    // smaller value wins.
    auto operator>(const State &other) {
      if (len() == other.len())
        return *head < *other.head;
      return len() > other.len();
    }
    State &operator=(const State &other) = default;

    [[gnu::used]]
    std::string to_string(const std::vector<int>& arr) const {
      std::stringstream ss;
      for (auto iter = arr.begin(); iter != arr.end(); ++iter) {
        if (iter == tail) ss << "[";
        ss << *iter;
        if (iter == head) ss << "]";
        if (iter + 1 != arr.end()) ss << " ";
      }
      return ss.str();
    }
  };

public:
  Algorithm(const std::vector<int> &numbers, const int k_init)
      : arr{sorted(numbers)}, k{k_init} {}

  std::pair<int, int> maximum_occurrence() const {
    State curr{arr.begin(), arr.begin()};
    State best = curr;

    auto required_k = 0ull;
    while (true) {
      auto prev = curr;

      // Increment head. If we're done, we're done.
      if (++curr.head == arr.end())
        break;

      // Fill all previous numbers with enough increment tokens to match the
      // current number.
      required_k += static_cast<unsigned long long>(*curr.head - *prev.head) * prev.len();

      // For as long as we're not within the token quota, pop the last element
      // and reclaim its tokens.
      while (required_k > k) {
        if (curr.head == curr.tail)
          throw std::logic_error("head==tail but k does not suffice");
        required_k -= (*curr.head - *curr.tail++);
      }

      // Possibly update the best element.
      if (curr > best) {
        best = curr;
      }
    }

    return {best.len(), *best.head};
  }
};

void TEST_ALGO(std::vector<int> arr, int k, std::pair<int, int> ret) {
  EXPECT_EQ(Algorithm(arr, k).maximum_occurrence(), ret);
}

} // namespace


TEST(MaximumOccurrenceTest, NoKLeft) {
  TEST_ALGO({3, 2, 2, 1, 1}, 0, {2, 1});
  TEST_ALGO({3, 2, 2}, 0, {2, 2});
}

TEST(MaximumOccurrenceTest, Left) {
  TEST_ALGO({3, 2, 2, 1, 1}, 0, {2, 1});
  TEST_ALGO({3, 2, 2, 1, 1}, 1, {3, 2});
  TEST_ALGO({3, 2, 2, 1, 1}, 2, {4, 2});
  TEST_ALGO({3, 2, 2, 1, 1}, 3, {4, 2});
  TEST_ALGO({3, 2, 2, 1, 1}, 4, {4, 2});
  TEST_ALGO({3, 2, 2, 1, 1}, 5, {4, 2});
  TEST_ALGO({3, 2, 2, 1, 1}, 6, {5, 3});
  TEST_ALGO({3, 2, 2, 1, 1}, 7, {5, 3});
}

TEST(MaximumOccurrenceTest, NoKMid) {
  TEST_ALGO({3, 2, 2, 1}, 0, {2, 2});
}

TEST(MaximumOccurrenceTest, Mid) {
  TEST_ALGO({3, 2, 2, 1}, 0, {2, 2});
  TEST_ALGO({3, 2, 2, 1}, 1, {3, 2});
  TEST_ALGO({3, 2, 2, 1}, 2, {3, 2});
  TEST_ALGO({3, 2, 2, 1}, 3, {3, 2});
  TEST_ALGO({3, 2, 2, 1}, 4, {4, 3});
}

TEST(MaximumOccurrenceTest, SelectsMinumum) {
  TEST_ALGO({3, 2, 2, 1, 1, 1}, 0, {3, 1});
  TEST_ALGO({3, 2, 2, 1, 1, 1}, 1, {3, 1});
  TEST_ALGO({3, 2, 2, 1, 1, 1}, 2, {4, 2});

  TEST_ALGO({4, 4, 3, 3, 2, 2, 1}, 2, {4, 3});
}

TEST(MaximumOccurrenceTest, TestCase9) {
  EXPECT_EQ(Algorithm({1, 1, 1, 1000000000}, 100).maximum_occurrence(), std::make_pair(3, 1));
}

/// A quick and dirty solution, took 15:18 - 15:27 = 9 minutes to implement.
std::pair<int, int> maximum_occurrence(std::vector<int> arr, const int k) {
  std::sort(arr.begin(), arr.end());
  auto tail = arr.begin();
  auto head = arr.begin();

  auto best_len = 1;
  auto best_val = arr[0];

  auto required_k = 0ull;
  while (true) {
    if (++head == arr.end()) break;
    auto columns_to_lift = head - tail;
    auto lift_by = *head - *(head-1);
    required_k += (0ull + columns_to_lift) * lift_by;
    while (required_k > k) {
      required_k -= (*head - *tail++);
    }
    if (head - tail + 1 > best_len) {
      best_len = head - tail + 1;
      best_val = *head;
    }
  }
  return {best_len, best_val};
}

TEST(MaximumOccurrenceTest, DirtySolution) {
  EXPECT_EQ(maximum_occurrence({1, 1, 1, 1000000000}, 100), std::make_pair(3, 1));
}