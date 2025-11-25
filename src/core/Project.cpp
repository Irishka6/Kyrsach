#include "Project.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

Project::Project(int id, const std::string& name, const std::string& deadline, int creatorId)
    : id(id), name(name), deadline(deadline), creatorId(creatorId) {
    
    if (!validateName(name)) {
        throw std::invalid_argument("Invalid project name");
    }
    // Автоматически добавляем создателя в проект
    developerIds.push_back(creatorId);
}

Project::Project() : id(0), name(""), deadline(""), creatorId(0) {}

void Project::setName(const std::string& newName) {
    if (validateName(newName)) {
        name = newName;
    } else {
        throw std::invalid_argument("Invalid project name");
    }
}

void Project::setDeadline(const std::string& newDeadline) {
    deadline = newDeadline;
}

void Project::addTask(const Tasks& task) {
    // Проверяем, нет ли уже задачи с таким ID
    for (const auto& existingTask : tasks) {
        if (existingTask.getId() == task.getId()) {
            throw std::runtime_error("Task with ID " + std::to_string(task.getId()) + " already exists in project");
        }
    }
    
    tasks.push_back(task);
    std::cout << "Задача '" << task.getTitle() << "' добавлена в проект '" << name << "'" << std::endl;
}

void Project::removeTask(int taskId) {
    auto it = std::remove_if(tasks.begin(), tasks.end(),
        [taskId](const Tasks& task) { return task.getId() == taskId; });
    
    if (it != tasks.end()) {
        tasks.erase(it, tasks.end());
        std::cout << "Задача ID:" << taskId << " удалена из проекта '" << name << "'" << std::endl;
    } else {
        throw std::runtime_error("Task with ID " + std::to_string(taskId) + " not found in project");
    }
}

Tasks* Project::findTaskById(int taskId) {
    for (auto& task : tasks) {
        if (task.getId() == taskId) {
            return &task;
        }
    }
    return nullptr;
}

void Project::addDeveloper(int developerId) {
    if (std::find(developerIds.begin(), developerIds.end(), developerId) == developerIds.end()) {
        developerIds.push_back(developerId);
        std::cout << "Разработчик ID:" << developerId << " добавлен в проект '" << name << "'" << std::endl;
        std::cout << "ВНИМАНИЕ: Не забудьте добавить проект разработчику!" << std::endl;
    }
}

void Project::removeDeveloper(int developerId) {
    // Не позволяем удалить создателя
    if (developerId == creatorId) {
        throw std::runtime_error("Cannot remove project creator from project");
    }
    
    developerIds.erase(
        std::remove(developerIds.begin(), developerIds.end(), developerId),
        developerIds.end()
    );
    std::cout << "Разработчик ID:" << developerId << " удален из проекта '" << name << "'" << std::endl;
    std::cout << "ВНИМАНИЕ: Не забудьте удалить проект у разработчика!" << std::endl;
}

bool Project::hasDeveloper(int developerId) const {
    return std::find(developerIds.begin(), developerIds.end(), developerId) != developerIds.end();
}

bool Project::isCreator(int developerId) const {
    return developerId == creatorId;
}

json Project::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["deadline"] = deadline;
    j["creator_id"] = creatorId;
    j["developer_ids"] = developerIds;
    
    // Сериализуем задачи
    json tasks_array = json::array();
    for (const auto& task : tasks) {
        json task_json;
        task_json["id"] = task.getId();
        task_json["title"] = task.getTitle();
        task_json["status"] = task.getStatus();
        task_json["project"] = task.getProjekt();
        task_json["creation_date"] = task.getFormattedCreationDate();
        tasks_array.push_back(task_json);
    }
    j["tasks"] = tasks_array;
    
    return j;
}

Project Project::fromJson(const json& j) {
    Project project;
    project.id = j.value("id", 0);
    project.name = j.value("name", "");
    project.deadline = j.value("deadline", "");
    project.creatorId = j.value("creator_id", 0);
    
    // Загружаем ID разработчиков
    if (j.contains("developer_ids") && j["developer_ids"].is_array()) {
        for (const auto& devId : j["developer_ids"]) {
            project.developerIds.push_back(devId);
        }
    }
    
    // Загружаем задачи
    if (j.contains("tasks") && j["tasks"].is_array()) {
        for (const auto& task_json : j["tasks"]) {
            int taskId = task_json.value("id", 0);
            std::string title = task_json.value("title", "");
            int status = task_json.value("status", 0);
            std::string projectName = task_json.value("project", "");
            
            Tasks task(taskId, title, status);
            task.setProject(projectName);
            project.tasks.push_back(task);
        }
    }
    
    return project;
}

void Project::saveProjectsToJson(const std::vector<Project>& projects, const std::string& filename) {
    json j_array = json::array();
    
    for (const auto& project : projects) {
        j_array.push_back(project.toJson());
    }
    
    std::ofstream file_out(filename, std::ios::binary);
    file_out << j_array.dump(4);
    file_out.close();
    
    std::cout << "Проекты сохранены в файл: " << filename << std::endl;
    std::cout << "Количество сохраненных проектов: " << projects.size() << std::endl;
}

std::vector<Project> Project::getProjectsFromJson(const std::string& filename) {
    std::vector<Project> projects;
    
    std::ifstream file_in(filename, std::ios::binary);
    if (!file_in.good()) {
        std::cout << "Файл " << filename << " не найден. Будет создан новый." << std::endl;
        return projects;
    }
    
    try {
        json j_array;
        file_in >> j_array;
        
        if (j_array.is_array()) {
            for (const auto& j_project : j_array) {
                projects.push_back(Project::fromJson(j_project));
            }
            std::cout << "Загружено проектов: " << projects.size() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при чтении файла проектов: " << e.what() << std::endl;
    }
    
    return projects;
}

std::vector<Project> Project::getProjectsByDeveloperId(const std::vector<Project>& projects, int developerId) {
    std::vector<Project> result;
    for (const auto& project : projects) {
        if (project.hasDeveloper(developerId)) {
            result.push_back(project);
        }
    }
    return result;
}

bool Project::validateName(const std::string& name) const {
    return !name.empty() && name.length() <= 100;
}

void Project::addDeveloperToProject(std::vector<Developer>& developers, Project& project, int developerId) {
    // Добавляем разработчика в проект
    if (std::find(project.developerIds.begin(), project.developerIds.end(), developerId) == project.developerIds.end()) {
        project.developerIds.push_back(developerId);
        std::cout << "Разработчик ID:" << developerId << " добавлен в проект '" << project.name << "'" << std::endl;
    }
    
    // Добавляем проект разработчику
    for (auto& developer : developers) {
        const auto& devIds = project.getDeveloperIds();
        if (std::find(devIds.begin(), devIds.end(), developer.getId()) != devIds.end()) {
            developer.addProject(project.getId());
            std::cout << "Проект '" << project.name << "' добавлен разработчику ID:" << developerId << std::endl;
        }
    }
}

void Project::removeDeveloperFromProject(std::vector<Developer>& developers, Project& project, int developerId) {
    // Не позволяем удалить создателя
    if (developerId == project.creatorId) {
        throw std::runtime_error("Cannot remove project creator from project");
    }
    
    // Удаляем разработчика из проекта
    project.developerIds.erase(
        std::remove(project.developerIds.begin(), project.developerIds.end(), developerId),
        project.developerIds.end()
    );
    
    // Удаляем проект у разработчика
    for (auto& developer : developers) {
        if (developer.getId() == developerId) {
            developer.removeProject(project.getId());
            std::cout << "Проект '" << project.name << "' удален у разработчика ID:" << developerId << std::endl;
            break;
        }
    }
    
    std::cout << "Разработчик ID:" << developerId << " удален из проекта '" << project.name << "'" << std::endl;
}