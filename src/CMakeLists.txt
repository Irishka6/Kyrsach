cmake_minimum_required(VERSION 3.18)


project(ScrumBoard VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


    include(FetchContent)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1 # Or a specific branch like 'master'
    )
    FetchContent_MakeAvailable(SFML)

    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2 # Or a specific commit hash
    )

    FetchContent_MakeAvailable(freetype)

# явно перечислите “ќЋ№ ќ ваши файлы
set(YOUR_SOURCES
    ui/main.cpp
    ui/ScrumBoard.cpp
    ui/Task.cpp
    core/Developer.cpp
    core/Project.cpp
    core/Tasks.cpp
)

set(YOUR_HEADERS
    ui/ScrumBoard.h
    ui/Task.h
    core/Developer.h
    core/Project.h
    core/Tasks.h
    core/CommonType.h
    core/json.hpp
)

add_executable(ScrumBoard ${YOUR_SOURCES} ${YOUR_HEADERS})

target_link_libraries(ScrumBoard PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
        freetype
)



