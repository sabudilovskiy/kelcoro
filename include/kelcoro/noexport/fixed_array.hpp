#pragma once

#include <cstddef>
#include <memory>
#include <memory_resource>

#include "kelcoro/common.hpp"

namespace dd::noexport {
template <typename T>
struct fixed_array {
 private:
  T* arr = nullptr;
  std::size_t n = 0;
  std::pmr::memory_resource* resourse = std::pmr::new_delete_resource();

 public:
  static auto default_factory() {
    return [](std::size_t) { return T{}; };
  }
  fixed_array() = default;
  fixed_array(fixed_array&& rhs) noexcept {
    *this = std::move(rhs);
  }
  fixed_array& operator=(fixed_array&& rhs) noexcept {
    std::swap(arr, rhs.arr);
    std::swap(n, rhs.n);
    std::swap(resourse, rhs.resourse);
  }
  fixed_array(std::size_t n, std::pmr::memory_resource& resource = *std::pmr::new_delete_resource())
      : fixed_array(n, default_factory(), resource) {
  }
  fixed_array(std::size_t n, std::invocable<std::size_t> auto&& factory,
              std::pmr::memory_resource& resource = *std::pmr::new_delete_resource()) noexcept
      : n(n), resourse(&resource) {
    arr = (T*)resource.allocate(sizeof(T) * n, alignof(T));
    std::size_t constructed = 0;
    bool fail = true;
    scope_exit _{[&] {
      if (fail) {
        std::destroy_n(arr, constructed);
        resourse->deallocate(arr, sizeof(T) * n, alignof(T));
      }
    }};
    for (; constructed < n; constructed++) {
      T* ptr = &arr[constructed];
      new (ptr) T(factory(constructed));
    }
    fail = false;
  }

  T* data() {
    return &arr[0];
  }

  T* data() const {
    return &arr[0];
  }

  void reset() noexcept {
    if (arr) {
      std::destroy_n(arr, n);
      resourse->deallocate(arr, sizeof(T) * n, alignof(T));
      arr = nullptr;
      n = 0;
    }
  }

  std::size_t size() const noexcept {
    return n;
  }

  T* begin() noexcept {
    return arr;
  }

  T* end() noexcept {
    return arr + n;
  }

  const T* begin() const noexcept {
    return arr;
  }

  const T* end() const noexcept {
    return arr + n;
  }

  T& operator[](std::size_t i) noexcept {
    assert(arr);
    assert(i < n);
    return arr[i];
  }

  const T& operator[](std::size_t i) const noexcept {
    assert(arr);
    assert(i < n);
    return arr[i];
  }

  ~fixed_array() {
    reset();
  }
};
}  // namespace dd::noexport