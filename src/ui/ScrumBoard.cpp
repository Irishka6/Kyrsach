#include "ScrumBoard.h"
#include "../core/Tasks.h"
#include "Task.h"
#include <iostream>
#include <algorithm>

// Путь к JSON файлу в папке core
const std::string TASKS_JSON_PATH = "../core/tasks.json";

// Константы окна
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

ScrumBoard::ScrumBoard() {
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
    draggingTaskSection = -1;
    draggingTaskIndex = -1;
    showProjectWindow = false;  
    showAddTaskWindow = false;
    currentTaskInput = "";
    isTaskInputActive = false;
    
    projects = {"Проект 1", "Проект 2", "Проект 3", "Проект 4", "Проект 5"};  
    
    // Загружаем задачи из JSON в папке core
    tasksData = getTasksFromJson(TASKS_JSON_PATH);
    std::cout << "Загружено задач: " << tasksData.size() << std::endl;
}

// Инициализирует ресурсы и создает интерфейс
bool ScrumBoard::initialize() {
    if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        return false;
    }
    
    createTitle();
    createTopPanel();
    createSections();
    createSampleTasks();
    createProjectWindow(); 
    createAddTaskWindow();
    
    return true;
}

// Создает заголовок приложения
void ScrumBoard::createTitle() {
    titleText.setString("Scrum Board - Управление задачами");
    titleText.setFont(font);
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color(50, 50, 100));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(650, 110);
}

// Создает верхнюю панель с кнопками
void ScrumBoard::createTopPanel() {
    // Панель
    topPanel.setSize(sf::Vector2f(1920, 60));
    topPanel.setFillColor(sf::Color(120, 165, 205));
    topPanel.setOutlineThickness(0);
    topPanel.setPosition(0, 20);

    // Кнопка проектов
    projectButton.setSize(sf::Vector2f(200, 40));
    projectButton.setFillColor(sf::Color(180, 210, 235));
    projectButton.setOutlineThickness(0);
    projectButton.setPosition(150, 30);

    // Текст кнопки проектов
    projectButtonText.setString("Проекты"); 
    projectButtonText.setFont(font);
    projectButtonText.setCharacterSize(24);
    projectButtonText.setFillColor(sf::Color(50, 50, 80));
    projectButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста кнопки проектов
    sf::FloatRect projectTextBounds = projectButtonText.getLocalBounds(); 
    projectButtonText.setPosition(
        150 + (200 - projectTextBounds.width) / 2,
        30 + (40 - projectTextBounds.height) / 2 - 5
    );

    // Кнопка сохранения
    saveButton.setSize(sf::Vector2f(200, 40));
    saveButton.setFillColor(sf::Color(180, 210, 235));
    saveButton.setOutlineThickness(0);
    saveButton.setPosition(1570, 30);

    // Текст кнопки сохранения
    saveButtonText.setString("Сохранить");
    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(24);
    saveButtonText.setFillColor(sf::Color(50, 50, 80));
    saveButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста кнопки сохранения
    sf::FloatRect saveTextBounds = saveButtonText.getLocalBounds();
    saveButtonText.setPosition(
        1570 + (200 - saveTextBounds.width) / 2,
        30 + (40 - saveTextBounds.height) / 2 - 5
    );

    // Кнопка добавления новой задачи
    addButton.setSize(sf::Vector2f(200, 40));
    addButton.setFillColor(sf::Color(180, 210, 235));
    addButton.setOutlineThickness(0);
    addButton.setPosition(1350, 30);

    // Текст кнопки добавления
    addButtonText.setString("Добавить");
    addButtonText.setFont(font);
    addButtonText.setCharacterSize(24);
    addButtonText.setFillColor(sf::Color(50, 50, 80));
    addButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста кнопки добавления
    sf::FloatRect addTextBounds = addButtonText.getLocalBounds();
    addButtonText.setPosition(
        1350 + (200 - addTextBounds.width) / 2,
        30 + (40 - addTextBounds.height) / 2 - 5
    );
}

// Создает окно выбора проектов
void ScrumBoard::createProjectWindow() { 
    float projectHeight = 55.0f;
    float padding = 15.0f;
    
    float windowWidth = 380.0f;
    float windowHeight = projectHeight * projects.size() + padding * 2; 
    
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    projectWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    projectWindow.setFillColor(sf::Color(180, 210, 235));
    projectWindow.setOutlineColor(sf::Color(160, 190, 220));
    projectWindow.setOutlineThickness(3);
    projectWindow.setPosition(startX, startY);
    
    projectRects.clear(); 
    projectTexts.clear(); 
    
    float projectWidth = windowWidth - padding * 2; 
    
    for (size_t i = 0; i < projects.size(); i++) { 
        sf::RectangleShape projectRect; 
        projectRect.setSize(sf::Vector2f(projectWidth, projectHeight));
        projectRect.setFillColor(sf::Color(180, 210, 235));
        projectRect.setOutlineColor(sf::Color(100, 130, 160));
        projectRect.setOutlineThickness(3);
        projectRect.setPosition(startX + padding, startY + padding + i * projectHeight);
        projectRects.push_back(projectRect);
        
        sf::Text projectText;
        projectText.setString(projects[i]);
        projectText.setFont(font);
        projectText.setCharacterSize(24);
        projectText.setFillColor(sf::Color(50, 50, 80));
        projectText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = projectText.getLocalBounds();
        projectText.setPosition(
            startX + padding + (projectWidth - textBounds.width) / 2,
            startY + padding + i * projectHeight + (projectHeight - textBounds.height) / 2 - 3
        );
        projectTexts.push_back(projectText); 
    }
}

// Создает окно добавления новой задачи
void ScrumBoard::createAddTaskWindow() {
    float windowWidth = 500.0f;
    float windowHeight = 400.0f;
    float padding = 20.0f;
    
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно
    addTaskWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    addTaskWindow.setFillColor(sf::Color(180, 210, 235));
    addTaskWindow.setOutlineColor(sf::Color(160, 190, 220));
    addTaskWindow.setOutlineThickness(3);
    addTaskWindow.setPosition(startX, startY);
    
    // Поле ввода названия задачи
    taskInputField.setSize(sf::Vector2f(windowWidth - padding * 2, 50.0f));
    taskInputField.setFillColor(sf::Color::White);
    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
    taskInputField.setOutlineThickness(2);
    taskInputField.setPosition(startX + padding, startY + padding);
    
    // Текст в поле ввода
    taskInputText.setString("Введите задачу на английском");
    taskInputText.setFont(font);
    taskInputText.setCharacterSize(20);
    taskInputText.setFillColor(sf::Color(150, 150, 150));
    taskInputText.setPosition(startX + padding + 10, startY + padding + 15);
    
    // Текст "Выберите секцию:"
    sectionLabelText.setString("Выберите секцию:");
    sectionLabelText.setFont(font);
    sectionLabelText.setCharacterSize(22);
    sectionLabelText.setFillColor(sf::Color(50, 50, 80));
    sectionLabelText.setStyle(sf::Text::Bold);
    sectionLabelText.setPosition(startX + padding, startY + padding + 80);
    
    // Кнопки выбора секции
    float sectionButtonWidth = (windowWidth - padding * 3) / 2;
    float sectionButtonHeight = 50.0f;
    
    sectionOptionRects.clear();
    sectionOptionTexts.clear();
    
    for (int i = 0; i < 4; i++) {
        int row = i / 2;
        int col = i % 2;
        
        sf::RectangleShape sectionRect;
        sectionRect.setSize(sf::Vector2f(sectionButtonWidth, sectionButtonHeight));
        sectionRect.setFillColor(sf::Color(200, 220, 240));
        sectionRect.setOutlineColor(sf::Color(100, 130, 160));
        sectionRect.setOutlineThickness(2);
        sectionRect.setPosition(
            startX + padding + col * (sectionButtonWidth + padding),
            startY + padding + 120 + row * (sectionButtonHeight + padding)
        );
        sectionOptionRects.push_back(sectionRect);
        
        sf::Text sectionText;
        sectionText.setString(sectionNames[i]);
        sectionText.setFont(font);
        sectionText.setCharacterSize(18);
        sectionText.setFillColor(sf::Color(50, 50, 80));
        sectionText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = sectionText.getLocalBounds();
        sectionText.setPosition(
            startX + padding + col * (sectionButtonWidth + padding) + (sectionButtonWidth - textBounds.width) / 2,
            startY + padding + 120 + row * (sectionButtonHeight + padding) + (sectionButtonHeight - textBounds.height) / 2 - 3
        );
        sectionOptionTexts.push_back(sectionText);
    }
    
    // Кнопка подтверждения добавления
    confirmAddButton.setSize(sf::Vector2f(150, 40));
    confirmAddButton.setFillColor(sf::Color(120, 180, 120));
    confirmAddButton.setOutlineColor(sf::Color(80, 140, 80));
    confirmAddButton.setOutlineThickness(2);
    confirmAddButton.setPosition(startX + padding, startY + windowHeight - padding - 50);
    
    confirmAddButtonText.setString("Добавить");
    confirmAddButtonText.setFont(font);
    confirmAddButtonText.setCharacterSize(20);
    confirmAddButtonText.setFillColor(sf::Color::White);
    confirmAddButtonText.setStyle(sf::Text::Bold);
    
    sf::FloatRect confirmTextBounds = confirmAddButtonText.getLocalBounds();
    confirmAddButtonText.setPosition(
        startX + padding + (150 - confirmTextBounds.width) / 2,
        startY + windowHeight - padding - 50 + (40 - confirmTextBounds.height) / 2 - 3
    );
    
    // Кнопка отмены
    cancelAddButton.setSize(sf::Vector2f(150, 40));
    cancelAddButton.setFillColor(sf::Color(180, 120, 120));
    cancelAddButton.setOutlineColor(sf::Color(140, 80, 80));
    cancelAddButton.setOutlineThickness(2);
    cancelAddButton.setPosition(startX + windowWidth - padding - 150, startY + windowHeight - padding - 50);
    
    cancelAddButtonText.setString("Отмена");
    cancelAddButtonText.setFont(font);
    cancelAddButtonText.setCharacterSize(20);
    cancelAddButtonText.setFillColor(sf::Color::White);
    cancelAddButtonText.setStyle(sf::Text::Bold);
    
    sf::FloatRect cancelTextBounds = cancelAddButtonText.getLocalBounds();
    cancelAddButtonText.setPosition(
        startX + windowWidth - padding - 150 + (150 - cancelTextBounds.width) / 2,
        startY + windowHeight - padding - 50 + (40 - cancelTextBounds.height) / 2 - 3
    );
}

// Создает секции для задач
void ScrumBoard::createSections() {
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),
        sf::Color(160, 195, 225),
        sf::Color(140, 180, 215),
        sf::Color(120, 165, 205)
    };
    
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 700));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(0);
        section.setPosition(startX + i * (sectionWidth + spacing), 240);
        sections.push_back(section);
        
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 
            200
        );
        sectionTexts.push_back(text);
    }
}

// Создает визуальные задачи из загруженных данных JSON
void ScrumBoard::createSampleTasks() {
    // Очищаем визуальные задачи
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }

    std::cout << "Создание визуальных задач из данных..." << std::endl;
    
    // Создаем визуальные задачи из данных
    for (const auto& task : tasksData){
        addTask(task.getId(), task.getTitle(), task.getStatus());
    }
    updateTaskPositions();
}

// Добавляет задачу в указанную секцию
void ScrumBoard::addTask(int id, const std::string& taskName, int section) {
    if (section >= 0 && section < 4) {
        float totalWidth = 1820.0f;
        float sectionWidth = (totalWidth - 120.0f) / 4.0f;
        float spacing = 40.0f;
        float startX = 50.0f;
        float taskWidth = sectionWidth - 30.0f;
        
        float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
        float y = 270 + tasks[section].size() * 90;
        
        Task newTask(id, taskName, font, x, y);
        newTask.currentSection = section;
        newTask.shape.setSize(sf::Vector2f(taskWidth, 85));
        
        newTask.text.setCharacterSize(24);
        newTask.text.setStyle(sf::Text::Bold);
        
        std::string displayText = taskName;
        if (displayText.length() > 30) {
            displayText = displayText.substr(0, 27) + "...";
        }
        newTask.text.setString(displayText);
        
        // Центрируем текст в задаче
        sf::FloatRect textBounds = newTask.text.getLocalBounds();
        newTask.text.setPosition(
            x + (taskWidth - textBounds.width) / 2,
            y + (85 - textBounds.height) / 2 - 5
        );
        
        tasks[section].push_back(newTask);
    }
}

// Обновляет статус задачи в данных при перемещении между секциями
void ScrumBoard::updateTaskStatusInData(int taskId, int newStatus) {
    for (auto& task : tasksData) {
        if (task.getId() == taskId){
            int oldStatus = task.getStatus();
            task.changeStatus(newStatus);
        }
    }
}

// Сохраняет все задачи в JSON файл
void ScrumBoard::saveTasksData() {
    saveTasksToJson(tasksData, TASKS_JSON_PATH);
}

// Обновляет позиции всех задач на доске
void ScrumBoard::updateTaskPositions() {
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    float taskWidth = sectionWidth - 30.0f;
    
    for (int section = 0; section < 4; section++) {
        float startY = 270.0f;
        for (size_t i = 0; i < tasks[section].size(); i++) {
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 95;
            tasks[section][i].setPosition(x, y);
            tasks[section][i].shape.setSize(sf::Vector2f(taskWidth, 85));
            
            // Обновляем позицию текста с центрированием
            sf::FloatRect textBounds = tasks[section][i].text.getLocalBounds();
            tasks[section][i].text.setPosition(
                x + (taskWidth - textBounds.width) / 2,
                y + (85 - textBounds.height) / 2 - 5
            );
        }
    }
}

// Обрабатывает ввод текста для новой задачи
void ScrumBoard::handleAddTaskInput(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Обрабатываем только печатные символы
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            if (event.text.unicode == 8) {
                if (!currentTaskInput.empty()) {
                    currentTaskInput.pop_back();
                }
            }
            // Enter - добавляем задачу
            else if (event.text.unicode == 13) {
                if (!currentTaskInput.empty()) {
                    confirmAddTask(0);
                    return;
                }
            }
            // Обычные символы
            else if (c >= 32 && c <= 126) {
                currentTaskInput += c;
            }
            
            // Обновляем отображаемый текст
            if (currentTaskInput.empty()) {
                taskInputText.setString("");
                taskInputText.setFillColor(sf::Color(150, 150, 150));
            } else {
                taskInputText.setString(currentTaskInput);
                taskInputText.setFillColor(sf::Color(50, 50, 80));
            }
        }
    }
}

// Подтверждает добавление новой задачи в выбранную секцию
void ScrumBoard::confirmAddTask(int selectedSection) {
    if (!currentTaskInput.empty() && selectedSection >= 0 && selectedSection < 4) {
        // Генерируем новый ID (максимальный существующий ID + 1)
        int newId = 1;
        for (const auto& task : tasksData) {
            if (task.getId() >= newId) {
                newId = task.getId() + 1;
            }
        }
        
        Tasks newTask(newId, currentTaskInput, selectedSection);
        tasksData.push_back(newTask);
        
        // Добавляем визуальную задачу
        addTask(newId, currentTaskInput, selectedSection);
        
        // Сохраняем в JSON
        saveTasksData();
        
        // Сбрасываем состояние
        currentTaskInput = "";
        isTaskInputActive = false;
        taskInputText.setString("Введите задачу на английском");
        taskInputText.setFillColor(sf::Color(150, 150, 150));
        taskInputField.setOutlineColor(sf::Color(100, 130, 160));
        showAddTaskWindow = false;
        
        std::cout << "Добавлена новая задача: '" << newTask.getTitle() 
                  << "' в секцию " << selectedSection << std::endl;
    }
}

// Обрабатывает все события ввода (мышь и клавиатура)
void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Обработка ввода текста для новой задачи
    if (showAddTaskWindow && isTaskInputActive) {
        handleAddTaskInput(event);
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            // Обработка клика по кнопке проектов
            if (projectButton.getGlobalBounds().contains(mousePos)) {
                showProjectWindow = !showProjectWindow; 
                return;
            }
            
            // Обработка клика по кнопке добавления задачи
            if (addButton.getGlobalBounds().contains(mousePos)) {
                showAddTaskWindow = !showAddTaskWindow;
                if (showAddTaskWindow) {
                    currentTaskInput = "";
                    taskInputText.setString("Введите задачу на английском");
                    taskInputText.setFillColor(sf::Color(150, 150, 150));
                    isTaskInputActive = false;
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                }
                return;
            }
            
            // Обработка клика по кнопке сохранения
            if (saveButton.getGlobalBounds().contains(mousePos)) {
                saveTasksData();
                std::cout << "Данные сохранены!" << std::endl;
                return;
            }
            
            // Обработка выбора проекта
            if (showProjectWindow) { 
                for (size_t i = 0; i < projectRects.size(); i++) {
                    if (projectRects[i].getGlobalBounds().contains(mousePos)) {
                        std::cout << "Выбран проект: " << projects[i] << std::endl;
                        showProjectWindow = false;
                        return;
                    }
                }
                if (!projectWindow.getGlobalBounds().contains(mousePos)) {
                    showProjectWindow = false;
                }
            }
            
            // Обработка окна добавления задачи
            if (showAddTaskWindow) {
                // Проверяем клик по полю ввода
                if (taskInputField.getGlobalBounds().contains(mousePos)) {
                    isTaskInputActive = true;
                    taskInputField.setOutlineColor(sf::Color(50, 100, 200));
                    if (currentTaskInput.empty()) {
                        taskInputText.setString("");
                        taskInputText.setFillColor(sf::Color(50, 50, 80));
                    }
                    return;
                }
                
                // Проверяем клик по кнопкам секций
                for (int i = 0; i < 4; i++) {
                    if (sectionOptionRects[i].getGlobalBounds().contains(mousePos)) {
                        confirmAddTask(i);
                        return;
                    }
                }
                
                // Проверяем клик по кнопке подтверждения
                if (confirmAddButton.getGlobalBounds().contains(mousePos)) {
                    confirmAddTask(0);
                    return;
                }
                
                // Проверяем клик по кнопке отмены
                if (cancelAddButton.getGlobalBounds().contains(mousePos)) {
                    showAddTaskWindow = false;
                    isTaskInputActive = false;
                    currentTaskInput = "";
                    taskInputText.setString("Введите задачу на английском");
                    taskInputText.setFillColor(sf::Color(150, 150, 150));
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                    return;
                }
                
                // Если клик вне поля ввода
                if (!taskInputField.getGlobalBounds().contains(mousePos)) {
                    isTaskInputActive = false;
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                    if (currentTaskInput.empty()) {
                        taskInputText.setString("Введите задачу на английском");
                        taskInputText.setFillColor(sf::Color(150, 150, 150));
                    }
                }
                
                // Если клик вне окна добавления - закрываем его
                if (!addTaskWindow.getGlobalBounds().contains(mousePos)) {
                    showAddTaskWindow = false;
                    isTaskInputActive = false;
                    currentTaskInput = "";
                    taskInputText.setString("Введите задачу на английском");
                    taskInputText.setFillColor(sf::Color(150, 150, 150));
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                }
                return;
            }
            
            // Обработка начала перетаскивания задачи
            for (int i = 0; i < 4; i++) {
                for (size_t j = 0; j < tasks[i].size(); j++) {
                    if (tasks[i][j].shape.getGlobalBounds().contains(mousePos)) {
                        tasks[i][j].isMoving = true;
                        draggingTaskSection = i;
                        draggingTaskIndex = j;
                        return;
                    }
                }
            }
        }
    }
    
    // Обработка отпускания кнопки мыши
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Обработка завершения перетаскивания задачи
            if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
                Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
                draggedTask.isMoving = false;
                
                sf::FloatRect taskBounds = draggedTask.shape.getGlobalBounds();
                sf::Vector2f taskCenter(
                    taskBounds.left + taskBounds.width / 2,
                    taskBounds.top + taskBounds.height / 2
                );
                
                // Определяем новую секцию для задачи
                for (int newSection = 0; newSection < 4; newSection++) {
                    if (sections[newSection].getGlobalBounds().contains(taskCenter)) {
                        if (newSection != draggingTaskSection) {
                            // Перемещаем задачу в новую секцию
                            Task movedTask = draggedTask;
                            movedTask.currentSection = newSection;
                            movedTask.isMoving = false;
                            tasks[newSection].push_back(movedTask);
                            updateTaskStatusInData(tasks[draggingTaskSection][draggingTaskIndex].id, newSection);
                            
                            // Удаляем из старой секции
                            tasks[draggingTaskSection].erase(tasks[draggingTaskSection].begin() + draggingTaskIndex);
                            
                            // Сохраняем изменения
                            saveTasksData();
                            
                            std::cout << "Задача перемещена в секцию " << newSection << std::endl;
                            
                            updateTaskPositions();
                            break;
                        } else {
                            updateTaskPositions();
                        }
                    }
                }
                
                draggingTaskSection = -1;
                draggingTaskIndex = -1;
            }
        }
    }
    
    // Обработка движения мыши
    if (event.type == sf::Event::MouseMoved) {
        // Перетаскивание задачи
        if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
            Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
            if (draggedTask.isMoving) {
                draggedTask.setPosition(event.mouseMove.x - 190, event.mouseMove.y - 40);
            }
        }
    }
}

void ScrumBoard::update(float deltaTime) {
}

// Отрисовывает все компоненты доски
void ScrumBoard::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    
    window.draw(topPanel);
    window.draw(projectButton);      
    window.draw(projectButtonText);  
    window.draw(saveButton);
    window.draw(saveButtonText);
    window.draw(addButton);
    window.draw(addButtonText);
    
    // Рисуем секции
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    // Рисуем заголовки секций
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    // Рисуем задачи
    for (int i = 0; i < 4; i++) {
        sf::FloatRect sectionBounds = sections[i].getGlobalBounds();
        
        for (const auto& task : tasks[i]) {
            sf::FloatRect taskBounds = task.shape.getGlobalBounds();
            
            if (taskBounds.top + taskBounds.height >= sectionBounds.top && 
                taskBounds.top <= sectionBounds.top + sectionBounds.height) {
                window.draw(task.shape);
                window.draw(task.text);
            }
        }
    }
    
    // Рисуем окно проектов
    if (showProjectWindow) { 
        // Затемняем фон
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(projectWindow); 
        
        // Рисуем прямоугольники проектов
        for (const auto& rect : projectRects) {  
            window.draw(rect);
        }
        
        // Рисуем текст проектов
        for (const auto& text : projectTexts) {  
            window.draw(text);
        }
    }
    
    // Рисуем окно добавления задачи
    if (showAddTaskWindow) {
        // Затемняем фон
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(addTaskWindow);
        window.draw(taskInputField);
        window.draw(taskInputText);
        window.draw(sectionLabelText);
        
        // Рисуем кнопки выбора секции
        for (const auto& rect : sectionOptionRects) {
            window.draw(rect);
        }
        for (const auto& text : sectionOptionTexts) {
            window.draw(text);
        }
        
        // Рисуем кнопки подтверждения и отмены
        window.draw(confirmAddButton);
        window.draw(confirmAddButtonText);
        window.draw(cancelAddButton);
        window.draw(cancelAddButtonText);
    }
}