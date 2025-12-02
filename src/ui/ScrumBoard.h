#ifndef SCRUMBOARD_H
#define SCRUMBOARD_H

#include "Task.h"
#include "../core/Tasks.h"
#include "../core/Project.h"
#include "../core/Developer.h"
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
    sf::RectangleShape loginButton;                                               
    sf::Text loginButtonText;                                                      
    sf::RectangleShape projectButton;                                              
    sf::Text projectButtonText;                                                   
    sf::RectangleShape editButton;                                                 
    sf::Text editButtonText;                                                        
    sf::RectangleShape addButton;                                                  
    sf::Text addButtonText;                                                       
    
    // Кнопки управления проектами
    sf::RectangleShape addProjectButton;                                           // Кнопка "Добавить проект"
    sf::Text addProjectButtonText;                                                 // Текст кнопки добавления проекта
    sf::RectangleShape addDeveloperButton;                                         // Кнопка "Добавить разработчика"
    sf::Text addDeveloperButtonText;                                               // Текст кнопки добавления разработчика
    
    // Секции 4 колонки
    std::vector<sf::RectangleShape> sections;                                     
    std::vector<sf::Text> sectionTexts;                                             
    std::vector<std::string> sectionNames;                                         
    
    // Задачи по секциям
    std::vector<std::vector<Task>> tasks;                                          
    
    // Перетаскивание задач
    int draggingTaskSection;                                                   
    int draggingTaskIndex;                                                         
    
    // Окно входа
    bool showLoginWindow;                                                          // Флаг показа окна входа
    sf::RectangleShape loginWindow;                                                // Основное окно входа
    sf::RectangleShape usernameField;                                              // Поле ввода имени пользователя
    sf::Text usernameText;                                                         // Текст в поле имени пользователя
    sf::RectangleShape passwordField;                                              // Поле ввода пароля
    sf::Text passwordText;                                                         // Текст в поле пароля
    sf::Text usernameLabel;                                                        // Метка "Имя пользователя"
    sf::Text passwordLabel;                                                        // Метка "Пароль"
    sf::RectangleShape confirmLoginButton;                                         // Кнопка "Войти"
    sf::Text confirmLoginButtonText;                                               // Текст кнопки входа
    sf::RectangleShape cancelLoginButton;                                          // Кнопка "Отмена"
    sf::Text cancelLoginButtonText;                                                // Текст кнопки отмены
    std::string currentUsernameInput;                                              // Текущий введенный логин
    std::string currentPasswordInput;                                              // Текущий введенный пароль
    bool isUsernameInputActive;                                                    // Флаг активности поля логина
    bool isPasswordInputActive;                                                    // Флаг активности поля пароля
    
    // Окно проектов
    Project idActiveProject;
    bool showProjectWindow;                                                        // Флаг показа окна проектов
    sf::RectangleShape projectWindow;                                              // Основное окно проектов
    std::vector<sf::RectangleShape> projectRects;                                  // Кнопки проектов
    std::vector<sf::Text> projectTexts;                                            // Тексты на кнопках проектов
    std::vector<Project> projects;                                                 // Список названий проектов
    
    // Окно добавления задач
    bool showAddTaskWindow;                                                        // Флаг показа окна добавления
    bool isTaskInputActive;                                                        // Флаг активности поля ввода
    sf::RectangleShape addTaskWindow;                                              // Основное окно добавления
    sf::RectangleShape taskInputField;                                             // Поле ввода названия задачи
    sf::Text taskInputText;                                                        // Текст в поле ввода
    std::string currentTaskInput;                                                  // Текущий введенный текст
    sf::Text sectionLabelText;                                                     // Метка "Выберите секцию:"
    std::vector<sf::RectangleShape> sectionOptionRects;                            // Кнопки выбора секции
    std::vector<sf::Text> sectionOptionTexts;                                      // Тексты на кнопках секций
    sf::RectangleShape confirmAddButton;                                           // Кнопка "Добавить"
    sf::Text confirmAddButtonText;                                                 // Текст кнопки добавления
    sf::RectangleShape cancelAddButton;                                            // Кнопка "Отмена"
    sf::Text cancelAddButtonText;                                                  // Текст кнопки отмены
    
    // Окно редактирования задач
    bool showEditMode;                                                             // Флаг режима редактирования
    bool showTaskEditWindow;                                                       // Флаг показа окна редактирования задачи
    sf::RectangleShape editModeWindow;                                             // Окно редактирования
    sf::RectangleShape editTaskInputField;                                         // Поле ввода для редактирования
    sf::Text editTaskInputText;                                                    // Текст в поле редактирования
    std::string currentEditTaskInput;                                              // Текущий редактируемый текст
    bool isEditTaskInputActive;                                                    // Флаг активности поля редактирования
    int editingTaskSection;                                                        // Секция редактируемой задачи
    int editingTaskIndex;                                                          // Индекс редактируемой задачи
    sf::RectangleShape saveEditButton;                                             // Кнопка сохранения
    sf::Text saveEditButtonText;                                                   // Текст кнопки сохранения
    sf::RectangleShape cancelEditButton;                                           // Кнопка отмены редактирования
    sf::Text cancelEditButtonText;                                                 // Текст кнопки отмены
    sf::RectangleShape deleteTaskButton;                                           // Кнопка удаления
    sf::Text deleteTaskButtonText;                                                 // Текст кнопки удаления
    sf::Text editTitleText;                                                        // Заголовок окна редактирования
    
    // Курсор для полей ввода
    sf::RectangleShape cursor;                                                     // Курсор ввода текста
    sf::Clock cursorClock;                                                         // Таймер для мигания курсора
    bool cursorVisible;                                                            // Видимость курсора
    
    // Данные задач
    std::vector<Tasks> tasksData;                                                  

    // Переменные для управления пользователем
    bool isLoggedIn;                                                              // Флаг вошел ли пользователь в систему
    std::string currentUser;                                                      // Имя текущего пользователя
    sf::RectangleShape userInfoButton;                                            // Кнопка с информацией о пользователе
    sf::Text userInfoText;                                                        // Текст с именем пользователя
    sf::RectangleShape logoutButton;                                              // Кнопка "Выйти"
    sf::Text logoutButtonText;                                                    // Текст на кнопке "Выйти"
    Developer* activeDeveloper = nullptr;                                         // Указатель на активного разработчика
    bool showLogoutButton;                                                        // Флаг показа кнопки выхода

    // Новые окна для управления проектами и разработчиками
    bool showAddProjectWindow;                                                     // Флаг показа окна добавления проекта
    sf::RectangleShape addProjectWindow;                                           // Основное окно добавления проекта
    sf::RectangleShape projectNameInputField;                                      // Поле ввода названия проекта
    sf::Text projectNameInputText;                                                 // Текст в поле ввода
    std::string currentProjectNameInput;                                           // Текущий введенный текст названия проекта
    bool isProjectNameInputActive;                                                 // Флаг активности поля ввода названия проекта
    sf::RectangleShape confirmAddProjectButton;                                    // Кнопка подтверждения добавления проекта
    sf::Text confirmAddProjectButtonText;                                          // Текст кнопки подтверждения
    sf::RectangleShape cancelAddProjectButton;                                     // Кнопка отмены добавления проекта
    sf::Text cancelAddProjectButtonText;                                           // Текст кнопки отмены

    bool showAddDeveloperWindow;                                                   // Флаг показа окна добавления разработчика
    sf::RectangleShape addDeveloperWindow;                                         // Основное окно добавления разработчика
    std::vector<sf::RectangleShape> developerRects;                                // Кнопки разработчиков
    std::vector<sf::Text> developerTexts;                                          // Тексты на кнопках разработчиков
    std::vector<Developer> availableDevelopers;                                    // Список доступных разработчиков

    // Кнопка регистрации
    sf::RectangleShape registerButton;
    sf::Text registerButtonText;

    // Сообщения
    bool showMessage;                                                              // Флаг показа сообщения
    float messageTimer;                                                            // Таймер для сообщения
    sf::Text messageText;                                                          // Текст сообщения
    sf::RectangleShape messageBackground;                                          // Фон сообщения


public:
    void confirmRegister();
    ScrumBoard();                                                                   // Конструктор - инициализация переменных
    ~ScrumBoard();                                                                  // Деструктор
    bool initialize();                                                              // Инициализация ресурсов и интерфейса
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);             // Обработка событий ввода
    void update(float deltaTime);                                                   // Обновление состояния
    void draw(sf::RenderWindow& window);                                            // Отрисовка всех компонентов

private:
    void saveCurrentProjectChanges();
    void createTitle();                                                             // Создание заголовка приложения
    void createTopPanel();                                                          // Создание верхней панели с кнопками
    void createProjectManagementButtons();                                          // Создание кнопок управления проектами
    void createSections();                                                          // Создание 4 секций для задач
    void createSampleTasks();                                                       // Создание задач из данных JSON
    void createLoginWindow();                                                       // Создание окна входа
    void createProjectWindow();                                                     // Создание окна выбора проектов
    void createAddTaskWindow();                                                     // Создание окна добавления задачи
    void createTaskEditWindow();                                                    // Создание окна редактирования задачи
    void createUserInfo();                                                          // Создание элемента информации о пользователе
    void createAddProjectWindow();                                                  // Создание окна добавления проекта
    void createAddDeveloperWindow();                                                // Создание окна добавления разработчика

    void addTask(int id, const std::string& taskName, int section);                 // Добавление визуальной задачи в секцию
    void updateTaskPositions();                                                     // Обновление позиций всех задач
    void updateTaskStatusInData(int taskId, int newStatus);                         // Обновление статуса задачи в данных
    void saveTasksData();                                                           // Сохранение задач в JSON файл
    void handleAddTaskInput(const sf::Event& event);                                // Обработка ввода текста для новой задачи
    void handleEditTaskInput(const sf::Event& event);                               // Обработка ввода текста при редактировании
    void handleLoginInput(const sf::Event& event);                                  // Обработка ввода текста для входа
    void handleAddProjectInput(const sf::Event& event);                             // Обработка ввода текста для нового проекта
    void confirmAddTask(int selectedSection);                                       // Подтверждение добавления новой задачи
    void confirmAddProject();                                                       // Подтверждение добавления проекта
    void addDeveloperToProject(int developerIndex);                                 // Добавление разработчика к проекту
    void confirmLogin();                                                            // Подтверждение входа
    void logout();                                                                  // Выход из системы
    void openEditMode();                                                            // Открытие режима редактирования
    void openTaskEditWindow();                                                      // Открытие окна редактирования задачи
    void selectTaskForEditing(int section, int index);                              // Выбор задачи для редактирования
    void saveEditedTask();                                                          // Сохранение изменений задачи
    void deleteCurrentTask();                                                       // Удаление текущей задачи
    void closeEditMode();                                                           // Закрытие режима редактирования
    void closeTaskEditWindow();                                                     // Закрытие окна редактирования задачи
    void closeLoginWindow();                                                        // Закрытие окна входа
    void closeAddProjectWindow();                                                   // Закрытие окна добавления проекта
    void closeAddDeveloperWindow();                                                 // Закрытие окна добавления разработчика
    
    // Методы для работы с сообщениями
    void showMessageFor(const std::string& message, float duration);                // Показать сообщение на экране
    void drawMessage(sf::RenderWindow& window);                                     // Отрисовка сообщения
    
    // Методы для работы с окнами
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);      // Центрирование текста в кнопке
    void drawTaskEditWindow(sf::RenderWindow& window);                              // Отрисовка окна редактирования задачи
    void drawLoginWindow(sf::RenderWindow& window);                                 // Отрисовка окна входа
    void drawAddProjectWindow(sf::RenderWindow& window);                            // Отрисовка окна добавления проекта
    void drawAddDeveloperWindow(sf::RenderWindow& window);                          // Отрисовка окна добавления разработчика
};
#endif 