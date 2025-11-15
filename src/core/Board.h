#ifndef BOARD_H
#define BOARD_H

#include "Task.h"
#include "Developer.h"
#include <vector>
#include <memory>
#include <algorithm>

class Board {
private:
    std::vector<std::unique_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Developer>> developers;
    int nextTaskId;
    int nextDeveloperId;

public:
    Board();
    
    // Основные методы управления доской
    void createEmptyBoard();
    
    // Методы для разработчиков
    int addDeveloper(const std::string& name, const std::string& email);
    void removeDeveloper(int developerId);
    Developer* getDeveloper(int id) const;
    std::vector<Developer*> getAllDevelopers() const;
    
    // Методы для задач
    int addTask(const std::string& title, const std::string& description, Priority priority);
    void removeTask(int taskId);
    Task* getTask(int id) const;
    
    // Методы назначения и изменения статуса
    void assignTask(int taskId, int developerId);
    void changeTaskStatus(int taskId, TaskStatus newStatus);
    
    // Методы получения задач по статусам
    std::vector<Task*> getBacklogTasks() const;
    std::vector<Task*> getTasksByStatus(TaskStatus status) const;
    std::vector<Task*> getAllTasks() const;
    
    // Статистика
    size_t getTaskCount() const { return tasks.size(); }
    size_t getDeveloperCount() const { return developers.size(); }
    
    // Валидация
    bool hasDeveloper(int developerId) const;
    bool hasTask(int taskId) const;

private:
    int generateTaskId();
    int generateDeveloperId();
};

#endif // BOARD_H