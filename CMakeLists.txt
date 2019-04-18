cmake_minimum_required(VERSION 3.0)
project(Homco2)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASIO_STANDALONE=1)

# ASIO uses some deprecated functionality (deprecated in c++17)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

# When using Clang on windows, asio gets confused and defines this twice, just define it here for ASIO.
add_definitions(-DASIO_HAS_STD_STRING_VIEW)

# Windows specific ASIO definitions
if (NOT APPLE)
  add_definitions(-DWINVER=0x0A00)
  add_definitions(-D_WIN32_WINNT=0x0A00)
  add_definitions(-DBOOST_ASIO_DISABLE_IOCP=1)
endif()

# For GPIO implementation specifics.
if (NOT LINUX)
  add_definitions(-DTEST=1)
endif()

add_subdirectory(contrib)
add_subdirectory(apps)