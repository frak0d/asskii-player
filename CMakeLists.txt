cmake_minimum_required(VERSION 3.10)

project(asskii-player)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------COMPILER SPECIFIC---------------- #

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "-O3 -s -Wall -flto -stdlib=libc++ -fuse-ld=lld")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "-O3 -s -Wall")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#	Nobody Uses Windows Anyways ;)
#	set(CMAKE_CXX_FLAGS "/O1 /strip /Wall")
	MESSAGE("For MSVC you need to Compile Manually !!")
endif()

# ----------------------------------------------- #

add_executable(asskii-player "main.cpp")
