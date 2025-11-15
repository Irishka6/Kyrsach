#include "Task.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

Task::Task(int id, const std::string& title)
    : id(id), title(title), status(0),
      assignedDeveloper(nullptr), creationDate(std::chrono::system_clock::now()),
      lastModifiedDate(creationDate) {}
int Task::getId() const { return id; }
std::string Task::getTitle() const { return title; }
int Task::getStatus() { return status; }
void Task::setTitle(const std::string& title) {
    this->title = title;
    lastModifiedDate = std::chrono::system_clock::now();
}

void Task::changeStatus(int newStatus) {
    status = newStatus;
    lastModifiedDate = std::chrono::system_clock::now();
}

void Task::assignDeveloper(Developer* developer) {
    assignedDeveloper = developer;
    lastModifiedDate = std::chrono::system_clock::now();
}

std::string Task::getFormattedCreationDate() const {
    std::time_t time = std::chrono::system_clock::to_time_t(creationDate);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
std::string Task::getProjekt() {return prodject;};
void Task::saveTask(){
    json j;
    j["title"] = getTitle();
    j["status"] = getStatus();
    j["programmer"] = "Vika";
    j["project"] = getProjekt();
    
    // Вывод в консоль
    std::cout << j.dump(4) << std::endl;
    
    // Сохранение в файл
    std::ofstream file("data.json");
    file << j.dump(4);
    file.close();
    
    // Чтение из файла
    std::ifstream file_in("data.json");
    json j2;
    file_in >> j2;
    
    std::cout << "Имя: " << j2["name"] << std::endl;
}
int main(){
    Task task(1, "Implement login feature");
    task.changeStatus(TaskStatus::InProgress);
    std::cout << task.getFormattedCreationDate() << " " << task.getTitle(); // Вывод: 2023-10-05 14:30:00
}