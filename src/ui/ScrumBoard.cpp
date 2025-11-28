#include "ScrumBoard.h"
#include "../core/Tasks.h"
#include "../core/Developer.h"
#include "../core/Project.h"
#include "Task.h"
#include <iostream>
#include <algorithm>

// Путь к JSON
const std::string TASKS_JSON_PATH = "../core/project_scrumboard.json";

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

// Инициализирует все переменные
ScrumBoard::ScrumBoard() :
    isLoggedIn(false),
    currentUser(""),
    showLogoutButton(false)  
{
    // Названия секций
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
    idActiveProject.setID(0);
    
    // Переменные для перетаскивания задач
    draggingTaskSection = -1; 
    draggingTaskIndex = -1;   
    
    // Флаги отображения окон
    showLoginWindow = false;    
    showProjectWindow = false;  
    showAddTaskWindow = false; 
    showEditMode = false;       
    showTaskEditWindow = false;  
    showAddProjectWindow = false;
    showAddDeveloperWindow = false;
    
    // Переменные для ввода текста
    currentUsernameInput = "";   
    currentPasswordInput = "";   
    currentTaskInput = "";    
    currentEditTaskInput = ""; 
    currentProjectNameInput = "";
    
    // Флаги активности полей ввода
    isUsernameInputActive = false; 
    isPasswordInputActive = false;  
    isTaskInputActive = false;  
    isEditTaskInputActive = false; 
    isProjectNameInputActive = false;
    
    // Переменные для редактирования задачи
    editingTaskSection = -1;    
    editingTaskIndex = -1;      
    
    // Инициализация курсора
    cursorVisible = true;
    cursor.setSize(sf::Vector2f(2, 30));
    cursor.setFillColor(sf::Color::Black);
    
    // Список проектов
    projects = {};  
    availableDevelopers = {};
    
    // Загружаем задачи из JSON файла
    tasksData = {};
}

// Создание интерфейса
bool ScrumBoard::initialize() {
    // Загрузка шрифта
    if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        return false;
    }
    
    // Создание всех компонентов
    createTitle();           
    createTopPanel();       
    createProjectManagementButtons();
    createSections();       
    createSampleTasks();     
    createLoginWindow();    
    createProjectWindow(); 
    createAddTaskWindow();   
    createTaskEditWindow(); 
    createUserInfo();      
    createAddProjectWindow();
    createAddDeveloperWindow();
    return true;
}

// Создание элемента информации о пользователе
void ScrumBoard::createUserInfo() {
    // Кнопка с именем пользователя
    userInfoButton.setSize(sf::Vector2f(200, 40));
    userInfoButton.setFillColor(sf::Color(180, 210, 235));
    userInfoButton.setOutlineThickness(0);
    userInfoButton.setPosition(150, 30);

    userInfoText.setString(""); 
    userInfoText.setFont(font);
    userInfoText.setCharacterSize(20);
    userInfoText.setFillColor(sf::Color(50, 50, 80));
    userInfoText.setStyle(sf::Text::Bold);
    
    // Кнопка выхода
    logoutButton.setSize(sf::Vector2f(120, 40));
    logoutButton.setFillColor(sf::Color(180, 210, 235));
    logoutButton.setOutlineThickness(0);
    logoutButton.setPosition(20, 30);

    logoutButtonText.setString("Выйти");
    logoutButtonText.setFont(font);
    logoutButtonText.setCharacterSize(22);
    logoutButtonText.setFillColor(sf::Color(50, 50, 80));
    logoutButtonText.setStyle(sf::Text::Bold);
    
    centerTextInButton(logoutButtonText, logoutButton);
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

    // Кнопка "Войти" 
    loginButton.setSize(sf::Vector2f(200, 40));
    loginButton.setFillColor(sf::Color(180, 210, 235));
    loginButton.setOutlineThickness(0);
    loginButton.setPosition(150, 30);

    loginButtonText.setString("Войти"); 
    loginButtonText.setFont(font);
    loginButtonText.setCharacterSize(24);
    loginButtonText.setFillColor(sf::Color(50, 50, 80));
    loginButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(loginButtonText, loginButton);

    // Кнопка "Проекты" (будет показываться только после входа)
    projectButton.setSize(sf::Vector2f(200, 40));
    projectButton.setFillColor(sf::Color(180, 210, 235));
    projectButton.setOutlineThickness(0);
    projectButton.setPosition(370, 30);

    projectButtonText.setString("Проекты"); 
    projectButtonText.setFont(font);
    projectButtonText.setCharacterSize(24);
    projectButtonText.setFillColor(sf::Color(50, 50, 80));
    projectButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(projectButtonText, projectButton);

    // Кнопка "Редактировать" (сдвинута вправо)
    editButton.setSize(sf::Vector2f(250, 40));
    editButton.setFillColor(sf::Color(180, 210, 235));
    editButton.setOutlineThickness(0);
    editButton.setPosition(1600, 30);

    editButtonText.setString("Редактировать");
    editButtonText.setFont(font);
    editButtonText.setCharacterSize(24);
    editButtonText.setFillColor(sf::Color(50, 50, 80));
    editButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(editButtonText, editButton);

    // Кнопка "Добавить задачу" (сдвинута вправо, текст изменен)
    addButton.setSize(sf::Vector2f(220, 40));
    addButton.setFillColor(sf::Color(180, 210, 235));
    addButton.setOutlineThickness(0);
    addButton.setPosition(1360, 30);

    addButtonText.setString("Добавить задачу");
    addButtonText.setFont(font);
    addButtonText.setCharacterSize(22);
    addButtonText.setFillColor(sf::Color(50, 50, 80));
    addButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(addButtonText, addButton);
}

// Создание кнопок управления проектами
void ScrumBoard::createProjectManagementButtons() {
    // Кнопка "Добавить проект"
    addProjectButton.setSize(sf::Vector2f(220, 40));
    addProjectButton.setFillColor(sf::Color(180, 210, 235));
    addProjectButton.setOutlineThickness(0);
    addProjectButton.setPosition(590, 30);

    addProjectButtonText.setString("Добавить проект");
    addProjectButtonText.setFont(font);
    addProjectButtonText.setCharacterSize(22);
    addProjectButtonText.setFillColor(sf::Color(50, 50, 80));
    addProjectButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(addProjectButtonText, addProjectButton);

    // Кнопка "Добавить разработчика"
    addDeveloperButton.setSize(sf::Vector2f(260, 40));
    addDeveloperButton.setFillColor(sf::Color(180, 210, 235));
    addDeveloperButton.setOutlineThickness(0);
    addDeveloperButton.setPosition(820, 30);

    addDeveloperButtonText.setString("Добавить разработчика");
    addDeveloperButtonText.setFont(font);
    addDeveloperButtonText.setCharacterSize(20);
    addDeveloperButtonText.setFillColor(sf::Color(50, 50, 80));
    addDeveloperButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопке
    centerTextInButton(addDeveloperButtonText, addDeveloperButton);
}

// Создание окна входа
void ScrumBoard::createLoginWindow() {
    float windowWidth = 400.0f;
    float windowHeight = 300.0f;
    float padding = 20.0f;
    
    // По центру экрана
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно
    loginWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    loginWindow.setFillColor(sf::Color(180, 210, 235));
    loginWindow.setOutlineColor(sf::Color(160, 190, 220));
    loginWindow.setOutlineThickness(3);
    loginWindow.setPosition(startX, startY);
    
    // Метка "Имя пользователя"
    usernameLabel.setString("Имя пользователя:");
    usernameLabel.setFont(font);
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color(50, 50, 80));
    usernameLabel.setStyle(sf::Text::Bold);
    usernameLabel.setPosition(startX + padding, startY + padding);
    
    // Поле ввода имени пользователя
    usernameField.setSize(sf::Vector2f(windowWidth - padding * 2, 40.0f));
    usernameField.setFillColor(sf::Color::White);
    usernameField.setOutlineColor(sf::Color(100, 130, 160));
    usernameField.setOutlineThickness(2);
    usernameField.setPosition(startX + padding, startY + padding + 30);
    
    // Текст в поле имени пользователя
    usernameText.setString("Введите имя пользователя");
    usernameText.setFont(font);
    usernameText.setCharacterSize(18);
    usernameText.setFillColor(sf::Color(150, 150, 150));
    usernameText.setPosition(startX + padding + 10, startY + padding + 40);
    
    // Метка "Пароль"
    passwordLabel.setString("Пароль:");
    passwordLabel.setFont(font);
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color(50, 50, 80));
    passwordLabel.setStyle(sf::Text::Bold);
    passwordLabel.setPosition(startX + padding, startY + padding + 90);
    
    // Поле ввода пароля
    passwordField.setSize(sf::Vector2f(windowWidth - padding * 2, 40.0f));
    passwordField.setFillColor(sf::Color::White);
    passwordField.setOutlineColor(sf::Color(100, 130, 160));
    passwordField.setOutlineThickness(2);
    passwordField.setPosition(startX + padding, startY + padding + 120);
    
    // Текст в поле пароля
    passwordText.setString("Введите пароль");
    passwordText.setFont(font);
    passwordText.setCharacterSize(18);
    passwordText.setFillColor(sf::Color(150, 150, 150));
    passwordText.setPosition(startX + padding + 10, startY + padding + 130);
    
    // Кнопка подтверждения входа
    confirmLoginButton.setSize(sf::Vector2f(120, 40));
    confirmLoginButton.setFillColor(sf::Color(120, 180, 120));
    confirmLoginButton.setOutlineColor(sf::Color(80, 140, 80));
    confirmLoginButton.setOutlineThickness(2);
    confirmLoginButton.setPosition(startX + padding, startY + windowHeight - padding - 50);
    
    confirmLoginButtonText.setString("Войти");
    confirmLoginButtonText.setFont(font);
    confirmLoginButtonText.setCharacterSize(20);
    confirmLoginButtonText.setFillColor(sf::Color::White);
    confirmLoginButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка отмены
    cancelLoginButton.setSize(sf::Vector2f(120, 40));
    cancelLoginButton.setFillColor(sf::Color(180, 120, 120));
    cancelLoginButton.setOutlineColor(sf::Color(140, 80, 80));
    cancelLoginButton.setOutlineThickness(2);
    cancelLoginButton.setPosition(startX + windowWidth - padding - 120, startY + windowHeight - padding - 50);
    
    cancelLoginButtonText.setString("Отмена");
    cancelLoginButtonText.setFont(font);
    cancelLoginButtonText.setCharacterSize(20);
    cancelLoginButtonText.setFillColor(sf::Color::White);
    cancelLoginButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопках
    centerTextInButton(confirmLoginButtonText, confirmLoginButton);
    centerTextInButton(cancelLoginButtonText, cancelLoginButton);
}

// Создание окна выбора проекта
void ScrumBoard::createProjectWindow() { 
    float projectHeight = 55.0f;
    float padding = 15.0f;
    
    // Если нет проектов, создаем маленькое окно с сообщением
    float windowWidth = 380.0f;
    float windowHeight = 0;
    
    if (projects.empty()) {
        windowHeight = 100.0f;
    } else {
        windowHeight = projectHeight * projects.size() + padding * 2; 
    }
    
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
    
    if (projects.empty()) {
        // Сообщение "Нет проектов"
        sf::Text noProjectsText;
        noProjectsText.setString("Нет доступных проектов");
        noProjectsText.setFont(font);
        noProjectsText.setCharacterSize(24);
        noProjectsText.setFillColor(sf::Color(50, 50, 80));
        noProjectsText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = noProjectsText.getLocalBounds();
        noProjectsText.setPosition(
            startX + (windowWidth - textBounds.width) / 2,
            startY + padding
        );
        projectTexts.push_back(noProjectsText);
    } else {
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
            projectText.setString(projects[i].getName());
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
}

// Создание окна добавления новой задачи
void ScrumBoard::createAddTaskWindow() {
    float windowWidth = 500.0f;
    float windowHeight = 400.0f;
    float padding = 20.0f;
    
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
    
    // Создание 4 кнопок для выбора секции
    for (int i = 0; i < 4; i++) {
        int row = i / 2;
        int col = i % 2;
        
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
    centerTextInButton(confirmAddButtonText, confirmAddButton);
    
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
    centerTextInButton(cancelAddButtonText, cancelAddButton);
}

// Создание окна редактирования задачи
void ScrumBoard::createTaskEditWindow() {
    float windowWidth = 500.0f;
    float windowHeight = 300.0f;
    float padding = 20.0f;
    
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
    editTitleText.setString("Редактировать");
    editTitleText.setFont(font);
    editTitleText.setCharacterSize(28);
    editTitleText.setFillColor(sf::Color(50, 50, 80));
    editTitleText.setStyle(sf::Text::Bold);
    editTitleText.setPosition(startX + padding, startY + padding);
    
    // Поле ввода для редактирования названия задачи
    editTaskInputField.setSize(sf::Vector2f(windowWidth - padding * 2, 50.0f));
    editTaskInputField.setFillColor(sf::Color::White);
    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
    editTaskInputField.setOutlineThickness(2);
    editTaskInputField.setPosition(startX + padding, startY + 80);
    
    // Подсказка в поле ввода
    editTaskInputText.setString("Введите текст на английском");
    editTaskInputText.setFont(font);
    editTaskInputText.setCharacterSize(20);
    editTaskInputText.setFillColor(sf::Color(150, 150, 150));
    editTaskInputText.setPosition(startX + padding + 10, startY + 95);
    
    // Кнопка сохранения изменений
    saveEditButton.setSize(sf::Vector2f(120, 40));
    saveEditButton.setFillColor(sf::Color(120, 180, 120));
    saveEditButton.setOutlineColor(sf::Color(80, 140, 80));
    saveEditButton.setOutlineThickness(2);
    saveEditButton.setPosition(startX + padding, startY + windowHeight - padding - 50);
    
    saveEditButtonText.setString("Сохранить");
    saveEditButtonText.setFont(font);
    saveEditButtonText.setCharacterSize(20);
    saveEditButtonText.setFillColor(sf::Color::White);
    saveEditButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка удаления задачи
    deleteTaskButton.setSize(sf::Vector2f(120, 40));
    deleteTaskButton.setFillColor(sf::Color(200, 100, 100));
    deleteTaskButton.setOutlineColor(sf::Color(160, 60, 60));
    deleteTaskButton.setOutlineThickness(2);
    deleteTaskButton.setPosition(startX + padding + 130, startY + windowHeight - padding - 50);
    
    deleteTaskButtonText.setString("Удалить");
    deleteTaskButtonText.setFont(font);
    deleteTaskButtonText.setCharacterSize(20);
    deleteTaskButtonText.setFillColor(sf::Color::White);
    deleteTaskButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка отмены редактирования
    cancelEditButton.setSize(sf::Vector2f(120, 40));
    cancelEditButton.setFillColor(sf::Color(180, 120, 120));
    cancelEditButton.setOutlineColor(sf::Color(140, 80, 80));
    cancelEditButton.setOutlineThickness(2);
    cancelEditButton.setPosition(startX + windowWidth - padding - 120, startY + windowHeight - padding - 50);
    
    cancelEditButtonText.setString("Отмена");
    cancelEditButtonText.setFont(font);
    cancelEditButtonText.setCharacterSize(20);
    cancelEditButtonText.setFillColor(sf::Color::White);
    cancelEditButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопках
    centerTextInButton(saveEditButtonText, saveEditButton);
    centerTextInButton(deleteTaskButtonText, deleteTaskButton);
    centerTextInButton(cancelEditButtonText, cancelEditButton);
}

// Создание окна добавления проекта
void ScrumBoard::createAddProjectWindow() {
    float windowWidth = 450.0f;
    float windowHeight = 250.0f;
    float padding = 20.0f;
    
    // Позиционирование по центру экрана
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно
    addProjectWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    addProjectWindow.setFillColor(sf::Color(180, 210, 235));
    addProjectWindow.setOutlineColor(sf::Color(160, 190, 220));
    addProjectWindow.setOutlineThickness(3);
    addProjectWindow.setPosition(startX, startY);
    
    // Поле ввода названия проекта
    projectNameInputField.setSize(sf::Vector2f(windowWidth - padding * 2, 50.0f));
    projectNameInputField.setFillColor(sf::Color::White);
    projectNameInputField.setOutlineColor(sf::Color(100, 130, 160));
    projectNameInputField.setOutlineThickness(2);
    projectNameInputField.setPosition(startX + padding, startY + 70);
    
    // Подсказка в поле ввода
    projectNameInputText.setString("Введите название проекта");
    projectNameInputText.setFont(font);
    projectNameInputText.setCharacterSize(20);
    projectNameInputText.setFillColor(sf::Color(150, 150, 150));
    projectNameInputText.setPosition(startX + padding + 10, startY + 85);
    
    // Кнопка подтверждения добавления
    confirmAddProjectButton.setSize(sf::Vector2f(150, 40));
    confirmAddProjectButton.setFillColor(sf::Color(120, 180, 120));
    confirmAddProjectButton.setOutlineColor(sf::Color(80, 140, 80));
    confirmAddProjectButton.setOutlineThickness(2);
    confirmAddProjectButton.setPosition(startX + padding, startY + windowHeight - padding - 50);
    
    confirmAddProjectButtonText.setString("Создать");
    confirmAddProjectButtonText.setFont(font);
    confirmAddProjectButtonText.setCharacterSize(20);
    confirmAddProjectButtonText.setFillColor(sf::Color::White);
    confirmAddProjectButtonText.setStyle(sf::Text::Bold);
    
    // Кнопка отмены
    cancelAddProjectButton.setSize(sf::Vector2f(150, 40));
    cancelAddProjectButton.setFillColor(sf::Color(180, 120, 120));
    cancelAddProjectButton.setOutlineColor(sf::Color(140, 80, 80));
    cancelAddProjectButton.setOutlineThickness(2);
    cancelAddProjectButton.setPosition(startX + windowWidth - padding - 150, startY + windowHeight - padding - 50);
    
    cancelAddProjectButtonText.setString("Отмена");
    cancelAddProjectButtonText.setFont(font);
    cancelAddProjectButtonText.setCharacterSize(20);
    cancelAddProjectButtonText.setFillColor(sf::Color::White);
    cancelAddProjectButtonText.setStyle(sf::Text::Bold);
    
    // Центрирование текста на кнопках
    centerTextInButton(confirmAddProjectButtonText, confirmAddProjectButton);
    centerTextInButton(cancelAddProjectButtonText, cancelAddProjectButton);
}

// Создание окна добавления разработчика
void ScrumBoard::createAddDeveloperWindow() {
    float developerHeight = 55.0f;
    float padding = 15.0f;
    
    // Загружаем ВСЕХ разработчиков из системы
    availableDevelopers = getDevelopersFromJson();
    
    // Фильтруем разработчиков - убираем текущего пользователя
    std::vector<Developer> filteredDevelopers;
    for (const auto& dev : availableDevelopers) {
        if (activeDeveloper && dev.getId() != activeDeveloper->getId()) {
            filteredDevelopers.push_back(dev);
        }
    }
    availableDevelopers = filteredDevelopers;
    
    // Если нет других разработчиков, создаем окно с сообщением
    float windowWidth = 400.0f;
    float windowHeight = 0;
    
    if (availableDevelopers.empty()) {
        windowHeight = 150.0f;
    } else {
        windowHeight = developerHeight * availableDevelopers.size() + padding * 2 + 40; 
    }
    
    // Позиционирование по центру экрана
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    // Основное окно
    addDeveloperWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    addDeveloperWindow.setFillColor(sf::Color(180, 210, 235));
    addDeveloperWindow.setOutlineColor(sf::Color(160, 190, 220));
    addDeveloperWindow.setOutlineThickness(3);
    addDeveloperWindow.setPosition(startX, startY);
    
    // Очистка предыдущих элементов
    developerRects.clear(); 
    developerTexts.clear(); 
    
    float developerWidth = windowWidth - padding * 2; 
    
    if (availableDevelopers.empty()) {
        // Сообщение "Нет других разработчиков"
        sf::Text noDevelopersText;
        noDevelopersText.setString("Нет других разработчиков\nв системе");
        noDevelopersText.setFont(font);
        noDevelopersText.setCharacterSize(22);
        noDevelopersText.setFillColor(sf::Color(50, 50, 80));
        noDevelopersText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = noDevelopersText.getLocalBounds();
        noDevelopersText.setPosition(
            startX + (windowWidth - textBounds.width) / 2,
            startY + padding + 20
        );
        developerTexts.push_back(noDevelopersText);
    } else {
        // Создание кнопок для каждого разработчика
        for (size_t i = 0; i < availableDevelopers.size(); i++) { 
            // Прямоугольник кнопки разработчика
            sf::RectangleShape developerRect; 
            developerRect.setSize(sf::Vector2f(developerWidth, developerHeight));
            developerRect.setFillColor(sf::Color(180, 210, 235));
            developerRect.setOutlineColor(sf::Color(100, 130, 160));
            developerRect.setOutlineThickness(3);
            developerRect.setPosition(startX + padding, startY + padding + i * developerHeight);
            developerRects.push_back(developerRect);
            
            // Текст разработчика
            sf::Text developerText;
            developerText.setString(availableDevelopers[i].getLogin());
            developerText.setFont(font);
            developerText.setCharacterSize(22);
            developerText.setFillColor(sf::Color(50, 50, 80));
            developerText.setStyle(sf::Text::Bold);
            
            // Центрирование текста
            sf::FloatRect textBounds = developerText.getLocalBounds();
            developerText.setPosition(
                startX + padding + (developerWidth - textBounds.width) / 2,
                startY + padding + i * developerHeight + (developerHeight - textBounds.height) / 2 - 3
            );
            developerTexts.push_back(developerText); 
        }
    }
}

// Вспомогательная функция для центрирования текста
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
        sf::Color(180, 210, 235),  
        sf::Color(160, 195, 225),
        sf::Color(140, 180, 215),  
        sf::Color(120, 165, 205)   
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
    std::cout << "=== СОЗДАНИЕ ВИЗУАЛЬНЫХ ЗАДАЧ ===" << std::endl;
    std::cout << "Задач в данных: " << tasksData.size() << std::endl;
    
    // Очистка существующих задач
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    // Добавление задач из JSON данных
    for (const auto& task : tasksData){
        std::cout << "Добавляем задачу: '" << task.getTitle() << "' в секцию: " << task.getStatus() << std::endl;
        addTask(task.getId(), task.getTitle(), task.getStatus());
    }
    
    // Обновление позиций задач
    updateTaskPositions();
    std::cout << "Визуальные задачи созданы" << std::endl;
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
        float taskWidth = sectionWidth - 30.0f;
        
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
        
        // Центрирование текста в задаче - сохраняем относительную позицию текста
        sf::FloatRect textBounds = newTask.text.getLocalBounds();
        newTask.text.setPosition(
            x + (taskWidth - textBounds.width) / 2,
            y + (85 - textBounds.height) / 2 - 5
        );
        
        // Сохраняем относительное смещение текста для корректного перемещения
        newTask.textOffset = sf::Vector2f((taskWidth - textBounds.width) / 2, (85 - textBounds.height) / 2 - 5);
        
        // Добавление задачи в соответствующую секцию
        tasks[section].push_back(newTask);
    }
}

// Обновление статуса задачи в данных при перемещении между секциями
void ScrumBoard::updateTaskStatusInData(int taskId, int newStatus) {
    std::cout << "=== ОБНОВЛЕНИЕ СТАТУСА ЗАДАЧИ ===" << std::endl;
    std::cout << "Активный проект: " << idActiveProject.getName() << " (ID: " << idActiveProject.getId() << ")" << std::endl;
    std::cout << "Задача ID: " << taskId << ", новый статус: " << newStatus << std::endl;
    
    // Обновляем в tasksData
    for (auto& task : tasksData) {
        if (task.getId() == taskId){
            std::cout << "Найдена в tasksData: '" << task.getTitle() << "' статус: " << task.getStatus() << " -> " << newStatus << std::endl;
            task.changeStatus(newStatus);
            idActiveProject.addTask(task);
            break;
        }
    }
    
    // НЕМЕДЛЕННО сохраняем изменения
    saveTasksData();
    std::cout << "Изменения сохранены" << std::endl;
}

// Сохранение всех задач в JSON файл
void ScrumBoard::saveTasksData() {
    std::cout<< std::endl << "=== СОХРАНЕНИЕ ДАННЫХ ===" << std::endl;
    std::cout << "Активный проект: " << idActiveProject.getName() << " (ID: " << idActiveProject.getId() << ")" << std::endl;
    std::cout << "Количество задач для сохранения: " << tasksData.size() << std::endl;
    
    if (idActiveProject.getId() == 0) {
        std::cout << "Ошибка: нет активного проекта!" << std::endl;
        return;
    }

    // 1. ОБНОВЛЯЕМ ЗАДАЧИ В АКТИВНОМ ПРОЕКТЕ
    Project updatedProject(idActiveProject.getId(), idActiveProject.getName(), 
                          idActiveProject.getDeadline(), idActiveProject.getCreatorId());
    
    // Копируем разработчиков
    for (int devId : idActiveProject.getDeveloperIds()) {
        updatedProject.addDeveloper(devId);
    }
    
    // Добавляем ВСЕ текущие задачи
    for (const auto& task : tasksData) {
        updatedProject.addTask(task);
    }
    
    // Заменяем старый проект обновленным
    idActiveProject = updatedProject;

    // 2. Загружаем ВСЕ проекты из файла
    std::vector<Project> allProjects = Project::getProjectsFromJson();
    std::cout << "Загружено проектов из файла: " << allProjects.size() << std::endl;
    
    // 3. Находим и заменяем активный проект в списке
    bool projectFound = false;
    for (auto& project : allProjects) {
        if (project.getId() == idActiveProject.getId()) {
            std::cout << "Найден проект для обновления: " << project.getName() << std::endl;
            project = idActiveProject;
            projectFound = true;
            break;
        }
    }
    
    if (!projectFound) {
        std::cout << "Проект не найден, добавляем новый" << std::endl;
        allProjects.push_back(idActiveProject);
    }
    
    // 4. Сохраняем обратно в файл
    Project::saveProjectsToJson(allProjects);
    std::cout << "Проекты сохранены в файл" << std::endl;
}

// Обновление позиций всех задач на доске - ИСПРАВЛЕННАЯ ВЕРСИЯ
void ScrumBoard::updateTaskPositions() {
    // Расчет размеров
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    float taskWidth = sectionWidth - 30.0f;
    
    // Обновление позиций для всех секций и всех задач
    for (int section = 0; section < 4; section++) {
        float startY = 270.0f;
        
        for (size_t i = 0; i < tasks[section].size(); i++) {
            // Расчет позиции задачи
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 95;  
            
            // Установка позиции задачи
            tasks[section][i].setPosition(x, y);
            tasks[section][i].shape.setSize(sf::Vector2f(taskWidth, 85));
            
            // ВОССТАНАВЛИВАЕМ правильную позицию текста относительно задачи
            sf::FloatRect textBounds = tasks[section][i].text.getLocalBounds();
            tasks[section][i].text.setPosition(
                x + (taskWidth - textBounds.width) / 2,
                y + (85 - textBounds.height) / 2 - 5
            );
            
            // Обновляем смещение текста
            tasks[section][i].textOffset = sf::Vector2f(
                (taskWidth - textBounds.width) / 2,
                (85 - textBounds.height) / 2 - 5
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
                    confirmAddTask(0);  
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
            
            // Сброс таймера курсора при вводе
            cursorClock.restart();
            cursorVisible = true;
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
            // Обработка Enter
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
            
            // Сброс таймера курсора при вводе
            cursorClock.restart();
            cursorVisible = true;
        }
    }
}

// Обработка ввода текста для входа
void ScrumBoard::handleLoginInput(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Обработка только ASCII символов
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            // Обработка Backspace
            if (event.text.unicode == 8) {
                if (isUsernameInputActive && !currentUsernameInput.empty()) {
                    currentUsernameInput.pop_back();
                } else if (isPasswordInputActive && !currentPasswordInput.empty()) {
                    currentPasswordInput.pop_back();
                }
            }
            // Обработка Enter
            else if (event.text.unicode == 13) {
                confirmLogin();
                return;
            }
            // Обработка печатных символов
            else if (c >= 32 && c <= 126) {
                if (isUsernameInputActive) {
                    currentUsernameInput += c;
                } else if (isPasswordInputActive) {
                    currentPasswordInput += c;
                }
            }
            
            // Обновление отображаемого текста
            if (isUsernameInputActive) {
                if (currentUsernameInput.empty()) {
                    usernameText.setString("Введите имя пользователя");
                    usernameText.setFillColor(sf::Color(150, 150, 150));
                } else {
                    usernameText.setString(currentUsernameInput);
                    usernameText.setFillColor(sf::Color(50, 50, 80));
                }
            } else if (isPasswordInputActive) {
                if (currentPasswordInput.empty()) {
                    passwordText.setString("Введите пароль");
                    passwordText.setFillColor(sf::Color(150, 150, 150));
                } else {
                    // Скрываем пароль звездочками
                    std::string hiddenPassword(currentPasswordInput.length(), '*');
                    passwordText.setString(hiddenPassword);
                    passwordText.setFillColor(sf::Color(50, 50, 80));
                }
            }
            
            // Сброс таймера курсора при вводе
            cursorClock.restart();
            cursorVisible = true;
        }
    }
}

// Обработка ввода текста для нового проекта
void ScrumBoard::handleAddProjectInput(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        // Обработка только ASCII символов
        if (event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            // Обработка Backspace
            if (event.text.unicode == 8) {
                if (!currentProjectNameInput.empty()) {
                    currentProjectNameInput.pop_back();
                }
            }
            // Обработка Enter
            else if (event.text.unicode == 13) {
                confirmAddProject();
                return;
            }
            // Обработка печатных символов
            else if (c >= 32 && c <= 126) {
                currentProjectNameInput += c;
            }
            
            // Обновление отображаемого текста
            if (currentProjectNameInput.empty()) {
                projectNameInputText.setString("Введите название проекта");
                projectNameInputText.setFillColor(sf::Color(150, 150, 150));
            } else {
                projectNameInputText.setString(currentProjectNameInput);
                projectNameInputText.setFillColor(sf::Color(50, 50, 80));
            }
            
            // Сброс таймера курсора при вводе
            cursorClock.restart();
            cursorVisible = true;
        }
    }
}

// Подтверждение добавления новой задачи в выбранную секцию - ИСПРАВЛЕННАЯ ВЕРСИЯ
void ScrumBoard::confirmAddTask(int selectedSection) {
    // Проверка наличия текста задачи и корректности секции
    if (!currentTaskInput.empty() && selectedSection >= 0 && selectedSection < 4 && activeDeveloper != nullptr) {
        // Проверяем, что пользователь имеет доступ к активному проекту
        bool hasAccess = false;
        for (int devId : idActiveProject.getDeveloperIds()) {
            if (devId == activeDeveloper->getId()) {
                hasAccess = true;
                break;
            }
        }
        
        if (!hasAccess) {
            std::cout << "Ошибка: у пользователя нет доступа к этому проекту!" << std::endl;
            return;
        }
        
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
        idActiveProject.addTask(newTask);
        
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
        cursorVisible = false;
        
        std::cout << "Добавлена новая задача: '" << newTask.getTitle() 
                  << "' в секцию " << selectedSection << std::endl;
        std::cout << "Теперь в секции " << selectedSection << " задач: " << tasks[selectedSection].size() << std::endl;
    }
}

// Подтверждение добавления нового проекта
void ScrumBoard::confirmAddProject() {
    if (!currentProjectNameInput.empty() && activeDeveloper != nullptr) {
        // Генерация нового ID проекта
        int newId = 1;
        std::vector<Project> allProjects = Project::getProjectsFromJson();
        for (const auto& project : allProjects) {
            if (project.getId() >= newId) {
                newId = project.getId() + 1;
            }
        }
        
        // Создание нового проекта с привязкой к текущему пользователю
        Project newProject(newId, currentProjectNameInput, "2024-12-31", activeDeveloper->getId());
        newProject.addDeveloper(activeDeveloper->getId());
        
        // Добавление проекта в общий список
        allProjects.push_back(newProject);
        
        // Сохраняем проекты в JSON
        Project::saveProjectsToJson(allProjects);
        
        // Обновляем разработчика в файле - добавляем ему проект
        std::vector<Developer> allDevelopers = getDevelopersFromJson();
        for (auto& dev : allDevelopers) {
            if (dev.getId() == activeDeveloper->getId()) {
                dev.addProject(newProject.getId());
                break;
            }
        }
        saveDevelopersToJson(allDevelopers);
        
        // Обновляем список проектов из файла
        projects = activeDeveloper->getProjects();
        createProjectWindow();
        
        // Закрываем окно добавления проекта
        closeAddProjectWindow();
        
        // Показываем окно выбора проектов с новым проектом
        showProjectWindow = true;
        
        std::cout << "Создан новый проект '" << currentProjectNameInput 
                  << "' для пользователя '" << activeDeveloper->getLogin() << "'" << std::endl;
    }
}

// Добавление разработчика к проекту
void ScrumBoard::addDeveloperToProject(int developerIndex) {
    if (developerIndex >= 0 && developerIndex < availableDevelopers.size() && idActiveProject.getId() != 0) {
        Developer& selectedDeveloper = availableDevelopers[developerIndex];
        
        // Проверяем, не добавлен ли уже этот разработчик к проекту
        bool alreadyAdded = false;
        for (int devId : idActiveProject.getDeveloperIds()) {
            if (devId == selectedDeveloper.getId()) {
                alreadyAdded = true;
                break;
            }
        }
        
        if (alreadyAdded) {
            std::cout << "Разработчик '" << selectedDeveloper.getLogin() << "' уже добавлен к проекту!" << std::endl;
            return;
        }
        
        // 1. Добавляем разработчика к проекту
        idActiveProject.addDeveloper(selectedDeveloper.getId());
        
        // 2. Обновляем проект в общем списке проектов
        std::vector<Project> allProjects = Project::getProjectsFromJson();
        bool projectUpdated = false;
        for (auto& project : allProjects) {
            if (project.getId() == idActiveProject.getId()) {
                project = idActiveProject;
                projectUpdated = true;
                break;
            }
        }
        
        if (projectUpdated) {
            Project::saveProjectsToJson(allProjects);
            std::cout << "Проект обновлен в файле projects.json" << std::endl;
        }
        
        // 3. Добавляем проект к разработчику в его списке проектов
        std::vector<Developer> allDevelopers = getDevelopersFromJson();
        bool developerUpdated = false;
        for (auto& dev : allDevelopers) {
            if (dev.getId() == selectedDeveloper.getId()) {
                // Проверяем, нет ли уже этого проекта у разработчика
                bool projectAlreadyAssigned = false;
                for (int projectId : dev.getProjectIds()) {
                    if (projectId == idActiveProject.getId()) {
                        projectAlreadyAssigned = true;
                        break;
                    }
                }
                
                if (!projectAlreadyAssigned) {
                    dev.addProject(idActiveProject.getId());
                    developerUpdated = true;
                    std::cout << "Проект добавлен разработчику '" << dev.getLogin() << "'" << std::endl;
                }
                break;
            }
        }
        
        if (developerUpdated) {
            saveDevelopersToJson(allDevelopers);
            std::cout << "Разработчики обновлены в файле developers.json" << std::endl;
        }
        
        // Закрываем окно
        closeAddDeveloperWindow();
        
        std::cout << "Разработчик '" << selectedDeveloper.getLogin() 
                  << "' добавлен к проекту '" << idActiveProject.getName() << "'" << std::endl;
    } else {
        std::cout << "Ошибка: не выбран активный проект!" << std::endl;
    }
}

// Подтверждение входа
void ScrumBoard::confirmLogin() {
    // Проверка наличия логина и пароля
    if (!currentUsernameInput.empty() && !currentPasswordInput.empty()) {
        
        // Загружаем разработчиков один раз
        std::vector<Developer> developers = getDevelopersFromJson();
        
        if (validateDeveloperCredentials(developers, currentUsernameInput, currentPasswordInput)) {
            std::cout << "Вход выполнен успешно!" << std::endl;
            
            // Используем ту же копию developers
            activeDeveloper = findDeveloperByLogin(developers, currentUsernameInput, currentPasswordInput);
            
            if (activeDeveloper != nullptr) {
                isLoggedIn = true;
                currentUser = currentUsernameInput;
                showLogoutButton = true; 
                
                // Получаем ТОЛЬКО проекты активного разработчика
                projects = activeDeveloper->getProjects();
                std::cout << "Пользователь '" << currentUser << "' имеет доступ к " 
                          << projects.size() << " проектам" << std::endl;
                
                createProjectWindow();
                closeLoginWindow();
                
                // Обновляем текст кнопки пользователя
                userInfoText.setString(currentUser);
                centerTextInButton(userInfoText, userInfoButton);
                
                // ЗАГРУЖАЕМ ЗАДАЧИ ТОЛЬКО ЕСЛИ ЕСТЬ ПРОЕКТЫ
                if (!projects.empty()) {
                    // Автоматически выбираем первый проект
                    projectButtonText.setString(projects[0].getName());
                    idActiveProject = projects[0];
                    tasksData = idActiveProject.getTasks();
                    createSampleTasks();
                    std::cout << "Автоматически выбран проект: " << idActiveProject.getName() 
                              << " (ID: " << idActiveProject.getId() << ")" << std::endl;
                } else {
                    // Если нет проектов, сбрасываем активный проект
                    idActiveProject = Project(0, "", "", 0);
                    tasksData.clear();
                    createSampleTasks();
                    std::cout << "У пользователя нет проектов" << std::endl;
                }
                
            } else {
                std::cout << "Ошибка: не удалось найти разработчика!" << std::endl;
            }
            
        } else {
            std::cout << "Неверный логин или пароль!" << std::endl;
        }
    } else {
        std::cout << "Введите логин и пароль!" << std::endl;
    }
}

// Выход из системы
void ScrumBoard::logout() {
    // Сохраняем изменения перед выходом
    if (isLoggedIn && idActiveProject.getId() != 0) {
        saveCurrentProjectChanges();
    }
    
    isLoggedIn = false;
    currentUser = "";
    activeDeveloper = nullptr;
    projects = {};
    tasksData.clear();
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    createProjectWindow();
    showLogoutButton = false;
    std::cout << "Выход выполнен" << std::endl;
}

// Открытие режима редактирования
void ScrumBoard::openEditMode() {
    showEditMode = !showEditMode;
    
    if (showEditMode) {
        // Делаем кнопку серой когда режим редактирования активен
        editButton.setFillColor(sf::Color(150, 150, 150));
        editButtonText.setString("Режим редактирования");
        // Уменьшаем шрифт для вмещения текста
        editButtonText.setCharacterSize(20);
    } else {
        // Возвращаем обычный цвет когда режим не активен
        editButton.setFillColor(sf::Color(180, 210, 235));
        editButtonText.setString("Редактировать");
        editButtonText.setCharacterSize(24);
        // Закрываем окно редактирования если оно было открыто
        closeTaskEditWindow();
    }
    
    // Центрирование текста на кнопке
    centerTextInButton(editButtonText, editButton);
}

// Открытие окна редактирования задачи
void ScrumBoard::openTaskEditWindow() {
    showTaskEditWindow = true;
    isEditTaskInputActive = true;
    editTaskInputField.setOutlineColor(sf::Color(50, 100, 200));
    
    // Получаем полный текст выбранной задачи
    if (editingTaskSection != -1 && editingTaskIndex != -1) {
        for (const auto& taskData : tasksData) {
            if (taskData.getId() == tasks[editingTaskSection][editingTaskIndex].id && taskData.getId() && (*activeDeveloper).validateTask(taskData.getId())){
                currentEditTaskInput = taskData.getTitle();
                break;
            }
        }
        editTaskInputText.setString(currentEditTaskInput);
        editTaskInputText.setFillColor(sf::Color(50, 50, 80));
    }
    
    // Сброс курсора
    cursorClock.restart();
    cursorVisible = true;
}

// Выбор задачи для редактирования
void ScrumBoard::selectTaskForEditing(int section, int index) {
    // Проверка корректности индексов
    if (section >= 0 && section < 4 && index >= 0 && index < tasks[section].size()) {
        editingTaskSection = section;
        editingTaskIndex = index;
        
        // Открываем окно редактирования
        openTaskEditWindow();
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
        
        // Центрирование текста с сохранением относительного смещения
        sf::FloatRect textBounds = task.text.getLocalBounds();
        sf::FloatRect taskBounds = task.shape.getGlobalBounds();
        task.text.setPosition(
            taskBounds.left + (taskBounds.width - textBounds.width) / 2,
            taskBounds.top + (taskBounds.height - textBounds.height) / 2 - 5
        );
        
        // Обновляем смещение текста
        task.textOffset = sf::Vector2f(
            (taskBounds.width - textBounds.width) / 2,
            (taskBounds.height - textBounds.height) / 2 - 5
        );
        
        // Сохранение в JSON
        saveTasksData();
        
        // Закрытие окна редактирования
        closeTaskEditWindow();
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
        
        // Закрытие окна редактирования
        closeTaskEditWindow();
        
        std::cout << "Задача удалена: ID " << taskId << std::endl;
    }
}

// Закрытие режима редактирования
void ScrumBoard::closeEditMode() {
    showEditMode = false;
    editButton.setFillColor(sf::Color(180, 210, 235));
    editButtonText.setString("Редактировать");
    editButtonText.setCharacterSize(24);
    
    // Центрирование текста на кнопке
    centerTextInButton(editButtonText, editButton);
    
    closeTaskEditWindow();
}

// Закрытие окна редактирования задачи
void ScrumBoard::closeTaskEditWindow() {
    showTaskEditWindow = false;
    isEditTaskInputActive = false;
    currentEditTaskInput = "";
    editingTaskSection = -1;
    editingTaskIndex = -1;
    editTaskInputField.setOutlineColor(sf::Color(100, 130, 160));
    editTaskInputText.setString("Введите текст на английском");
    editTaskInputText.setFillColor(sf::Color(150, 150, 150));
    cursorVisible = false;
}

// Закрытие окна входа
void ScrumBoard::closeLoginWindow() {
    showLoginWindow = false;
    isUsernameInputActive = false;
    isPasswordInputActive = false;
    currentUsernameInput = "";
    currentPasswordInput = "";
    usernameField.setOutlineColor(sf::Color(100, 130, 160));
    passwordField.setOutlineColor(sf::Color(100, 130, 160));
    usernameText.setString("Введите имя пользователя");
    usernameText.setFillColor(sf::Color(150, 150, 150));
    passwordText.setString("Введите пароль");
    passwordText.setFillColor(sf::Color(150, 150, 150));
    cursorVisible = false;
}

// Закрытие окна добавления проекта
void ScrumBoard::closeAddProjectWindow() {
    showAddProjectWindow = false;
    isProjectNameInputActive = false;
    currentProjectNameInput = "";
    projectNameInputField.setOutlineColor(sf::Color(100, 130, 160));
    projectNameInputText.setString("Введите название проекта");
    projectNameInputText.setFillColor(sf::Color(150, 150, 150));
    cursorVisible = false;
}

// Закрытие окна добавления разработчика
void ScrumBoard::closeAddDeveloperWindow() {
    showAddDeveloperWindow = false;
    cursorVisible = false;
}

void ScrumBoard::saveCurrentProjectChanges() {
    if (idActiveProject.getId() != 0) {
        std::cout << "Сохранение изменений в проекте: " << idActiveProject.getName() << std::endl;
        
        // Обновляем задачи в активном проекте
        for (auto& task : tasksData) {
            Tasks* projectTask = idActiveProject.findTaskById(task.getId());
            if (projectTask) {
                // Обновляем существующую задачу
                projectTask->setTitle(task.getTitle());
                projectTask->changeStatus(task.getStatus());
            } else {
                // Добавляем новую задачу
                idActiveProject.addTask(task);
            }
        }
        
        // Сохраняем изменения в файл
        std::vector<Project> allProjects = Project::getProjectsFromJson();
        
        // Находим и обновляем активный проект в списке
        bool projectFound = false;
        for (auto& project : allProjects) {
            if (project.getId() == idActiveProject.getId()) {
                project = idActiveProject;
                projectFound = true;
                break;
            }
        }
        
        if (projectFound) {
            Project::saveProjectsToJson(allProjects);
            std::cout << "Изменения проекта сохранены в JSON" << std::endl;
        } else {
            std::cout << "Ошибка: проект не найден в списке!" << std::endl;
        }
    }
}

// Обработка всех событий ввода - ИСПРАВЛЕННАЯ ВЕРСИЯ ДЛЯ ПЕРЕТАСКИВАНИЯ
void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Обработка движения мыши для перетаскивания задач
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        
        // Перетаскивание задачи - ИСПРАВЛЕНО: корректное обновление позиции текста
        if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
            Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
            if (draggedTask.isMoving) {
                // Обновление позиции перетаскиваемой задачи
                float newX = event.mouseMove.x - 190;
                float newY = event.mouseMove.y - 40;
                draggedTask.setPosition(newX, newY);
                
                // Восстанавливаем правильную позицию текста относительно задачи
                sf::FloatRect taskBounds = draggedTask.shape.getGlobalBounds();
                draggedTask.text.setPosition(
                    taskBounds.left + draggedTask.textOffset.x,
                    taskBounds.top + draggedTask.textOffset.y
                );
            }
        }
    }
    
    // Обработка ввода текста для входа
    if (showLoginWindow && (isUsernameInputActive || isPasswordInputActive)) {
        handleLoginInput(event);
    }
    
    // Обработка ввода текста для новой задачи
    if (showAddTaskWindow && isTaskInputActive) {
        handleAddTaskInput(event);
    }
    
    // Обработка ввода текста при редактировании
    if (showTaskEditWindow && isEditTaskInputActive) {
        handleEditTaskInput(event);
    }
    
    // Обработка ввода текста для нового проекта
    if (showAddProjectWindow && isProjectNameInputActive) {
        handleAddProjectInput(event);
    }
    
    // Обработка нажатия кнопки мыши
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            // Обработка кнопки выхода (всегда видна после входа)
            if (isLoggedIn && logoutButton.getGlobalBounds().contains(mousePos)) {
                logout();
                return;
            }
            
            // Обработка информации о пользователе (если вошел)
            if (isLoggedIn && userInfoButton.getGlobalBounds().contains(mousePos)) {
                return;
            }
            
            // Обработка клика по кнопке "Войти" (только если не вошел)
            if (!isLoggedIn && loginButton.getGlobalBounds().contains(mousePos)) {
                showLoginWindow = !showLoginWindow; 
                return;
            }
            
            // Обработка клика по кнопке проектов (только после входа)
            if (isLoggedIn && projectButton.getGlobalBounds().contains(mousePos)) {
                showProjectWindow = !showProjectWindow; 
                return;
            }
            
            // Обработка клика по кнопке добавления проекта
            if (isLoggedIn && addProjectButton.getGlobalBounds().contains(mousePos)) {
                showAddProjectWindow = !showAddProjectWindow;
                if (showAddProjectWindow) {
                    currentProjectNameInput = "";
                    projectNameInputText.setString("Введите название проекта");
                    projectNameInputText.setFillColor(sf::Color(150, 150, 150));
                    isProjectNameInputActive = false;
                    projectNameInputField.setOutlineColor(sf::Color(100, 130, 160));
                    cursorVisible = false;
                    
                    // Закрываем другие окна
                    showProjectWindow = false;
                }
                return;
            }
            
            // Обработка клика по кнопке добавления разработчика (только после входа)
            if (isLoggedIn && addDeveloperButton.getGlobalBounds().contains(mousePos)) {
                if (idActiveProject.getId() != 0) {
                    showAddDeveloperWindow = !showAddDeveloperWindow;
                    if (showAddDeveloperWindow) {
                        createAddDeveloperWindow();
                    }
                } else {
                    std::cout << "Сначала выберите проект!" << std::endl;
                }
                return;
            }
            
            // Обработка клика по кнопке добавления задачи
            if (addButton.getGlobalBounds().contains(mousePos)) {
                if (isLoggedIn) {
                    // Проверяем доступ пользователя к активному проекту
                    if (idActiveProject.getId() != 0) {
                        bool hasAccess = false;
                        for (int devId : idActiveProject.getDeveloperIds()) {
                            if (devId == activeDeveloper->getId()) {
                                hasAccess = true;
                                break;
                            }
                        }
                        
                        if (hasAccess) {
                            showAddTaskWindow = !showAddTaskWindow;
                            if (showAddTaskWindow) {
                                currentTaskInput = "";
                                taskInputText.setString("Введите задачу на английском");
                                taskInputText.setFillColor(sf::Color(150, 150, 150));
                                isTaskInputActive = false;
                                taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                                cursorVisible = false;
                            }
                        } else {
                            std::cout << "Ошибка: нет доступа к проекту!" << std::endl;
                        }
                    } else {
                        std::cout << "Сначала выберите проект!" << std::endl;
                    }
                } else {
                    std::cout << "Для добавления задач необходимо войти в систему!" << std::endl;
                }
                return;
            }
            
            // Обработка клика по кнопке редактирования
            if (editButton.getGlobalBounds().contains(mousePos)) {
                if (isLoggedIn) {
                    openEditMode();
                } else {
                    std::cout << "Для редактирования задач необходимо войти в систему!" << std::endl;
                }
                return;
            }
            
            // Обработка окна входа
            if (showLoginWindow) {
                // Клик по полю ввода имени пользователя
                if (usernameField.getGlobalBounds().contains(mousePos)) {
                    isUsernameInputActive = true;
                    isPasswordInputActive = false;
                    usernameField.setOutlineColor(sf::Color(50, 100, 200));
                    passwordField.setOutlineColor(sf::Color(100, 130, 160));
                    if (currentUsernameInput.empty()) {
                        usernameText.setString("");
                        usernameText.setFillColor(sf::Color(50, 50, 80));
                    }
                    cursorClock.restart();
                    cursorVisible = true;
                    return;
                }
                
                // Клик по полю ввода пароля
                if (passwordField.getGlobalBounds().contains(mousePos)) {
                    isUsernameInputActive = false;
                    isPasswordInputActive = true;
                    usernameField.setOutlineColor(sf::Color(100, 130, 160));
                    passwordField.setOutlineColor(sf::Color(50, 100, 200));
                    if (currentPasswordInput.empty()) {
                        passwordText.setString("");
                        passwordText.setFillColor(sf::Color(50, 50, 80));
                    }
                    cursorClock.restart();
                    cursorVisible = true;
                    return;
                }
                
                // Клик по кнопке подтверждения входа
                if (confirmLoginButton.getGlobalBounds().contains(mousePos)) {
                    confirmLogin();
                    return;
                }
                
                // Клик по кнопке отмены входа
                if (cancelLoginButton.getGlobalBounds().contains(mousePos)) {
                    closeLoginWindow();
                    return;
                }
                
                // Закрытие окна при клике вне его
                if (!loginWindow.getGlobalBounds().contains(mousePos)) {
                    closeLoginWindow();
                    return;
                }
                return;
            }
            
            // Обработка окна добавления проекта
            if (showAddProjectWindow) {
                // Клик по полю ввода названия проекта
                if (projectNameInputField.getGlobalBounds().contains(mousePos)) {
                    isProjectNameInputActive = true;
                    projectNameInputField.setOutlineColor(sf::Color(50, 100, 200));
                    if (currentProjectNameInput.empty()) {
                        projectNameInputText.setString("");
                        projectNameInputText.setFillColor(sf::Color(50, 50, 80));
                    }
                    cursorClock.restart();
                    cursorVisible = true;
                    return;
                }
                
                // Клик по кнопке подтверждения
                if (confirmAddProjectButton.getGlobalBounds().contains(mousePos)) {
                    confirmAddProject();
                    return;
                }
                
                // Клик по кнопке отмены
                if (cancelAddProjectButton.getGlobalBounds().contains(mousePos)) {
                    closeAddProjectWindow();
                    return;
                }
                
                // Закрытие окна при клике вне его
                if (!addProjectWindow.getGlobalBounds().contains(mousePos)) {
                    closeAddProjectWindow();
                }
                return;
            }
            
            // Обработка окна добавления разработчика
            if (showAddDeveloperWindow) {
                for (size_t i = 0; i < developerRects.size(); i++) {
                    if (developerRects[i].getGlobalBounds().contains(mousePos)) {
                        addDeveloperToProject(i);
                        return;
                    }
                }
                
                // Закрытие окна при клике вне его
                if (!addDeveloperWindow.getGlobalBounds().contains(mousePos)) {
                    closeAddDeveloperWindow();
                }
                return;
            }

            // Обработка окна выбора проекта
            if (showProjectWindow) { 
                for (size_t i = 0; i < projectRects.size(); i++) {
                    if (projectRects[i].getGlobalBounds().contains(mousePos)) {
                        
                        // Сохраняем текущий проект
                        if (idActiveProject.getId() != 0) {
                            saveTasksData();
                        }
                        
                        // Загружаем СВЕЖИЕ данные выбранного проекта из файла
                        std::vector<Project> allProjects = Project::getProjectsFromJson();
                        for (const auto& project : allProjects) {
                            if (project.getId() == projects[i].getId()) {
                                idActiveProject = project;
                                break;
                            }
                        }
                        projectButtonText.setString(idActiveProject.getName());
                        tasksData = idActiveProject.getTasks();
                        createSampleTasks();
                        showProjectWindow = false;
                        return;
                    }
                }
                
                // Закрытие окна при клике вне его
                if (!projectWindow.getGlobalBounds().contains(mousePos)) {
                    showProjectWindow = false;
                }
                return;
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
                    cursorClock.restart();
                    cursorVisible = true;
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
                    cursorVisible = false;
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
                    cursorVisible = false;
                }
                
                // Закрытие окна при клике вне его
                if (!addTaskWindow.getGlobalBounds().contains(mousePos)) {
                    showAddTaskWindow = false;
                    isTaskInputActive = false;
                    currentTaskInput = "";
                    taskInputText.setString("Введите задачу на английском");
                    taskInputText.setFillColor(sf::Color(150, 150, 150));
                    taskInputField.setOutlineColor(sf::Color(100, 130, 160));
                    cursorVisible = false;
                }
                return;
            }
            
            // Обработка окна редактирования задачи
            if (showTaskEditWindow) {
                // Клик по полю ввода редактирования
                if (editTaskInputField.getGlobalBounds().contains(mousePos)) {
                    isEditTaskInputActive = true;
                    editTaskInputField.setOutlineColor(sf::Color(50, 100, 200));
                    cursorClock.restart();
                    cursorVisible = true;
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
                    closeTaskEditWindow();
                    return;
                }
                
                // Закрытие окна при клике вне его
                if (!editModeWindow.getGlobalBounds().contains(mousePos)) {
                    closeTaskEditWindow();
                }
                return;
            }
            
            // Обработка выбора задачи для редактирования
            if (showEditMode && !showTaskEditWindow) {
                for (int i = 0; i < 4; i++) {
                    for (size_t j = 0; j < tasks[i].size(); j++) {
                        if (tasks[i][j].shape.getGlobalBounds().contains(mousePos)) {
                            selectTaskForEditing(i, j);
                            return;
                        }
                    }
                }
                
                // Клик вне задач - закрываем режим редактирования
                closeEditMode();
                return;
            }
            
            // Обработка начала перетаскивания задачи (только если не открыты другие окна)
            if (!showEditMode && !showAddTaskWindow && !showProjectWindow && !showTaskEditWindow && !showLoginWindow && !showAddProjectWindow && !showAddDeveloperWindow) {
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
            // Завершение перетаскивания задачи - ИСПРАВЛЕНО: корректное восстановление позиции текста
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
                            
                            // Обновление статуса в данных и сохранение
                            updateTaskStatusInData(tasks[draggingTaskSection][draggingTaskIndex].id, newSection);
                            
                            // Удаление из старой секции
                            tasks[draggingTaskSection].erase(tasks[draggingTaskSection].begin() + draggingTaskIndex);
                            
                            // Обновление позиций задач
                            updateTaskPositions();
                            std::cout << "Задача перемещена в секцию " << newSection << std::endl;
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
}

// Обновление состояния
void ScrumBoard::update(float deltaTime) {
    // Мигание курсора
    if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        cursorVisible = !cursorVisible;
        cursorClock.restart();
    }
}

// Отрисовка окна входа
void ScrumBoard::drawLoginWindow(sf::RenderWindow& window) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Основное окно входа
    window.draw(loginWindow);
    window.draw(usernameLabel);
    window.draw(usernameField);
    window.draw(usernameText);
    window.draw(passwordLabel);
    window.draw(passwordField);
    window.draw(passwordText);
    window.draw(confirmLoginButton);
    window.draw(confirmLoginButtonText);
    window.draw(cancelLoginButton);
    window.draw(cancelLoginButtonText);
    
    // Отрисовка курсора для полей ввода
    if ((isUsernameInputActive || isPasswordInputActive) && cursorVisible) {
        sf::FloatRect textBounds;
        float cursorX, cursorY;
        
        if (isUsernameInputActive) {
            textBounds = usernameText.getLocalBounds();
            cursorX = usernameField.getPosition().x + textBounds.width + 15;
            cursorY = usernameField.getPosition().y + 5;
        } else {
            textBounds = passwordText.getLocalBounds();
            cursorX = passwordField.getPosition().x + textBounds.width + 15;
            cursorY = passwordField.getPosition().y + 5;
        }
        
        cursor.setPosition(cursorX, cursorY);
        window.draw(cursor);
    }
}

// Отрисовка окна редактирования задачи
void ScrumBoard::drawTaskEditWindow(sf::RenderWindow& window) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Основное окно редактирования
    window.draw(editModeWindow);
    window.draw(editTitleText);
    window.draw(editTaskInputField);
    window.draw(editTaskInputText);
    window.draw(saveEditButton);
    window.draw(saveEditButtonText);
    window.draw(deleteTaskButton);
    window.draw(deleteTaskButtonText);
    window.draw(cancelEditButton);
    window.draw(cancelEditButtonText);
}

// Отрисовка окна добавления проекта
void ScrumBoard::drawAddProjectWindow(sf::RenderWindow& window) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Основное окно
    window.draw(addProjectWindow);
    
    // Заголовок окна
    sf::Text titleText;
    titleText.setString("Добавить новый проект");
    titleText.setFont(font);
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color(50, 50, 80));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(addProjectWindow.getPosition().x + 20, addProjectWindow.getPosition().y + 20);
    window.draw(titleText);
    
    window.draw(projectNameInputField);
    window.draw(projectNameInputText);
    window.draw(confirmAddProjectButton);
    window.draw(confirmAddProjectButtonText);
    window.draw(cancelAddProjectButton);
    window.draw(cancelAddProjectButtonText);
    
    // Отрисовка курсора для поля ввода
    if (isProjectNameInputActive && cursorVisible) {
        sf::FloatRect textBounds = projectNameInputText.getLocalBounds();
        float cursorX = projectNameInputField.getPosition().x + textBounds.width + 15;
        float cursorY = projectNameInputField.getPosition().y + 10;
        cursor.setPosition(cursorX, cursorY);
        window.draw(cursor);
    }
}

// Отрисовка окна добавления разработчика
void ScrumBoard::drawAddDeveloperWindow(sf::RenderWindow& window) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Основное окно
    window.draw(addDeveloperWindow);
    
    // Заголовок окна
    sf::Text titleText;
    titleText.setString("Выберите разработчика");
    titleText.setFont(font);
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color(50, 50, 80));
    titleText.setStyle(sf::Text::Bold);
    
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        addDeveloperWindow.getPosition().x + (addDeveloperWindow.getSize().x - titleBounds.width) / 2,
        addDeveloperWindow.getPosition().y + 15
    );
    window.draw(titleText);
    
    // Кнопки разработчиков
    for (const auto& rect : developerRects) {  
        window.draw(rect);
    }
    
    // Текст разработчиков
    for (const auto& text : developerTexts) {  
        window.draw(text);
    }
}

// Основной метод отрисовки
void ScrumBoard::draw(sf::RenderWindow& window) {
    // Отрисовка основных элементов доски
    window.draw(titleText);
    window.draw(topPanel);
    
    // Отрисовка кнопки входа или информации о пользователе
    if (isLoggedIn) {
        // Сначала рисуем кнопку выхода, затем имя пользователя
        window.draw(logoutButton);
        window.draw(logoutButtonText);
        
        window.draw(userInfoButton);
        window.draw(userInfoText);
        
        // Кнопки управления проектами (только для вошедших пользователей)
        window.draw(projectButton);      
        window.draw(projectButtonText);
        window.draw(addProjectButton);
        window.draw(addProjectButtonText);
        window.draw(addDeveloperButton);
        window.draw(addDeveloperButtonText);
    } else {
        // Только кнопка входа для неавторизованных пользователей
        window.draw(loginButton);
        window.draw(loginButtonText);
    }
    
    // Правые кнопки (всегда видны, но функциональность зависит от авторизации)
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
            
            // Отрисовка только видимых задач
            if (taskBounds.top + taskBounds.height >= sectionBounds.top && 
                taskBounds.top <= sectionBounds.top + sectionBounds.height) {
                window.draw(task.shape);
                window.draw(task.text);
            }
        }
    }
    
    // Окно входа
    if (showLoginWindow) {
        drawLoginWindow(window);
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
        
        // Отрисовка курсора для поля ввода новой задачи
        if (isTaskInputActive && cursorVisible) {
            sf::FloatRect textBounds = taskInputText.getLocalBounds();
            float cursorX = taskInputField.getPosition().x + textBounds.width + 15;
            float cursorY = taskInputField.getPosition().y + 10;
            cursor.setPosition(cursorX, cursorY);
            window.draw(cursor);
        }
    }
    
    // Окно редактирования задачи
    if (showTaskEditWindow) {
        drawTaskEditWindow(window);
        
        // Отрисовка курсора для поля редактирования задачи
        if (isEditTaskInputActive && cursorVisible) {
            sf::FloatRect textBounds = editTaskInputText.getLocalBounds();
            float cursorX = editTaskInputField.getPosition().x + textBounds.width + 15;
            float cursorY = editTaskInputField.getPosition().y + 10;
            cursor.setPosition(cursorX, cursorY);
            window.draw(cursor);
        }
    }
    
    // Окно добавления проекта
    if (showAddProjectWindow) {
        drawAddProjectWindow(window);
    }
    
    // Окно добавления разработчика
    if (showAddDeveloperWindow) {
        drawAddDeveloperWindow(window);
    }
}