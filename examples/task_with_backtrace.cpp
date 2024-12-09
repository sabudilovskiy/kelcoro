#include <cassert>
#include <iostream>
#include <kelcoro/kelcoro.hpp>
#include <boost/stacktrace.hpp>
#include "boost/stacktrace/stacktrace.hpp"
#include "kelcoro/async_task.hpp"

std::string get_current_coro_name(const boost::stacktrace::stacktrace& st) {
  return "";
}

dd::async_task<void> task() {
}

struct trace_ctx {
  std::vector<std::string> stack;

  template <typename R1, typename R2>
  void on_owner_setted(std::coroutine_handle<dd::task_promise<R1, trace_ctx>> owner,
                       std::coroutine_handle<dd::task_promise<R2, trace_ctx>> task) noexcept {
    stack = owner.promise().ctx.stack;
  }
  template <typename P1, typename P2>
  void on_owner_setted(std::coroutine_handle<P1>, std::coroutine_handle<P2>) noexcept {
  }
  template <typename TaskPromise>
  void on_start(std::coroutine_handle<TaskPromise> task) noexcept {
    auto st = boost::stacktrace::stacktrace();
    auto& v_st = st.as_vector();
    for (auto& s : v_st)
      std::cout << "[" << s << "]\n";
    std::cout << "===\n";
  }
  template <typename TaskPromise>
  void on_end(std::coroutine_handle<TaskPromise>) noexcept {
  }
};

dd::task<int, trace_ctx> another_coro() {
  co_return 5;
}

dd::task<int, trace_ctx> main_coro() {
  auto r = co_await another_coro();
  co_return r;
}

int main() {
  main_coro().start_and_detach();
}