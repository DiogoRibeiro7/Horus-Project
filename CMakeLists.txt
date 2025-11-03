cmake_minimum_required(VERSION 3.16)
project(HORUS_PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)  # Important for resources
set(CMAKE_AUTOUIC ON)

# Set Qt6 path for macOS Homebrew
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Find Qt6
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(HORUS_PROJECT
        main.cpp
        attitudeindicator.h
        resources.qrc          # Add this line
)

target_link_libraries(HORUS_PROJECT Qt6::Core Qt6::Widgets)