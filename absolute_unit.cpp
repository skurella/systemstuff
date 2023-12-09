#include <gtest/gtest.h>

#include <exception>
#include <memory>
#include <variant>

enum class Operation {
  Add,
  Multiply
};

struct Unit {
  int meters = 0;
  int seconds = 0;
  int kilograms = 0;

  bool operator==(const Unit& other) const = default;
  Unit operator+(const Unit& other) const {
    return Unit {meters + other.meters, seconds + other.seconds, kilograms + other.kilograms};
  }

  int linear_complexity() const {
    return abs(meters) + abs(seconds) + abs(kilograms);
  }
};

struct CustomUnit {
  std::string symbol;
  Unit unit;
};

struct UnitRepresentation {
  std::map<CustomUnit, int> custom_units;
  Unit remaining_unit;
};

struct ValueNode {
  ValueNode(float _value, const Unit& _unit) : value(_value), unit(_unit) {}
  float value;
  Unit unit;
};

struct OpNode;
using NodePtr = std::variant<std::unique_ptr<OpNode>, std::unique_ptr<ValueNode>>;

struct OpNode {
  OpNode(Operation _op, NodePtr _left, NodePtr _right) : op{_op}, left{std::move(_left)}, right{std::move(_right)} {}

  Operation op;
  NodePtr left;
  NodePtr right;
};

constexpr Unit Newton {
  .meters = 2,
  .seconds = -1,
  .kilograms = 1,
};

Unit get_unit(const NodePtr& node) {
  struct Visitor {
    Unit operator()(const std::unique_ptr<OpNode>& op_node) {
      auto left_unit = get_unit(op_node->left);
      auto right_unit = get_unit(op_node->right);
      switch (op_node->op) {
        case Operation::Add:
          if (left_unit != right_unit) throw std::domain_error("units don't match");
          return left_unit;
        case Operation::Multiply:
          return left_unit + right_unit;
      }
    }
    Unit operator()(const std::unique_ptr<ValueNode>& value_node) {
      return value_node->unit;
    }
  };
  return std::visit(Visitor{}, node);
}

TEST(AbsoluteUnit, GoodAddUnits) {
  // 2 m + 2 m
  auto left = std::make_unique<ValueNode>(2.0f, Unit {.meters = 1});
  auto right = std::make_unique<ValueNode>(2.0f, Unit {.meters = 1});
  NodePtr root = std::make_unique<OpNode>(Operation::Add, std::move(left), std::move(right));
  EXPECT_EQ(get_unit(root), Unit {.meters = 1});
}

TEST(AbsoluteUnit, WrongAddUnits) {
  // 2 m + 2 s
  auto left = std::make_unique<ValueNode>(2.0f, Unit {.meters = 1});
  auto right = std::make_unique<ValueNode>(2.0f, Unit {.seconds = 1});
  NodePtr root = std::make_unique<OpNode>(Operation::Add, std::move(left), std::move(right));
  EXPECT_THROW(get_unit(root), std::domain_error);
}

TEST(AbsoluteUnit, MultiplyUnits) {
  // 2 kg * 3 m^2/s
  auto left = std::make_unique<ValueNode>(2.0f, Unit {.kilograms = 1});
  auto right = std::make_unique<ValueNode>(3.0f, Unit {.meters = 2, .seconds = -1});
  NodePtr root = std::make_unique<OpNode>(Operation::Multiply, std::move(left), std::move(right));
  EXPECT_EQ(get_unit(root), Newton);
}
