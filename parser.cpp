#include <concepts>

#include <gtest/gtest.h>

template <typename Value, typename Error>
class Expected {
  std::variant<Value, Error> val;

public:
  Expected(Value init) : val{init} {}
  Expected(Error init) : val{init} {}
  bool ok() { return std::holds_alternative<Value>(val); }
  Value& value() { return std::get<Value>(val); }
  Error& error() { return std::get<Error>(val); }

  template <typename F>
  Expected<std::invoke_result_t<F, Value>, Error> transform(F&& f) {
    if (ok()) return f(value());
    else return error();
  }
};

TEST(ExpectedTest, TransformsValue) {
  struct Err {};
  Expected<int, Err> a{10};
  EXPECT_TRUE(a.ok());
  EXPECT_EQ(a.value(), 10);
  auto b = a.transform([](auto i) { return i * 2; });
  EXPECT_EQ(b.value(), 20);
  auto c = b.transform([](auto i) { return std::to_string(i); });
  EXPECT_EQ(c.value(), "20");
}

TEST(ExpectedTest, TransformsError) {
  struct Err {};
  Expected<int, Err> a{Err{}};
  EXPECT_FALSE(a.ok());
  auto b = a.transform([](auto i) { return std::to_string(i); });
  EXPECT_FALSE(b.ok());
}

struct ParseError : public std::string {};

template <typename Result>
struct ParserState {
  Expected<Result, ParseError> result;
  const std::string& input;
  size_t cursor;

  template <typename NewResult = Result>
  ParserState<NewResult> raise(ParseError err) {
    return {err, input, cursor};
  }

  template <typename F, typename NewResult = std::invoke_result_t<F, Result>>
  ParserState<NewResult> transform(F&& f, size_t cursor_advance = 0) {
    return {result.transform(f), input, cursor + cursor_advance};
  }
};

struct Empty {};

// template <typename Result, typename T>
// concept ParserState = std::same_as<T, ParserState<Result>>;

// template<typename T, typename U, typename P>
// concept Parser = requires(P a) {
//   { a.parse(ParserState<T>) } -> ParserState<U>
// };

template <typename Result>
ParserState<Result> parse_end(ParserState<Result> state) {
  if (state.cursor == state.input.size()) return state;
  else return state.raise("unexpected symbol");
}

namespace Parser {

struct Token {
  const std::string_view token;
  ParserState<std::string_view> parse(ParserState<Empty> state) const {
    // TODO: replace input.starts_with with an input slice
    if (state.input.starts_with(token)) return state.transform([&](auto) { return token; }, token.size());
    else return state.raise<std::string_view>(ParseError{std::string("expected ") + std::string(token)});
  }
};

}

TEST(ParserTest, ParsesTokenSuccess) {
  std::string s {"asdf"};
  constexpr Parser::Token token{"asdf"};
  ParserState<Empty> state_in { Empty{}, s, 0 };
  auto state_out = token.parse(state_in);
  EXPECT_TRUE(state_out.result.ok());
  EXPECT_EQ(state_out.result.value(), "asdf");
  EXPECT_EQ(state_out.cursor, 4);
}

TEST(ParserTest, ParsesTokenFailure) {
  std::string s {"asdf"};
  constexpr Parser::Token token{"qwerty"};
  ParserState<Empty> state_in { Empty{}, s, 0 };
  auto state_out = token.parse(state_in);
  EXPECT_FALSE(state_out.result.ok());
  EXPECT_EQ(state_out.result.error(), "expected qwerty");
  EXPECT_EQ(state_out.cursor, 0);
}
