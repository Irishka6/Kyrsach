#ifndef ENUMS_H
#define ENUMS_H

#include <string>

enum class TaskStatus {
    Backlog = 0,
    Assigned = 1,
    InProgress = 2,
    Blocked = 3,
    Done = 4
};

enum class Priority {
    Low = 0,
    Medium = 1,
    High = 2,
    Critical = 3
};

// Константы для максимальных длин строк
constexpr int MAX_TITLE_LENGTH = 100;
constexpr int MAX_DESCRIPTION_LENGTH = 500;
constexpr int MAX_NAME_LENGTH = 50;
constexpr int MAX_EMAIL_LENGTH = 100;

// Вспомогательные функции для преобразования enum в строку
namespace EnumUtils {
    std::string taskStatusToString(TaskStatus status);
    std::string priorityToString(Priority priority);
    TaskStatus stringToTaskStatus(const std::string& statusStr);
    Priority stringToPriority(const std::string& priorityStr);
}

#endif // ENUMS_H