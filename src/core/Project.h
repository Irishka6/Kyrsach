#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include <algorithm>
#include "Tasks.h"
#include "json.hpp"
using json = nlohmann::json;

class Project {
private:
    int id;
    std::string name;
    std::string deadline;
    int creatorId; // ID разработчика-создателя
    std::vector<Tasks> tasks;
    std::vector<int> developerIds; // ID разработчиков в проекте

public:
    Project(int id, const std::string& name, const std::string& deadline, int creatorId);
    Project(); // Конструктор по умолчанию для JSON
    
    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDeadline() const { return deadline; }
    int getCreatorId() const { return creatorId; }
    const std::vector<Tasks>& getTasks() const { return tasks; }
    const std::vector<int>& getDeveloperIds() const { return developerIds; }
    
    // Сеттеры
    void setName(const std::string& newName);
    void setDeadline(const std::string& newDeadline);
    
    // Методы управления задачами
    void addTask(const Tasks& task);
    void removeTask(int taskId);
    Tasks* findTaskById(int taskId);
    
    // Методы управления разработчиками
    void addDeveloper(int developerId);
    void removeDeveloper(int developerId);
    bool hasDeveloper(int developerId) const;
    bool isCreator(int developerId) const;
    
    // Методы работы с JSON
    json toJson() const;
    static Project fromJson(const json& j);
    
    // Статические методы для работы с файлами
    static void saveProjectsToJson(const std::vector<Project>& projects, const std::string& filename = "projects.json");
    static std::vector<Project> getProjectsFromJson(const std::string& filename = "projects.json");
    
    // Методы для поиска
    static std::vector<Project> getProjectsByDeveloperId(const std::vector<Project>& projects, int developerId);
    static void addDeveloperToProject(std::vector<Developer>& developers, Project& project, int developerId);
    
    // Метод для удаления разработчика с синхронизацией  
    static void removeDeveloperFromProject(std::vector<Developer>& developers, Project& project, int developerId);
    
private:
    bool validateName(const std::string& name) const;
};

#endif // PROJECT_H