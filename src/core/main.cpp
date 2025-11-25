#include <iostream>
#include "Developer.h"
#include "Tasks.h"
#include "Project.h"

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        // Создание разработчиков
        Developer dev1(1, "ivanov", "pass123");
        Developer dev2(2, "petrov", "qwerty");
        std::vector<Developer> developers = {dev1, dev2};
        
        // Создание проекта
        Project project(1, "ScrumBoard", "2024-12-31", 1);
        
        // Правильное добавление разработчика с синхронизацией
        Project::addDeveloperToProject(developers, project, 2);
        
        // Проверка
        std::cout << "Разработчики в проекте: ";
        for (int devId : project.getDeveloperIds()) {
            std::cout << devId << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Проекты у разработчика 2: ";
        for (int projectId : developers[1].getProjectIds()) {
            std::cout << projectId << " ";
        }
        std::cout << std::endl;
        
        // Сохранение
        saveDevelopersToJson(developers);
        
        std::vector<Project> projects = {project};
        Project::saveProjectsToJson(projects);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}