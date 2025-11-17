cmake_minimum_required(VERSION 3.10)
project(ScrumBoard)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(ScrumBoard 
    main.cpp
    ScrumBoard.cpp
    Task.cpp
)
target_link_libraries(ScrumBoard sfml-graphics sfml-window sfml-system)