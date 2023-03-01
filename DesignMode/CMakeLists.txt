cmake_minimum_required(VERSION 3.20)

#set the project name
project(DesignMode)

if(MSVC)
    #设置msvc输入代码编码格式为utf-8
    set(CMAKE_MFC_FLAG /source-charset:utf-8)
endif()

file(GLOB 
    _SRCS 
	*.cpp
    *.h
)

add_executable(${PROJECT_NAME} ${_SRCS})