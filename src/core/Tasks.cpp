#include "Tasks.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

Tasks::Tasks(int id, const std::string& title, int status)
    : id(id), title(title), status(status), creatorId(0),
      creationDate(std::chrono::system_clock::now()),
      lastModifiedDate(creationDate) {}

int Tasks::getId() const { return id; }
std::string Tasks::getTitle() const { return title; }
int Tasks::getStatus() const { return status; }

void Tasks::setTitle(const std::string& title) {
    this->title = title;
    lastModifiedDate = std::chrono::system_clock::now();
}

void Tasks::changeStatus(int newStatus) {
    status = newStatus;
    lastModifiedDate = std::chrono::system_clock::now();
}

void Tasks::setProject(const std::string& name){
    this->prodject = name;
} 

void Tasks::assignDeveloper(std::string developer) {
    // Реализация метода assignDeveloper
}

std::string Tasks::getFormattedCreationDate() const {
    std::time_t time = std::chrono::system_clock::to_time_t(creationDate);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Tasks::getProjekt() const { return prodject; }

// ДОБАВЛЕННЫЕ МЕТОДЫ ДЛЯ creatorId
void Tasks::setCreatorId(int creatorId) {
    this->creatorId = creatorId;
}

int Tasks::getCreatorId() const {
    return creatorId;
}

// Реализации обычных функций
std::vector<Tasks> getTasksFromJson(const std::string& filename) {
    std::vector<Tasks> tasks;
    
    std::ifstream file_in(filename, std::ios::binary);
    if (!file_in.good()) {
        std::cout << "Файл " << filename << " не найден. Будет создан новый." << std::endl;
        return tasks;
    }
    
    try {
        json j_array;
        file_in >> j_array;
        
        if (j_array.is_array()) {
            for (const auto& j_task : j_array) {
                int id = j_task.value("id", 0);
                std::string title = j_task.value("title", "");
                int status = j_task.value("status", 0);
                std::string project = j_task.value("project", "");
                int creatorId = j_task.value("creatorId", 0); 
                
                Tasks task(id, title, status);
                task.setCreatorId(creatorId); 
                tasks.push_back(task);
                
                std::cout << "Загружена задача: " << title 
                          << " (ID: " << id 
                          << ", Статус: " << status 
                          << ", Creator ID: " << creatorId << ")" << std::endl;
            }
            std::cout << "Всего загружено задач: " << tasks.size() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла: " << e.what() << std::endl;
    }
    
    return tasks;
}

void saveTasksToJson(const std::vector<Tasks>& tasks, const std::string& filename) {
    json j_array = json::array();
    
    for (const auto& task : tasks) {
        json j_task;
        j_task["id"] = task.getId();
        j_task["title"] = task.getTitle();
        j_task["status"] = task.getStatus();
        j_task["creatorId"] = task.getCreatorId();
        j_array.push_back(j_task);
    }
    
    std::ofstream file_out(filename);
    if (file_out.is_open()) {
        file_out << j_array.dump(4);
        file_out.close();
        std::cout << "Задачи сохранены: " << filename << " (задач: " << tasks.size() << ")" << std::endl;
    } else {
        std::cerr << "Ошибка сохранения: " << filename << std::endl;
    }
}