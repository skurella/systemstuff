#include <cstddef>

namespace seb {

template <typename T, typename... Ts> class tuple {
  T _val;
  tuple<Ts...> _vals;

public:
  tuple(T &&val, Ts &&...vals) : _val{val}, _vals{std::forward<Ts>(vals)...} {}

  template <size_t I> auto &get() { return _vals.template get<I - 1>(); }
  template <> auto &get<0>() { return _val; }
};

template <typename T> class tuple<T> {
  T _val;

public:
  tuple(T &&val) : _val{val} {}
  template <size_t I> auto &get();
  template <> auto &get<0>() { return _val; }
};

} // namespace seb
