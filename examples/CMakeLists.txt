file(GLOB SOURCES *.cpp)

include (${CMAKE_SOURCE_DIR}/examples/get_cpm.cmake)
set(BOOST_INCLUDE_LIBRARIES stacktrace)
CPMAddPackage(
  NAME Boost
  VERSION 1.84.0
  URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
  OPTIONS "BOOST_ENABLE_CMAKE ON"
)
unset(BOOST_INCLUDE_LIBRARIES)
find_package(Boost 1.84 COMPONENTS stacktrace REQUIRED)

add_executable(task_with_backtrace task_with_backtrace.cpp)
target_link_libraries(task_with_backtrace INTERFACE kelcorolib)
target_link_libraries(task_with_backtrace PUBLIC Boost::stacktrace_basic kelcorolib)

