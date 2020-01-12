cmake_minimum_required(VERSION 3.10)

# set the project name
project(PiEPaper VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -pedantic -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3 -Wall -pedantic -Werror -DNDEBUG")

add_subdirectory(src)
