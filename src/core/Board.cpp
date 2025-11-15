#include "Board.h"
#include <stdexcept>
#include <algorithm>

Board::Board() : nextTaskId(1), nextDeveloperId(1) {
    createEmptyBoard();
}

void Board::createEmptyBoard() {
    tasks.clear();
    developers.clear();
    nextTaskId = 1;
    nextDeveloperId = 1;
}

int Board::addDeveloper(const std::string& name, const std::string& email) {
    int id = generateDeveloperId();
    developers.push_back(std::make_unique<Developer>(id, name, email));
    return id;
}

void Board::removeDeveloper(int developerId) {
    // Сначала убираем все назначения задач этому разработчику
    for (auto& task : tasks) {
        if (task->getAssignedDeveloper() != nullptr && 
            task->getAssignedDeveloper()->getId() == developerId) {
            task->unassignDeveloper();
        }
    }
    
    // Затем удаляем разработчика
    auto it = std::find_if(developers.begin(), developers.end(),
        [developerId](const auto& dev) { return dev->getId() == developerId; });
    
    if (it != developers.end()) {
        developers.erase(it);
    }
}

Developer* Board::getDeveloper(int id) const {
    auto it = std::find_if(developers.begin(), developers.end(),
        [id](const auto& dev) { return dev->getId() == id; });
    
    return (it != developers.end()) ? it->get() : nullptr;
}

std::vector<Developer*> Board::getAllDevelopers() const {
    std::vector<Developer*> result;
    for (const auto& dev : developers) {
        result.push_back(dev.get());
    }
    return result;
}

int Board::addTask(const std::string& title, const std::string& description, Priority priority) {
    int id = generateTaskId();
    tasks.push_back(std::make_unique<Task>(id, title, description, priority));
    return id;
}

void Board::removeTask(int taskId) {
    // Убираем задачу у разработчика, если она назначена
    Task* task = getTask(taskId);
    if (task && task->getAssignedDeveloper()) {
        task->getAssignedDeveloper()->removeTask(taskId);
    }
    
    // Удаляем задачу
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [taskId](const auto& task) { return task->getId() == taskId; });
    
    if (it != tasks.end()) {
        tasks.erase(it);
    }
}

Task* Board::getTask(int id) const {
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [id](const auto& task) { return task->getId() == id; });
    
    return (it != tasks.end()) ? it->get() : nullptr;
}

void Board::assignTask(int taskId, int developerId) {
    Task* task = getTask(taskId);
    Developer* developer = getDeveloper(developerId);
    
    if (!task) {
        throw std::invalid_argument("Task not found with id: " + std::to_string(taskId));
    }
    if (!developer) {
        throw std::invalid_argument("Developer not found with id: " + std::to_string(developerId));
    }
    
    task->assignDeveloper(developer);
}

void Board::changeTaskStatus(int taskId, TaskStatus newStatus) {
    Task* task = getTask(taskId);
    if (!task) {
        throw std::invalid_argument("Task not found with id: " + std::to_string(taskId));
    }
    
    task->changeStatus(newStatus);
}

std::vector<Task*> Board::getBacklogTasks() const {
    return getTasksByStatus(TaskStatus::Backlog);
}

std::vector<Task*> Board::getTasksByStatus(TaskStatus status) const {
    std::vector<Task*> result;
    for (const auto& task : tasks) {
        if (task->getStatus() == status) {
            result.push_back(task.get());
        }
    }
    return result;
}

std::vector<Task*> Board::getAllTasks() const {
    std::vector<Task*> result;
    for (const auto& task : tasks) {
        result.push_back(task.get());
    }
    return result;
}

bool Board::hasDeveloper(int developerId) const {
    return getDeveloper(developerId) != nullptr;
}

bool Board::hasTask(int taskId) const {
    return getTask(taskId) != nullptr;
}

int Board::generateTaskId() {
    return nextTaskId++;
}

int Board::generateDeveloperId() {
    return nextDeveloperId++;
}