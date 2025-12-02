cmake_minimum_required(VERSION 3.16)
project(ScrumBoard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно перечислите ТОЛЬКО ваши файлы
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

# Проверьте существование файлов
foreach(src ${YOUR_SOURCES})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${src})
        message(WARNING "File not found: ${src}")
    endif()
endforeach()

add_executable(ScrumBoard ${YOUR_SOURCES} ${YOUR_HEADERS})

# Путь к SFML (исключаем examples из поиска)
set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/ui/SFML-2.6.1")

if(EXISTS ${SFML_ROOT})
    message(STATUS "SFML found at: ${SFML_ROOT}")
    
    target_include_directories(ScrumBoard PRIVATE 
        "${SFML_ROOT}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/core"
    )
    
    target_link_directories(ScrumBoard PRIVATE "${SFML_ROOT}/lib")
    target_link_libraries(ScrumBoard PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
        freetype
    )
    
    # Копирование DLL
    if(WIN32 AND EXISTS "${SFML_ROOT}/bin")
        add_custom_command(TARGET ScrumBoard POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_ROOT}/bin/sfml-graphics-2.dll"
            "${SFML_ROOT}/bin/sfml-window-2.dll"
            "${SFML_ROOT}/bin/sfml-system-2.dll"
            $<TARGET_FILE_DIR:ScrumBoard>
        )
    endif()
else()
    message(FATAL_ERROR "SFML not found at ${SFML_ROOT}")
endif()