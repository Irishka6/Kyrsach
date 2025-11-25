#include "Developer.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <regex>

Developer::Developer(int id, const std::string& login, const std::string& password) 
    : id(id), login(login), password(password) {
    
    if (!validateLogin(login)) {
        throw std::invalid_argument("Invalid developer login");
    }
    if (!validatePassword(password)) {
        throw std::invalid_argument("Invalid developer password");
    }
}

Developer::Developer() : id(0), login(""), password("") {}

void Developer::setLogin(const std::string& newLogin) {
    if (!validateLogin(newLogin)) {
        throw std::invalid_argument("Invalid developer login");
    }
    login = newLogin;
}

std::vector<Project> Developer::getProject() const {
    std::vector<Project> projects;
    
    // Загружаем все проекты из файла
    auto allProjects = Project::getProjectsFromJson();
    
    // Фильтруем только те проекты, где есть этот разработчик
    for (const auto& project : allProjects) {
        if (project.hasDeveloper(this->id)) {
            projects.push_back(project);
        }
    }
    
    return projects;
}
bool Developer::validateProject(int projectID) const {
    for (auto& proj : projectIds){
        if (proj == projectID){
            return true;
        }
        return false;
    }

}

void Developer::setPassword(const std::string& newPassword) {
    if (!validatePassword(newPassword)) {
        throw std::invalid_argument("Invalid developer password");
    }
    password = newPassword;
}

void Developer::addProject(int projectId) {
    if (std::find(projectIds.begin(), projectIds.end(), projectId) == projectIds.end()) {
        projectIds.push_back(projectId);
    }
}

void Developer::removeProject(int projectId) {
    projectIds.erase(
        std::remove(projectIds.begin(), projectIds.end(), projectId),
        projectIds.end()
    );
}

json Developer::toJson() const {
    json j;
    j["id"] = id;
    j["login"] = login;
    j["password"] = password;
    j["project_ids"] = projectIds;
    return j;
}

Developer Developer::fromJson(const json& j) {
    Developer dev;
    dev.id = j.value("id", 0);
    dev.login = j.value("login", "");
    dev.password = j.value("password", "");
    
    if (j.contains("project_ids") && j["project_ids"].is_array()) {
        for (const auto& projectId : j["project_ids"]) {
            dev.projectIds.push_back(projectId);
        }
    }
    
    return dev;
}

bool Developer::validateLogin(const std::string& login) const {
    return !login.empty() && login.length() >= 3 && login.length() <= 20;
}

bool Developer::validatePassword(const std::string& password) const {
    return !password.empty() && password.length() >= 4;
}

bool Developer::checkCredentials(const std::string& inputLogin, const std::string& inputPassword) const {
    return (login == inputLogin && password == inputPassword);
}

bool Developer::validateName(const std::string& name) const {
    return !name.empty() && name.length() <= 50;
}

// Функции для работы с файлом разработчиков
void saveDevelopersToJson(const std::vector<Developer>& developers, const std::string& filename) {
    json j_array = json::array();
    
    for (const auto& developer : developers) {
        j_array.push_back(developer.toJson());
    }
    
    std::ofstream file_out(filename, std::ios::binary);
    file_out << j_array.dump(4);
    file_out.close();
    
    std::cout << "Разработчики сохранены в файл: " << filename << std::endl;
    std::cout << "Количество сохраненных разработчиков: " << developers.size() << std::endl;
}

std::vector<Developer> getDevelopersFromJson(const std::string& filename) {
    std::vector<Developer> developers;
    
    std::ifstream file_in(filename, std::ios::binary);
    if (!file_in.good()) {
        std::cout << "Файл " << filename << " не найден. Будет создан новый." << std::endl;
        return developers;
    }
    
    try {
        json j_array;
        file_in >> j_array;
        
        if (j_array.is_array()) {
            for (const auto& j_dev : j_array) {
                developers.push_back(Developer::fromJson(j_dev));
            }
            std::cout << "Загружено разработчиков: " << developers.size() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла разработчиков: " << e.what() << std::endl;
    }
    
    return developers;
}

Developer* findDeveloperById(std::vector<Developer>& developers, int id) {
    for (auto& dev : developers) {
        if (dev.getId() == id) {
            return &dev;
        }
    }
    return nullptr;
}

bool validateDeveloperCredentials(const std::vector<Developer>& developers, const std::string& login, const std::string& password) {
    for (const auto& dev : developers) {
        if (dev.checkCredentials(login, password)) {
            return true;
        }
    }
    return false;
}

Developer* findDeveloperByLogin(std::vector<Developer> developers, const std::string& login, const std::string& password){
    for (auto& dev : developers){
        if (dev.getLogin() == login && dev.getPassword() == password){
            return &dev;
        } else {
            return nullptr;
        }
    }
}