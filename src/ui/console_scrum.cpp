#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

class Task {
public:
    std::string title;
    std::string description;
    int status; // 0-3: Assigned, InProgress, Blocked, Done
    
    Task(const std::string& t, const std::string& d) 
        : title(t), description(d), status(0) {}
};

class ScrumBoard {
private:
    std::vector<Task> tasks;
    std::vector<std::string> statusNames = {"Назначено", "В работе", "Блокировано", "Выполнено"};
    std::vector<char> arrows = {'?', '?', '?'}; // Стрелки между колонками

public:
    void addTask(const std::string& title, const std::string& desc) {
        tasks.push_back(Task(title, desc));
    }
    
    void toggleArrow(int index) {
        arrows[index] = (arrows[index] == '?') ? '?' : '?';
    }
    
    void display() {
        std::cout << "\n=== SCRUM ДОСКА ===\n\n";
        
        // Заголовки колонок
        for(int i = 0; i < 4; i++) {
            std::cout << std::setw(20) << std::left << statusNames[i];
            if(i < 3) std::cout << " " << arrows[i] << " ";
        }
        std::cout << "\n" << std::string(80, '-') << "\n";
        
        // Задачи по колонкам
        for(int status = 0; status < 4; status++) {
            std::cout << std::setw(20) << std::left << "";
        }
        std::cout << "\n";
        
        for(int status = 0; status < 4; status++) {
            std::cout << std::setw(20) << std::left << "[Колонка " << statusNames[status] << "]";
            if(status < 3) std::cout << "     " << "     ";
        }
        std::cout << "\n";
        
        // Показываем задачи
        int maxTasks = 0;
        for(int status = 0; status < 4; status++) {
            int count = 0;
            for(const auto& task : tasks) {
                if(task.status == status) count++;
            }
            if(count > maxTasks) maxTasks = count;
        }
        
        for(int row = 0; row < maxTasks; row++) {
            for(int status = 0; status < 4; status++) {
                int taskIndex = 0;
                for(const auto& task : tasks) {
                    if(task.status == status) {
                        if(taskIndex == row) {
                            std::cout << std::setw(20) << std::left << task.title;
                            break;
                        }
                        taskIndex++;
                    }
                }
                if(taskIndex <= row) std::cout << std::setw(20) << std::left << "";
                
                if(status < 3) std::cout << "     " << "     ";
            }
            std::cout << "\n";
        }
    }
    
    void showMenu() {
        std::cout << "\n=== МЕНЮ ===\n";
        std::cout << "1. Добавить задачу\n";
        std::cout << "2. Переключить стрелку 1 (?/?)\n";
        std::cout << "3. Переключить стрелку 2 (?/?)\n";
        std::cout << "4. Переключить стрелку 3 (?/?)\n";
        std::cout << "5. Выход\n";
        std::cout << "Выберите: ";
    }
};

int main() {
    ScrumBoard board;
    int choice;
    
    // Добавляем примеры задач
    board.addTask("Задача 1", "Разработать UI");
    board.addTask("Задача 2", "Написать тесты");
    board.addTask("Задача 3", "Исправить баги");
    
    do {
        board.display();
        board.showMenu();
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::string title, desc;
                std::cout << "Название задачи: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "Описание: ";
                std::getline(std::cin, desc);
                board.addTask(title, desc);
                break;
            }
            case 2: board.toggleArrow(0); break;
            case 3: board.toggleArrow(1); break;
            case 4: board.toggleArrow(2); break;
            case 5: std::cout << "Выход...\n"; break;
            default: std::cout << "Неверный выбор!\n";
        }
    } while(choice != 5);
    
    return 0;
}