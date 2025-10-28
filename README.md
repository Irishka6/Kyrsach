# Kyrsach
����� ����, [18.10.2025 21:45]
# ����������� �������: ������� ���������� Scrum-������

## ����� ��������
���������� ���������� ��� ���������� Scrum-������ � ������������ ���������� ��������, �������������� � ����������� ��������� � ����.

## ��������� �������

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

## ��������� �������� ������

### 1. Core Module (������ ����)

#### Enums.h
����������: ����������� ������������ � ��������
����������:
- enum class TaskStatus { Backlog, Assigned, InProgress, Blocked, Done }
- enum class Priority { Low, Medium, High, Critical }
- ��������� ��� ������������ ���� �����

����������: ����������� ���������� C++17

#### Task.h / Task.cpp
����������: ����� ������
����������:
- ����: id, ��������, ��������, ���������, ������, ����������� �����������, ���� ��������, ���� ���������
- ������:
  - ������������
  - �������/�������
  - changeStatus(TaskStatus)
  - assignDeveloper(Developer*)
  - getFormattedCreationDate()

����������: ����������� ���������� C++17, <string>, <chrono>

#### Developer.h / Developer.cpp
����������: ����� ������������
����������:
- ����: id, ���, email, ������ ����������� �����
- ������:
  - ������������
  - �������/�������
  - addTask(Task*), removeTask(Task*)
  - getTaskCount(), getTasksByStatus()

����������: ����������� ���������� C++17, <string>, <vector>

#### Board.h / Board.cpp
����������: ���������� ������
����������:
- ����: ������ �����, ������ �������������
- ������:
  - createEmptyBoard()
  - addDeveloper(Developer)
  - addTask(Task)
  - assignTask(int taskId, int developerId)
  - changeTaskStatus(int taskId, TaskStatus)
  - getBacklogTasks(), getTasksByStatus()
  - getDeveloper(int id), getTask(int id)

����������: ����������� ���������� C++17, <vector>, <algorithm>

#### ScrumBoard.h
����������: ����� ��� ���� �������
����������:
- ����: ��������� Board, ��������� FileManager
- ������:
  - ��� ��������� ������ Board
  - saveToFile(const std::string& filename)
  - loadFromFile(const std::string& filename)

����������: ����������� ���������� C++17

### 2. Persistence Module (������ ����������)

#### FileManager.h / FileManager.cpp
����������: ������ � �������
����������:
- ������:
  - saveBoard(const Board& board, const std::string& filename)
  - loadBoard(Board& board, const std::string& filename)
  - exportToJSON(const Board& board, const std::string& filename)
  - importFromJSON(Board& board, const std::string& filename)

����������: ����������� ���������� C++17, <fstream>, ������������� nlohmann/json ��� JSON

### 3. UI Module (������ ����������������� ����������)

#### MainWindow.h / MainWindow.cpp
����������: ������� ���� ����������
����������:
- ����: ScrumBoard, ������� ��� ����������� ����� �� ��������
- ������:
  - ������������� UI
  - ���������� ����������� �����
  - ����������� ������� ��� ������
  - Drag & Drop ��� ����������� �����

����������: Qt Widgets (QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton, QLabel)

#### TaskDialog.h / TaskDialog.cpp
����������: ������ ��������/�������������� ������
����������:
- ����: ����� � ������ ����� (��������, ��������, ���������)
- ������:
  - ��������� �����
  - ��������/���������� ������

����������: Qt Widgets (QDialog, QLineEdit, QTextEdit, QComboBox)

#### DeveloperDialog.h / DeveloperDialog.cpp
����������: ������ ���������� ������������
����������:
- ����: ����� � ������ ����� (���, email)
- ������:
  - ��������� �����
  - �������� ������������

����������: Qt Widgets

����� ����, [18.10.2025 21:45]
### 4. Tests Module (������ ������������)

#### TestTask.cpp
�����:
- �������� ������
- ��������� �������
- ���������� ������������
- ��������� ������

#### TestDeveloper.cpp
�����:
- �������� ������������
- ����������/�������� �����
- ������� ����� �� ��������

#### TestBoard.cpp
�����:
- �������� ������ �����
- ���������� ����� � �������������
- ���������� �����
- ���������� �����

#### TestFileManager.cpp
�����:
- ����������/�������� �����
- ��������� ������ ������

#### main_test.cpp
����������: ����� ����� ��� ������

### 5. ���������������� �����

#### CMakeLists.txt
����������:
- ��������� ���������� ��� C++17
- ����������� Qt5
- ��������� gtest
- ����������� ����������� ������ ��� ���������� � ������

## ������������� ����� ����� ��������������

### ����������� #1: Backend & Core Logic
���������������:
1. ������ Core (Task, Developer, Board, ScrumBoard)
2. ������ Persistence (FileManager)
3. ������ ������������ ��� Core � Persistence
4. ��������� CMake

�����:
- ��� ����� � src/core/
- ��� ����� � src/persistence/
- CMakeLists.txt (������� ���������)
- tests/TestTask.cpp, tests/TestDeveloper.cpp, tests/TestBoard.cpp, tests/TestFileManager.cpp

### ����������� #2: UI & Integration
���������������:
1. ������ UI (��� ������� � ������� ����)
2. ���������� Core � UI
3. ������������ UI �����������
4. ������������ � �����

�����:
- ��� ����� � src/ui/
- src/main.cpp
- resources/styles.qss
- docs/README.md
- �������������� ����� ��� UI

## ��������������� ����

### �������� ����������:
- Qt5 (Core, Widgets) - ��� UI
- Google Test - ��� ���������� ������������
- CMake - ������� ������

### ������������� �������������� ����������:
- nlohmann/json - ��� ������ � JSON (�����������)
- Qt5 Test - ��� ������������ UI �����������

### ����������� ����������:
- Compiler � ���������� C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- Git ��� �������� ������

## ���������� � �������� ����

1. ����� �����������: Google C++ Style Guide
2. �������� ��������: ?80% ��� core �������
3. ��������� ������: ���������� ��� ����������� ������, ������� ����� ��� ���������
4. ������������: Doxygen-����������� ��� ��������� API
5. ���������: �������� ������� ������ �� ���� �������

## �������� �������

- [ ] �������� ������ �����
- [ ] ����������/�������� �������������
- [ ] ����������/�������������� �����
- [ ] ���������� ����� �������������
- [ ] ��������� �������� �����
- [ ] ����������/�������� ���������
- [ ] ��� ��������� ����� ��������
- [ ] UI ��������� ���������� ��������� �����