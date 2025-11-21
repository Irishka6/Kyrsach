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
    sf::RectangleShape loginButton;                                               
    sf::Text loginButtonText;                                                      
    sf::RectangleShape projectButton;                                              
    sf::Text projectButtonText;                                                   
    sf::RectangleShape editButton;                                                 
    sf::Text editButtonText;                                                        
    sf::RectangleShape addButton;                                                  
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
    bool showProjectWindow;                                                         // Флаг показа окна проектов
    sf::RectangleShape projectWindow;                                               // Основное окно проектов
    std::vector<sf::RectangleShape> projectRects;                                   // Кнопки проектов
    std::vector<sf::Text> projectTexts;                                             // Тексты на кнопках проектов
    std::vector<std::string> projects;                                              // Список названий проектов
    
    // Окно добавления задач
    bool showAddTaskWindow;                                                         // Флаг показа окна добавления
    bool isTaskInputActive;                                                         // Флаг активности поля ввода
    sf::RectangleShape addTaskWindow;                                               // Основное окно добавления
    sf::RectangleShape taskInputField;                                              // Поле ввода названия задачи
    sf::Text taskInputText;                                                         // Текст в поле ввода
    std::string currentTaskInput;                                                   // Текущий введенный текст
    sf::Text sectionLabelText;                                                      // Метка "Выберите секцию:"
    std::vector<sf::RectangleShape> sectionOptionRects;                             // Кнопки выбора секции
    std::vector<sf::Text> sectionOptionTexts;                                       // Тексты на кнопках секций
    sf::RectangleShape confirmAddButton;                                            // Кнопка "Добавить"
    sf::Text confirmAddButtonText;                                                  // Текст кнопки добавления
    sf::RectangleShape cancelAddButton;                                             // Кнопка "Отмена"
    sf::Text cancelAddButtonText;                                                   // Текст кнопки отмены
    
    // Окно редактирования задач
    bool showEditMode;                                                              // Флаг режима редактирования
    bool showTaskEditWindow;                                                        // Флаг показа окна редактирования задачи
    sf::RectangleShape editModeWindow;                                              // Окно редактирования
    sf::RectangleShape editTaskInputField;                                          // Поле ввода для редактирования
    sf::Text editTaskInputText;                                                     // Текст в поле редактирования
    std::string currentEditTaskInput;                                               // Текущий редактируемый текст
    bool isEditTaskInputActive;                                                     // Флаг активности поля редактирования
    int editingTaskSection;                                                         // Секция редактируемой задачи
    int editingTaskIndex;                                                           // Индекс редактируемой задачи
    sf::RectangleShape saveEditButton;                                              // Кнопка сохранения
    sf::Text saveEditButtonText;                                                    // Текст кнопки сохранения
    sf::RectangleShape cancelEditButton;                                            // Кнопка отмены редактирования
    sf::Text cancelEditButtonText;                                                  // Текст кнопки отмены
    sf::RectangleShape deleteTaskButton;                                            // Кнопка удаления
    sf::Text deleteTaskButtonText;                                                  // Текст кнопки удаления
    sf::Text editTitleText;                                                         // Заголовок окна редактирования
    
    // Курсор для полей ввода
    sf::RectangleShape cursor;                                                      // Курсор ввода текста
    sf::Clock cursorClock;                                                          // Таймер для мигания курсора
    bool cursorVisible;                                                             // Видимость курсора
    
    // Данные задач
    std::vector<Tasks> tasksData;                                                   

    // Переменные для управления пользователем
    bool isLoggedIn;
    std::string currentUser;
    sf::RectangleShape userInfoButton;
    sf::Text userInfoText;
    sf::RectangleShape logoutButton;
    sf::Text logoutButtonText;
    bool showLogoutButton;

public:
    ScrumBoard();                                                                   // Конструктор - инициализация переменных
    bool initialize();                                                              // Инициализация ресурсов и интерфейса
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);             // Обработка событий ввода
    void update(float deltaTime);                                                   // Обновление состояния
    void draw(sf::RenderWindow& window);                                            // Отрисовка всех компонентов

private:
    void createTitle();                                                             // Создание заголовка приложения
    void createTopPanel();                                                          // Создание верхней панели с кнопками
    void createSections();                                                          // Создание 4 секций для задач
    void createSampleTasks();                                                       // Создание задач из данных JSON
    void createLoginWindow();                                                       // Создание окна входа
    void createProjectWindow();                                                     // Создание окна выбора проектов
    void createAddTaskWindow();                                                     // Создание окна добавления задачи
    void createTaskEditWindow();                                                    // Создание окна редактирования задачи
    void createUserInfo();                                                          // Создание элемента информации о пользователе

    void addTask(int id, const std::string& taskName, int section);                 // Добавление визуальной задачи в секцию
    void updateTaskPositions();                                                     // Обновление позиций всех задач
    void updateTaskStatusInData(int taskId, int newStatus);                         // Обновление статуса задачи в данных
    void saveTasksData();                                                           // Сохранение задач в JSON файл
    void handleAddTaskInput(const sf::Event& event);                                // Обработка ввода текста для новой задачи
    void handleEditTaskInput(const sf::Event& event);                               // Обработка ввода текста при редактировании
    void handleLoginInput(const sf::Event& event);                                  // Обработка ввода текста для входа
    void confirmAddTask(int selectedSection);                                       // Подтверждение добавления новой задачи
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
    
    // Методы для работы с окном редактирования
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);      // Центрирование текста в кнопке
    void drawTaskEditWindow(sf::RenderWindow& window);                              // Отрисовка окна редактирования задачи
    void drawLoginWindow(sf::RenderWindow& window);                                 // Отрисовка окна входа
};
#endif