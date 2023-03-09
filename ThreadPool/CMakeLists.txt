cmake_minimum_required(VERSION 3.20)

project(ThreadPool)

file(GLOB _SRCS *.cpp *.h *.hpp)

add_executable(${PROJECT_NAME} ${_SRCS})
