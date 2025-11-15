#ifndef SCRUMBOARD_H
#define SCRUMBOARD_H

#include "Board.h"
#include "./persistence/FileManager.h"
#include <string>

class ScrumBoard {
private:
    Board board;
    FileManager fileManager;

public:
    // Делегирование методов Board
    int addDeveloper(const std::string& name, const std::string& email) {
        return board.addDeveloper(name, email);
    }
    
    void removeDeveloper(int developerId) {
        board.removeDeveloper(developerId);
    }
    
    Developer* getDeveloper(int id) const {
        return board.getDeveloper(id);
    }
    
    std::vector<Developer*> getAllDevelopers() const {
        return board.getAllDevelopers();
    }
    
    int addTask(const std::string& title, const std::string& description, Priority priority) {
        return board.addTask(title, description, priority);
    }
    
    void removeTask(int taskId) {
        board.removeTask(taskId);
    }
    
    Task* getTask(int id) const {
        return board.getTask(id);
    }
    
    void assignTask(int taskId, int developerId) {
        board.assignTask(taskId, developerId);
    }
    
    void changeTaskStatus(int taskId, TaskStatus newStatus) {
        board.changeTaskStatus(taskId, newStatus);
    }
    
    std::vector<Task*> getBacklogTasks() const {
        return board.getBacklogTasks();
    }
    
    std::vector<Task*> getTasksByStatus(TaskStatus status) const {
        return board.getTasksByStatus(status);
    }
    
    std::vector<Task*> getAllTasks() const {
        return board.getAllTasks();
    }
    
    size_t getTaskCount() const {
        return board.getTaskCount();
    }
    
    size_t getDeveloperCount() const {
        return board.getDeveloperCount();
    }
    
    // Методы для работы с файлами
    bool saveToFile(const std::string& filename) {
        return fileManager.saveBoard(board, filename);
    }
    
    bool loadFromFile(const std::string& filename) {
        return fileManager.loadBoard(board, filename);
    }
    
    bool exportToJSON(const std::string& filename) {
        return fileManager.exportToJSON(board, filename);
    }
    
    bool importFromJSON(const std::string& filename) {
        return fileManager.importFromJSON(board, filename);
    }
};

#endif // SCRUMBOARD_H