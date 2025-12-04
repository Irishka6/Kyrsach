cmake_minimum_required(VERSION 3.16)
project(ScrumBoard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить поддержку тестов
enable_testing()

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

# Основное приложение
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

# ==================== GOOGLE TEST КОНФИГУРАЦИЯ ====================

# Путь к Google Test
set(GTEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../googletest-old")
if(EXISTS ${GTEST_ROOT})
    message(STATUS "Google Test found at: ${GTEST_ROOT}")
    
    # Добавляем подпроект для GTest
    add_subdirectory(${GTEST_ROOT} googletest EXCLUDE_FROM_ALL)
    
    # Тестовые файлы
    set(TEST_SOURCES
        core/tests.cpp
        core/Developer.cpp
        core/Project.cpp
        core/Tasks.cpp
    )
    
    # Тестовые заголовки
    set(TEST_HEADERS
        core/Developer.h
        core/Project.h
        core/Tasks.h
        core/CommonType.h
        core/json.hpp
    )
    
    # Исполняемый файл тестов
    add_executable(ScrumBoardTests ${TEST_SOURCES} ${TEST_HEADERS})
    
    # Включаем директории
    target_include_directories(ScrumBoardTests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/core"
        "${GTEST_ROOT}/googletest/include"
        "${GTEST_ROOT}/googlemock/include"
    )
    
    # Связываем с gtest
    target_link_libraries(ScrumBoardTests PRIVATE
        gtest
        gtest_main
        pthread
    )
    
    # Добавляем тест в CTest
    add_test(NAME ScrumBoardTests COMMAND ScrumBoardTests)
    
else()
    message(WARNING "Google Test not found at ${GTEST_ROOT}. Tests will not be built.")
endif()

# ==================== АЛЬТЕРНАТИВНЫЙ ПРОСТОЙ ТЕСТ ====================

# Создаем простые тесты без Google Test
set(SIMPLE_TESTS_SOURCES
    core/simple_tests.cpp
    core/Developer.cpp
    core/Project.cpp
    core/Tasks.cpp
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/core/simple_tests.cpp")
    add_executable(SimpleTests ${SIMPLE_TESTS_SOURCES})
    
    target_include_directories(SimpleTests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/core"
    )
    
    add_test(NAME SimpleTests COMMAND SimpleTests)
endif()

# ==================== КОМАНДЫ СБОРКИ ====================

# Команда для сборки всех тестов
add_custom_target(build_all_tests
    COMMENT "Building all tests..."
    DEPENDS ScrumBoardTests SimpleTests
)

# Команда для запуска тестов
add_custom_target(run_tests
    COMMAND ctest --output-on-failure
    COMMENT "Running tests..."
    DEPENDS ScrumBoardTests
)