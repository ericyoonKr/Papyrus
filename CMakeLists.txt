# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Papyrus VERSION 1.0 LANGUAGES CXX)

if(WIN32)
    message(STATUS "Windows detected, using default MSVC compiler.")
else()
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
    message(STATUS "Non-Windows detected, using Clang compiler.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

find_package(Qt6 COMPONENTS Widgets REQUIRED)


file(GLOB HEADER_FILES "include/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")


set(RESOURCES resources/resources.qrc)
if(WIN32)
    set(WIN_RESOURCE_FILE resources/icon.rc)
endif()

add_executable(Papyrus main.cpp ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCES} ${WIN_RESOURCE_FILE})
target_include_directories(Papyrus PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Papyrus PRIVATE Qt6::Widgets)