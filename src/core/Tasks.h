#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include "Developer.h"

class Tasks {
    public:
    Tasks(int id, const std::string& title, int status);

    int getId() const;
    std::string getTitle() const;
    void setTitle(const std::string& title);
    void changeStatus(int status);
    void setProject(const std::string& name);
    void assignDeveloper(std::string developer);
    std::string getFormattedCreationDate() const;
    int getStatus() const;
    std::string getProjekt() const;
    
    private:
    int id;
    std::string title;
    int status;
    std::string prodject;
    std::chrono::system_clock::time_point creationDate;
    std::chrono::system_clock::time_point lastModifiedDate;

};

std::vector<Tasks> getTasksFromJson(const std::string& filename = "tasks.json");
void saveTasksToJson(const std::vector<Tasks>& tasks, const std::string& filename = "tasks.json");
