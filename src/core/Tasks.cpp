#include "Tasks.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

// Конструктор задачи
Tasks::Tasks(int id, const std::string& title, int status)
    : id(id), title(title), status(status), creatorId(0),
      creationDate(std::chrono::system_clock::now()),
      lastModifiedDate(creationDate) {}

// Геттер для получения ID задачи
int Tasks::getId() const { return id; }

// Геттер для получения названия задачи
std::string Tasks::getTitle() const { return title; }

// Геттер для получения статуса задачи
int Tasks::getStatus() const { return status; }

// Сеттер для установки названия задачи
void Tasks::setTitle(const std::string& title) {
    this->title = title;
    lastModifiedDate = std::chrono::system_clock::now();
}

// Изменение статуса задачи
void Tasks::changeStatus(int newStatus) {
    status = newStatus;
    lastModifiedDate = std::chrono::system_clock::now();
}

// Установка названия проекта для задачи
void Tasks::setProject(const std::string& name){
    this->prodject = name;
} 

// Назначение разработчика задаче (заглушка для будущей реализации)
void Tasks::assignDeveloper(std::string developer) {
    // Реализация метода assignDeveloper
}

// Получение форматированной даты создания задачи
std::string Tasks::getFormattedCreationDate() const {
    std::time_t time = std::chrono::system_clock::to_time_t(creationDate);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Получение названия проекта для задачи
std::string Tasks::getProjekt() const { return prodject; }

// Установка ID создателя задачи
void Tasks::setCreatorId(int creatorId) {
    this->creatorId = creatorId;
}

// Получение ID создателя задачи
int Tasks::getCreatorId() const {
    return creatorId;
}

// Функция загрузки задач из JSON файла
std::vector<Tasks> getTasksFromJson(const std::string& filename) {
    std::vector<Tasks> tasks;
    
    // Открытие файла для чтения
    std::ifstream file_in(filename, std::ios::binary);
    if (!file_in.good()) {
        // Если файл не найден
        std::cout << "Файл " << filename << " не найден. Будет создан новый." << std::endl;
        return tasks;
    }
    
    try {
        json j_array;
        file_in >> j_array;
        
        // Проверка, что JSON является массивом
        if (j_array.is_array()) {
            // Проход по всем задачам в JSON
            for (const auto& j_task : j_array) {
                int id = j_task.value("id", 0);
                std::string title = j_task.value("title", "");
                int status = j_task.value("status", 0);
                std::string project = j_task.value("project", "");
                int creatorId = j_task.value("creatorId", 0); 
                
                // Создание задачи
                Tasks task(id, title, status);
                task.setCreatorId(creatorId); 
                tasks.push_back(task);
                
                // Вывод информации о загруженной задаче
                std::cout << "Загружена задача: " << title 
                          << " (ID: " << id 
                          << ", Статус: " << status 
                          << ", Creator ID: " << creatorId << ")" << std::endl;
            }
            std::cout << "Всего загружено задач: " << tasks.size() << std::endl;
        }
    } catch (const std::exception& e) {
        // Обработка ошибок при чтении файла
        std::cerr << "Ошибка при чтении файла: " << e.what() << std::endl;
    }
    
    return tasks;
}

// Функция сохранения задач в JSON файл
void saveTasksToJson(const std::vector<Tasks>& tasks, const std::string& filename) {
    json j_array = json::array();
    
    // Конвертация задач в JSON
    for (const auto& task : tasks) {
        json j_task;
        j_task["id"] = task.getId();
        j_task["title"] = task.getTitle();
        j_task["status"] = task.getStatus();
        j_task["creatorId"] = task.getCreatorId();
        j_array.push_back(j_task);
    }
    
    // Сохранение в файл
    std::ofstream file_out(filename);
    if (file_out.is_open()) {
        file_out << j_array.dump(4);
        file_out.close();
        std::cout << "Задачи сохранены: " << filename << " (задач: " << tasks.size() << ")" << std::endl;
    } else {
        // Обработка ошибки открытия файла
        std::cerr << "Ошибка сохранения: " << filename << std::endl;
    }
}