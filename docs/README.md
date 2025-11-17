Ирина??, [18.10.2025 21:44]
# Техническое задание: Система управления Scrum-доской

## Общее описание
Разработка приложения для управления Scrum-доской с возможностью управления задачами, разработчиками и сохранением состояния в файл.

## Структура проекта

scrum-board/
??? CMakeLists.txt
??? src/
?   ??? core/
?   ?   ??? Board.h
?   ?   ??? Board.cpp
?   ?   ??? Developer.h
?   ?   ??? Developer.cpp
?   ?   ??? Task.h
?   ?   ??? Task.cpp
?   ?   ??? Enums.h
?   ?   ??? ScrumBoard.h
?   ??? persistence/
?   ?   ??? FileManager.h
?   ?   ??? FileManager.cpp
?   ??? ui/
?   ?   ??? MainWindow.h
?   ?   ??? MainWindow.cpp
?   ?   ??? TaskDialog.h
?   ?   ??? TaskDialog.cpp
?   ?   ??? DeveloperDialog.h
?   ?   ??? DeveloperDialog.cpp
?   ??? main.cpp
??? tests/
?   ??? TestBoard.cpp
?   ??? TestTask.cpp
?   ??? TestDeveloper.cpp
?   ??? TestFileManager.cpp
?   ??? main_test.cpp
??? docs/
?   ??? README.md
??? resources/
    ??? styles.qss

## Детальное описание файлов

### 1. Core Module (Модуль ядра)

#### Enums.h
Назначение: Определение перечислений и констант
Содержание:
- enum class TaskStatus { Backlog, Assigned, InProgress, Blocked, Done }
- enum class Priority { Low, Medium, High, Critical }
- Константы для максимальных длин строк

Библиотеки: Стандартная библиотека C++17

#### Task.h / Task.cpp
Назначение: Класс задачи
Содержание:
- Поля: id, название, описание, приоритет, статус, назначенный разработчик, дата дания, дата изменения
- Методы:
  - Конструкторы
  - Геттеры/сеттеры
  - changeStatus(TaskStatus)
  - assignDeveloper(Developer*)
  - getFormattedCreationDate()

Библиотеки: Стандартная библиотека C++17, <string>, <chrono>

#### Developer.h / Developer.cpp
Назначение: Класс разработчика
Содержание:
- Поля: id, имя, email, список назначенных задач
- Методы:
  - Конструкторы
  - Геттеры/сеттеры
  - addTask(Task*), removeTask(Task*)
  - getTaskCount(), getTasksByStatus()

Библиотеки: Стандартная библиотека C++17, <string>, <vector>

#### Board.h / Board.cpp
Назначение: Управление доской
Содержание:
- Поля: список задач, список разработчиков
- Методы:
  - createEmptyBoard()
  - addDeveloper(Developer)
  - addTask(Task)
  - assignTask(int taskId, int developerId)
  - changeTaskStatus(int taskId, TaskStatus)
  - getBacklogTasks(), getTasksByStatus()
  - getDeveloper(int id), getTask(int id)

Библиотеки: Стандартная библиотека C++17, <vector>, <algorithm>

#### ScrumBoard.h
Назначение: Фасад для всей системы
Содержание:
- Поля: экземпляр Board, экземпляр FileManager
- Методы:
  - Все публичные методы Board
  - saveToFile(const std::string& filename)
  - loadFromFile(const std::string& filename)

Библиотеки: Стандартная библиотека C++17

### 2. Persistence Module (Модуль сохранения)

#### FileManager.h / FileManager.cpp
Назначение: Работа с файлами
Содержание:
- Методы:
  - saveBoard(const Board& board, const std::string& filename)
  - loadBoard(Board& board, const std::string& filename)
  - exportToJSON(const Board& board, const std::string& filename)
  - importFromJSON(Board& board, const std::string& filename)

Библиотеки: Стандартная библиотека C++17, <fstream>, рекомендуется nlohmann/json для JSON

### 3. UI Module (Модуль пользовательского интерфейса)

#### MainWindow.h / MainWindow.cpp
Назначение: Главное окно приложения
Содержание:
- Поля: ScrumBoard, виджеты для отображения задач по статусам
- Методы:
  - Инициализация UI
  - Обновление отображения доски
  - Обработчики событий для кнопок
  - Drag & Drop для перемещения задач

Библиотеки: Qt Widgets (QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QLabel)

#### TaskDialog.h / TaskDialog.cpp
Назначение: Диалог дания/редактирования задачи
Содержание:
- Поля: форма с полями ввода (название, описание, приоритет)
- Методы:
  - Валидация ввода
  - дание/обновление задачи

Библиотеки: Qt Widgets (QDialog, QLineEdit, QTextEdit, QComboBox)

#### DeveloperDialog.h / DeveloperDialog.cpp
Назначение: Диалог добавления разработчика
Содержание:
- Поля: форма с полями ввода (имя, email)
- Методы:
  - Валидация ввода
  - дание разработчика

Библиотеки: Qt Widgets

Ирина??, [18.10.2025 21:44]
### 4. Tests Module (Модуль тестирования)

#### TestTask.cpp
Тесты:
- дание задачи
- Изменение статуса
- Назначение разработчика
- Валидация данных

#### TestDeveloper.cpp
Тесты:
- дание разработчика
- Добавление/удаление задач
- Подсчет задач по статусам

#### TestBoard.cpp
Тесты:
- дание пустой доски
- Добавление задач и разработчиков
- Назначение задач
- Фильтрация задач

#### TestFileManager.cpp
Тесты:
- Сохранение/загрузка доски
- Обработка ошибок файлов

#### main_test.cpp
Назначение: Точка входа для тестов

### 5. Конфигурационные файлы

#### CMakeLists.txt
Содержание:
- Настройка компиляции для C++17
- Подключение Qt5
- Настройка gtest
- Определение исполняемых файлов для приложения и тестов

## Распределение задач между разработчиками

### Разработчик #1: Backend & Core Logic
Ответственность:
1. Модуль Core (Task, Developer, Board, ScrumBoard)
2. Модуль Persistence (FileManager)
3. Модуль тестирования для Core и Persistence
4. Настройка CMake

Файлы:
- Все файлы в src/core/
- Все файлы в src/persistence/
- CMakeLists.txt (базовая настройка)
- tests/TestTask.cpp, tests/TestDeveloper.cpp, tests/TestBoard.cpp, tests/TestFileManager.cpp

### Разработчик #2: UI & Integration
Ответственность:
1. Модуль UI (все диалоги и главное окно)
2. Интеграция Core с UI
3. Тестирование UI компонентов
4. Документация и стили

Файлы:
- Все файлы в src/ui/
- src/main.cpp
- resources/styles.qss
- docs/README.md
- Дополнительные тесты для UI

## Технологический стек

### Основные библиотеки:
- Qt5 (Core, Widgets) - для UI
- Google Test - для модульного тестирования
- CMake - система сборки

### Рекомендуемые дополнительные библиотеки:
- nlohmann/json - для работы с JSON (опционально)
- Qt5 Test - для тестирования UI компонентов

### Инструменты разработки:
- Compiler с поддержкой C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- Git для контроля версий

## Требования к качеству кода

1. Стиль кодирования: Google C++ Style Guide
2. Тестовое покрытие: ?80% для core модулей
3. Обработка ошибок: Исключения для критических ошибок, возврат кодов для ожидаемых
4. Документация: Doxygen-комментарии для публичных API
5. Валидация: Проверка входных данных на всех уровнях

## Критерии приемки

- [ ] дание пустой доски
- [ ] Добавление/удаление разработчиков
- [ ] Добавление/редактирование задач
- [ ] Назначение задач разработчикам
- [ ] Изменение статусов задач
- [ ] Сохранение/загрузка состояния
- [ ] Все модульные тесты проходят
- [ ] UI корректно отображает состояние доски