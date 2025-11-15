#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "Developer.h"
#include "Enums.h"

class Task {
    public:
    Task(int id, const std::string& title);

    int getId() const;
    std::string getTitle() const;
    void setTitle(const std::string& title);
    void changeStatus(int status);
    void assignDeveloper(Developer* developer);
    std::string getFormattedCreationDate() const;
    void saveTask();
    int Task::getStatus();
    std::string Task::getProjekt();

    private:
    int id;
    std::string title;
    int status;
    std::string prodject;
    Developer* assignedDeveloper;
    std::chrono::system_clock::time_point creationDate;
    std::chrono::system_clock::time_point lastModifiedDate;
};
