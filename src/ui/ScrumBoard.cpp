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

// Конструктор класса ScrumBoard - инициализирует все переменные
ScrumBoard::ScrumBoard() {
    // Названия секций (колонок) доски
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    
    // Инициализация вектора задач (4 секции)
    tasks.resize(4);
    
    // Переменные для перетаскивания задач
    draggingTaskSection = -1;  // Индекс секции перетаскиваемой задачи
    draggingTaskIndex = -1;    // Индекс перетаскиваемой задачи в секции
    
    // Флаги отображения окон
    showProjectWindow = false;   // Окно выбора проекта
    showAddTaskWindow = false;   // Окно добавления задачи
    showEditMode = false;        // Режим редактирования
    
    // Переменные для ввода текста
    currentTaskInput = "";       // Текст новой задачи
    currentEditTaskInput = "";   // Текст редактируемой задачи
    
    // Флаги активности полей ввода
    isTaskInputActive = false;    // Поле ввода новой задачи активно
    isEditTaskInputActive = false; // Поле редактирования задачи активно
    
    // Переменные для редактирования задачи
    editingTaskSection = -1;     // Секция редактируемой задачи
    editingTaskIndex = -1;       // Индекс редактируемой задачи в секции
    
    // Список проектов
    projects = {"Проект 1", "Проект 2", "Проект 3", "Проект 4", "Проект 5"};  
    
    // Загружаем задачи из JSON файла
    tasksData = getTasksFromJson(TASKS_JSON_PATH);
    std::cout << "Загружено задач: " << tasksData.size() << std::endl;
}

// Инициализация ресурсов и создание интерфейса
bool ScrumBoard::initialize() {
    // Загрузка шрифта
    if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        return false;
    }
    
    // Создание всех компонентов интерфейса
    createTitle();           // Заголовок приложения
    createTopPanel();        // Верхняя панель с кнопками
    createSections();        // 4 секции для задач
    createSampleTasks();     // Загрузка задач из JSON
    createProjectWindow();   // Окно выбора проекта
    createAddTaskWindow();   // Окно добавления задачи
    createEditModeWindow();  // Окно редактирования задач
    
    return true;
}

// Создание заголовка приложения
void ScrumBoard::createTitle() {
    titleText.setString("Scrum Board - Управление задачами");
    titleText.setFont(font);
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color(50, 50, 100));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(650, 110);
}

// Создание верхней панели с кнопками управления
void ScrumBoard::createTopPanel() {
    // Основная панель
    topPanel.setSize(sf::Vector2f(1920, 60));
    topPanel.setFillColor(sf::Color(120, 165, 205));
    topPanel.setOutlineThickness(0);
    topPanel.setPosition(0, 20);

    // Кнопка "Проекты"
    projectButton.setSize(sf::Vector2f(200, 40));
    projectButton.setFillColor(sf::Color(180, 210, 235));
    projectButton.setOutlineThickness(0);
    projectButton.setPosition(150, 30);

    projectButtonText.setString("Проекты"); 
    projectButtonText.setFont(font);
    projectButtonText.setCharacterSize(24);
    projectButtonText.setFillColor(sf::Color(50, 50, 80));
    projectButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    sf::FloatRect projectTextBounds = projectButtonText.getLocalBounds(); 
    projectButtonText.setPosition(
        150 + (200 - projectTextBounds.width) / 2,
        30 + (40 - projectTextBounds.height) / 2 - 5
    );

    // Кнопка "Изменить" (открывает режим редактирования)
    editButton.setSize(sf::Vector2f(200, 40));
    editButton.setFillColor(sf::Color(180, 210, 235));
    editButton.setOutlineThickness(0);
    editButton.setPosition(1570, 30);

    editButtonText.setString("Редактировать");
    editButtonText.setFont(font);
    editButtonText.setCharacterSize(24);
    editButtonText.setFillColor(sf::Color(50, 50, 80));
    editButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    sf::FloatRect editTextBounds = editButtonText.getLocalBounds();
    editButtonText.setPosition(
        1570 + (200 - editTextBounds.width) / 2,
        30 + (40 - editTextBounds.height) / 2 - 5
    );

    // Кнопка "Добавить" (открывает окно добавления задачи)
    addButton.setSize(sf::Vector2f(200, 40));
    addButton.setFillColor(sf::Color(180, 210, 235));
    addButton.setOutlineThickness(0);
    addButton.setPosition(1350, 30);

    addButtonText.setString("Добавить");
    addButtonText.setFont(font);
    addButtonText.setCharacterSize(24);
    addButtonText.setFillColor(sf::Color(50, 50, 80));
    addButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    sf::FloatRect addTextBounds = addButtonText.getLocalBounds();
    addButtonText.setPosition(
        1350 + (200 - addTextBounds.width) / 2,
        30 + (40 - addTextBounds.height) / 2 - 5
    );
}

// Создание окна выбора проекта
void ScrumBoard::createProjectWindow() { 
    float projectHeight = 55.0f;    // Высота кнопки проекта
    float padding = 15.0f;          // Отступы
    
    // Расчет размеров окна
    float windowWidth = 380.0f;
    float windowHeight = projectHeight * projects.size() + padding * 2; 
    
    // Позиционирование по центру экрана
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно
    projectWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    projectWindow.setFillColor(sf::Color(180, 210, 235));
    projectWindow.setOutlineColor(sf::Color(160, 190, 220));
    projectWindow.setOutlineThickness(3);
    projectWindow.setPosition(startX, startY);
    
    // Очистка предыдущих элементов
    projectRects.clear(); 
    projectTexts.clear(); 
    
    float projectWidth = windowWidth - padding * 2; 
    
    // Создание кнопок для каждого проекта
    for (size_t i = 0; i < projects.size(); i++) { 
        // Прямоугольник кнопки проекта
        sf::RectangleShape projectRect; 
        projectRect.setSize(sf::Vector2f(projectWidth, projectHeight));
        projectRect.setFillColor(sf::Color(180, 210, 235));
        projectRect.setOutlineColor(sf::Color(100, 130, 160));
        projectRect.setOutlineThickness(3);
        projectRect.setPosition(startX + padding, startY + padding + i * projectHeight);
        projectRects.push_back(projectRect);
        
        // Текст проекта
        sf::Text projectText;
        projectText.setString(projects[i]);
        projectText.setFont(font);
        projectText.setCharacterSize(24);
        projectText.setFillColor(sf::Color(50, 50, 80));
        projectText.setStyle(sf::Text::Bold);
        
        // Центрирование текста
        sf::FloatRect textBounds = projectText.getLocalBounds();
        projectText.setPosition(
            startX + padding + (projectWidth - textBounds.width) / 2,
            startY + padding + i * projectHeight + (projectHeight - textBounds.height) / 2 - 3
        );
        projectTexts.push_back(projectText); 
    }
}

// Создание окна добавления новой задачи
void ScrumBoard::createAddTaskWindow() {
    float windowWidth = 500.0f;     // Ширина окна
    float windowHeight = 400.0f;    // Высота окна
    float padding = 20.0f;          // Отступы
    
    // Позиционирование по центру экрана
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
    
    // Подсказка в поле ввода
    taskInputText.setString("Введите задачу на английском");
    taskInputText.setFont(font);
    taskInputText.setCharacterSize(20);
    taskInputText.setFillColor(sf::Color(150, 150, 150));
    taskInputText.setPosition(startX + padding + 10, startY + padding + 15);
    
    // Метка выбора секции
    sectionLabelText.setString("Выберите секцию:");
    sectionLabelText.setFont(font);
    sectionLabelText.setCharacterSize(22);
    sectionLabelText.setFillColor(sf::Color(50, 50, 80));
    sectionLabelText.setStyle(sf::Text::Bold);
    sectionLabelText.setPosition(startX + padding, startY + padding + 80);
    
    // Расчет размеров кнопок секций
    float sectionButtonWidth = (windowWidth - padding * 3) / 2;
    float sectionButtonHeight = 50.0f;
    
    // Очистка предыдущих элементов
    sectionOptionRects.clear();
    sectionOptionTexts.clear();
    
    // Создание 4 кнопок для выбора секции (2x2)
    for (int i = 0; i < 4; i++) {
        int row = i / 2;  // Номер ряда (0 или 1)
        int col = i % 2;  // Номер колонки (0 или 1)
        
        // Прямоугольник кнопки секции
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
        
        // Текст секции
        sf::Text sectionText;
        sectionText.setString(sectionNames[i]);
        sectionText.setFont(font);
        sectionText.setCharacterSize(18);
        sectionText.setFillColor(sf::Color(50, 50, 80));
        sectionText.setStyle(sf::Text::Bold);
        
        // Центрирование текста
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
    
    // Центрирование текста кнопки
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
    
    // Центрирование текста кнопки
    sf::FloatRect cancelTextBounds = cancelAddButtonText.getLocalBounds();
    cancelAddButtonText.setPosition(
        startX + windowWidth - padding - 150 + (150 - cancelTextBounds.width) / 2,
        startY + windowHeight - padding - 50 + (40 - cancelTextBounds.height) / 2 - 3
    );
}

// Создание окна редактирования задач
void ScrumBoard::createEditModeWindow() {
    float windowWidth = 1800.0f;    // Ширина окна (почти на весь экран)
    float windowHeight = 900.0f;    // Высота окна
    float padding = 20.0f;          // Отступы
    
    // Позиционирование по центру экрана
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно редактирования
    editModeWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    editModeWindow.setFillColor(sf::Color(180, 210, 235));
    editModeWindow.setOutlineColor(sf::Color(160, 190, 220));
    editModeWindow.setOutlineThickness(3);
    editModeWindow.setPosition(startX, startY);
    
    // Заголовок окна редактирования
    editTitleText.setString("Выберите задачу для редактирования");
    editTitleText.setFont(font);
    editTitleText.setCharacterSize(32);
    editTitleText.setFillColor(sf::Color(50, 50, 80));
    editTitleText.setStyle(sf::Text::Bold);
    editTitleText.setPosition(startX + padding, startY + padding);
    
    // Очистка предыдущих элементов
    editSections.clear();
    editSectionTexts.clear();
    
    // Расчет размеров секций внутри окна редактирования
    float sectionWidth = (windowWidth - padding * 5) / 4.0f;
    float sectionStartX = startX + padding;
    float sectionStartY = startY + 80;
    
    // Цвета для секций
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),  // Назначено
        sf::Color(160, 195, 225),  // В процессе
        sf::Color(140, 180, 215),  // Блокировано
        sf::Color(120, 165, 205)   // Готово
    };
    
    // Создание 4 секций внутри окна редактирования
    for (int i = 0; i < 4; i++) {
        // Прямоугольник секции
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 600));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(2);
        section.setOutlineColor(sf::Color(100, 130, 160));
        section.setPosition(sectionStartX + i * (sectionWidth + padding), sectionStartY);
        editSections.push_back(section);
        
        // Текст названия секции
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        // Центрирование текста над секцией
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            sectionStartX + i * (sectionWidth + padding) + (sectionWidth - textBounds.width) / 2,
            sectionStartY - 40
        );
        editSectionTexts.push_back(text);
    }
    
    // Поле ввода для редактирования названия задачи
    editTaskInputField.setSize(sf::Vector2f(600, 50));
    editTaskInputField.setFillColor(sf::Color::White);
    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
    editTaskInputField.setOutlineThickness(2);
    editTaskInputField.setPosition(startX + padding, startY + 700);
    
    // Подсказка в поле ввода
    editTaskInputText.setString("Выберите задачу для редактирования");
    editTaskInputText.setFont(font);
    editTaskInputText.setCharacterSize(20);
    editTaskInputText.setFillColor(sf::Color(150, 150, 150));
    editTaskInputText.setPosition(startX + padding + 10, startY + 715);
    
    // Кнопка сохранения изменений
    saveEditButton.setSize(sf::Vector2f(150, 40));
    saveEditButton.setFillColor(sf::Color(120, 180, 120));
    saveEditButton.setOutlineColor(sf::Color(80, 140, 80));
    saveEditButton.setOutlineThickness(2);
    saveEditButton.setPosition(startX + 650, startY + 700);
    
    saveEditButtonText.setString("Сохранить");
    saveEditButtonText.setFont(font);
    saveEditButtonText.setCharacterSize(20);
    saveEditButtonText.setFillColor(sf::Color::White);
    saveEditButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка удаления задачи
    deleteTaskButton.setSize(sf::Vector2f(150, 40));
    deleteTaskButton.setFillColor(sf::Color(200, 100, 100));
    deleteTaskButton.setOutlineColor(sf::Color(160, 60, 60));
    deleteTaskButton.setOutlineThickness(2);
    deleteTaskButton.setPosition(startX + 820, startY + 700);
    
    deleteTaskButtonText.setString("Удалить");
    deleteTaskButtonText.setFont(font);
    deleteTaskButtonText.setCharacterSize(20);
    deleteTaskButtonText.setFillColor(sf::Color::White);
    deleteTaskButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка закрытия окна редактирования
    cancelEditButton.setSize(sf::Vector2f(150, 40));
    cancelEditButton.setFillColor(sf::Color(180, 120, 120));
    cancelEditButton.setOutlineColor(sf::Color(140, 80, 80));
    cancelEditButton.setOutlineThickness(2);
    cancelEditButton.setPosition(startX + 990, startY + 700);
    
    cancelEditButtonText.setString("Закрыть");
    cancelEditButtonText.setFont(font);
    cancelEditButtonText.setCharacterSize(20);
    cancelEditButtonText.setFillColor(sf::Color::White);
    cancelEditButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопках
    centerTextInButton(saveEditButtonText, saveEditButton);
    centerTextInButton(deleteTaskButtonText, deleteTaskButton);
    centerTextInButton(cancelEditButtonText, cancelEditButton);
}

// Вспомогательная функция для центрирования текста в кнопке
void ScrumBoard::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    text.setPosition(
        buttonBounds.left + (buttonBounds.width - textBounds.width) / 2,
        buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - 5
    );
}

// Создание 4 секций основной доски
void ScrumBoard::createSections() {
    // Цвета для секций
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),  // Назначено
        sf::Color(160, 195, 225),  // В процессе
        sf::Color(140, 180, 215),  // Блокировано
        sf::Color(120, 165, 205)   // Готово
    };
    
    // Расчет размеров и позиций секций
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    
    // Создание 4 секций
    for (int i = 0; i < 4; i++) {
        // Прямоугольник секции
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 700));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(0);
        section.setPosition(startX + i * (sectionWidth + spacing), 240);
        sections.push_back(section);
        
        // Текст названия секции
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        // Центрирование текста над секцией
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 
            200
        );
        sectionTexts.push_back(text);
    }
}

// Создание визуальных задач из загруженных данных JSON
void ScrumBoard::createSampleTasks() {
    // Очистка существующих задач
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    // Добавление задач из JSON данных
    for (const auto& task : tasksData){
        addTask(task.getId(), task.getTitle(), task.getStatus());
    }
    
    // Обновление позиций задач
    updateTaskPositions();
}

// Добавление визуальной задачи в указанную секцию
void ScrumBoard::addTask(int id, const std::string& taskName, int section) {
    // Проверка корректности номера секции
    if (section >= 0 && section < 4) {
        // Расчет размеров и позиции
        float totalWidth = 1820.0f;
        float sectionWidth = (totalWidth - 120.0f) / 4.0f;
        float spacing = 40.0f;
        float startX = 50.0f;
        float taskWidth = sectionWidth - 30.0f;  // Ширина задачи с отступами
        
        // Позиция задачи в секции
        float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
        float y = 270 + tasks[section].size() * 90;  // Позиция по Y зависит от количества задач в секции
        
        // Создание новой задачи
        Task newTask(id, taskName, font, x, y);
        newTask.currentSection = section;
        newTask.shape.setSize(sf::Vector2f(taskWidth, 85));
        
        // Настройка текста задачи
        newTask.text.setCharacterSize(24);
        newTask.text.setStyle(sf::Text::Bold);
        
        // Обрезка длинного текста с добавлением многоточия
        std::string displayText = taskName;
        if (displayText.length() > 30) {
            displayText = displayText.substr(0, 27) + "...";
        }
        newTask.text.setString(displayText);
        
        // Центрирование текста в задаче
        sf::FloatRect textBounds = newTask.text.getLocalBounds();
        newTask.text.setPosition(
            x + (taskWidth - textBounds.width) / 2,
            y + (85 - textBounds.height) / 2 - 5
        );
        
        // Добавление задачи в соответствующую секцию
        tasks[section].push_back(newTask);
    }
}

// Обновление статуса задачи в данных при перемещении между секциями
void ScrumBoard::updateTaskStatusInData(int taskId, int newStatus) {
    // Поиск задачи по ID и обновление ее статуса
    for (auto& task : tasksData) {
        if (task.getId() == taskId){
            task.changeStatus(newStatus);
        }
    }
}

// Сохранение всех задач в JSON файл
void ScrumBoard::saveTasksData() {
    saveTasksToJson(tasksData, TASKS_JSON_PATH);
}

// Обновление позиций всех задач на доске
void ScrumBoard::updateTaskPositions() {
    // Расчет размеров
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    float taskWidth = sectionWidth - 30.0f;
    
    // Обновление позиций для всех секций и всех задач
    for (int section = 0; section < 4; section++) {
        float startY = 270.0f;  // Начальная позиция Y для первой задачи в секции
        
        for (size_t i = 0; i < tasks[section].size(); i++) {
            // Расчет позиции задачи
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 95;  // Каждая следующая задача на 95 пикселей ниже
            
            // Установка позиции задачи
            tasks[section][i].setPosition(x, y);
            tasks[section][i].shape.setSize(sf::Vector2f(taskWidth, 85));
            
            // Центрирование текста в задаче
            sf::FloatRect textBounds = tasks[section][i].text.getLocalBounds();
            tasks[section][i].text.setPosition(
                x + (taskWidth - textBounds.width) / 2,
                y + (85 - textBounds.height) / 2 - 5
            );
        }
    }
}

// Обработка ввода текста для новой задачи
void ScrumBoard::handleAddTaskInput(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Обработка только ASCII символов
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            // Обработка Backspace
            if (event.text.unicode == 8) {
                if (!currentTaskInput.empty()) {
                    currentTaskInput.pop_back();
                }
            }
            // Обработка Enter (добавление задачи)
            else if (event.text.unicode == 13) {
                if (!currentTaskInput.empty()) {
                    confirmAddTask(0);  // Добавление в первую секцию по умолчанию
                    return;
                }
            }
            // Обработка печатных символов
            else if (c >= 32 && c <= 126) {
                currentTaskInput += c;
            }
            
            // Обновление отображаемого текста
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

// Обработка ввода текста при редактировании задачи
void ScrumBoard::handleEditTaskInput(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Обработка только ASCII символов
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            // Обработка Backspace
            if (event.text.unicode == 8) {
                if (!currentEditTaskInput.empty()) {
                    currentEditTaskInput.pop_back();
                }
            }
            // Обработка Enter (сохранение изменений)
            else if (event.text.unicode == 13) {
                saveEditedTask();
                return;
            }
            // Обработка печатных символов
            else if (c >= 32 && c <= 126) {
                currentEditTaskInput += c;
            }
            
            // Обновление отображаемого текста
            if (currentEditTaskInput.empty()) {
                editTaskInputText.setString("");
                editTaskInputText.setFillColor(sf::Color(150, 150, 150));
            } else {
                editTaskInputText.setString(currentEditTaskInput);
                editTaskInputText.setFillColor(sf::Color(50, 50, 80));
            }
        }
    }
}

// Подтверждение добавления новой задачи в выбранную секцию
void ScrumBoard::confirmAddTask(int selectedSection) {
    // Проверка наличия текста задачи и корректности секции
    if (!currentTaskInput.empty() && selectedSection >= 0 && selectedSection < 4) {
        // Генерация нового ID (максимальный существующий + 1)
        int newId = 1;
        for (const auto& task : tasksData) {
            if (task.getId() >= newId) {
                newId = task.getId() + 1;
            }
        }
        
        // Создание новой задачи
        Tasks newTask(newId, currentTaskInput, selectedSection);
        tasksData.push_back(newTask);
        
        // Добавление визуальной задачи
        addTask(newId, currentTaskInput, selectedSection);
        
        // Сохранение в JSON файл
        saveTasksData();
        
        // Сброс состояния
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

// Открытие режима редактирования
void ScrumBoard::openEditMode() {
    showEditMode = true;
    currentEditTaskInput = "";
    editTaskInputText.setString("Выберите задачу для редактирования");
    editTaskInputText.setFillColor(sf::Color(150, 150, 150));
    isEditTaskInputActive = false;
    editingTaskSection = -1;
    editingTaskIndex = -1;
    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
}

// Выбор задачи для редактирования
void ScrumBoard::selectTaskForEditing(int section, int index) {
    // Проверка корректности индексов
    if (section >= 0 && section < 4 && index >= 0 && index < tasks[section].size()) {
        editingTaskSection = section;
        editingTaskIndex = index;
        
        // Получение выбранной задачи
        Task& task = tasks[section][index];
        
        // Поиск полного текста задачи в данных
        for (const auto& taskData : tasksData) {
            if (taskData.getId() == task.id) {
                currentEditTaskInput = taskData.getTitle();
                break;
            }
        }
        
        // Обновление поля ввода
        editTaskInputText.setString(currentEditTaskInput);
        editTaskInputText.setFillColor(sf::Color(50, 50, 80));
        isEditTaskInputActive = true;
        editTaskInputField.setOutlineColor(sf::Color(50, 100, 200));
        
        std::cout << "Выбрана задача для редактирования: '" << currentEditTaskInput << "'" << std::endl;
    }
}

// Сохранение изменений задачи
void ScrumBoard::saveEditedTask() {
    // Проверка наличия текста и выбранной задачи
    if (!currentEditTaskInput.empty() && editingTaskSection != -1 && editingTaskIndex != -1) {
        Task& task = tasks[editingTaskSection][editingTaskIndex];
        
        // Обновление данных задачи
        for (auto& taskData : tasksData) {
            if (taskData.getId() == task.id) {
                taskData.setTitle(currentEditTaskInput);
                break;
            }
        }
        
        // Обновление визуального представления
        std::string displayText = currentEditTaskInput;
        if (displayText.length() > 30) {
            displayText = displayText.substr(0, 27) + "...";
        }
        task.text.setString(displayText);
        
        // Центрирование текста
        sf::FloatRect textBounds = task.text.getLocalBounds();
        sf::FloatRect taskBounds = task.shape.getGlobalBounds();
        task.text.setPosition(
            taskBounds.left + (taskBounds.width - textBounds.width) / 2,
            taskBounds.top + (taskBounds.height - textBounds.height) / 2 - 5
        );
        
        // Сохранение в JSON
        saveTasksData();
        
        // Сброс состояния редактирования
        editingTaskSection = -1;
        editingTaskIndex = -1;
        currentEditTaskInput = "";
        editTaskInputText.setString("");
        editTaskInputText.setFillColor(sf::Color(150, 150, 150));
        isEditTaskInputActive = false;
        editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
        
        std::cout << "Задача обновлена: '" << currentEditTaskInput << "'" << std::endl;
    }
}

// Удаление текущей задачи
void ScrumBoard::deleteCurrentTask() {
    // Проверка наличия выбранной задачи
    if (editingTaskSection != -1 && editingTaskIndex != -1) {
        Task& task = tasks[editingTaskSection][editingTaskIndex];
        int taskId = task.id;
        
        // Удаление из визуального представления
        tasks[editingTaskSection].erase(tasks[editingTaskSection].begin() + editingTaskIndex);
        
        // Удаление из данных
        for (auto it = tasksData.begin(); it != tasksData.end(); ++it) {
            if (it->getId() == taskId) {
                tasksData.erase(it);
                break;
            }
        }
        
        // Сохранение в JSON
        saveTasksData();
        
        // Обновление позиций оставшихся задач
        updateTaskPositions();
        
        // Сброс состояния редактирования
        editingTaskSection = -1;
        editingTaskIndex = -1;
        currentEditTaskInput = "";
        editTaskInputText.setString("");
        editTaskInputText.setFillColor(sf::Color(150, 150, 150));
        isEditTaskInputActive = false;
        editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
        
        std::cout << "Задача удалена: ID " << taskId << std::endl;
    }
}

// Закрытие режима редактирования
void ScrumBoard::closeEditMode() {
    showEditMode = false;
    isEditTaskInputActive = false;
    currentEditTaskInput = "";
    editingTaskSection = -1;
    editingTaskIndex = -1;
    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
}

// Обработка всех событий ввода
void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Обработка ввода текста для новой задачи
    if (showAddTaskWindow && isTaskInputActive) {
        handleAddTaskInput(event);
    }
    
    // Обработка ввода текста при редактировании
    if (showEditMode && isEditTaskInputActive) {
        handleEditTaskInput(event);
    }
    
    // Обработка нажатия кнопки мыши
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
            
            // Обработка клика по кнопке изменения
            if (editButton.getGlobalBounds().contains(mousePos)) {
                openEditMode();
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
                // Закрытие окна проектов при клике вне его
                if (!projectWindow.getGlobalBounds().contains(mousePos)) {
                    showProjectWindow = false;
                }
            }
            
            // Обработка окна добавления задачи
            if (showAddTaskWindow) {
                // Клик по полю ввода
                if (taskInputField.getGlobalBounds().contains(mousePos)) {
                    isTaskInputActive = true;
                    taskInputField.setOutlineColor(sf::Color(50, 100, 200));
                    if (currentTaskInput.empty()) {
                        taskInputText.setString("");
                        taskInputText.setFillColor(sf::Color(50, 50, 80));
                    }
                    return;
                }
                
                // Клик по кнопкам выбора секции
                for (int i = 0; i < 4; i++) {
                    if (sectionOptionRects[i].getGlobalBounds().contains(mousePos)) {
                        confirmAddTask(i);
                        return;
                    }
                }
                
                // Клик по кнопке подтверждения
                if (confirmAddButton.getGlobalBounds().contains(mousePos)) {
                    confirmAddTask(0);
                    return;
                }
                
                // Клик по кнопке отмены
                if (cancelAddButton.getGlobalBounds().contains(mousePos)) {
                    showAddTaskWindow = false;
                    isTaskInputActive = false;
                    currentTaskInput = "";
                    taskInputText.setString("Введите задачу на английском");
                    taskInputText.setFillColor(sf::Color(150, 150, 150));
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                    return;
                }
                
                // Деактивация поля ввода при клике вне его
                if (!taskInputField.getGlobalBounds().contains(mousePos)) {
                    isTaskInputActive = false;
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                    if (currentTaskInput.empty()) {
                        taskInputText.setString("Введите задачу на английском");
                        taskInputText.setFillColor(sf::Color(150, 150, 150));
                    }
                }
                
                // Закрытие окна при клике вне его
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
            
            // Обработка режима редактирования
            if (showEditMode) {
                // Клик по полю ввода редактирования
                if (editTaskInputField.getGlobalBounds().contains(mousePos)) {
                    isEditTaskInputActive = true;
                    editTaskInputField.setOutlineColor(sf::Color(50, 100, 200));
                    return;
                }
                
                // Клик по кнопке сохранения
                if (saveEditButton.getGlobalBounds().contains(mousePos)) {
                    saveEditedTask();
                    return;
                }
                
                // Клик по кнопке удаления
                if (deleteTaskButton.getGlobalBounds().contains(mousePos)) {
                    deleteCurrentTask();
                    return;
                }
                
                // Клик по кнопке отмены
                if (cancelEditButton.getGlobalBounds().contains(mousePos)) {
                    closeEditMode();
                    return;
                }
                
                // Выбор задачи для редактирования
                for (int i = 0; i < 4; i++) {
                    for (size_t j = 0; j < tasks[i].size(); j++) {
                        if (tasks[i][j].shape.getGlobalBounds().contains(mousePos)) {
                            selectTaskForEditing(i, j);
                            return;
                        }
                    }
                }
                
                // Деактивация поля ввода при клике вне его
                if (!editTaskInputField.getGlobalBounds().contains(mousePos)) {
                    isEditTaskInputActive = false;
                    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
                }
                
                return;
            }
            
            // Обработка начала перетаскивания задачи (только если не открыты другие окна)
            if (!showEditMode && !showAddTaskWindow && !showProjectWindow) {
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
    }
    
    // Обработка отпускания кнопки мыши
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Завершение перетаскивания задачи
            if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
                Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
                draggedTask.isMoving = false;
                
                // Определение центра задачи
                sf::FloatRect taskBounds = draggedTask.shape.getGlobalBounds();
                sf::Vector2f taskCenter(
                    taskBounds.left + taskBounds.width / 2,
                    taskBounds.top + taskBounds.height / 2
                );
                
                // Поиск новой секции для задачи
                for (int newSection = 0; newSection < 4; newSection++) {
                    if (sections[newSection].getGlobalBounds().contains(taskCenter)) {
                        if (newSection != draggingTaskSection) {
                            // Перемещение задачи в новую секцию
                            Task movedTask = draggedTask;
                            movedTask.currentSection = newSection;
                            movedTask.isMoving = false;
                            tasks[newSection].push_back(movedTask);
                            
                            // Обновление статуса в данных
                            updateTaskStatusInData(tasks[draggingTaskSection][draggingTaskIndex].id, newSection);
                            
                            // Удаление из старой секции
                            tasks[draggingTaskSection].erase(tasks[draggingTaskSection].begin() + draggingTaskIndex);
                            
                            // Сохранение изменений
                            saveTasksData();
                            
                            std::cout << "Задача перемещена в секцию " << newSection << std::endl;
                            
                            // Обновление позиций задач
                            updateTaskPositions();
                            break;
                        } else {
                            // Возврат задачи в исходную позицию
                            updateTaskPositions();
                        }
                    }
                }
                
                // Сброс переменных перетаскивания
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
                // Обновление позиции перетаскиваемой задачи
                draggedTask.setPosition(event.mouseMove.x - 190, event.mouseMove.y - 40);
            }
        }
    }
}

// Обновление состояния (не используется, но требуется для интерфейса)
void ScrumBoard::update(float deltaTime) {
    // Метод может быть использован для анимаций или других временных обновлений
}

// Отрисовка окна редактирования
void ScrumBoard::drawEditMode(sf::RenderWindow& window) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Основное окно редактирования
    window.draw(editModeWindow);
    window.draw(editTitleText);
    
    // Секции внутри окна редактирования
    for (const auto& section : editSections) {
        window.draw(section);
    }
    
    // Заголовки секций
    for (const auto& text : editSectionTexts) {
        window.draw(text);
    }
    
    // Расчет размеров для позиционирования задач
    float sectionWidth = (1800.0f - 20.0f * 5) / 4.0f;
    float startX = (WINDOW_WIDTH - 1800.0f) / 2 + 20.0f;
    float startY = (WINDOW_HEIGHT - 900.0f) / 2 + 80.0f;
    
    // Отрисовка задач внутри окна редактирования
    for (int section = 0; section < 4; section++) {
        float sectionX = startX + section * (sectionWidth + 20.0f);
        
        for (size_t i = 0; i < tasks[section].size(); i++) {
            // Создание копии задачи для отрисовки в окне редактирования
            Task task = tasks[section][i];
            
            // Позиционирование задачи внутри секции окна редактирования
            float x = sectionX + (sectionWidth - (sectionWidth - 30.0f)) / 2;
            float y = startY + i * 95;
            
            task.shape.setPosition(x, y);
            task.shape.setSize(sf::Vector2f(sectionWidth - 30.0f, 85));
            task.shape.setOutlineThickness(2);
            task.shape.setOutlineColor(sf::Color(80, 110, 140));
            
            // Обновление позиции текста
            sf::FloatRect textBounds = task.text.getLocalBounds();
            task.text.setPosition(
                x + ((sectionWidth - 30.0f) - textBounds.width) / 2,
                y + (85 - textBounds.height) / 2 - 5
            );
            
            // Подсветка выбранной задачи
            if (editingTaskSection == section && editingTaskIndex == i) {
                task.shape.setFillColor(sf::Color(255, 255, 200)); // Желтый фон
            }
            
            window.draw(task.shape);
            window.draw(task.text);
        }
    }
    
    // Элементы управления редактированием
    window.draw(editTaskInputField);
    window.draw(editTaskInputText);
    window.draw(saveEditButton);
    window.draw(saveEditButtonText);
    window.draw(deleteTaskButton);
    window.draw(deleteTaskButtonText);
    window.draw(cancelEditButton);
    window.draw(cancelEditButtonText);
}

// Основной метод отрисовки
void ScrumBoard::draw(sf::RenderWindow& window) {
    // Отрисовка основных элементов доски
    window.draw(titleText);
    
    window.draw(topPanel);
    window.draw(projectButton);      
    window.draw(projectButtonText);  
    window.draw(editButton);         
    window.draw(editButtonText);     
    window.draw(addButton);
    window.draw(addButtonText);
    
    // Секции доски
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    // Заголовки секций
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    // Задачи
    for (int i = 0; i < 4; i++) {
        sf::FloatRect sectionBounds = sections[i].getGlobalBounds();
        
        for (const auto& task : tasks[i]) {
            sf::FloatRect taskBounds = task.shape.getGlobalBounds();
            
            // Отрисовка только видимых задач (в пределах секции)
            if (taskBounds.top + taskBounds.height >= sectionBounds.top && 
                taskBounds.top <= sectionBounds.top + sectionBounds.height) {
                window.draw(task.shape);
                window.draw(task.text);
            }
        }
    }
    
    // Окно проектов
    if (showProjectWindow) { 
        // Затемнение фона
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(projectWindow); 
        
        // Кнопки проектов
        for (const auto& rect : projectRects) {  
            window.draw(rect);
        }
        
        // Текст проектов
        for (const auto& text : projectTexts) {  
            window.draw(text);
        }
    }
    
    // Окно добавления задачи
    if (showAddTaskWindow) {
        // Затемнение фона
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(addTaskWindow);
        window.draw(taskInputField);
        window.draw(taskInputText);
        window.draw(sectionLabelText);
        
        // Кнопки выбора секции
        for (const auto& rect : sectionOptionRects) {
            window.draw(rect);
        }
        for (const auto& text : sectionOptionTexts) {
            window.draw(text);
        }
        
        // Кнопки управления
        window.draw(confirmAddButton);
        window.draw(confirmAddButtonText);
        window.draw(cancelAddButton);
        window.draw(cancelAddButtonText);
    }
    
    // Режим редактирования
    if (showEditMode) {
        drawEditMode(window);
    }
}