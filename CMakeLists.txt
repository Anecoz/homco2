# This is a bit yikers, should probably set an env var for this.
if (WIN32)
  set(CMAKE_TOOLCHAIN_FILE "E:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

cmake_minimum_required(VERSION 3.7)
project(Homco2)
set(CMAKE_CXX_STANDARD 17)

# Qt stuff
if (NOT LINUX)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  set(CMAKE_AUTOUIC ON)

  set(Qt5_DIR "E:/dev/qtInstall/5.12.6/msvc2017_64/lib/cmake/Qt5" CACHE STRING "")
endif()

# For GPIO implementation specifics.
if (NOT LINUX)
  add_definitions(-DTEST=1)
endif()

include(contrib/contrib.cmake)
add_subdirectory(apps)