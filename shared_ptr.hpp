#include <utility>

class SharedPtrTest;

namespace seb {

template <typename T> class shared_ptr {
  friend class ::SharedPtrTest;
  T *_ptr;
  int *_use_count;

public:
  shared_ptr() : _ptr{nullptr}, _use_count{nullptr} {}
  shared_ptr(T *ptr) : _ptr{ptr}, _use_count{new int(1)} {}
  shared_ptr(const shared_ptr<T> &other) : shared_ptr() { *this = other; }
  shared_ptr(shared_ptr<T> &&other) : shared_ptr() {
    *this = std::move(other);
  }
  shared_ptr<T> &operator=(const shared_ptr<T> &other) {
    *this = shared_ptr();
    _ptr = other._ptr;
    _use_count = other._use_count;
    if (_use_count) {
      ++(*_use_count);
    }
    return *this;
  }
  shared_ptr<T> &operator=(shared_ptr<T> &&other) {
    if (_use_count) {
      if (--(*_use_count) == 0) {
        delete _ptr;
        delete _use_count;
      }
    }
    _ptr = std::exchange(other._ptr, nullptr);
    _use_count = std::exchange(other._use_count, nullptr);
    return *this;
  }
  ~shared_ptr() {
    if (_use_count) { *this = shared_ptr(); }
  }

  T& operator*() { return *_ptr; }
};

} // namespace seb