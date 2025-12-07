#include "Developer.h"
#include "Project.h"  
#include "Tasks.h" 
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <regex>

// Конструктор разработчика с параметрами
Developer::Developer(int id, const std::string& login, const std::string& password) 
    : id(id), login(login), password(password) {
    
    // Проверка валидности логина
    if (!validateLogin(login)) {
        throw std::invalid_argument("Invalid developer login");
    }
    // Проверка валидности пароля
    if (!validatePassword(password)) {
        throw std::invalid_argument("Invalid developer password");
    }
}

// Конструктор по умолчанию
Developer::Developer() : id(0), login(""), password("") {}

// Установка нового логина разработчика
void Developer::setLogin(const std::string& newLogin) {
    // Проверка валидности нового логина
    if (!validateLogin(newLogin)) {
        throw std::invalid_argument("Invalid developer login");
    }
    login = newLogin;
}

// Получение проектов разработчика
std::vector<Project> Developer::getProjects() const {
    std::vector<Project> projects;
    
    // Загрузка всех проектов из JSON файла
    auto allProjects = Project::getProjectsFromJson();
    
    // Фильтрация проектов, в которых участвует разработчик
    for (const auto& project : allProjects) {
        if (project.hasDeveloper(this->id)) {
            projects.push_back(project);
        }
    }
    
    return projects;
}

// Проверка валидности задачи для разработчика
bool Developer::validateTask(int taskID) const {
    // Получаем все проекты разработчика
    auto developerProjects = this->getProjects();
    
    // Ищем задачу во всех проектах разработчика
    for (const auto& project : developerProjects) {
        for (const auto& task : project.getTasks()) {
            if (task.getId() == taskID) {
                return true; // Задача найдена в одном из проектов разработчика
            }
        }
    }
    
    return false; // Задача не найдена ни в одном проекте разработчика
}

// Установка нового пароля разработчика
void Developer::setPassword(const std::string& newPassword) {
    // Проверка валидности нового пароля
    if (!validatePassword(newPassword)) {
        throw std::invalid_argument("Invalid developer password");
    }
    password = newPassword;
}

// Добавление проекта разработчику
void Developer::addProject(int projectId) {
    // Проверка, чтобы не добавлять один проект дважды
    if (std::find(projectIds.begin(), projectIds.end(), projectId) == projectIds.end()) {
        projectIds.push_back(projectId);
    }
}

// Удаление проекта у разработчика
void Developer::removeProject(int projectId) {
    // Удаление projectId из вектора
    projectIds.erase(
        std::remove(projectIds.begin(), projectIds.end(), projectId),
        projectIds.end()
    );
}

// Конвертация разработчика в JSON объект
json Developer::toJson() const {
    json j;
    j["id"] = id;
    j["login"] = login;
    j["password"] = password;
    j["project_ids"] = projectIds;
    return j;
}

// Создание разработчика из JSON объекта
Developer Developer::fromJson(const json& j) {
    Developer dev;
    dev.id = j.value("id", 0);
    dev.login = j.value("login", "");
    dev.password = j.value("password", "");
    
    // Загрузка идентификаторов проектов
    if (j.contains("project_ids") && j["project_ids"].is_array()) {
        for (const auto& projectId : j["project_ids"]) {
            dev.projectIds.push_back(projectId);
        }
    }
    
    return dev;
}

// Проверка валидности логина
bool Developer::validateLogin(const std::string& login) const {
    return !login.empty() && login.length() >= 3 && login.length() <= 20;
}

// Проверка валидности пароля
bool Developer::validatePassword(const std::string& password) const {
    return !password.empty() && password.length() >= 4;
}

// Проверка учетных данных
bool Developer::checkCredentials(const std::string& inputLogin, const std::string& inputPassword) const {
    return (login == inputLogin && password == inputPassword);
}

// Проверка валидности имени
bool Developer::validateName(const std::string& name) const {
    return !name.empty() && name.length() <= 50;
}

// Функция сохранения разработчиков в JSON файл
void saveDevelopersToJson(const std::vector<Developer>& developers, const std::string& filename) {
    json j_array = json::array();
    
    // Конвертация всех разработчиков в JSON
    for (const auto& developer : developers) {
        j_array.push_back(developer.toJson());
    }
    
    // Сохранение в файл
    std::ofstream file_out(filename, std::ios::binary);
    file_out << j_array.dump(4);
    file_out.close();
    
    // Вывод информации о сохранении
    std::cout << "Разработчики сохранены в файл: " << filename << std::endl;
}

// Функция загрузки разработчиков из JSON файла
std::vector<Developer> getDevelopersFromJson(const std::string& filename) {
    std::vector<Developer> developers;
    
    // Открытие файла
    std::ifstream file_in(filename);
    if (!file_in.good()) {
        // Если файл не найден
        std::cout << "Файл " << filename << " не найден. Будет создан новый." << std::endl;
        return developers; // возвращаем пустой вектор
    }
    
    // Проверяем, не пустой ли файл
    file_in.seekg(0, std::ios::end);
    if (file_in.tellg() == 0) {
        std::cout << "Файл " << filename << " пустой." << std::endl;
        file_in.close();
        return developers;
    }
    file_in.seekg(0, std::ios::beg); // возвращаемся в начало файла
    
    try {
        json j_array;
        file_in >> j_array;
        
        // Проверяем, что JSON является массивом
        if (j_array.is_array()) {
            for (const auto& j_dev : j_array) {
                developers.push_back(Developer::fromJson(j_dev));
            }
            std::cout << "Загружено разработчиков: " << developers.size() << std::endl;
        } else {
            std::cerr << "Ошибка: JSON не является массивом" << std::endl;
        }
    } catch (const std::exception& e) {
        // Обработка ошибок при чтении файла
        std::cerr << "Ошибка при чтении файла разработчиков: " << e.what() << std::endl;
        // Вывод содержимого файла для отладки
        file_in.clear();
        file_in.seekg(0, std::ios::beg);
        std::string content((std::istreambuf_iterator<char>(file_in)), 
                           std::istreambuf_iterator<char>());
    }
    
    file_in.close();
    return developers;
}

// Поиск разработчика по идентификатору
Developer* findDeveloperById(std::vector<Developer>& developers, int id) {
    for (auto& dev : developers) {
        if (dev.getId() == id) {
            return &dev;
        }
    }
    return nullptr;
}

// Проверка учетных данных разработчика
bool validateDeveloperCredentials(const std::vector<Developer>& developers, const std::string& login, const std::string& password) {
    for (const auto& dev : developers) {
        if (dev.checkCredentials(login, password)) {
            return true;
        }
    }
    return false;
}

// Поиск разработчика по логину и паролю
Developer* findDeveloperByLogin(std::vector<Developer>& developers, const std::string& login, const std::string& password) {
    for (auto& dev : developers) {
        if (dev.getLogin() == login && dev.getPassword() == password) {
            // Создаем новый объект разработчика
            Developer* newDev = new Developer(dev.getId(), dev.getLogin(), dev.getPassword());
            // Копируем проекты
            for (int projectId : dev.getProjectIds()) {
                newDev->addProject(projectId);
            }
            return newDev;
        }
    }
    return nullptr;
}