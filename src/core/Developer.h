#ifndef DEVELOPER_H
#define DEVELOPER_H
#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "json.hpp"
#include "CommonType.h"
#include "GlobalConfig.h" 

using json = nlohmann::json;

class Developer {
private:
    int id;
    std::string login;
    std::string password;
    std::vector<int> projectIds; // ID проектов, в которых участвует разработчик

public:
    Developer(int id, const std::string& login, const std::string& password);
    Developer(); // Конструктор по умолчанию для JSON
    
    // Геттеры
    int getId() const { return id; }
    std::string getLogin() const { return login; }
    std::string getPassword() const { return password; }
    const std::vector<int>& getProjectIds() const { return projectIds; }
    std::vector<Project> getProjects() const;
    
    // Сеттеры
    void setLogin(const std::string& newLogin);
    void setPassword(const std::string& newPassword);
    void addProject(int projectId);
    void removeProject(int projectId);
    
    // Методы работы с JSON
    json toJson() const;
    static Developer fromJson(const json& j);
    
    // Валидация
    bool validateLogin(const std::string& login) const;
    bool validatePassword(const std::string& password) const;
    bool checkCredentials(const std::string& inputLogin, const std::string& inputPassword) const;
    bool validateTask(int taskID) const;
    
private:
    bool validateName(const std::string& name) const;
};

// Функции для работы с файлом разработчиков
void saveDevelopersToJson(const std::vector<Developer>& developers, const std::string& filename = patf_developer);
std::vector<Developer> getDevelopersFromJson(const std::string& filename = patf_developer);
Developer* findDeveloperById(std::vector<Developer>& developers, int id);
bool validateDeveloperCredentials(const std::vector<Developer>& developers, const std::string& login, const std::string& password);
Developer* findDeveloperByLogin(std::vector<Developer>& developers, const std::string& login, const std::string& password);
#endif // DEVELOPER_H