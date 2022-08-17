cmake_minimum_required(VERSION 3.20)
project(libxid VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIBXID_ENABLE_COVERAGE "Enable code coverage" OFF)
option(LIBXID_ENABLE_TESTING "Enable testing" OFF)

include(cmake/dependencies.cmake)

if (LIBXID_ENABLE_TESTING)
	enable_testing()
endif()

add_subdirectory(lib)

