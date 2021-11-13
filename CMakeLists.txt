cmake_minimum_required(VERSION 3.10)

project(asskii-player)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CLANG_DEFAULT_CXX_STDLIB libc++)
set(CLANG_DEFAULT_RTLIB compiler-rt)
set(CLANG_DEFAULT_LINKER lld)

# --------------COMPILER SPECIFIC---------------- #

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "-O3 -Wall -flto -stdlib=libc++ -lc++ -fuse-ld=lld")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "-O3 -Wall -flto -s")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#	Nobody Uses Windows Anyways ;)
#	set(CMAKE_CXX_FLAGS "/O1 /strip /Wall")
	MESSAGE("For MSVC you need to Compile Manually !!")
endif()

# ----------------------------------------------- #

add_executable(asskii-player "main.cpp")
