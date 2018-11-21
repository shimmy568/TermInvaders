cmake_minimum_required(VERSION 2.8.9)
project(opengl)

SET(CMAKE_CXX_FLAGS "-std=c++11 -O3")
SET(THREADS_PREFER_PTHREAD_FLAG ON)
SET(CMAKE_BUILD_TYPE Debug)

#Bring the headers, such as Student.h into the project
include_directories(include)

find_package(Threads REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(term-invaders ${SOURCES})

set_target_properties(term-invaders PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
)

target_link_libraries(term-invaders Threads::Threads)
