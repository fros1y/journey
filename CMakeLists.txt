cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project (Journey VERSION 0.0.1 LANGUAGES CXX)
include(ExternalProject)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra -Wno-error=gnu-case-range -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DRELEASE")
set(CMAKE_CXX_FLAGS_DEBUG    "-O0")

set(entityx_DIR "${Journey_SOURCE_DIR}/vendor/entityx")
set(entityx_LIBRARY "${entityx_DIR}/lib/libentityx.a")
set(entityx_INCLUDE_DIR "${entityx_DIR}/include")
ExternalProject_Add(entityx
	PREFIX ${entityx_DIR}
	GIT_REPOSITORY https://github.com/alecthomas/entityx.git
	INSTALL_DIR ${entityx_DIR}
	CMAKE_ARGS -DENTITYX_DT_TYPE=int -DENTITYX_RUN_BENCHMARKS=0 -DCMAKE_INSTALL_PREFIX=${entityx_DIR}
)

add_subdirectory(src)
