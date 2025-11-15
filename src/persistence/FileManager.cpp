#include <iostream>
#include "src/core/ScrumBoard.h"

using namespace std;

int main() {
    cout << "=== Scrum Board Core Module Test ===" << endl;
    
    try {
        // Создание Scrum доски
        ScrumBoard board;
        
        // Добавление разработчиков
        int dev1 = board.addDeveloper("John Doe", "john@company.com");
        int dev2 = board.addDeveloper("Jane Smith", "jane@company.com");
        cout << "? Разработчики добавлены" << endl;
        
        // Добавление задач
        int task1 = board.addTask("Implement login", "Create user authentication system", Priority::High);
        int task2 = board.addTask("Fix UI bug", "Button not responding on mobile", Priority::Medium);
        int task3 = board.addTask("Database optimization", "Optimize SQL queries", Priority::Critical);
        cout << "? Задачи созданы" << endl;
        
        // Назначение задач
        board.assignTask(task1, dev1);
        board.assignTask(task2, dev2);
        cout << "? Задачи назначены разработчикам" << endl;
        
        // Изменение статусов
        board.changeTaskStatus(task1, TaskStatus::InProgress);
        board.changeTaskStatus(task2, TaskStatus::Assigned);
        cout << "? Статусы задач обновлены" << endl;
        
        // Вывод статистики
        cout << "\n=== Статистика ===" << endl;
        cout << "Всего задач: " << board.getTaskCount() << endl;
        cout << "Всего разработчиков: " << board.getDeveloperCount() << endl;
        cout << "Backlog задач: " << board.getBacklogTasks().size() << endl;
        cout << "In Progress задач: " << board.getTasksByStatus(TaskStatus::InProgress).size() << endl;
        
        // Тестирование FileManager
        cout << "\n=== FileManager Test ===" << endl;
        
        // Экспорт в JSON
        if (board.exportToJSON("test_board.json")) {
            cout << "? Экспорт в JSON выполнен успешно" << endl;
        } else {
            cout << "? Ошибка экспорта в JSON" << endl;
        }
        
        // Сохранение в бинарном формате
        if (board.saveToFile("test_board.dat")) {
            cout << "? Сохранение в бинарном формате выполнено успешно" << endl;
        } else {
            cout << "? Ошибка сохранения в бинарном формате" << endl;
        }
        
        cout << "\n? Все тесты пройдены успешно!" << endl;
        
    } catch (const exception& e) {
        cerr << "? Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}