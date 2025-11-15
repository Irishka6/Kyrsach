#ifndef DEVELOPER_H
#define DEVELOPER_H

#include "Enums.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// Forward declaration
class Task;

class Developer {
private:
    int id;
    std::string name;
    std::string email;
    std::vector<Task*> assignedTasks;

public:
    Developer(int id, const std::string& name, const std::string& email);
    
    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    const std::vector<Task*>& getAssignedTasks() const { return assignedTasks; }
    
    // Сеттеры
    void setName(const std::string& newName);
    void setEmail(const std::string& newEmail);
    
    // Методы управления задачами
    void addTask(Task* task);
    void removeTask(Task* task);
    void removeTask(int taskId);
    
    // Статистические методы
    size_t getTaskCount() const { return assignedTasks.size(); }
    size_t getTasksByStatus(TaskStatus status) const;
    
    // Валидация
    bool isValid() const;
    
private:
    bool validateName(const std::string& name) const;
    bool validateEmail(const std::string& email) const;
};

#endif // DEVELOPER_H