#include <concepts>
#include <memory>
#include <utility>

class UniquePtrTest;

namespace seb {

template <typename T, std::default_initializable Deleter = std::default_delete<T>>
class unique_ptr {
  friend class ::UniquePtrTest;
  T *_ptr;

public:
  unique_ptr(T *ptr) : _ptr(ptr) {}
  unique_ptr(const unique_ptr<T> &) = delete;
  unique_ptr(unique_ptr<T> &&other) : _ptr{std::exchange(other._ptr, nullptr)} {}
  unique_ptr<T> &operator=(const unique_ptr<T> &) = delete;
  unique_ptr<T> &operator=(unique_ptr<T> &&other) {
    if (this != &other) {
      std::exchange(other._ptr, nullptr);
    }
    return *this;
  }
  ~unique_ptr() { Deleter()(_ptr); }
};

} // namespace seb