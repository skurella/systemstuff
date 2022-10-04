#include <concepts>
#include <string>

#include <gtest/gtest.h>

template <typename T> struct TreeNodeBase {
  std::unique_ptr<T> left, right;

  template <std::invocable<T &> F> void traverse(F &&f) {
    static_assert(std::is_base_of_v<TreeNodeBase<T>, T>);
    f(static_cast<T &>(*this));
    if (left) {
      left->traverse(f);
    }
    if (right) {
      right->traverse(f);
    }
  }
};

struct TreeNode : TreeNodeBase<TreeNode> {
  int val;
  TreeNode(int val_) : val{val_} {}
};

TEST(CrtpTest, Works) {
  TreeNode n{10};
  n.left = std::make_unique<TreeNode>(20);
  std::vector<int> v;
  n.traverse([&v](auto &node) { v.push_back(node.val); });
  EXPECT_EQ(v.at(0), 10);
  EXPECT_EQ(v.at(1), 20);
}
