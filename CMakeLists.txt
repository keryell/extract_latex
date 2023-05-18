cmake_minimum_required(VERSION 3.24)
project(extract_latex LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23 CACHE STRING "C++ standard to use")

# Pick the Release build type if not defined (to have range-v3 happy)
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# To get some content directly at the source
include(FetchContent)

# Get directly a recent version of range-v3 at the source because
# there are some issues with some old versions provided as
# distribution packages
FetchContent_Declare(range_v3
  GIT_REPOSITORY    https://github.com/ericniebler/range-v3
  GIT_SHALLOW       TRUE
  GIT_TAG           master
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(range_v3)

# Need to install with apt install libfmt-dev for example
find_package(fmt REQUIRED)

add_executable(extract_latex extract_latex.cpp)
# Link dependencies
target_link_libraries(extract_latex PUBLIC range-v3::range-v3 fmt::fmt)

add_executable(remove_skipped_slides remove_skipped_slides.cpp)
# Link dependencies
target_link_libraries(remove_skipped_slides PUBLIC range-v3::range-v3)
