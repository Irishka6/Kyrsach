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
    std::vector<std::string> statusNames = {"���������", "� ������", "�����������", "���������"};
    std::vector<char> arrows = {'?', '?', '?'}; // ������� ����� ���������

public:
    void addTask(const std::string& title, const std::string& desc) {
        tasks.push_back(Task(title, desc));
    }
    
    void toggleArrow(int index) {
        arrows[index] = (arrows[index] == '?') ? '?' : '?';
    }
    
    void display() {
        std::cout << "\n=== SCRUM ����� ===\n\n";
        
        // ��������� �������
        for(int i = 0; i < 4; i++) {
            std::cout << std::setw(20) << std::left << statusNames[i];
            if(i < 3) std::cout << " " << arrows[i] << " ";
        }
        std::cout << "\n" << std::string(80, '-') << "\n";
        
        // ������ �� ��������
        for(int status = 0; status < 4; status++) {
            std::cout << std::setw(20) << std::left << "";
        }
        std::cout << "\n";
        
        for(int status = 0; status < 4; status++) {
            std::cout << std::setw(20) << std::left << "[������� " << statusNames[status] << "]";
            if(status < 3) std::cout << "     " << "     ";
        }
        std::cout << "\n";
        
        // ���������� ������
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
        std::cout << "\n=== ���� ===\n";
        std::cout << "1. �������� ������\n";
        std::cout << "2. ����������� ������� 1 (?/?)\n";
        std::cout << "3. ����������� ������� 2 (?/?)\n";
        std::cout << "4. ����������� ������� 3 (?/?)\n";
        std::cout << "5. �����\n";
        std::cout << "��������: ";
    }
};

int main() {
    ScrumBoard board;
    int choice;
    
    // ��������� ������� �����
    board.addTask("������ 1", "����������� UI");
    board.addTask("������ 2", "�������� �����");
    board.addTask("������ 3", "��������� ����");
    
    do {
        board.display();
        board.showMenu();
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::string title, desc;
                std::cout << "�������� ������: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "��������: ";
                std::getline(std::cin, desc);
                board.addTask(title, desc);
                break;
            }
            case 2: board.toggleArrow(0); break;
            case 3: board.toggleArrow(1); break;
            case 4: board.toggleArrow(2); break;
            case 5: std::cout << "�����...\n"; break;
            default: std::cout << "�������� �����!\n";
        }
    } while(choice != 5);
    
    return 0;
}