#ifndef SCRUMBOARD_H
#define SCRUMBOARD_H

#include "Task.h"
#include "../core/Tasks.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ScrumBoard {
private:
    // Графика и текст
    sf::Font font;
    sf::Text titleText;
    
    // Верхняя панель управления
    sf::RectangleShape topPanel;
    sf::RectangleShape projectButton;
    sf::Text projectButtonText;
    sf::RectangleShape saveButton;
    sf::Text saveButtonText;
    sf::RectangleShape addButton;               // Кнопка открытия окна добавления новой задачи
    sf::Text addButtonText;
    
    // Секции 4 колонки
    std::vector<sf::RectangleShape> sections;
    std::vector<sf::Text> sectionTexts;
    std::vector<std::string> sectionNames;
    
    // Задачи по секциям
    std::vector<std::vector<Task>> tasks;
    
    // Перетаскивание задач
    int draggingTaskSection;
    int draggingTaskIndex;
    
    // Окно проектов
    bool showProjectWindow;
    sf::RectangleShape projectWindow;
    std::vector<sf::RectangleShape> projectRects;
    std::vector<sf::Text> projectTexts;
    std::vector<std::string> projects;
    
    // Окно добавления задач
    bool showAddTaskWindow;                     // Флаг видимости окна добавления задачи
    bool isTaskInputActive;                     // Флаг активности поля ввода
    sf::RectangleShape addTaskWindow;           // Основное окно добавления задачи
    sf::RectangleShape taskInputField;          // Поле ввода названия задачи
    sf::Text taskInputText;                     // Текст в поле ввода
    std::string currentTaskInput;               // Текущий введенный текст задачи
    sf::Text sectionLabelText;                  // Текст "Выберите секцию"
    std::vector<sf::RectangleShape> sectionOptionRects; // Кнопки выбора секций
    std::vector<sf::Text> sectionOptionTexts;   // Тексты секций для выбора
    sf::RectangleShape confirmAddButton;        // Кнопка подтверждения добавления
    sf::Text confirmAddButtonText;              // Текст кнопки подтверждения
    sf::RectangleShape cancelAddButton;         // Кнопка отмены добавления
    sf::Text cancelAddButtonText;               // Текст кнопки отмены
    
    // Данные задач
    std::vector<Tasks> tasksData;

public:
    ScrumBoard();
    bool initialize();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    void createTitle();                         // Создает и настраивает заголовок приложения
    void createTopPanel();                      // Создает верхнюю панель с кнопками управления
    void createSections();                      // Создает 4 секции для организации задач
    void createSampleTasks();                   // Преобразует данные JSON в визуальные задачи
    void createProjectWindow();                 // Создает всплывающее окно выбора проектов
    void createAddTaskWindow();                 // Создает окно для добавления новой задачи

    void addTask(int id, const std::string& taskName, int section); // Добавляет визуальную задачу в указанную секцию
    void updateTaskPositions();                 // Пересчитывает позиции всех задач
    void updateTaskStatusInData(int taskId, int newStatus); // Обновляет статус задачи в данных при перемещении
    void saveTasksData();                       // Сохраняет все изменения задач в JSON файл
    void handleAddTaskInput(const sf::Event& event); // Обрабатывает ввод текста для новой задачи
    void confirmAddTask(int selectedSection);   // Подтверждает добавление новой задачи в выбранную секцию
};
#endif