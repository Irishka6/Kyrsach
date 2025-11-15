#include "Enums.h"
#include <stdexcept>

std::string EnumUtils::taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::Backlog: return "Backlog";
        case TaskStatus::Assigned: return "Assigned";
        case TaskStatus::InProgress: return "InProgress";
        case TaskStatus::Blocked: return "Blocked";
        case TaskStatus::Done: return "Done";
        default: return "Unknown";
    }
}

std::string EnumUtils::priorityToString(Priority priority) {
    switch (priority) {
        case Priority::Low: return "Low";
        case Priority::Medium: return "Medium";
        case Priority::High: return "High";
        case Priority::Critical: return "Critical";
        default: return "Unknown";
    }
}

TaskStatus EnumUtils::stringToTaskStatus(const std::string& statusStr) {
    if (statusStr == "Backlog") return TaskStatus::Backlog;
    if (statusStr == "Assigned") return TaskStatus::Assigned;
    if (statusStr == "InProgress") return TaskStatus::InProgress;
    if (statusStr == "Blocked") return TaskStatus::Blocked;
    if (statusStr == "Done") return TaskStatus::Done;
    throw std::invalid_argument("Unknown task status: " + statusStr);
}

Priority EnumUtils::stringToPriority(const std::string& priorityStr) {
    if (priorityStr == "Low") return Priority::Low;
    if (priorityStr == "Medium") return Priority::Medium;
    if (priorityStr == "High") return Priority::High;
    if (priorityStr == "Critical") return Priority::Critical;
    throw std::invalid_argument("Unknown priority: " + priorityStr);
}