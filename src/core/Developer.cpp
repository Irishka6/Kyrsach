#include "Developer.h"
#include "Task.h"
#include <stdexcept>
#include <regex>

Developer::Developer(int id, const std::string& name, const std::string& email) 
    : id(id), name(name), email(email) {
    
    if (!validateName(name)) {
        throw std::invalid_argument("Invalid developer name");
    }
    if (!validateEmail(email)) {
        throw std::invalid_argument("Invalid developer email");
    }
}

void Developer::setName(const std::string& newName) {
    if (!validateName(newName)) {
        throw std::invalid_argument("Invalid developer name");
    }
    name = newName;
}

void Developer::setEmail(const std::string& newEmail) {
    if (!validateEmail(newEmail)) {
        throw std::invalid_argument("Invalid developer email");
    }
    email = newEmail;
}

void Developer::addTask(Task* task) {
    if (task == nullptr) {
        throw std::invalid_argument("Cannot add null task");
    }
    
    // Проверяем, не назначена ли уже задача
    auto it = std::find(assignedTasks.begin(), assignedTasks.end(), task);
    if (it == assignedTasks.end()) {
        assignedTasks.push_back(task);
    }
}

void Developer::removeTask(Task* task) {
    if (task == nullptr) return;
    
    auto it = std::find(assignedTasks.begin(), assignedTasks.end(), task);
    if (it != assignedTasks.end()) {
        assignedTasks.erase(it);
    }
}

void Developer::removeTask(int taskId) {
    assignedTasks.erase(
        std::remove_if(assignedTasks.begin(), assignedTasks.end(),
            [taskId](Task* task) { return task->getId() == taskId; }),
        assignedTasks.end()
    );
}

size_t Developer::getTasksByStatus(TaskStatus status) const {
    return std::count_if(assignedTasks.begin(), assignedTasks.end(),
        [status](Task* task) { return task->getStatus() == status; });
}

bool Developer::isValid() const {
    return validateName(name) && validateEmail(email);
}

bool Developer::validateName(const std::string& name) const {
    return !name.empty() && name.length() <= MAX_NAME_LENGTH;
}

bool Developer::validateEmail(const std::string& email) const {
    if (email.empty() || email.length() > MAX_EMAIL_LENGTH) {
        return false;
    }
    
    // Простая проверка email с помощью regex
    static const std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}